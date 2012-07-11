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
    //loose
    SetOfCuts::LooseElectrons.Examine=true;
    SetOfCuts::LooseElectrons.NUM.Set(-1,"above","no loose electrons");
    //tight
    SetOfCuts::TightElectrons.Examine=true;
    SetOfCuts::TightElectrons.NUM.Set(0,"equal","no tight electrons");
    //veto
    SetOfCuts::VetoElectrons.NUM.Set(0,"equal","no veto electrons");
    //============================================
    // MUONS
    //============================================
    SetOfCuts::LooseMuons.NUM.Set(-1,"above","any number of loose");
    SetOfCuts::TightMuons.NUM.Set(1,"equal","exactly one tight");
    SetOfCuts::SoftMuons.NUM.Set(-1,"above","any number of soft");
    SetOfCuts::VetoMuons.NUM.Set(0,"equal","no veto-muons");

    //============================================
    //JETS
    //============================================
    SetOfCuts::Jets.Examine=true;
    SetOfCuts::Jets.NUM.Set(3,"above","jets>3");

    //============================================
    //EVENT
    //============================================
    SetOfCuts::Event.NumberOfBtags.Set(-1,"above","any number of btags");
    SetOfCuts::Event.HT.Set(300.0,"above","HT above 300");
    SetOfCuts::Event.MET.Set(100.,"above","MET above 100");
    SetOfCuts::Event.TransMass.Set(-1,"above","Any Transverse Mass");
    //============================================

  }  
  else if(mySampleInformation.GetEstimation()=="RA4b-el" ){
    
    //============================================
    //ELECTRONS
    //============================================
    SetOfCuts::LooseElectrons.Examine=true;
    SetOfCuts::LooseElectrons.NUM.Set(-1,"above","one or more electrons");
    //
    SetOfCuts::TightElectrons.NUM.Set(1,"equal","exactly one tight");
    //
    SetOfCuts::VetoElectrons.NUM.Set(0,"equal","any number of veto electrons");

    //============================================
    // MUONS
    //============================================
    //cout<<"in the right place"<<endl;
    SetOfCuts::LooseMuons.Examine=true;
    SetOfCuts::LooseMuons.NUM.Set(-1,"above","at least one loose muon");
    SetOfCuts::TightMuons.NUM.Set(0,"equal","exactly one tight");
    SetOfCuts::VetoMuons.NUM.Set(0,"equal","no veto-muons");
    SetOfCuts::SoftMuons.NUM.Set(-1,"above","any number of soft");
    //============================================
    //JETS
    //============================================
    SetOfCuts::Jets.Examine=true;
    SetOfCuts::Jets.NUM.Set(3,"above","jets>3");
    //============================================
    //EVENT
    //============================================
    SetOfCuts::Event.NumberOfBtags.Set(-1,"above","any number of btags");
    SetOfCuts::Event.HT.Set(300.0,"above","HT above 300");
    SetOfCuts::Event.MET.Set(100.,"above","MET above 100");
    SetOfCuts::Event.TransMass.Set(-1,"above","Any Transverse Mass");
    //============================================

  }
  else if(mySampleInformation.GetEstimation()=="newRA4b-mu" ){
    
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
    SetOfCuts::Event.MET.Set(100.,"above","MET above 100");
    //============================================

  }  
  else if(mySampleInformation.GetEstimation()=="newRA4b-el" ){
    
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
}




//  int main(){
//    cout<<"hello!"<<endl;
//    static SetOfCuts mySetOfCuts;
//    string one="TTJets";
//    string two="nosub";
//    string three="RA4b";
//    //SetConditions_RA4b("TTJets","nosub","RA4b", &mySetOfCuts);
//    //  SetConditions_RA4b(one,two,three, &mySetOfCuts);
   
//    SampleInformation mySampleInformation;
//    mySampleInformation.ReadInformation("./src/informationtest.txt");
//    //print it
//    mySampleInformation.DumpInformation();
//    SetConditions_RA4b(mySampleInformation, &mySetOfCuts);
//    //SetOfCuts mySetOfCuts=SetConditions_RA4b(one,two,three);
//    SetOfCuts::myEventCuts.NumberOfBtags.PrintCondition();
//    SetOfCuts::myEventCuts.MET.PrintCondition();
// }









