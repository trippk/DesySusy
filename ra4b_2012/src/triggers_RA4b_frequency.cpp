//THIS PROGRAM IS MEANT TO BE RUN AFTER THE FINAL SELECTION TO SEE 
//WHAT TRIGGERS CONTRIBUTE TO THE SELECTION
#include <map>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
//#include "muons_RA5.h"
//#include "vertices_RA5.h"


using namespace std;

extern bool pcp;

bool triggers_RA4b_frequency(EasyChain* tree, vector<const char*>&triggernames){

  bool OK=false;

  //  extern vector<string>triggernames;   
  //extern bool checkthisevent;
  static ConfigReader config;
  
  static CutSet TriggerFlowFrequency("trigger flow frequency");
  TriggerFlowFrequency.autoprint=true;
  TriggerFlowFrequency.autodump=true;
  


  //if(checkthisevent){
  if(pcp){
    cout<<"======================================"<<endl;
    cout<<"CHECKING THE TRIGGER ON THIS EVENT "<<endl;
  }
 

    
  map<string,int>& HLTprescaled = tree->Get(&HLTprescaled, "prescaled");
  map<string,bool>& HLTtrigger = tree->Get(&HLTtrigger, "triggered");

  bool atleastoneOK=false;


  for(map<string,bool>::iterator it2=HLTtrigger.begin(); it2 != HLTtrigger.end(); ++it2){
    
    TriggerFlowFrequency.keepIf(it2->first,it2->second && HLTprescaled[it2->first]==1);
  }

  //  for (unsigned itr=0;itr<triggernames.size(); ++itr){
//     OK=false;
//     if(HLTtrigger[triggernames.at(itr)] && HLTprescaled[triggernames.at(itr)]==1){
//       OK=true;
//       atleastoneOK=true;
//     }
//     TriggerFlowFrequency.keepIf(triggernames.at(itr),OK);


//     if(checkthisevent){
//       cout<<"triggers that are re-checked "<<triggernames.at(itr)<<" "<<HLTtrigger[triggernames.at(itr)]<<endl;
//     }
//   }

  /*  if(!atleastoneOK){
    cout<<"all the triggers should pass now. ERROR and STOP "<<endl;
    unsigned int Event   = tree->Get(Event,"event"); 
    cout<<"this happens for the event "<<Event;

    exit(1);
  }
  */
  return true;

}

//     // default trigger - in case that triggerlist is not in config file
//     config.getTString("triggerlist"," ");
//     // number of different triggers in list
//     int size = config.ListSize("triggerlist");

//     if(checkthisevent)cout<<"size of the triggerlist = "<<size<<endl;
//     if(size==0) return true; // no trigger check at all for empty (" ") triggerlist
  
//     // check if any trigger from triggerlist is available
//     if(checkthisevent)cout<<"mapping the string HLT"<<endl;
//     map<string,bool>& HLTtrigger = tree->Get(&HLTtrigger, "triggered");
  
//     if(checkthisevent)cout<<"mapping the string HLT prescaled"<<endl;
//     map<string,int>& HLTprescaled = tree->Get(&HLTprescaled, "prescaled");
//     //  map<string,int>& HLTprescaled; 


//     if(checkthisevent)cout<<"size of the HLTtrigger "<<HLTtrigger.size()<<endl;
//     if( HLTtrigger.size()==0 ) return false;
//     if(checkthisevent)cout<<"still here after the return"<<endl;
//     // loop over triggers from triggerlist until any found
//     OK=false;
//     for(int i=0;i<size;i++){
//       if(checkthisevent){
// 	cout<<"trigger "<<i<<" = "<<config.getString("triggerlist",i)<<endl;
//       }
//       //    if( HLTtrigger[ config.getString("triggerlist",i) ] )return true;
//       TriggerFlowFrequency.keepIf(  config.getString("triggerlist",i), HLTtrigger[ config.getString("triggerlist",i) ] && HLTprescaled[config.getString("triggerlist",i)]==1  );
//       if(checkthisevent)cout<<"what is going on with the prescales "<< config.getString("triggerlist",i)<<"=========>  "<<HLTprescaled[config.getString("triggerlist",i)]<<endl;
//       if( HLTtrigger[ config.getString("triggerlist",i) ] && HLTprescaled[config.getString("triggerlist",i)]==1 ){
// 	OK=true;
//       }
//     }


//     return OK;
//   }  


  
 
