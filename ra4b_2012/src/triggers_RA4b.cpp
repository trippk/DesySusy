#include "triggers_RA4b.h"
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
							    
bool triggerFired(EasyChain* tree, const std::string & triggername) {

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

  if(pcp)cout<<"checking if the trigger "<< triggername <<endl;

  std::string tname=TriggerMap[triggername];
  bool trigFired = false;
  if (HLTtrigger[tname] && HLTprescaled[tname]==1) trigFired = true;
  
  return trigFired;
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

void getDiLepTriggers(EasyChain* tree, std::map<std::string,bool>& triggerMap, std::set<diLepEvent_t>& firedDiLepTypes ) {

  //Create MAP from event-type (or dataset) to corresponding triggers
  std::map<diLepEvent_t, std::vector<std::string> > typeToTrigMap;
  typeToTrigMap[MUMU].push_back("HLT_Mu17_Mu8");
  typeToTrigMap[MUMU].push_back("HLT_Mu17_TkMu8");

  typeToTrigMap[MUEL].push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
  typeToTrigMap[MUEL].push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");

  typeToTrigMap[ELEL].push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");

  //Create map of all trigger names to bools
  //Create the set of diLepTrigTypes that were fired.
  triggerMap.clear();
  firedDiLepTypes.clear();
  std::map<diLepEvent_t, std::vector<std::string> >::const_iterator mapIt;
  for (mapIt = typeToTrigMap.begin() ; mapIt != typeToTrigMap.end() ; mapIt++ ) {
    for (int iTrig = 0 ; iTrig < mapIt->second.size() ; iTrig++ ) {
      bool trigWasFired = triggerFired(tree, mapIt->second.at(iTrig) );
      triggerMap.insert( make_pair(mapIt->second.at(iTrig),trigWasFired) );
      if (trigWasFired) firedDiLepTypes.insert(mapIt->first);
    }
  }

  return;
}

diLepEvent_t getDiLepTrigType(const std::string& triggername) {

    std::size_t muLoc = triggername.find("Mu");
    std::size_t eleLoc = triggername.find("Ele");
    diLepEvent_t diLepEventType; 
    if (muLoc != std::string::npos ) {
      if (eleLoc != std::string::npos) diLepEventType = MUEL;
      else diLepEventType = MUMU;
    }
    else if (eleLoc != std::string::npos) diLepEventType = ELEL;
    else diLepEventType = FAIL;

    return diLepEventType;
}
