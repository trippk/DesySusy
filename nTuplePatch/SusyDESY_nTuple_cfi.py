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
                         * self.reducer() * self.tree() )
    
    def tree(self) :
        self.process.susyTree = cms.EDAnalyzer("SusyTree", outputCommands = cms.untracked.vstring(
            'drop *',
            'keep *_susycaf*_*_*',
            'keep *_susydesy*_*_*',
            'keep *_filterResult*_*_*',
            'keep *_*FilterFlag__*',
            'keep double_susyScan*_*_*',
            'drop edmErrorSummaryEntrys_*_*_*') + (
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
        
        self.process.susydesymetPF  = met('Pat' , 'patMETsPF', 'PF', 'DESYmet', calo=False)
        
        self.process.susydesycorMetGlobalMuons = met('Calo', 'corMetGlobalMuons', 'Calo',  'corMetGlobalMuons')
        self.process.susydesymetHO = met('Calo', 'metHO', 'Calo',  'metHO')
        self.process.susydesytcMet = met('Calo', 'tcMet', 'Calo',  'tcMet')
        self.process.susydesytcMetWithPFclusters = met('Calo', 'tcMetWithPFclusters', 'Calo',  'tcMetWithPFclusters')

        self.process.susydesyCaloMET = cms.Sequence( self.process.susydesycorMetGlobalMuons +
                                                     self.process.susydesymetHO +
                                                     self.process.susydesytcMet +
                                                     self.process.susydesytcMetWithPFclusters
                                                     )
        
        return ( self.evalSequence('susycafhcalnoise%s', ['filter','filternoiso','rbx','summary']) +
                 self.evalSequence('susycaf%s', (['event','track','pfsump4','beamspot','logerror','vertex','calotowers','rho','rho25'] +
                                                 (['beamhalosummary'] if self.options.beamHaloVars else [] ) +
                                                 (['triggers','L1triggers','l1extra'] if self.options.triggers else [])) ) +
                 self.evalSequence('susycaf%sdeadchannels', ['ecal','hcal']) +
                 self.evalSequence('susycaf%srechit', [ 'hbhe', 'hf', 'eb', 'ee' ]) +
                 self.evalSequence('susycafpfrechitcluster%s', ['ecal','hcal','hfem','hfhad','ps']) +
                 self.evalSequence('susycafpfrechit%s',        ['ecal','hcal','hfem','hfhad','ps']) +

                 self.process.susydesymetPF + self.process.susydesyCaloMET + self.process.susydesyscan +
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

        #SusyDESY_Jet
        from SUSYBSMAnalysis.DesySusy.SusyDESY_Module_cfi import susydesyjets
        self.process.susydesyjets = susydesyjets.clone (JetsTag = "selectionsusycafak5pfjet" + ("Matched" if not self.options.isData else "") +"0" )
                                    
        ## ecalDeadCellBEFilterFlag
        from RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi import EcalDeadCellBoundaryEnergyFilter

        self.process.ecalDeadCellBEFilterFlag = EcalDeadCellBoundaryEnergyFilter.clone(taggingMode = True,
                                                                                       cutBoundEnergyDeadCellsEB = 10.0,
                                                                                       cutBoundEnergyDeadCellsEE = 10.0,
                                                                                       cutBoundEnergyGapEB = 100.0,
                                                                                       cutBoundEnergyGapEE = 100.0,
                                                                                       enableGap = False,
                                                                                       limitDeadCellToChannelStatusEB = cms.vint32(12,14),
                                                                                       limitDeadCellToChannelStatusEE = cms.vint32(12,14))
        
        # ecalLaserCorrFilterFilterFlag
        self.process.load('RecoMET.METFilters.ecalLaserCorrFilter_cfi')
        self.process.ecalLaserCorrFilterFlag = self.process.ecalLaserCorrFilter.clone(taggingMode = True)

        #hcalLaserFilterFlag
        from SUSYBSMAnalysis.DesySusy.SusyDESY_Module_cfi import EventFilterFromList
        self.process.hcalLaser2012EventFilterFlag = EventFilterFromList.clone( ListFile  = 'EventFilter/HcalRawToDigi/data/HCALLaser2012AllDatasets.txt.gz',
                                                                               taggingMode = cms.untracked.bool (True)
                                                                               ) if self.options.isData else self.empty
        
        ## The tracking POG filters _________________________________________________
        ## The following filters output a flag with an opposite meaning to what we have for the other MET cleaning filters. In thi case: true = bad event; false = good event
        
        from RecoMET.METFilters.trackingPOGFilters_cff import logErrorTooManyClusters, logErrorTooManyTripletsPairs, logErrorTooManySeeds, logErrorTooManySeedsDefault, manystripclus53X, toomanystripclus53X, logErrorTooManyTripletsPairsMainIterations, logErrorTooManySeedsMainIterations

        self.process.logErrorTooManyClustersFilterFlag = logErrorTooManyClusters.clone ( taggedMode = cms.untracked.bool(True),
                                                                                         forcedValue = cms.untracked.bool(True) )
        self.process.logErrorTooManyTripletsPairsFilterFlag = logErrorTooManyTripletsPairs.clone ( taggedMode = cms.untracked.bool(True),
                                                                                                   forcedValue = cms.untracked.bool(True) )
        self.process.logErrorTooManySeedsFilterFlag = logErrorTooManySeeds.clone ( taggedMode = cms.untracked.bool(True),
                                                                                   forcedValue = cms.untracked.bool(True) )
        self.process.logErrorTooManySeedsDefaultFilterFlag = logErrorTooManySeedsDefault.clone ( taggedMode = cms.untracked.bool(True),
                                                                                                 forcedValue = cms.untracked.bool(True) )
        self.process.manystripclus53XFilterFlag = manystripclus53X.clone ( taggedMode = cms.untracked.bool(True),
                                                                           forcedValue = cms.untracked.bool(True) )
        self.process.toomanystripclus53XFilterFlag = toomanystripclus53X.clone ( taggedMode = cms.untracked.bool(True),
                                                                                 forcedValue = cms.untracked.bool(True) )
        self.process.logErrorTooManyTripletsPairsMainIterationsFilterFlag = logErrorTooManyTripletsPairsMainIterations.clone ( taggedMode = cms.untracked.bool(True),
                                                                                                                               forcedValue = cms.untracked.bool(True) )
        self.process.logErrorTooManySeedsMainIterationsFilterFlag = logErrorTooManySeedsMainIterations.clone ( taggedMode = cms.untracked.bool(True),
                                                                                                               forcedValue = cms.untracked.bool(True) )

        self.process.trkPOGFilters = cms.Sequence( self.process.logErrorTooManyClustersFilterFlag +
                                                   self.process.logErrorTooManyTripletsPairsFilterFlag +
                                                   self.process.logErrorTooManySeedsFilterFlag +
                                                   self.process.logErrorTooManySeedsDefaultFilterFlag +
                                                   self.process.manystripclus53XFilterFlag +
                                                   self.process.toomanystripclus53XFilterFlag +
                                                   self.process.logErrorTooManyTripletsPairsMainIterationsFilterFlag +
                                                   self.process.logErrorTooManySeedsMainIterationsFilterFlag
                                                   )

        #METMVA:
        self.process.load('JetMETCorrections.Configuration.JetCorrectionProducers_cff')
        self.process.load('JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_cff')

        self.process.pfMEtMVA.verbosity=0

        if self.options.isData:
            self.process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
        else:
            self.process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3")
            
        from SUSYBSMAnalysis.SusyCAF.SusyCAF_MET_cfi import met
        self.process.susydesymetMVA  = met('PF' , 'pfMEtMVA', 'MVA', 'DESYmet', calo=False)
        
        return ( self.patJet() + self.PileUpJetID() +
                 self.patLepton('Electron') + self.patLepton('Muon') +
                 self.evalSequence('susycaf%s',  ['photon']+(['tau','HPStau','pftau'] if self.options.taus else [])) +
                 self.evalSequence('susycafmet%s', ['AK5','AK5TypeII','AK5TypeI','PF','TypeIPFPat','TypeIPF','TC']) +
                 self.process.hcalLaser2012EventFilterFlag +
                 self.process.ecalDeadCellBEFilterFlag +
                 self.process.ecalLaserCorrFilterFlag +
                 self.process.trkPOGFilters +
                 self.evalSequence('susydesy%s', ['patelectrons','pfelectrons','patmuons','pfmuons','tau','trigger', 'track', 'tobtecfakesProducer']+
                                   ([] if self.options.isData else ['pu']))+
                 self.process.pfMEtMVAsequence + self.process.susydesymetMVA +
                 self.process.susydesyjets +
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
            selectors += helpers.applySelection(self.process, module, "pt > 10", patJetSelector)[0]
        return selectors + sum(modules,self.empty)    

    def allTracks(self) :
        if not self.options.AllTracks : return self.empty
        self.process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_AllTracks_cfi')
        return self.process.susycafalltracks

    def trackIsolation(self) :
        self.process.load("SUSYBSMAnalysis.DesySusy.trackIsolationMaker_cfi")
        return self.process.susydesytrackIsolationMaker

    def PileUpJetID(self) :
        from RecoJets.JetProducers.PileupJetIDParams_cfi import full_53x, full_5x, cutbased
        from RecoJets.JetProducers.PileupJetID_cfi import pileupJetIdProducer

        full_53x.tmvaWeights = cms.string("RecoJets/JetProducers/data/TMVAClassificationCategory_JetID_53X_Dec2012.weights.xml")
        full_53x.label = cms.string("full53x")
        full_5x.label = cms.string("full5x")

        JetCollection = "selectionsusycafak5pfjet" + ("Matched0" if not self.options.isData else "0")
        
        self.process.puJetId = pileupJetIdProducer.clone(
            produceJetIds = cms.bool(True),
            jetids = cms.InputTag(""),
            runMvas = cms.bool(False),
            jets = cms.InputTag( JetCollection),
            vertexes = cms.InputTag("offlinePrimaryVertices"),
            algos = cms.VPSet(cutbased)
            )
        
        self.process.puJetMva = pileupJetIdProducer.clone(
            produceJetIds = cms.bool(False),
            jetids = cms.InputTag("puJetId"),
            runMvas = cms.bool(True),
            jets = cms.InputTag( JetCollection),
            vertexes = cms.InputTag("offlinePrimaryVertices"),
            algos = cms.VPSet(full_53x, full_5x)
            )
        return (self.process.puJetId * self.process.puJetMva )
