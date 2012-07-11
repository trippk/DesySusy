#include "NtupleTools2_h.h"

using namespace std;

bool trackingFailure_RA4b(EasyChain* tree){
  
  bool trackingfailurefilterflag=tree->Get(trackingfailurefilterflag,"trackingFailureFilterFlag");

  return trackingfailurefilterflag;
}

