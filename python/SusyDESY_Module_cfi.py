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
                                 inputTag     = cms.InputTag('susycaftriggers:triggered')
                                 )


susydesypu = cms.EDProducer("SusyDESY_PU",
                            PileUp     = cms.InputTag('susycafpileupsummary:pileupTrueNumInteractionsBX0')
                            )


