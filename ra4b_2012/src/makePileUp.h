#ifndef makePileUp_h
#define makePileUp_h

#include "ConfigReader.h"
#include "SampleInformation.h"
#include "NtupleTools2.h"

ConfigReader config;
class mySampleInfo; //forward declaration



class pileUpInfo{
private:
  bool oldpuw;
  int nobinsmc;
  int nobinsdata;
  int nobinsdata_up;
  int nobinsdata_down;  

  double PUWeight;
  double PUWeight_up;
  double PUWeight_down;
  int relevantNumPU;


public:
  vector<double> PUmc;
  vector<double> PUdata;
  vector<double> PUdata_up;
  vector<double> PUdata_down;

  pileUpInfo(){
    nobinsmc=0;
    nobinsdata=0;
    nobinsdata_up=0;
    nobinsdata_down=0;
    oldpuw = false;
    PUWeight=1; 
    PUWeight_up=1;
    PUWeight_down=1;
    relevantNumPU=0;
  }
  
  int Initialize(SampleInformation& mySampleInfo){
    //PU
  
    ConfigReader config;
    string WhatSample=mySampleInfo.GetSample();
    string WhatSubSample=mySampleInfo.GetSubSample();

    if(!oldpuw){
      nobinsmc = config.getDouble("PU_"+WhatSample+"_"+WhatSubSample+"_mc",PUmc," ");
      nobinsdata = config.getDouble("PU_data",PUdata," ");
      nobinsdata = config.getDouble("PU_data_up",  PUdata_up  ," ");
      nobinsdata = config.getDouble("PU_data_down",PUdata_down," ");
    }
    else if(oldpuw){
      nobinsmc = config.getDouble("oldPU_"+WhatSample+"_"+WhatSubSample+"_mc",PUmc," ");
    } 

    if(nobinsmc!=nobinsdata && !oldpuw){
      cout << "problem in pu inf in para_config - number of bins in MC PU dist is different than data!" << endl;
      return -1;
    }
  
    return 0;
  }



  void RescaleWeight(EasyChain* tree, double& weight, string type=""){
  
    float PUnumInter    = tree->Get( PUnumInter, "pileupTrueNumInteractionsBX0");
    relevantNumPU = (int) PUnumInter;
    if( relevantNumPU >= nobinsmc ) {
      cout << "something wrong with the pile up info!!! - exceed max number of vertex:     " << nobinsmc <<endl;
      return; 
    }
  
    else if( oldpuw) {
      PUWeight= PUmc.at( relevantNumPU );
    }
    else {
      if (type=="" || type=="central"){
	PUWeight= PUdata.at( relevantNumPU )/PUmc.at( relevantNumPU );
      }else if (type=="up"){
	PUWeight_up= PUdata_up.at( relevantNumPU )/PUmc.at( relevantNumPU );
      }else if (type=="down"){
	PUWeight_down= PUdata_down.at( relevantNumPU )/PUmc.at( relevantNumPU );
      }else{
	cout<<"wrong pileup weight chice. ERROR"<<endl;
      }
    }
    weight *= PUWeight;
  }

  double GetWeight(string type=""){
      if (type=="" || type=="central"){
	if(PUWeight == 1.){
	  PUWeight= PUdata.at( relevantNumPU )/PUmc.at( relevantNumPU );
	  return PUWeight;
	}
      }else if (type=="up"){
	if(PUWeight_up==1.){
	  PUWeight_up= PUdata_up.at( relevantNumPU )/PUmc.at( relevantNumPU );
	  return PUWeight_up;
	}
      }else if (type=="down"){
	if(PUWeight_down==1.){
	  PUWeight_down= PUdata_down.at( relevantNumPU )/PUmc.at( relevantNumPU );
	}
	  return PUWeight_down;
      }else{
	cout<<"wrong pileup weight chice 2.ERROR"<<endl;
      }    
  }
  
};


#endif
