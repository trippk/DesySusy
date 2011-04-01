import FWCore.ParameterSet.Config as cms

runTree = cms.EDAnalyzer("DesySusy_RunTreeMaker",
                         InputTag = cms.InputTag("runProducer")
)

