#include "TSystem.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "THTools.h"
#include "eventselection.h"
#include "SampleInformation.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;


//WHAT ESTIMATION AND WHAT SAMPLE

void SetConditions_RA4b(SampleInformation mySampleInformation){
//SetOfCuts SetConditions_RA4b(string Sample,string SubSample,string WhatEstimation){

  ConfigReader config;
  //  SetOfCuts mySetOfCuts;


  if(mySampleInformation.GetEstimation()=="RA4b-mu" ){
    
    //============================================
    //ELECTRONS
    //============================================
    SetOfCuts::SignalElectrons.NUM.Set(0,"equal","no signal electron");
    SetOfCuts::WideElectrons.NUM.Set(0,"equal","no wider electrons");
    //============================================
    // MUONS
    //============================================
    SetOfCuts::SignalMuons.NUM.Set(1,"equal","one signal muon");
    SetOfCuts::WideMuons.NUM.Set(0,"equal","no wider muons");
    //============================================
    //JETS
    //============================================
    SetOfCuts::Jets.NUM.Set(3,"above","jets>3");
    //============================================
    //EVENT
    //============================================
    SetOfCuts::Event.NumberOfBtags.Set(-1,"above","any number of btags");
    SetOfCuts::Event.HT.Set(300.0,"above","HT above 300");
    SetOfCuts::Event.MET.Set(60.,"above","MET above 60");
    //============================================

  }  
  else if(mySampleInformation.GetEstimation()=="RA4b-el" ){
    
    //============================================
    //ELECTRONS
    //============================================
    SetOfCuts::SignalElectrons.NUM.Set(1,"equal","one signal electron");
    SetOfCuts::WideElectrons.NUM.Set(0,"equal","no wider electrons");
    //============================================
    // MUONS
    //============================================
    SetOfCuts::SignalMuons.NUM.Set(0,"equal","no muons");
    SetOfCuts::WideMuons.NUM.Set(0,"equal","no wider muons");
    //============================================
    //JETS
    //============================================
    SetOfCuts::Jets.NUM.Set(3,"above","jets>3");
    //============================================
    //EVENT
    //============================================
    SetOfCuts::Event.NumberOfBtags.Set(-1,"above","any number of btags");
    SetOfCuts::Event.HT.Set(300.0,"above","HT above 300");
    SetOfCuts::Event.MET.Set(100.,"above","MET above 100");
    //============================================


  }
  else if(mySampleInformation.GetEstimation()=="RA4b-mu-HighPt" ){
    
    //============================================
    //ELECTRONS
    //============================================
    SetOfCuts::SignalElectrons.NUM.Set(0,"equal","no signal electron");
    SetOfCuts::WideElectrons.NUM.Set(0,"equal","no wider electrons");
    //============================================
    // MUONS
    //============================================
    //
    SetOfCuts::SignalMuons.NUM.Set(1,"equal","one signal muon");
    SetOfCuts::WideMuons.NUM.Set(0,"equal","no wider muons");
    //SetOfCut
    //============================================
    //JETS
    //============================================
    SetOfCuts::Jets.NUM.Set(3,"above","jets>3");
    //============================================
    //EVENT
    //============================================
    SetOfCuts::Event.NumberOfBtags.Set(-1,"above","any number of btags");
    SetOfCuts::Event.HT.Set(300.0,"above","HT above 300");
    SetOfCuts::Event.MET.Set(60.,"above","MET above 100");
    //============================================

  }  
  else if(mySampleInformation.GetEstimation()=="RA4b-el-HighPt" ){
    
    //============================================
    //ELECTRONS
    //============================================
    SetOfCuts::SignalElectrons.NUM.Set(1,"equal","one signal electron");
    SetOfCuts::WideElectrons.NUM.Set(0,"equal","no wider electrons");
    //============================================
    // MUONS
    //============================================
    SetOfCuts::SignalMuons.NUM.Set(0,"equal","no muons");
    SetOfCuts::WideMuons.NUM.Set(0,"equal","no wider muons");
    //============================================
    //JETS
    //============================================
    SetOfCuts::Jets.NUM.Set(3,"above","jets>3");
    //============================================
    //EVENT
    //============================================
    SetOfCuts::Event.NumberOfBtags.Set(-1,"above","any number of btags");
    SetOfCuts::Event.HT.Set(300.0,"above","HT above 300");
    SetOfCuts::Event.MET.Set(60.,"above","MET above 100");
    //============================================


  }
}








