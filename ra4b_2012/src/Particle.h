#ifndef Particle_h
#define Particle_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "AnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class Particle: public AnalysisObject {
  
 protected:
  int       charge;
  string    flavor;
  int       particleID;
  float     relIso;

 public:
  Particle(){
    pp4=0;
    charge=99;
    flavor="undefined";
    particleID=0;
    relIso=-1.;
    maptotree=-1;
    allIDs.clear();
    id.clear();
  }

  ~Particle(){}

  int Charge();
  string Flavor();
  int ParticleID();
  float RelIso();
  bool IsIso(double ISO_CUT=0.10);

  void SetCharge(int charge_In);
  void SetFlavor(string flavor_In);
  void SetParticleID(int particleID_In);
  void SetRelIso(double reliso_in);


  //SET IS OVERLOADED
  virtual void Set(int maptotree_In, LorentzM * momuntum_In);
  virtual void Set(int maptotree_In, LorentzM * momuntum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In);
  virtual void Set(int maptotree_In, LorentzM* momuntum_In, int charge_In, double relIso_In);
};

#endif
