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

susydesypatmuons = cms.EDProducer("SusyDESY_Muons",
                               Prefix       = cms.string('DESYmuon'),
                               Suffix       = cms.string('Pat'),
                                   
                               PatMuons     = cms.InputTag('cleanPatMuons'),

                               #any other parameter
                               )

susydesypfmuons = cms.EDProducer("SusyDESY_Muons",
                               Prefix       = cms.string('DESYmuon'),
                               Suffix       = cms.string('PF'),
                                   
                               PatMuons     = cms.InputTag('selectedPatMuonsPF'),

                               #any other parameter
                               )


susydesypuinfo = cms.EDProducer("SusyDESY_PileUpInfo",
                                Prefix       = cms.string('DESYPUinfo'),
                                Suffix       = cms.string(''),
                                PUinfo       = cms.InputTag('addPileupInfo'),
                                )

susydesytrigger = cms.EDProducer("SusyDESY_Trigger",
                                 Prefix       = cms.string('DESYtrigger'),
                                 Suffix       = cms.string(''),
                                 inputTag     = cms.InputTag('TriggerResults'),
                                 SourceName = cms.string(""),
                                 TriggerEventInputTag = cms.InputTag('hltTriggerSummaryAOD'),

                                 )

pdfWeights = cms.EDProducer("PdfWeightProducer",
                            # Fix POWHEG if buggy (this PDF set will also appear on output,
                            # so only two more PDF sets can be added in PdfSetNames if not "")
                            #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            GenTag = cms.untracked.InputTag("genParticles"),
                            #FixPOWHEG = cms.untracked.bool(False),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring(
                                                                "cteq66.LHgrid"
                                                                , "MRST2008nlo68cl.LHgrid"
                                                                , "NNPDF10_100.LHgrid"
                                                                )
                            )

susydesypdfweightscteq66 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('CTEQ66'),
                                          PDFset       = cms.InputTag('pdfWeights:cteq66'),
                                          )

susydesypdfweightsmstw = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('MSTW'),
                                          PDFset       = cms.InputTag('pdfWeights:MRST2008nlo68cl'),
                                          )

susydesypdfweightsnnpdf = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF10'),
                                          PDFset       = cms.InputTag('pdfWeights:NNPDF10'),
                                          )
