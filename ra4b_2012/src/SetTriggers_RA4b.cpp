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

  if(mySampleInformation.GetSample()=="MuHad"){
    if(mySampleInformation.GetSubSample()=="Run2012A-PromptReco-v1"){
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
    }
    else if(mySampleInformation.GetSubSample()=="Run2012B-PromptReco-v1"){
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
    }
  }

  else if(mySampleInformation.GetSample()=="SingleMu"){
    if(mySampleInformation.GetSubSample()=="Run2012A-PromptReco-v1"){

    }
    else if(mySampleInformation.GetSubSample()=="Run2012B-PromptReco-v1"){
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v1");
      triggernames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v1");
    }
  }



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
    
  }else{
    cout <<"triggers HAVE NOT been set. Warning"<<endl;
  }
  
}
