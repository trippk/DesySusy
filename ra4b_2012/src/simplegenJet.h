#ifndef simplegenJet_h
#define simplegenJet_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
//#include "simpleJet.h"
#include "simpleAnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

//FORWARD declaration of Jet to avoid infinite recursion
//class Jet;class WPtr_Jet;class Ptr_Jet;
//======================================================

class simpleJet;
class simpleGenJet: public simpleAnalysisObject {
  
 protected:
  int      genFlavor;
  bool     isMatch;
  string   type;
  int matchedDetJet;
  
 public:

  simpleGenJet(){
    genFlavor=0;
    isMatch=0;
    type = "";
    //the shared pointer is initialized to NULL automatically
    //matchedDetJet.rest();
  }


  simpleGenJet(int maptotree_In, LorentzM momentum_In, string type_In=""){

    genFlavor=0;
    isMatch=0;
    type = "";
    this->Set(maptotree_In,momentum_In,type_In);
    //cout<<"created the genJet"<<this<<endl;
  }

  ~simpleGenJet(){
    //cout<<"destroying the gen jet"<<this<<endl;
  }

  int GenFlavor();
  bool   IsMatch();
  string Type();

  void SetGenFlavor(int genFlavor_In);
  void SetIsMatch(bool isMatch_In);
  void SetType(string type_In);
  virtual void Set(int maptotree_In, LorentzM momentum_In, string type_In="");
  void SetPartner(int Jet_in);
  int GetPartner();

  friend class GenJet;
};

#endif
