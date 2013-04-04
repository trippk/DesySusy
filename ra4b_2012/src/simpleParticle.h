#ifndef simpleParticle_h
#define simpleParticle_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "simpleAnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class simpleParticle : public simpleAnalysisObject {
  
 protected:
  int       charge;
  string    flavor;
  int       particleID;
  float     relIso;

 public:
  simpleParticle(){
    charge=99;
    flavor="undefined";
    particleID=0;
    relIso=-1.;
  }

  ~simpleParticle(){}

  int Charge() const ;
  string Flavor() const;
  int ParticleID() const;
  float RelIso() const;
  bool IsIso(double ISO_CUT=0.10) const;

  void SetCharge(int charge_In);
  void SetFlavor(string flavor_In);
  void SetParticleID(int particleID_In);
  void SetRelIso(double reliso_in);


  //SET IS OVERLOADED
/*   virtual void Set(int maptotree_In, LorentzM * momuntum_In); */
  virtual void Set(int maptotree_In, LorentzM momuntum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In);
  virtual void Set(int maptotree_In, LorentzM momuntum_In, int charge_In, double relIso_In);
  ClassDef(simpleParticle,1);
};

bool compare_Particle_Pt(simpleParticle* p1, simpleParticle* p2);
#endif

//  LocalWords:  ifndef
