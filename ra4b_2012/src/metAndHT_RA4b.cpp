#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include <vector>
#include "eventselection.h"
//check variables code is incomplete


bool metAndHT_RA4b(EasyChain* tree){

  ConfigReader config;

  static bool  quick       = config.getBool("quick");
  
  static CutSet metAndHTFlow("met and HT");
  //  metAndHTFlow.autoprint=true;
  metAndHTFlow.autodump=true;

  //if quick, let's get out fast
  //if(!quick)return false;

  //=======================================  
  //Getting The PF Collections
  LorentzM& PFmet = tree->Get(&PFmet, "metP4TypeIPF");
  LorentzM& TCmet = tree->Get(&TCmet, "metP4TC");

  //save the cut performance
  bool OK=SetOfCuts::Event.MET.Examine(PFmet.pt());
  metAndHTFlow.keepIf(SetOfCuts::Event.MET.name, OK);

  return  OK;

}
