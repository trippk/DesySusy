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
                         * self.reducer() * self.trackIsolation()
                         * self.tree() )
    
    def tree(self) :
        self.process.susyTree = cms.EDAnalyzer("SusyTree", outputCommands = cms.untracked.vstring(
            'drop *',
            'keep *_susycaf*_*_*',
            'keep *_susydesy*_*_*',
            'keep *_filterResult*_*_*',
            'keep *_*FilterFlag__*',
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
                       ['Event','Track','Triggers','L1Triggers', 'L1Extra',
                        'BeamSpot','BeamHaloSummary','LogError','Vertex',
                        'HcalRecHit','EcalRecHit','PFRecHit','MET','SumP4',
                        'HcalDeadChannels','EcalDeadChannels','CaloTowers','Double','TrackerIsolation'] +
                       [['Gen','Scan','PileupSummary'],['DQMFlags','DCSBits']][self.options.isData]) :
            self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%module)

        from SUSYBSMAnalysis.SusyCAF.SusyCAF_Scan_cfi import susycafscanFunc as susycafscanFunc
        self.process.susycafscan = susycafscanFunc(self.options.scan) if self.options.scan else self.empty
        if self.options.scan.find("T2tt") > -1 :
            self.process.susycafscan.ScanFormat = r"# model T2tt_(\\d*\.\\d*)_(\\d*\.\\d*)\\s\\s(\\d*\.\\d*)\\s*"
            self.process.susycafscan.ScanParameters = ['mStop', 'mLSP', 'XSEC']
        from SUSYBSMAnalysis.DesySusy.SusyDESY_Module_cfi import susydesyscanTemp
        self.process.susydesyscan = susydesyscanTemp.clone( ScanFormat = self.process.susycafscan.ScanFormat,
                                                            ScanParameters = self.process.susycafscan.ScanParameters) if self.options.scan else self.empty

                
        self.process.susycaftriggers.SourceName  = self.options.SourceName
        
        from SUSYBSMAnalysis.SusyCAF.SusyCAF_MET_cfi import met
        self.process.patMETsPF.metSource = cms.InputTag("pfMet")
        self.process.susydesymetPF         = met('Pat' , 'patMETsPF', 'PF', 'DESYmet', calo=False)

        return ( self.evalSequence('susycafhcalnoise%s', ['filter','filternoiso','rbx','summary']) +
                 self.evalSequence('susycaf%s', (['event','track','pfsump4','beamspot','logerror','vertex','calotowers','rho','rho25','TrackerIsolation'] +
                                                 (['beamhalosummary'] if self.options.beamHaloVars else [] ) +
                                                 (['triggers','L1triggers','l1extra'] if self.options.triggers else [])) ) +
                 self.process.susycafmet + self.process.susycafmetnohf +
                 self.evalSequence('susycaf%sdeadchannels', ['ecal','hcal']) +
                 self.evalSequence('susycaf%srechit', [ 'hbhe', 'hf', 'eb', 'ee' ]) +
                 self.evalSequence('susycafpfrechitcluster%s', ['ecal','hcal','hfem','hfhad','ps']) +
                 self.evalSequence('susycafpfrechit%s',        ['ecal','hcal','hfem','hfhad','ps']) +

                 self.process.susydesymetPF + self.process.susydesyscan +
                 self.evalSequence(*[ ('susycaf%s',['gen','genMetCalo','genMetCaloAndNonPrompt','genMetTrue','scan','pileupsummary']), # Gen
                                      ('susycaf%s',['dqmflags','dcsbits'][(not self.options.dqm):]) # Data
                                      ][self.options.isData])
                 )

    def reco(self) :
        for module in ['Jet','Photon','Muon','Electron','PFTau'] :
            self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%module)
            
        return sum( [ self.evalSequence('susycaf%sjetreco', filter(lambda x:"pf2pat" not in x, self.options.jetCollections)),
                      self.evalSequence('susycaf%sreco', ['photon','electron','muon'])],
                    [ self.process.susycafPFtau ] if self.options.taus else [] )


    def pat(self) :
        for module in ['MET','Photon','PFTau'] :
            self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%module)
        for module in ['Module','FilterResultProducer','Filter'] :
            self.process.load('SUSYBSMAnalysis.DesySusy.SusyDESY_%s_cfi'%module)
        #self.process.load('RecoMET.METFilters.eeBadScFilter_cfi')
        #self.process.eeBadScFilterFlag = self.process.eeBadScFilter.clone(taggingMode = True)
        
        self.process.load('RecoMET.METFilters.ecalLaserCorrFilter_cfi')
        self.process.ecalLaserCorrFilterFlag = self.process.ecalLaserCorrFilter.clone(taggingMode = True)
        
        return ( self.patJet() + self.PileUpJetID() +
                 self.patLepton('Electron') + self.patLepton('Muon') +
                 self.evalSequence('susycaf%s',  ['photon']+(['tau','HPStau','pftau'] if self.options.taus else [])) +
                 self.evalSequence('susycafmet%s', ['AK5','AK5TypeII','AK5TypeI','PF','TypeIPFPat','TypeIPF','TC']) +
                 #self.process.eeBadScFilterFlag +
                 self.process.ecalLaserCorrFilterFlag +
                 self.evalSequence('susydesy%s', ['patelectrons','pfelectrons','patmuons','pfmuons','jets','tau','trigger']+
                                   ([] if self.options.isData else ['pu']))+
                 self.evalSequence('filterResult%s', ['OneLepton']) +
                 self.evalSequence('filter%s'      , ['OneLepton'])
                 )

    def patLepton(self,lepton) :
        self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_%s_cfi'%lepton)

        if (lepton.find("Muon") > -1 ):
            self.process.susycafmuon.InputTag = cms.InputTag("selectedPatMuons")
            self.process.susycafmuon.SelectedMuons = cms.InputTag("cleanPatMuons") 

        if (lepton.find("Electron") > -1 ):
            self.process.susycafelectron.InputTag = cms.InputTag("selectedPatElectrons")
            self.process.susycafelectron.SelectedElectrons = cms.InputTag("cleanPatElectrons")
        
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

        jets.susycafak5pfjet.InputTag = cms.InputTag("selectedPatJetsAK5PF")
        jets.susycafak5pfjet.AllJets  = cms.InputTag("selectedPatJetsAK5PF")
        jets.susycafak5pfjetMatched.InputTag = cms.InputTag("selectedPatJetsAK5PF")
        jets.susycafak5pfjetMatched.AllJets  = cms.InputTag("selectedPatJetsAK5PF")
        
        for mod in filter(lambda mod: mod.startswith('susycaf'), dir(jets)) :
            if mod.endswith('reco') : exec('self.process.%s = jets.%s'%(mod,mod))
            else : exec('self.process.%s = jets.%s.clone( JetCorrections = [%s])'%(mod,mod,','.join(["'%s'"%s for s in self.options.jetCorrections])))
                
        modules = [getattr(self.process,('susycaf%sjet'+['Matched',''][self.options.isData])%algo) for algo in self.options.jetCollections]
        from SUSYBSMAnalysis.SusyCAF.SusyCAF_Selection.selectors_cfi import patJetSelector
        selectors = self.process.SusyCAFPatJetSelectors = cms.Sequence()
        for module in modules :
            selectors += helpers.applySelection(self.process, module, "pt > 0", patJetSelector)[0]
        return selectors + sum(modules,self.empty)    

    def allTracks(self) :
        if not self.options.AllTracks : return self.empty
        self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_AllTracks_cfi')
        return self.process.susycafalltracks

    def trackIsolation(self) :
        self.process.load("SUSYBSMAnalysis.DesySusy.trackIsolationMaker_cfi")
        return self.process.susydesytrackIsolationMaker

    def PileUpJetID(self) :
        from CMGTools.External.puJetIDAlgo_cff import full_53x, full_5x, cutbased
        from CMGTools.External.pujetidproducer_cfi import pileupJetIdProducer

        full_53x.tmvaWeights = cms.string("CMGTools/External/data/TMVAClassificationCategory_JetID_53X_Dec2012.weights.xml")
        full_53x.label = cms.string("full53x")
        full_5x.label = cms.string("full5x")
        
        self.process.puJetId = pileupJetIdProducer.clone(
            produceJetIds = cms.bool(True),
            jetids = cms.InputTag(""),
            runMvas = cms.bool(False),
            jets = cms.InputTag("selectionsusycafak5pfjetMatched0"),
            vertexes = cms.InputTag("offlinePrimaryVertices"),
            algos = cms.VPSet(cutbased)
            )
        
        self.process.puJetMva = pileupJetIdProducer.clone(
            produceJetIds = cms.bool(False),
            jetids = cms.InputTag("puJetId"),
            runMvas = cms.bool(True),
            jets = cms.InputTag("selectionsusycafak5pfjetMatched0"),
            vertexes = cms.InputTag("offlinePrimaryVertices"),
            algos = cms.VPSet(full_53x, full_5x)
            )
        return (self.process.puJetId * self.process.puJetMva )
