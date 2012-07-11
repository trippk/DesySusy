#include "NtupleTools2_h.h"

using namespace std;

bool cschalo_RA4b(EasyChain* tree){
  
  // check event quality
  // calo noise filter
  //  bool Loose =  tree->Get(haloloose,"beamHaloCSCLooseHaloId");
  bool Tight =  tree->Get(Tight,"beamHaloCSCTightHaloId");
  
  return !Tight;
}

