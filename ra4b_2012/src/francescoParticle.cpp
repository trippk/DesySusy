#include "francescoParticle.h"

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"

using namespace ROOT::Math::VectorUtil;

//ClassImp( fParticle);


fParticle::fParticle(){
  charge = 99.;
  pdgID = 0;
  iso = -1.;
  dz = -1.;
}

fParticle::fParticle( const fParticle& copy){
  //AnalysisObject( (AnalysisObject) copy);
  maptotree = copy.GetIndexInTree();
  p4 = copy.P4();
  //id = copy.IDMap();
  id = copy.id;
  charge = copy.Charge();
  pdgID = copy.PdgID();
  iso = copy.Iso();
  dz = copy.Dz();
}

fParticle::~fParticle(){}

double fParticle::Charge() const {return charge;}
int    fParticle::PdgID()  const {return pdgID;}
double fParticle::Iso()    const {return iso;}
double fParticle::RelIso() const {return (iso/this->pt());}
bool   fParticle::IsIso(double ISO_CUT) const {return this->RelIso()<ISO_CUT;}
double fParticle::Dz()     const {return dz;}

void fParticle::Set(const int maptotree_In, const LorentzM momentum_In, const int& charge_In, const double& iso_In){

  LorentzM copy = momentum_In;
  LorentzM* pmom = &copy;
  AnalysisObject::Set(maptotree_In, pmom);
  SetCharge(charge_In);
  SetIso(iso_In);
}

void fParticle::Set(const int maptotree_In, const LorentzM momentum_In, const int& pdgId_In, const int& charge_In,
	 const double& iso_In, const double& dz_In){
  Set(maptotree_In, momentum_In, charge_In, iso_In);
  SetPdgID( pdgId_In);
  SetDz( dz_In);
}
void fParticle::Set(const int maptotree_In, const LorentzM momentum_In, const int& pdgId_In, const int& charge_In,
		    const double& iso_In, const double& dz_In, const map< string, bool>& id_In){
  Set( maptotree_In, momentum_In, pdgId_In, charge_In, iso_In, dz_In);
  //SetIDMap( id_In); 
  id = id_In;
}
void fParticle::SetCharge(const double& charge_In){charge=charge_In;}
void fParticle::SetPdgID(const int& pdgID_In){pdgID=pdgID_In;};
void fParticle::SetIso(const double& iso_In){iso=iso_In;}
void fParticle::SetRelIso(const double& reliso_In){iso=reliso_In * this->pt();}
void fParticle::SetDz(const double& dz_In){dz=dz_In;}


