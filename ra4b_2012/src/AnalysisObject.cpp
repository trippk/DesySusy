#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "AnalysisObject.h"
//#ifndef __CINT__
//  #include <boost/shared_ptr.hpp>
//  #include "typedefs.h"
//#else

using namespace std;
using namespace ROOT::Math::VectorUtil;



//There are several ways in which a LorentzM can be included in the class://
//
// 1) The class has a pointer to it, and the object lives somewhere else. This is
// the case with LorentzM objects that live in the TTree.
//
// 2) The class owns the object through a shared_ptr. When the last instance of 
//    AnalysisObject that holds the LorentzM goes out of scope, the LorentzM will
//    be deleted
//
// 3) The class has the object as one of the members
//
//
// for each of this options, there is a corresponding Set() method


//=======POINTER TO A LORENTZM WHICH IS OWNED BY THE OBJECT
AnalysisObject::AnalysisObject() :  shared_pp4(boost::shared_ptr<LorentzM>()), pp4(0), maptotree(-1), ownsP4(false) {}



AnalysisObject::AnalysisObject(const AnalysisObject& copy){
  p4 = copy.p4;
  pp4 = copy.pp4;
  shared_pp4=copy.shared_pp4;
  maptotree = copy.maptotree;
  id = copy.id;

}

AnalysisObject::~AnalysisObject(){}



//GET THE OBJECT
LorentzM AnalysisObject::P4() const {return p4;}

bool AnalysisObject::HasP4(){return hasP4;}
bool AnalysisObject::OwnsP4(){return ownsP4;}




LorentzM* AnalysisObject::GetPointerP4() const {
//======return a standard pointer to the lorentz vector

  if(!hasP4){
    if(ownsP4){
      return shared_pp4.get();
    }else{
      cout<<"this should not happen"<<endl;
      cout<<"hasP4 is false and ownsP4 is also false"<<endl;
      return pp4;
    }
  }else{
    return pp4;
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



void AnalysisObject::Set(const int maptotree_In, LorentzM* const momentum_In){
//=========SET WITH EXTERNAL POINTER,
//         the object is NOT owned by the AnalsysisObject instance
//         When the instance goes out of scope, the LorentzM will NOT
//         be removed

  ownsP4=false;
  hasP4=false;
  this->SetExternalPointer(maptotree_In,momentum_In);
}

void AnalysisObject::SetExternalPointer(const int maptotree_In, LorentzM* const momentum_In){
  //  cout<<"SetExternalPointer is deprecated, it should not be called"<<endl;
  shared_pp4.reset();
  //SET THE FOURVECTOR
  if (momentum_In) {
    pp4=momentum_In;
    p4=*pp4;
  }
  else {
    cout << "AnalysisObject::Set >> ERROR momentum_In is a NULL pointer 3!" << endl;
    pp4=0;
    p4.SetPxPyPzE(0.,0.,0.,0.);
  }

  //SET THE MAP TO THE TREE
  maptotree=maptotree_In;
  return;
}




void AnalysisObject::Set(const int maptotree_In,  Ptr_LorentzM momentum_In){
//=========OWNED POINTER, SET WITH shared_ptr<LorentzM>
//         When the last instance holding the LorentzM goes out of scope, 
//         the LorentzM will also go out of scope
  
  ownsP4=true;
  hasP4=false;
  if(momentum_In){
    shared_pp4=momentum_In;
    p4=*shared_pp4;
  }
  else {
    cout << "AnalysisObject::Set >> ERROR momentum_In is a NULL pointer 1!" << endl;
    shared_pp4.reset();
    p4.SetPxPyPzE(0.,0.,0.,0.);
  }
  maptotree=maptotree_In;
} 



void AnalysisObject::SetOwnedPointer(const int maptotree_In, LorentzM* const momentum_In){
  //=========THE OBJECT IS OWNED, BUT IT IS SET WITH A NORMAL POINTER
  ownsP4=true;
  hasP4=false;
  if(momentum_In){
    shared_pp4.reset(momentum_In);
    p4=*shared_pp4;
  }
  else {
    cout << "AnalysisObject::Set >> ERROR momentum_In is a NULL pointer 2!" << endl;
    shared_pp4.reset();
    p4.SetPxPyPzE(0.,0.,0.,0.);
  }
  maptotree=maptotree_In;
  return;
}
//==============================================================

  


void AnalysisObject::Set(const int maptotree_In, LorentzM momentum_In){
//=========OWNED POINTER, SET WITH LorentzM object
//         Copies the object into the class
//         there is NO shared pointer
//         only a normal pointer

  ownsP4=true;
  hasP4=true;
  this->SetP4(momentum_In);
  pp4=&p4;
  maptotree=maptotree_In;
}
void AnalysisObject::SetP4(LorentzM momentum_In){p4=momentum_In;}


void AnalysisObject::CopyLorentzM(const AnalysisObject* target){

  hasP4=target->hasP4;
  ownsP4=target->ownsP4;
  //copies the appropriate structure
  if(!hasP4){
    if(ownsP4){
      //create a new shared_ptr pointing to the same object
      shared_pp4.reset((target->shared_pp4).get());
      p4=*shared_pp4;
    }else{
      //create a new normal pointer pointing to the object
      pp4=target->pp4;
      p4=*pp4;
      shared_pp4.reset();
    }
  }else{
    //create a new object from the target
    p4=target->p4;
    pp4=&p4;
    shared_pp4.reset();
  }
}





//#endif

