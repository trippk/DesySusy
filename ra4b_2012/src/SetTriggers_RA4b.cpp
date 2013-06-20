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
  string triggermenu="nomenu";


  if(mySampleInformation.GetEstimation()=="CR1" || mySampleInformation.GetEstimation()=="CR1Muons" ||  mySampleInformation.GetEstimation()=="CRFMuons" ||
     mySampleInformation.GetEstimation()=="CR1Muons-baseline"){
    triggermenu="singlemuon";
  }
  else if(mySampleInformation.GetEstimation()=="CR1Electrons" || mySampleInformation.GetEstimation()=="CRFElectrons" ){
    triggermenu="singleelectron";
  }
  //
  //
  //
  if (triggermenu=="singlemuon"){
    triggernames.push_back("HLT_IsoMu24");
    triggernames.push_back("HLT_IsoMu24_eta2p1");
  }else if(triggermenu=="singleelectron"){
    triggernames.push_back("HLT_Ele27_WP80");    
  }
  else{
    cout<<"NO TRIGGERS SET"<<endl;
  }
}
