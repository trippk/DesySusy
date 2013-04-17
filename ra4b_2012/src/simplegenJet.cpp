#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "simplegenJet.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;


int    simpleGenJet::GenFlavor()      {return genFlavor;};
bool   simpleGenJet::IsMatch()        {return isMatch;};
string simpleGenJet::Type()           {return type;};



void simpleGenJet::SetGenFlavor(int genFlavor_In){
  genFlavor=genFlavor_In;
};
void simpleGenJet::SetIsMatch(bool isMatch_In){
  isMatch=isMatch_In;
};

void simpleGenJet::SetType(string type_In){
  type=type_In;
};


void simpleGenJet::Set(int maptotree_In, LorentzM pmomentum_In,  string type_In){
  simpleAnalysisObject::Set(maptotree_In, pmomentum_In);
  type=type_In;
}

void simpleGenJet::SetPartner(int matchedDetJet_in){
  matchedDetJet=matchedDetJet_in;
}
//

int simpleGenJet::GetPartner(){return matchedDetJet;}

