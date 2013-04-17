#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "genJet.h"
#include "Jet.h"
//#ifndef __CINT__
//  #include <boost/shared_ptr.hpp>
//  #include "typedefs.h"
//#else


using namespace std;
using namespace ROOT::Math::VectorUtil;


int    GenJet::GenFlavor()      {return genFlavor;};
bool   GenJet::IsMatch()        {return isMatch;};
string GenJet::Type()           {return type;};



void GenJet::SetGenFlavor(int genFlavor_In){
  genFlavor=genFlavor_In;
};
void GenJet::SetIsMatch(bool isMatch_In){
  isMatch=isMatch_In;
};

void GenJet::SetType(string type_In){
  type=type_In;
};


void GenJet::Set(int maptotree_In, LorentzM* pmomentum_In,  string type_In){
  AnalysisObject::Set(maptotree_In, pmomentum_In);
  type=type_In;
}

void GenJet::SetPartner(Ptr_Jet matchedDetJet_in){
  matchedDetJet=matchedDetJet_in;
}
//
//boost::shared_ptr<Jet> GenJet::GetPartner(){return matchedDetJet;}

//return a shared pointer from a weak pointer
Ptr_Jet GenJet::GetPartner(){return matchedDetJet.lock();}





simpleGenJet GenJet::makeSimpleGenJet(){
  //it makes a simpleJet out of this
  
  simpleGenJet dummySimpleJet(maptotree,p4);
  dummySimpleJet.isMatch=isMatch;
  dummySimpleJet.type=type;
  dummySimpleJet.genFlavor=genFlavor;
  if(this->IsMatch()){
    dummySimpleJet.matchedDetJet=this->GetPartner()->GetIndexInTree();
  }else{
    dummySimpleJet.matchedDetJet=-1;
  }
  return dummySimpleJet;
}



