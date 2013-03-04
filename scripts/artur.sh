#!/bin/zsh

cmsRun susydesy_cfg.py isData=0 output=SusyCAF_Tree_mc.root triggers=1 GlobalTag=START53_V7G::All maxEvents=100 taus=0 jetCollections=ak5pf files=/store/mc/Summer12/SMS-T2tt_mStop-500to650_mLSP-0to225_8TeV-Pythia6Z/AODSIM/START52_V9_FSIM-v1/00001/FA7501D4-E761-E211-911D-003048C692A2.root doTauReco=1 beamHaloVars=0 scan=T2tt

nafJobSplitterCfgMaker.sh /SMS-T2tt_mStop-500to650_mLSP-0to225_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/AODSIM START53_V7G::All SMS_T2tt_mStop-500to650_mLSP-0to225_cfg.py
nafJobSplitterCfgMaker.sh /SMS-T2tt_mStop-375to475_mLSP-0to375_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/AODSIM START53_V7G::All SMS_T2tt_mStop-375to475_mLSP-0to375_cfg.py


nafJobSplitter.pl -J -q 12 151 SMS_T2tt_mStop-500to650_mLSP-0to225_cfg.py
nafJobSplitter.pl -J -q 12 150 SMS_T2tt_mStop-375to475_mLSP-0to375_cfg.py

nohup nafJobSplitterMonitor.sh &
