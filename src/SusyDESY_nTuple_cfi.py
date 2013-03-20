import FWCore.ParameterSet.Config as cms

from  SUSYBSMAnalysis.SusyDESY.SusyDESY_Module_cfi import *

susyTree = cms.EDAnalyzer("SusyTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_susycaf*_*_*',
    'keep *_susydesy*_*_*'
    ))

nTupleSequenceDESY = cms.Sequence(susydesypatelectrons +
                                  susydesypfelectrons +
                                  susydesymuons
                                  )



