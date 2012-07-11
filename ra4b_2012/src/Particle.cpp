#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "Particle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;




int Particle::Charge(){return charge;}
string Particle::Flavor(){return flavor;};
int Particle::ParticleID(){return particleID;};

float Particle::RelIso(){return relIso;}
bool  Particle::IsIso(double ISO_CUT){return relIso<ISO_CUT;}

void Particle::SetCharge(int charge_In){charge=charge_In;}
void Particle::SetFlavor(string flavor_In){flavor=flavor_In;};
void Particle::SetParticleID(int particleID_In){particleID=particleID_In;};

void Particle::Set(int maptotree_In, LorentzM * momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In){


  Set(maptotree_In, momentum_In);

  //SET THE PARTICLE ID
  particleID=particleID_In;

  //SET THE CHARGE
  SetCharge(charge_In);
  
  //SET THE FLAVOUR
  SetFlavor(flavor_In);

  //SET relIso:
  SetRelIso(relIso_In);

}

void Particle::Set(int maptotree_In, LorentzM * momentum_In){

  //SET THE FOURVECTOR
  pp4=momentum_In;
  p4=*pp4;

  //SET THE MAP TO THE TREE
  maptotree=maptotree_In;
 
}


void Particle::Set(int maptotree_In, LorentzM* momentum_In, int charge_In, double relIso_In){

  Set(maptotree_In, momentum_In);

  //SET THE CHARGE
  SetCharge(charge_In);

  //SET THE ISO
  SetRelIso(relIso_In);
 
}

void Particle::SetRelIso(double reliso_In){relIso=reliso_In;}




