#include <map>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
//#include "jets_RA4b.h"

using namespace std;

bool evtqual_RA4b(EasyChain* tree){

  bool hbheNoiseFilterResult =  tree->Get(hbheNoiseFilterResult,"hbheNoiseFilterResult");
  
  return hbheNoiseFilterResult;
}

