import FWCore.ParameterSet.Config as cms

susydesypatelectrons = cms.EDProducer("SusyDESY_Electrons",
                                      Prefix       = cms.string('DESYelectron'),
                                      Suffix       = cms.string('Pat'),
                                      
                                      PatElectrons = cms.InputTag('cleanPatElectrons'),
                                      #any other parameter
                                      )

susydesypfelectrons = cms.EDProducer("SusyDESY_Electrons",
                                     Prefix       = cms.string('DESYelectron'),
                                     Suffix       = cms.string('PF'),
                                     
                                     PatElectrons = cms.InputTag('selectedPatElectronsPF'),
                                     #any other parameter
                                     )

susydesymuons = cms.EDProducer("SusyDESY_Muons",
                               Prefix       = cms.string('muon'),
                               Suffix       = cms.string('DESY'),
                                   
                               PatMuons     = cms.InputTag('cleanPatMuons'),

                               #any other parameter
                               )
