#ifndef Electron_h
#define Electron_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "Particle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class Electron: public Particle {
  
 public:
  
  Electron(){
    charge=99;
    flavor="el";
    particleID=11;
    relIso=-1.;
  }

  ~Electron(){}

  void Set(int maptotree_In, LorentzM* momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In);
  void Set(int maptotree_In, LorentzM* momentum_In, int charge_In, double relIso_In);
/*   void Set(int maptotree_In, LorentzM * momentum_In); */
};

#endif
