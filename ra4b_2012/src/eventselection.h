//this class encapsulates the requirements for one sample


#ifndef eventselection_h
#define eventselection_h


#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"


using namespace std;
using namespace ROOT::Math::VectorUtil;


  


template <class T>
class VarCondition{
private:


  T inf, sup;

  bool tbool;

public:

  string rel;
  T test;

  string name;
  VarCondition(){
    rel="norelation";
  }
  
  
  bool  (VarCondition::*SelectedCondition)(T);

  bool IsItAbove(T num){
    //    cout<<"I'm in this one with"<<num<<" and "<<test<<endl;
    return num > test;
  }
  bool IsItEqual(T num){
    return num == test;
  }
  bool IsItNot(T num){
    return num != test;
  }
  bool IsItBelow(T num){
    return num < test;
  }
  bool IsItBetween(T num){
    return num < sup && num > inf ;
  }
  bool IsItNotBetween(T num){
    return num > sup || num < inf ;
  }
  bool IsItOr(T num){
    return num == sup || num== inf;
  }
  bool IsItAnd(T num){
    return num == sup && num==inf;
  }
  T GiveMeCut(){
    return test;
  }
  string GiveMeRelation(){
    return rel;
  }
  void PrintCondition(){
    cout<<"the variable has to be "<<rel<<" "<<test<<" and its description is "<<name<<endl;
  }


  void Set(T testinteger,string relation,string nom){
    test=testinteger;
    rel=relation;
    name=nom;
    
    if(rel=="above"){
      SelectedCondition=&VarCondition::IsItAbove;
    }else if(rel=="equal"){
      SelectedCondition=&VarCondition::IsItEqual;
    }else if(rel=="below"){
      SelectedCondition=&VarCondition::IsItBelow;
    }else if(rel=="not"){
      SelectedCondition=&VarCondition::IsItNot;
    }else{
      SelectedCondition=NULL;
    }
  }



  void Set(T liminf,T limsup, string relation,string nom){
    inf=liminf;
    sup=limsup;
    rel=relation;
    name=nom;

    if(rel=="between"){
      SelectedCondition=&VarCondition::IsItBetween;
    }else if(rel=="not between"){
      SelectedCondition=&VarCondition::IsItNotBetween;
    }else if(rel=="or"){
      SelectedCondition=&VarCondition::IsItOr;
    }else if(rel=="and"){
      SelectedCondition=&VarCondition::IsItAnd;

    }else{
      SelectedCondition=NULL;
    }
  }




  bool Examine(T num){
    return (this->*SelectedCondition)(num);
  }

};




struct JetCuts{
  bool Examine;
  VarCondition<int>   NUM;
  VarCondition<float> Pt;
  VarCondition<float> Eta;
  VarCondition<float> IsoForCleaning;
};
//
struct EventCuts{
  VarCondition<int>   NumberOfBtags;
  VarCondition<float> HT;
  VarCondition<float> MET;
  VarCondition<float> TransMass;
};
//
struct MuonCuts{
  bool Examine;
  VarCondition<int>   NUM;
  VarCondition<float> Pt;
  VarCondition<float> Eta;
  VarCondition<float> Iso;
  VarCondition<float> PtLeading;
};
//
struct ElectronCuts{
  bool Examine;
  VarCondition<int>   NUM;
  VarCondition<float> Pt;
  VarCondition<float> Eta;
  VarCondition<float> Iso;
  VarCondition<float> PtLeading;
  VarCondition<double> Dist2Muons;
};
//
struct LeptonCuts{
  bool Examine;
  VarCondition<int>      NUM;
  VarCondition<float>    Pt;
  VarCondition<double>   Iso;
  VarCondition<float>    PtLeading;
};

struct LeptonPairCuts{
  bool Examine;
  VarCondition<int> NUM;
  VarCondition<double> InvMass;
  VarCondition<string> Flavor;
  VarCondition<bool> Charge;

};


//
class SetOfCuts{
public:

  static LeptonCuts   Leptons;
  static ElectronCuts TightElectrons;
  static ElectronCuts LooseElectrons;
  static ElectronCuts VetoElectrons;
  static ElectronCuts SignalElectrons;
  static ElectronCuts WideElectrons;
  static MuonCuts     LooseMuons;
  static MuonCuts     TightMuons;
  static MuonCuts     VetoMuons;
  static MuonCuts     SoftMuons;
  static MuonCuts     SignalMuons;
  static MuonCuts     WideMuons;
  static EventCuts    Event;
  static JetCuts      Jets;
  static LeptonPairCuts LeptonPairs;

};




#endif
