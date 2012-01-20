import FWCore.ParameterSet.Config as cms
import SusyCAF_Drop_cfi
import SusyCAF_Selection.helpers as helpers

class SusyCAF(object) :
    def __init__(self,process,options) :
        self.process = process
        self.options = options
        self.empty = process.empty = cms.Sequence()

    def path(self) :
        return cms.Path( ( self.common() +
                           [ self.reco,  self.pat][self.options.patify]() +
                           self.allTracks() )
                         * self.reducer()
                         * self.tree() )
    
    def tree(self) :
        self.process.susyTree = cms.EDAnalyzer("SusyTree", outputCommands = cms.untracked.vstring(
            'drop *',
            'keep *_susycaf*_*_*',
            'keep *_susydesy*_*_*',
            'keep double_kt6PFJets_rho_%s'%self.process.name_(),
            'keep *_ecaldeadcellfilterflag_*_*',
            'keep *_trackingfailurefilterflag_*_*',
            'keep double_susyScan*_*_*') + (
            ["drop %s"%s for s in SusyCAF_Drop_cfi.drop(self.options.dropMore)] +
            ["keep %s"%s for s in SusyCAF_Drop_cfi.keep()]) +
            ["drop %s"%s for s in SusyCAF_Drop_cfi.reduce()] )
        return self.process.susyTree
    
    def reducer(self) :
        self.process.susycafReducer = cms.EDProducer("ProductReducer",
                                                     selectionCommands = cms.untracked.vstring('drop *') +
                                                     ["keep %s"%s for s in SusyCAF_Drop_cfi.reduce()])
        return self.process.susycafReducer
    

    def evalSequence(self, pattern, names) :
        return sum([getattr(self.process, pattern%name) for name in names], self.empty)


    def common(self) :
        for module in (['HcalNoise%s'%s for s in ['Filter','RBX','Summary']] +
                       ['Event','Track','Triggers','L1Triggers',
                        'BeamSpot','BeamHaloSummary','LogError','Vertex',
                        'HcalRecHit','EcalRecHit','PFRecHit','MET','SumP4',
                        'HcalDeadChannels','EcalDeadChannels','CaloTowers'] +
                       [['Gen','Scan','PileupSummary'],['DQMFlags','DCSBits']][self.options.isData]) :
            self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%module)

        from SUSYBSMAnalysis.SusyCAF.SusyCAF_Scan_cfi import susycafscanFunc as susycafscanFunc
        self.process.susycafscan = susycafscanFunc(self.options.scan) if self.options.scan else self.empty
        self.process.susycaftriggers.SourceName  = self.options.SourceName
        return ( self.evalSequence('susycafhcalnoise%s', ['filter','rbx','summary']) +
                 self.evalSequence('susycaf%s', ['event','track','triggers','L1triggers','pfsump4',
                                                 'beamspot','beamhalosummary','logerror','vertex','calotowers']) +
                 self.process.susycafmet + self.process.susycafmetnohf +
                 self.evalSequence('susycaf%sdeadchannels', ['ecal','hcal']) +
                 self.evalSequence('susycaf%srechit', [ 'hbhe', 'hf', 'eb', 'ee' ]) +
                 self.evalSequence('susycafpfrechitcluster%s', ['ecal','hcal','hfem','hfhad','ps']) +
                 self.evalSequence('susycafpfrechit%s',        ['ecal','hcal','hfem','hfhad','ps']) +
                 
                 self.evalSequence(*[ ('susycaf%s',['gen','genMetCalo','genMetCaloAndNonPrompt','genMetTrue','scan','pileupsummary']), # Gen
                                      ('susycaf%s',['dqmflags','dcsbits']) # Data
                                      ][self.options.isData])
                 )

    def reco(self) :
        for module in ['Jet','Photon','Muon','Electron','PFTau'] :
            self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%module)
        return ( self.process.susycafPFtau +
                 self.evalSequence('susycaf%sjetreco', filter(lambda x:"pf2pat" not in x, self.options.jetCollections)) +
                 self.evalSequence('susycaf%sreco', ['photon','electron','muon']) )

    def pat(self) :
        for module in ['MET','Photon','PFTau'] :
            self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%module)
        for module in ['Module','TotalKinematicsFilter'] :
            self.process.load('SUSYBSMAnalysis.DesySusy.SusyDESY_%s_cfi'%module)
        return ( self.patJet() +
                 self.process.susydesytotakinematicsfilter +
                 self.patLepton('Electron') + self.patLepton('Muon') +
                 self.evalSequence('susydesy%s', ['patelectrons','pfelectrons','patmuons','pfmuons','puinfo','trigger']) +
                 self.evalSequence('susycaf%s',  ['tau',
                                                  'HPStau',
                                                  'pftau','photon']) +
                 self.evalSequence('susycafmet%s', ['AK5','AK5TypeII','PF','TypeIPF','TC'])
                 )

    def patLepton(self,lepton) :
        self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%lepton)
        exec('from SUSYBSMAnalysis.SusyCAF.SusyCAF_Selection.selectors_cfi import pat%sSelector as patSelector'%lepton)
        selectors = cms.Sequence()
        setattr(self.process,'SusyCAFPat%sSelectors'%lepton, selectors)
        modules = [getattr(self.process, pre%lepton.lower()) for pre in ['susycaf%s','susycafpf%s']]
        for module in modules :
            if self.options.leptonPtMin :
                selectors += helpers.applySelection(self.process, module, "pt > %d"%self.options.leptonPtMin, patSelector)[0]
        return selectors + sum(modules, self.empty)

    def patJet(self) :
        import SUSYBSMAnalysis.SusyCAF.SusyCAF_Jet_cfi as jets
        for mod in filter(lambda mod: mod.startswith('susycaf'), dir(jets)) :
            if mod.endswith('reco') : exec('self.process.%s = jets.%s'%(mod,mod))
            else : exec('self.process.%s = jets.%s.clone( JetCorrections = [%s])'%(mod,mod,','.join(["'%s'"%s for s in self.options.jetCorrections])))
                
        modules = [getattr(self.process,('susycaf%sjet'+['Matched',''][self.options.isData])%algo) for algo in self.options.jetCollections]
        from SUSYBSMAnalysis.SusyCAF.SusyCAF_Selection.selectors_cfi import patJetSelector
        selectors = self.process.SusyCAFPatJetSelectors = cms.Sequence()
        for module in modules :
            selectors += helpers.applySelection(self.process, module, "pt > 15", patJetSelector)[0]
        return selectors + sum(modules,self.empty)

    def allTracks(self) :
        if not self.options.AllTracks : return self.empty
        process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_AllTracks_cfi')
        return process.susycafalltracks
