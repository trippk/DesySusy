#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "AnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

LorentzM AnalysisObject::P4(){return p4;}
LorentzM* AnalysisObject::pP4(){return pp4;}

double AnalysisObject::Pt() {return pp4->Pt();}
double AnalysisObject::pt() {return pp4->Pt();}
double AnalysisObject::Eta(){return pp4->Eta();}
double AnalysisObject::eta(){return pp4->Eta();}
double AnalysisObject::Phi(){return pp4->Phi();}
double AnalysisObject::phi(){return pp4->Phi();}

double AnalysisObject::E() {return pp4->E();}
double AnalysisObject::e() {return pp4->E();}
double AnalysisObject::Px(){return pp4->Px();}
double AnalysisObject::px(){return pp4->Px();}
double AnalysisObject::Py(){return pp4->Py();}
double AnalysisObject::py(){return pp4->Py();}
double AnalysisObject::Pz(){return pp4->Pz();}
double AnalysisObject::pz(){return pp4->Pz();}

bool AnalysisObject::IsID(string key){

  return id[key];
}

void AnalysisObject::SetID(string key, bool value){
  id[key]=value;
  if( find(allIDs.begin(),allIDs.end(),key) == allIDs.end() ) allIDs.push_back(key);
}

int AnalysisObject::GetIndexInTree(){return maptotree;}

void AnalysisObject::Set(int maptotree_In, LorentzM* momuntum_In){

  //SET THE FOURVECTOR
  pp4=momuntum_In;
  p4=*pp4;

  //SET THE MAP TO THE TREE
  maptotree=maptotree_In;

}



//DEFINE THE COPY CONSTRUCTOR
AnalysisObject::AnalysisObject(const AnalysisObject& copy){


  pp4=copy.pp4;
  p4 = *pp4;
  //otherwise, it would be pp4 = copy.pp4, and that's not
  //what it should be.
  
  maptotree=copy.maptotree;
  vector<TString> allIDS(copy.allIDs);
  map<TString, bool> id(copy.id);
  


}

