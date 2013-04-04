#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "Particle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

int Particle::Charge() const {return charge;}
string Particle::Flavor() const {return flavor;};
int Particle::ParticleID() const {return particleID;};
float Particle::RelIso() const {return relIso;}
bool  Particle::IsIso(double ISO_CUT) const {return relIso<ISO_CUT;}

void Particle::SetCharge(int charge_In){charge=charge_In;}
void Particle::SetFlavor(string flavor_In){flavor=flavor_In;};
void Particle::SetParticleID(int particleID_In){particleID=particleID_In;};
void Particle::SetRelIso(double reliso_In){relIso=reliso_In;}

void Particle::Set(int maptotree_In, LorentzM * momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In){

  AnalysisObject::Set(maptotree_In, momentum_In);

  //SET THE PARTICLE ID
  particleID=particleID_In;

  //SET THE CHARGE
  SetCharge(charge_In);
  
  //SET THE FLAVOUR
  SetFlavor(flavor_In);

  //SET relIso:
  SetRelIso(relIso_In);

}

void Particle::Set(int maptotree_In, LorentzM momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In){

  AnalysisObject::Set(maptotree_In, momentum_In);

  //SET THE PARTICLE ID
  particleID=particleID_In;

  //SET THE CHARGE
  SetCharge(charge_In);
  
  //SET THE FLAVOUR
  SetFlavor(flavor_In);

  //SET relIso:
  SetRelIso(relIso_In);

}

void Particle::Set(int maptotree_In, LorentzM* momentum_In, int charge_In, double relIso_In){

  AnalysisObject::Set(maptotree_In, momentum_In);

  //SET THE CHARGE
  SetCharge(charge_In);

  //SET THE ISO
  SetRelIso(relIso_In);
}

void Particle::Set(int maptotree_In, LorentzM momentum_In, int charge_In, double relIso_In){

  AnalysisObject::Set(maptotree_In, momentum_In);

  //SET THE CHARGE
  SetCharge(charge_In);

  //SET THE ISO
  SetRelIso(relIso_In);
}




bool compare_Particle_Pt(Particle* p1, Particle* p2) {

  //Ordering mechanism for Particles. Used for stl containers with sort.
  //If p1 has a higher Pt than p2, it should return true. This means that p1
  //should be placed before p2 in the list.
  
  if ( p1->Pt() > p2->Pt() ) {
    return true;
  }
  else return false;
}




