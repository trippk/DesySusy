#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "simpleAnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;
ClassImp(simpleAnalysisObject);

simpleAnalysisObject::simpleAnalysisObject() : pp4(0), maptotree(-1) {}
simpleAnalysisObject::~simpleAnalysisObject(){}

simpleAnalysisObject::simpleAnalysisObject(const simpleAnalysisObject& copy){
  p4 = copy.p4;
  pp4 = &p4;
  maptotree = copy.maptotree;
  id = copy.id;
}



int simpleAnalysisObject::GetIndexInTree()const {return maptotree;}



LorentzM* simpleAnalysisObject::GetPointerP4() const {return pp4;}



double simpleAnalysisObject::Pt() const {return p4.Pt();}
double simpleAnalysisObject::pt() const {return p4.Pt();}
double simpleAnalysisObject::Eta() const {return p4.Eta();}
double simpleAnalysisObject::eta() const {return p4.Eta();}
double simpleAnalysisObject::Phi() const {return p4.Phi();}
double simpleAnalysisObject::phi() const {return p4.Phi();}

double simpleAnalysisObject::E() const {return p4.E();}
double simpleAnalysisObject::e() const {return p4.E();}
double simpleAnalysisObject::Px() const {return p4.Px();}
double simpleAnalysisObject::px() const {return p4.Px();}
double simpleAnalysisObject::Py() const {return p4.Py();}
double simpleAnalysisObject::py() const {return p4.Py();}
double simpleAnalysisObject::Pz() const {return p4.Pz();}
double simpleAnalysisObject::pz() const {return p4.Pz();}






bool simpleAnalysisObject::IsID(const string & key) const {
  std::map<std::string, bool>::const_iterator it = id.find(key);

  if (it == id.end() ) {
    //std::cout << "simpleAnalysisObject::IsID >> WARNING : Trying to get ID '" << key << "', which does not exist!" << std::endl;
    return false;
  }
  else {
    return it->second;
  }

}

void simpleAnalysisObject::SetID(const string & key, bool value){
  id[key]=value;
  return;
}


void simpleAnalysisObject::Set(const int maptotree_In, LorentzM momentum_In){
  this->SetP4(momentum_In);
  pp4=&p4;
  maptotree=maptotree_In;
}
void simpleAnalysisObject::SetP4(LorentzM momentum_In){p4=momentum_In;}
