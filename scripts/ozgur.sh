#!/bin/zsh

cmsRun susydesy_cfg.py isData=0 output=SusyCAF_Tree_mc.root triggers=1 GlobalTag=START53_V7G::All maxEvents=100 taus=0 jetCollections=ak5pf files=/store/relval/CMSSW_5_3_2_patch1-START53_V7A/RelValTTbar/GEN-SIM-RECO/v1/0000/64542608-69C7-E111-AFC8-002618943953.root doTauReco=1

nafJobSplitterCfgMaker.sh /TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM START53_V7G::All TTJets_FullLept_cfg.py
nafJobSplitterCfgMaker.sh /W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM START53_V7G::All W4JetsToLNu_cfg.py

nafJobSplitter.pl -J -q 12 209 TTJets_FullLept_cfg.py
nafJobSplitter.pl -J -q 12 221 W4JetsToLNu_cfg.py

nohup nafJobSplitterMonitor.sh &
