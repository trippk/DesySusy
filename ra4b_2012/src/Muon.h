#ifndef Muon_h
#define Muon_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "Particle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class Muon: public Particle {

 public:
  

  Muon(){
    pp4=0;
    charge=99;
    flavor="mu";
    particleID=13;
    relIso=-1.;
    maptotree=-1;
    allIDs.clear();
    id.clear();
  }

  ~Muon(){}

  void Set(int maptotree_In, LorentzM * momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In);
  void Set(int maptotree_In, LorentzM * momentum_In, int charge_In, double relIso_In);
  void Set(int maptotree_In, LorentzM * momentum_In);

};

#endif
