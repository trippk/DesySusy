import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.patTemplate_cfg import process
process.setName_("SUSYCAF")
from SUSYBSMAnalysis.SusyCAF.SusyCAF_options_cff import options
options = options()

process.maxEvents.input = options.maxEvents
process.GlobalTag.globaltag = options.GlobalTag
process.source = cms.Source('PoolSource', fileNames = cms.untracked.vstring(options.files) )
process.add_( cms.Service( "TFileService", fileName = cms.string( options.output ), closeFileFast = cms.untracked.bool(True) ) )

from SUSYBSMAnalysis.SusyCAF.SusyCAF_nTuple_cfi import SusyCAF
import SUSYBSMAnalysis.SusyCAF.SusyCAF_ProcessAdjustments_cfi as adjust
adjust.messageLogger(process,options.quiet)
adjust.loadAndConfigureHcalSeverityLevelProducer(process, options.isData)
adjust.loadAndConfigureEcalSeverityLevelProducer(process)

def runTree(process) :
	process.load('SUSYBSMAnalysis.DesySusy.SusyDESY_RunTreeMaker_cfi')
	return cms.Path(process.runTree)

process.p_susyPat  = adjust.susyPat(process,options)
process.p_hbheFlag = adjust.addHbheNoiseFilterResult(process,options)
process.p_ecalFlag = adjust.addEcalDeadCellFlag(process,options)
process.p_trackFlag= adjust.addTrackingFailureFlag(process,options)
process.p_lumi     = adjust.lumiTree(process)
process.p_run      = runTree(process)
process.p_susyCAF  = SusyCAF(process,options).path()

####
##import GeneratorInterface.GenFilters.TotalKinemticsFilter
#from SUSYBSMAnalysis.DesySusy.SusyDESY_Module_cfi import totalKinematicsFilter
#process.totalkinematicsfilterflag = totalKinematicsFilter.clone(verbose=True)
#    return cms.Path(process.ecaldeadcellfilterflag)



####

schedule = cms.Schedule( process.p_susyPat,
                         process.p_hbheFlag,
                         process.p_ecalFlag,
                         process.p_trackFlag,
                         #cms.Path(process.totalkinematicsfilterflag),
                         process.p_run,
                         process.p_lumi,
                         process.p_susyCAF )

# write this config as a single file
file = open(options.output.replace('.root','_cfg.py'),'w')
file.write(str(process.dumpPython()))
file.close()
