//=============================================
//TRIGGERS
//=============================================

#include "TSystem.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "THTools.h"
#include "SampleInformation.h"
#include <iostream>

using namespace std;
using namespace ROOT::Math::VectorUtil;

void SetTriggers_RA4b(SampleInformation mySampleInformation, vector<const char*>& triggernames){

  cout <<"inside set triggers"<<endl;

  static ConfigReader config;
  static bool isData=config.getBool("isData");
  vector<const char *>El_HighPtMenu;
  vector<const char *>El_LowPtMenu;
  El_HighPtMenu.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30");
  El_HighPtMenu.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30");
  El_HighPtMenu.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20");
  El_LowPtMenu.push_back("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45");

  if(mySampleInformation.GetEstimation()=="RA4b-mu"){
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
    
  }else if (mySampleInformation.GetEstimation()=="RA4b-mu-HighPt"){
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
  }else if (mySampleInformation.GetEstimation()=="RA4b-mu-4Jets"){
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
  }

  else if(mySampleInformation.GetEstimation()=="RA4b-el-HighPt"){
    cout<<"setting triggers allright"<<endl;
    for (int i=0; i<El_HighPtMenu.size(); ++i){
      triggernames.push_back(El_HighPtMenu.at(i));
    }

  }
  else if(mySampleInformation.GetEstimation()=="RA4b-el"){
    for (int i=0; i<El_HighPtMenu.size(); ++i){
      triggernames.push_back(El_HighPtMenu.at(i));
    }
  }
  else if (mySampleInformation.GetEstimation()=="RA4b-diMu"){
    triggernames.push_back("HLT_Mu17_Mu8");
  }
  else if (mySampleInformation.GetEstimation()=="RA4b-diMuSync"){
    triggernames.push_back("HLT_Mu17_Mu8");
    triggernames.push_back("HLT_Mu17_TkMu8");
  }
  else if (mySampleInformation.GetEstimation()=="RA4b-diElSync"){
    triggernames.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
  }
  else if (mySampleInformation.GetEstimation()=="RA4b-MuEGSync"){
    triggernames.push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
    triggernames.push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
    
  }else if (mySampleInformation.GetEstimation()=="TrigStudy-mu"){
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30");
    triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
  }
  else if (mySampleInformation.GetEstimation()=="TrigStudy-el"){
    for (int i=0; i<El_HighPtMenu.size(); ++i){
      triggernames.push_back(El_HighPtMenu.at(i));
    }
  }
  else {
    cout <<"triggers HAVE NOT been set. Warning"<<endl;
  }
  
}










  /*
  else if(mySampleInformation.GetSample()=="ElectronHad"){

    if(mySampleInformation.GetSubSample()=="Run2012A-PromptReco-v1" ){
      triggernames.push_back("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45");
    }
    else if(mySampleInformation.GetSubSample()=="Run2012B-PromptReco-v1"){
      triggernames.push_back("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45");
    }
    
    //TTJETS
  }else if(mySampleInformation.GetSample()=="TTJets"){
    //triggernames.push_back("HLT_PFHT350_PFMET100")
    if(mySampleInformation.GetEstimation()=="RA4b-mu"){
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
    }else if(mySampleInformation.GetEstimation()=="RA4b-el"){
      triggernames.push_back("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45");
      //      triggernames.push_back("HLT_Ele8_CaloIdT_TrkIdT_TriJet30");
    }

    //DYJETS
  }else if(mySampleInformation.GetSample()=="DYJets"){
    if(mySampleInformation.GetEstimation()=="RA4b-mu"){
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
    }else if(mySampleInformation.GetEstimation()=="RA4b-el"){
      triggernames.push_back("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45");
    }

    //WJetsToLNu
  }else if(mySampleInformation.GetSample()=="WJetsToLNu"){
    if(mySampleInformation.GetEstimation()=="RA4b-mu"){
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
    }else if(mySampleInformation.GetEstimation()=="RA4b-el"){
      triggernames.push_back("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45");
      //      triggernames.push_back("HLT_Ele8_CaloIdT_TrkIdT_TriJet30");
    }

  }else if(mySampleInformation.GetSample()=="SingleTop"){
    if(mySampleInformation.GetEstimation()=="RA4b-mu"){
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
    }else if(mySampleInformation.GetEstimation()=="RA4b-el"){
      triggernames.push_back("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45");
    }
  */

