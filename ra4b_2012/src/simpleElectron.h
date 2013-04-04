#ifndef simpleElectron_h
#define simpleElectron_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "simpleParticle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class simpleElectron: public simpleParticle {
  
 public:
  
  simpleElectron(){
    charge=99;
    flavor="el";
    particleID=11;
    relIso=-1.;
  }

  ~simpleElectron(){}

  void Set(int maptotree_In, LorentzM& momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In);
  void Set(int maptotree_In, LorentzM& momentum_In, int charge_In, double relIso_In);
  void SetElectronStuff(int charge_In, double relIso_In);
  friend class Electron;

  ClassDef(simpleElectron,1)
/*   void Set(int maptotree_In, LorentzM * momentum_In); */
};

#endif
