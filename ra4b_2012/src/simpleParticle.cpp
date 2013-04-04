#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "simpleParticle.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;
ClassImp(simpleParticle);
int simpleParticle::Charge() const {return charge;}
string simpleParticle::Flavor() const {return flavor;};
int simpleParticle::ParticleID() const {return particleID;};
float simpleParticle::RelIso() const {return relIso;}
bool  simpleParticle::IsIso(double ISO_CUT) const {return relIso<ISO_CUT;}

void simpleParticle::SetCharge(int charge_In){charge=charge_In;}
void simpleParticle::SetFlavor(string flavor_In){flavor=flavor_In;};
void simpleParticle::SetParticleID(int particleID_In){particleID=particleID_In;};
void simpleParticle::SetRelIso(double reliso_In){relIso=reliso_In;}

void simpleParticle::Set(int maptotree_In, LorentzM momentum_In, int particleID_In, int charge_In, string flavor_In, double relIso_In){

  simpleAnalysisObject::Set(maptotree_In, momentum_In);

  //SET THE PARTICLE ID
  particleID=particleID_In;

  //SET THE CHARGE
  SetCharge(charge_In);
  
  //SET THE FLAVOUR
  SetFlavor(flavor_In);

  //SET relIso:
  SetRelIso(relIso_In);

}

void simpleParticle::Set(int maptotree_In, LorentzM momentum_In, int charge_In, double relIso_In){

  simpleAnalysisObject::Set(maptotree_In, momentum_In);

  //SET THE CHARGE
  SetCharge(charge_In);

  //SET THE ISO
  SetRelIso(relIso_In);
}




bool compare_Particle_Pt(simpleParticle* p1, simpleParticle* p2) {

  //Ordering mechanism for Particles. Used for stl containers with sort.
  //If p1 has a higher Pt than p2, it should return true. This means that p1
  //should be placed before p2 in the list.
  
  if ( p1->Pt() > p2->Pt() ) {
    return true;
  }
  else return false;
}




