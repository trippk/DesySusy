#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "AnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

AnalysisObject::AnalysisObject() : pp4_original(0), maptotree(-1) {

}
AnalysisObject::AnalysisObject(const AnalysisObject& copy){
  p4 = copy.p4;
  pp4_original = copy.pp4_original;
  maptotree = copy.maptotree;
  id = copy.id;
}
AnalysisObject::~AnalysisObject() {

}

LorentzM AnalysisObject::P4() const {return p4;}
LorentzM* AnalysisObject::pOriginalP4() const {return pp4_original;}

double AnalysisObject::Pt() const {return p4.Pt();}
double AnalysisObject::pt() const {return p4.Pt();}
double AnalysisObject::Eta() const {return p4.Eta();}
double AnalysisObject::eta() const {return p4.Eta();}
double AnalysisObject::Phi() const {return p4.Phi();}
double AnalysisObject::phi() const {return p4.Phi();}

double AnalysisObject::E() const {return p4.E();}
double AnalysisObject::e() const {return p4.E();}
double AnalysisObject::Px() const {return p4.Px();}
double AnalysisObject::px() const {return p4.Px();}
double AnalysisObject::Py() const {return p4.Py();}
double AnalysisObject::py() const {return p4.Py();}
double AnalysisObject::Pz() const {return p4.Pz();}
double AnalysisObject::pz() const {return p4.Pz();}

int AnalysisObject::GetIndexInTree()const {return maptotree;}

bool AnalysisObject::IsID(const string & key) const {
  std::map<std::string, bool>::const_iterator it = id.find(key);

  if (it == id.end() ) {
    //std::cout << "AnalysisObject::IsID >> WARNING : Trying to get ID '" << key << "', which does not exist!" << std::endl;
    return false;
  }
  else {
    return it->second;
  }

}

void AnalysisObject::SetID(const string & key, bool value){
  id[key]=value;
  return;
}

void AnalysisObject::Set(int maptotree_In, LorentzM* momuntum_In){

  //SET THE FOURVECTOR
  if (momuntum_In) {
    pp4_original=momuntum_In;
    p4=*pp4_original;
  }
  else {
    cout << "AnalysisObject::Set >> ERROR momuntum_In is a NULL pointer!" << endl;
    pp4_original = 0;
    p4.SetPxPyPzE(0.,0.,0.,0.);
  }

  //SET THE MAP TO THE TREE
  maptotree=maptotree_In;

  return;
}




