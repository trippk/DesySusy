#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "simpleMuon.h"
#include "simpleParticle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

ClassImp(simpleMuon);
void simpleMuon::Set(int maptotree_In, LorentzM& momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In){
  this->Set(maptotree_In, momentum_In, charge_In, relIso_In);
}

void simpleMuon::Set(int maptotree_In, LorentzM& momentum_In, int charge_In, double relIso_In){
  this->simpleAnalysisObject::Set(maptotree_In,momentum_In);
  this->SetMuonStuff(charge_In, relIso_In);
}

void simpleMuon::SetMuonStuff(int charge_In, double relIso_In){
  //SET THE CHARGE
  this->SetCharge(charge_In);

  //SET THE PARTICLE ID
  if (charge == 1) {
    this->SetParticleID(-13);
  }
  else if(charge == -1){
    this->SetParticleID(13);
  }else{
    cout<<"THE MUON CHARGE IS NOT CORRECT.ERROR "<<endl;
  }

  //SET THE FLAVOUR
  this->SetFlavor("mu");

  //SET relIso:
  this->SetRelIso(relIso_In);
}

