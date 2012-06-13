import FWCore.ParameterSet.Config as cms

filterResultOneLepton = cms.EDProducer('SusyDESY_FilterResultProducerOneLepton',
                                       ## object collection with 3 strings:
                                       ## name of producer, prefix of variables, suffix of variables
                                       ## for each object a standard and an alternative collection can be specified.
                                       
                                       StdMuCollection    = cms.vstring("susycafmuon"      ,    "muon"    ,"Pat"),
                                       AltMuCollection    = cms.vstring("susydesymuon"     ,"DESYmuon"    ,"Pat"),
                                       StdElCollection    = cms.vstring("susycafelectron"  ,    "electron","Pat"),
                                       AltElCollection    = cms.vstring("susydesyelectron" ,"DESYelectron","Pat"),
                                       
                                       StdMuCollectionPF  = cms.vstring("susycafpfmuon"    ,    "muon"    ,"PF" ),
                                       AltMuCollectionPF  = cms.vstring("susydesymuon"     ,"DESYmuon"    ,"PF" ),
                                       StdElCollectionPF  = cms.vstring("susycafpfelectron",    "electron","PF" ),
                                       AltElCollectionPF  = cms.vstring("susydesyelectron" ,"DESYelectron","PF" ),

                                       Prescale           = cms.int32(10)
                                       )


