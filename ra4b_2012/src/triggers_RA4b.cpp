
#include <map>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
//#include "muons_RA4b.h"
//#include "vertices_RA4b.h"


using namespace std;




bool triggers_RA4b(EasyChain* tree, vector<const char*>& triggernames, double& EventWeight){

  //extern vector<string>triggernames_short;
  extern bool pcp;
  //  extern double EventWeight;


  static ConfigReader config;
  
  static CutSet TriggerFlow("trigger flow");
  //  static string lastsuccesful;

  TriggerFlow.autoprint=false;
  TriggerFlow.autodump=true;

  bool OK=false;

  if( pcp){
    cout<<"======================================"<<endl;
    cout<<"CHECKING THE TRIGGER ON THIS EVENT "<<endl;
  }


    
  if(pcp)cout<<"going to get the prescaled map from the tree" <<endl;
  map<string,int>& HLTprescaled = tree->Get(&HLTprescaled, "prescaled");
  if(pcp)cout<<"going to get the trigger map from the tree" <<endl;
  map<string,bool>&   HLTtrigger = tree->Get(&HLTtrigger, "triggered");
  map<string,string>& TriggerMap = tree->Get(&TriggerMap, "DESYtriggerNameMap");

  if(pcp)cout<<"got the triggers from the tree!" <<endl;


  OK=false;



//   for (map<string,string>::iterator it = TriggerMap.begin(); it != TriggerMap.end(); ++it){
//     cout<<"the trigger map key is "<<it->first;
//     cout<<"the trigger map item is "<<it->second;
//   }

//   if(triggernames.empty()){
//     cout<<"there are no triggers to check. ERROR"<<endl;
//   }


  string dummytrigger;  
  string tname;

  for (unsigned itr=0;itr<triggernames.size(); ++itr){
    if(pcp)cout<<"checking if the trigger "<< triggernames.at(itr)<<endl;


    //tname=triggernames.at(itr);
    tname=TriggerMap[triggernames.at(itr)];
    //cout<<"the fucking trigger is"<<tname<<endl;
    //cout<<"which is associated to "<<triggernames.at(itr)<<endl;
    //check the trigger prescale, now it works with
    //only one trigger
    //if(TriggerFlow.keepIf(tname, HLTtrigger[tname])){
    //triggers_prescale->Fill(HLTprescaled[tname]);
    //}
    //
    if(TriggerFlow.keepIf(tname, HLTtrigger[tname] && HLTprescaled[tname]==1 )){
	//if(HLTprescaled[triggernames.at(itr)] > 1.)cout <<"the prescale of the trigger "<<HLTprescaled[triggernames.at(itr)]<<endl;
	
	//EventWeight=EventWeight*HLTprescaled[triggernames.at(itr)];
	
	OK=true;
      //      lastsuccesful=triggernames.at(itr);
	if(pcp)cout<<"the trigger that fires is "<<dummytrigger<<endl;
	break;
    }
  }

  return OK;

}
							    
bool triggersFired_RA4b(EasyChain* tree, std::map<std::string, bool>& firedTriggers) {

  extern bool pcp;

  if( pcp){
    cout<<"======================================"<<endl;
    cout<<"CHECKING THE TRIGGER ON THIS EVENT "<<endl;
  }
    
  if(pcp)cout<<"going to get the prescaled map from the tree" <<endl;
  map<string,int>& HLTprescaled = tree->Get(&HLTprescaled, "prescaled");
  if(pcp)cout<<"going to get the trigger map from the tree" <<endl;
  map<string,bool>&   HLTtrigger = tree->Get(&HLTtrigger, "triggered");
  map<string,string>& TriggerMap = tree->Get(&TriggerMap, "DESYtriggerNameMap");

  if(pcp)cout<<"got the triggers from the tree!" <<endl;

  bool atLeastOneTrigFired = false;

  std::map<std::string,bool>::iterator trigIt;

  for (trigIt =firedTriggers.begin();trigIt != firedTriggers.end(); trigIt++){
    if(pcp)cout<<"checking if the trigger "<< trigIt->first <<endl;

    std::string tname=TriggerMap[trigIt->first];
    if (HLTtrigger[tname] && HLTprescaled[tname]==1) {
      trigIt->second=true;
      atLeastOneTrigFired = true;
    }

  }

  return atLeastOneTrigFired;
}

bool isMuEGPlateau(const std::map<std::string, bool>& trigsFiredMap, double MuPt, double ElPt) {

  std::map<std::string, bool>::const_iterator trigIt;
  for (trigIt = trigsFiredMap.begin() ; trigIt != trigsFiredMap.end() ; trigIt++) {
    if (trigIt->second) {
      //Trig fired.
      //Apply thresholds.
      if (trigIt->first.find("Mu17_Ele8") != std::string::npos) {
	if ((MuPt > 20.) && (ElPt > 10.)) return true;
      }
      else if (trigIt->first.find("Mu8_Ele17") != std::string::npos) {
	if ((MuPt > 10.) && (ElPt > 20.)) return true;
      }
      else {
	std::cout << "isMuEGPlateau >> ERROR! Unknown Thresholds for trigger: " << trigIt->first << std::endl;
      }
    }
  }

  //If here, no trigger was found that fired and satisfied thresholds.
  return false;
}
