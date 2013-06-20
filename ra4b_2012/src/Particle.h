#ifndef Particle_h
#define Particle_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "AnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class Particle : public AnalysisObject {
  
 protected:
  int       charge;
  string    flavor;
  int       particleID;
  float     relIso;
  double dz;


 public:
  Particle(){
    charge=99;
    flavor="undefined";
    particleID=0;
    relIso=-1.;
    dz=0;
  }

  ~Particle(){}

  int Charge() const ;
  string Flavor() const;
  int ParticleID() const;
  float RelIso() const;
  bool IsIso(double ISO_CUT=0.10) const;
  double Iso() const;
  double Dz() const;


  void SetCharge(int charge_In);
  void SetFlavor(string flavor_In);
  void SetParticleID(int particleID_In);
  void SetRelIso(double reliso_in);
  void SetDz(const double& dz_in);

  //SET IS OVERLOADED
/*   virtual void Set(int maptotree_In, LorentzM * momuntum_In); */

  virtual void Set(int maptotree_In, LorentzM* momuntum_In, int charge_In, double relIso_In);
  virtual void Set(int maptotree_In, LorentzM* momuntum_In, int charge_In, double relIso_In, double dz_In);
  virtual void Set(int maptotree_In, LorentzM* momuntum_In, int charge_In, double relIso_In, string flavor_In);
  virtual void Set(int maptotree_In, LorentzM* momuntum_In, int charge_In, double relIso_In, string flavor_In, int particleID_In);


};

bool compare_Particle_Pt(Particle* p1, Particle* p2);
#endif
