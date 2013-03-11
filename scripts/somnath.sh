#!/bin/zsh

cmsRun susydesy_cfg.py isData=0 output=SusyCAF_Tree_mc.root triggers=1 GlobalTag=START53_V7G::All maxEvents=100 taus=0 jetCollections=ak5pf files=/store/relval/CMSSW_5_3_2_patch1-START53_V7A/RelValTTbar/GEN-SIM-RECO/v1/0000/64542608-69C7-E111-AFC8-002618943953.root doTauReco=1

nafJobSplitterCfgMaker.sh /ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM       START53_V7G::All  ZZJetsTo4L_cfg.py
nafJobSplitterCfgMaker.sh /WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM         START53_V7G::All  WZJetsTo3LNu_cfg.py
nafJobSplitterCfgMaker.sh /WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM    START53_V7G::All  WWJetsTo2L2Nu_cfg.py
nafJobSplitterCfgMaker.sh /ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM    START53_V7G::All  ZZJetsTo2L2Nu_cfg.py
nafJobSplitterCfgMaker.sh /ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM     START53_V7G::All  ZJetsTo2L2Q_cfg.py
nafJobSplitterCfgMaker.sh /WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM     START53_V7G::All  WZJetsTo2L2Q_cfg.py
nafJobSplitterCfgMaker.sh /WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM    START53_V7G::All  WGstarToLNu2E_cfg.py
nafJobSplitterCfgMaker.sh /WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM   START53_V7G::All  WGstarToLNu2Mu_cfg.py
nafJobSplitterCfgMaker.sh /WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM  START53_V7G::All  WGstarToLNu2Tau_cfg.py


nafJobSplitterCfgMaker.sh /ZZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM  START53_V7G::All  ZZZNoGstarJets_cfg.py
nafJobSplitterCfgMaker.sh /WWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM         START53_V7G::All  WWWJets_cfg.py
nafJobSplitterCfgMaker.sh /WWZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM  START53_V7G::All  WWZNoGstarJets_cfg.py
nafJobSplitterCfgMaker.sh /WZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM  START53_V7G::All  WZZNoGstarJets_cfg.py
nafJobSplitterCfgMaker.sh /WWGJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM         START53_V7G::All  WWGJets_cfg.py


nafJobSplitterCfgMaker.sh /TTZJets_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM   START53_V7G::All  TTZJets_cfg.py
nafJobSplitterCfgMaker.sh /TTWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM      START53_V7G::All  TTWJets_cfg.py
nafJobSplitterCfgMaker.sh /TTGJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM      START53_V7G::All  TTGJets_cfg.py
nafJobSplitterCfgMaker.sh /TTWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM     START53_V7G::All  TTWWJets_cfg.py
nafJobSplitterCfgMaker.sh /TBZToLL_4F_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM   START53_V7G::All  TBZToLL_cfg.py



nafJobSplitter.pl -J -q 12 74  ZZJetsTo4L_cfg.py
nafJobSplitter.pl -J -q 12 32  WZJetsTo3LNu_cfg.py
nafJobSplitter.pl -J -q 12 35  WWJetsTo2L2Nu_cfg.py
nafJobSplitter.pl -J -q 12 15  ZZJetsTo2L2Nu_cfg.py
nafJobSplitter.pl -J -q 12 31  ZJetsTo2L2Q_cfg.py
nafJobSplitter.pl -J -q 12 54  WZJetsTo2L2Q_cfg.py
nafJobSplitter.pl -J -q 12  5  WGstarToLNu2E_cfg.py
nafJobSplitter.pl -J -q 12  5  WGstarToLNu2Mu_cfg.py
nafJobSplitter.pl -J -q 12  1  WGstarToLNu2Tau_cfg.py


nafJobSplitter.pl -J -q 12 3  ZZZNoGstarJets_cfg.py
nafJobSplitter.pl -J -q 12 3  WWWJets_cfg.py
nafJobSplitter.pl -J -q 12 3  WWZNoGstarJets_cfg.py
nafJobSplitter.pl -J -q 12 3  WZZNoGstarJets_cfg.py
nafJobSplitter.pl -J -q 12 3  WWGJets_cfg.py


nafJobSplitter.pl -J -q 12 3  TTZJets_cfg.py
nafJobSplitter.pl -J -q 12 3  TTWJets_cfg.py
nafJobSplitter.pl -J -q 12 2  TTGJets_cfg.py
nafJobSplitter.pl -J -q 12 4  TTWWJets_cfg.py
nafJobSplitter.pl -J -q 12 3  TBZToLL_cfg.py



nohup nafJobSplitterMonitor.sh &
