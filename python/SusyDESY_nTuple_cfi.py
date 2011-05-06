import FWCore.ParameterSet.Config as cms
#import SUSYBSMAnalysis.SusyCAF.SusyCAF_Drop_cfi

from  SUSYBSMAnalysis.SusyCAF.SusyCAF_Drop_cfi import *
from  SUSYBSMAnalysis.DesySusy.SusyDESY_Module_cfi import *

TsusyTree = cms.EDAnalyzer("SusyTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_susycaf*_*_*',
    'keep *_desysusy*_*_*',
    'keep double_susyScan*_*_*') + (
    ["drop %s"%s for s in drop(False)] +
    ["keep %s"%s for s in keep()]) +
    ["drop %s"%s for s in reduce()] )




nTupleSequenceDESY = cms.Sequence(susydesypatelectrons +
                                  susydesypfelectrons +
                                  susydesypatmuons +
                                  susydesypfmuons
                                  )

def DESYpatch(process):
        process.p_susyPat.replace(process.susycafelectron,process.susycafelectron+process.susydesypatelectrons)
        process.p_susyPat.replace(process.susycafpfelectron,process.susycafpfelectron+process.susydesypfelectrons)
        process.p_susyPat.replace(process.susycafmuon,process.susycafmuon+process.susydesypatmuons)
        process.p_susyPat.replace(process.susycafpfmuon,process.susycafpfmuon+process.susydesypfmuons)

	process.p_susyPat.remove(process.susycafic5calojet)
	process.p_susyPat.remove(process.susycafic5calojetMatched)
	process.p_susyPat.remove(process.susycafmetIC5)
	process.p_susyPat.remove(process.susycafphoton)
	
	process.p_susyCAF.replace(process.susyTree,process.TsusyTree)

	
def runTree(process) :
	process.load('SUSYBSMAnalysis.DesySusy.SusyDESY_RunTreeMaker_cfi')
	return cms.Path(process.runTree)
