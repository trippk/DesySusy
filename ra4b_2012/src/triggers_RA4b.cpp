
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
							    
