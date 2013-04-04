#ifndef simpleMuon_h
#define simpleMuon_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "simpleParticle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class simpleMuon: public simpleParticle {

 public:
  simpleMuon(){
    charge=99;
    flavor="mu";
    particleID=13;
    relIso=-1.;
  }

  ~simpleMuon(){}
  void Set(int maptotree_In, LorentzM& momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In);
  void Set(int maptotree_In, LorentzM& momentum_In, int charge_In, double relIso_In);
  void SetMuonStuff(int charge_In, double relIso_In);
  friend class Muon;

  ClassDef(simpleMuon,1)
};

#endif
