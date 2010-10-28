import FWCore.ParameterSet.Config as cms

from  SUSYBSMAnalysis.DesySusy.SusyDESY_Module_cfi import *

susyTree = cms.EDAnalyzer("SusyTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_susycaf*_*_*',
    'keep *_susydesy*_*_*'
    ))

nTupleSequenceDESY = cms.Sequence(susydesypatelectrons +
                                  susydesypfelectrons +
                                  susydesypatmuons +
                                  susydesypfmuons
                                  )

def DESYpatch(process):
        process.nPat.replace(process.susycafelectron,process.susycafelectron+process.susydesypatelectrons)
        process.nPat.replace(process.susycafpfelectron,process.susycafpfelectron+process.susydesypfelectrons)
        process.nPat.replace(process.susycafmuon,process.susycafmuon+process.susydesypatmuons)
        process.nPat.replace(process.susycafpfmuon,process.susycafpfmuon+process.susydesypfmuons)

