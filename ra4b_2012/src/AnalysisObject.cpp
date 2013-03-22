#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "AnalysisObject.h"
#include <boost/shared_ptr.hpp>
#include "typedefs.h"
using namespace std;
using namespace ROOT::Math::VectorUtil;



//=======POINTER TO A LORENTZ M WHICH IS OWNED BY THE OBJECT
AnalysisObject::AnalysisObject() :  owned_pp4_original(boost::shared_ptr<LorentzM>()), pp4_original(0), maptotree(-1), ownsP4(false) {}



AnalysisObject::AnalysisObject(const AnalysisObject& copy){
  p4 = copy.p4;
  pp4_original = copy.pp4_original;
  owned_pp4_original=copy.owned_pp4_original;
  maptotree = copy.maptotree;
  id = copy.id;

}

AnalysisObject::~AnalysisObject(){}



//GET THE OBJECT
LorentzM AnalysisObject::P4() const {return p4;}


//==========THIS RETURNS A STANDARD POINTER
LorentzM* AnalysisObject::pOriginalP4() const {

  if(ownsP4){
    return owned_pp4_original.get();
  }else{
    cout<<"this should not happen"<<endl;
    return pp4_original;
  }

}


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



//=========SET WITH EXTERNAL POINTER
//         the object is NOT owned by the AnalsysisObject instance
void AnalysisObject::Set(const int maptotree_In, LorentzM* const momentum_In){
  //cout<<"this call to AnalysisObject::Set now deprecated, it should not happen"<<endl;
  this->SetExternalPointer(maptotree_In,momentum_In);
}

void AnalysisObject::SetExternalPointer(const int maptotree_In, LorentzM* const momentum_In){
  //  cout<<"SetExternalPointer is deprecated, it should not be called"<<endl;
  //SET THE FOURVECTOR
  if (momentum_In) {
    pp4_original=momentum_In;
    p4=*pp4_original;
    ownsP4=false;
  }
  else {
    cout << "AnalysisObject::Set >> ERROR momentum_In is a NULL pointer 3!" << endl;
    pp4_original=0;
    p4.SetPxPyPzE(0.,0.,0.,0.);
    ownsP4=false;
  }

  //SET THE MAP TO THE TREE
  maptotree=maptotree_In;
  return;
}




//=========OWNED POINTER, SET WITH shared_ptr<LorentzM>
void AnalysisObject::Set(const int maptotree_In,  Ptr_LorentzM momentum_In){
 
  if(momentum_In){
    owned_pp4_original=momentum_In;
    p4=*owned_pp4_original;
    ownsP4=true;
  }
  else {
    cout << "AnalysisObject::Set >> ERROR momentum_In is a NULL pointer 1!" << endl;
    owned_pp4_original.reset();
    p4.SetPxPyPzE(0.,0.,0.,0.);
    ownsP4=true;
  }
  maptotree=maptotree_In;
} 


//=========OWNED POINTER, SET WITH A NORMAL POINTER
void AnalysisObject::SetOwnedPointer(const int maptotree_In, LorentzM* const momentum_In){
  if(momentum_In){
    owned_pp4_original.reset(momentum_In);
    p4=*owned_pp4_original;
    ownsP4=true;
  }
  else {
    cout << "AnalysisObject::Set >> ERROR momentum_In is a NULL pointer 2!" << endl;
    owned_pp4_original.reset();
    p4.SetPxPyPzE(0.,0.,0.,0.);
    ownsP4=true;
  }
  maptotree=maptotree_In;
  return;
}
//==============================================================

  

