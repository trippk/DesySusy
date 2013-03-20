import FWCore.ParameterSet.Config as cms

susydesypatelectrons = cms.EDProducer("SusyDESY_Electrons",
                                      Prefix       = cms.string('DESYelectron'),
                                      Suffix       = cms.string('Pat'),
                                      
                                      PatElectrons = cms.InputTag('cleanPatElectrons'),
                                      beamSpotInputTag = cms.InputTag('offlineBeamSpot'),
                                      conversionsInputTag = cms.InputTag('allConversions')
                                      )

susydesypfelectrons = cms.EDProducer("SusyDESY_Electrons",
                                     Prefix       = cms.string('DESYelectron'),
                                     Suffix       = cms.string('PF'),
                                     
                                     PatElectrons = cms.InputTag('patAllElectronsPF'),
                                     beamSpotInputTag = cms.InputTag('offlineBeamSpot'),
                                     conversionsInputTag = cms.InputTag('allConversions')
                                     )

susydesypatmuons = cms.EDProducer("SusyDESY_Muons",
                               Prefix       = cms.string('DESYmuon'),
                               Suffix       = cms.string('Pat'),
                                   
                               PatMuons     = cms.InputTag('cleanPatMuons'),
                               )

susydesypfmuons = cms.EDProducer("SusyDESY_Muons",
                               Prefix       = cms.string('DESYmuon'),
                               Suffix       = cms.string('PF'),
                                   
                               PatMuons     = cms.InputTag('patAllMuonsPF'),
                               )

susydesytrigger = cms.EDProducer("SusyDESY_Trigger",
                                 Prefix       = cms.string('DESYtrigger'),
                                 Suffix       = cms.string(''),
                                 inputTag     = cms.InputTag('susycaftriggers:triggered'),
                                 muons        = cms.InputTag("muonTriggerMatchHLTMuonsEmbedder"),
                                 electrons    = cms.InputTag("electronTriggerMatchHLTElectronsEmbedder")
                                 )

susydesypu = cms.EDProducer("SusyDESY_PU",
                            PileUp     = cms.InputTag('susycafpileupsummary:pileupTrueNumInteractionsBX0')
                            )

muonTriggerMatchHLTMuons = cms.EDProducer(
  "PATTriggerMatcherDRLessByR"
  , src     = cms.InputTag( 'cleanPatMuons' )
  , matched = cms.InputTag( 'patTrigger' )
  , matchedCuts = cms.string('filter("*") && (type( "TriggerMuon" ) || type( "TriggerL1Mu" ))')
  #, matchedCuts = cms.string('path("*")')
  #, matchedCuts = cms.string('path("HLT_IsoMu17_eta2p1_TriCentralPFJet30_v5",0)')
  #hltL1sMu16Eta2p1
  , maxDeltaR   = cms.double( 0.5 )
  , resolveAmbiguities    = cms.bool( True )
  , resolveByMatchQuality = cms.bool( True )
)

electronTriggerMatchHLTElectrons = muonTriggerMatchHLTMuons.clone()
electronTriggerMatchHLTElectrons.src = 'cleanPatElectrons'
electronTriggerMatchHLTElectrons.matchedCuts = 'filter("*") && (type("TriggerElectron") || type("TriggerL1IsoEG"))'

muonTriggerMatchHLTMuonsEmbedder = cms.EDProducer(
  "PATTriggerMatchMuonEmbedder",
  src = cms.InputTag("cleanPatMuons"),
  matches = cms.VInputTag("muonTriggerMatchHLTMuons")
)

electronTriggerMatchHLTElectronsEmbedder = cms.EDProducer(
  "PATTriggerMatchElectronEmbedder",
  src = cms.InputTag("cleanPatElectrons"),
  matches = cms.VInputTag("electronTriggerMatchHLTElectrons")
)









