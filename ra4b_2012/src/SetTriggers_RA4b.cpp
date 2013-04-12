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
  //vector<const char *>El_HighPtMenu;
  //  vector<const char *>El_LowPtMenu;


  triggernames.push_back("HLT_IsoMu24");
  triggernames.push_back("HLT_IsoMu24_eta2p1");
  
}
