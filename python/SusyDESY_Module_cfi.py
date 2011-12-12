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
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            ##  GenTag = cms.untracked.InputTag("prunedGenParticles"),#"genParticles"),
                            #FixPOWHEG = cms.untracked.bool(True),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring(
                                        ##"cteq66.LHgrid",
                                                                 "MSTW2008nlo68cl.LHgrid",
                                                                 "NNPDF20_100.LHgrid"
                                                                )
                            )
pdfWeightsAlphaCTEQ = cms.EDProducer("PdfWeightProducer",
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring("cteq66alphas.LHgrid"))

pdfWeightsAlphaMSTW = cms.EDProducer("PdfWeightProducer",
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring("MSTW2008nlo68cl_asmz+68cl.LHgrid" ,
                                                                "MSTW2008nlo68cl_asmz-68cl.LHgrid" ))

pdfWeightsAlphaNNPDF1 = cms.EDProducer("PdfWeightProducer",
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring("NNPDF20_as_0114_100.LHgrid" ,
                                                                "NNPDF20_as_0115_100.LHgrid" ))

pdfWeightsAlphaNNPDF2 = cms.EDProducer("PdfWeightProducer",
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring("NNPDF20_as_0116_100.LHgrid" ,
                                                                "NNPDF20_as_0117_100.LHgrid" ))

pdfWeightsAlphaNNPDF3 = cms.EDProducer("PdfWeightProducer",
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring("NNPDF20_as_0118_100.LHgrid" ,
                                                                "NNPDF20_as_0120_100.LHgrid" ))

pdfWeightsAlphaNNPDF4 = cms.EDProducer("PdfWeightProducer",
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring("NNPDF20_as_0121_100.LHgrid" ,
                                                                "NNPDF20_as_0122_100.LHgrid" ))

pdfWeightsAlphaNNPDF5 = cms.EDProducer("PdfWeightProducer",
                            FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                            PdfInfoTag = cms.untracked.InputTag("generator"),
                            PdfSetNames = cms.untracked.vstring("NNPDF20_as_0123_100.LHgrid" ,
                                                                "NNPDF20_as_0124_100.LHgrid" ))

susydesypdfweightscteq66 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('CTEQ66'),
                                          PDFset       = cms.InputTag('pdfWeights:cteq66'),
                                          )

susydesypdfweightsmstw = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('MSTW'),
                                          PDFset       = cms.InputTag('pdfWeights:MSTW2008nlo68cl'),
                                          )

susydesypdfweightsnnpdf = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20'),
                                          PDFset       = cms.InputTag('pdfWeights:NNPDF20100'),
                                          )


susydesypdfweightsalphacteq66 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('CTEQ66alphas'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaCTEQ:cteq66alphas'),
                                          )

susydesypdfweightsalphamstwp = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('MSTWalphaP'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaMSTW:MSTW2008nlo68clasmz+68cl'),
                                          )
susydesypdfweightsalphamstwm = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('MSTWalphaM'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaMSTW:MSTW2008nlo68clasmz-68cl'),
                                          )

susydesypdfweightsalphannpdf114 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha114'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF1:NNPDF20as0114100'),
                                         )
susydesypdfweightsalphannpdf115 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha115'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF1:NNPDF20as0115100'),
                                         )
susydesypdfweightsalphannpdf116 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha116'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF2:NNPDF20as0116100'),
                                         )
susydesypdfweightsalphannpdf117 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha117'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF2:NNPDF20as0117100'),
                                         )
susydesypdfweightsalphannpdf118 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha118'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF3:NNPDF20as0118100'),
                                         )
susydesypdfweightsalphannpdf120 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha120'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF3:NNPDF20as0120100'),
                                         )
susydesypdfweightsalphannpdf121 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha121'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF4:NNPDF20as0121100'),
                                         )
susydesypdfweightsalphannpdf122 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha122'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF4:NNPDF20as0122100'),
                                         )
susydesypdfweightsalphannpdf123 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha123'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF5:NNPDF20as0123100'),
                                         )
susydesypdfweightsalphannpdf124 = cms.EDProducer("SusyDESY_PDFweights",
                                          Prefix       = cms.string('DESYpdfWeights'),
                                          Suffix       = cms.string('NNPDF20alpha124'),
                                          PDFset       = cms.InputTag('pdfWeightsAlphaNNPDF5:NNPDF20as0124100'),
                                         )
