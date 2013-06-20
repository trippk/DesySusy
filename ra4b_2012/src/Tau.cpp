#include "Tau.h"

using namespace std;

Tau::Tau(){
  charge=99.;
  pdgID=15;
  iso=-1.;
  dz = -1.;
}

Tau::Tau( const Tau& copy){
  maptotree = copy.GetIndexInTree();
  p4 = copy.P4();
  id = copy.id;

  charge = copy.Charge();
  pdgID = copy.PdgID();
  iso = copy.Iso();
  dz = copy.Dz();
}

Tau::~Tau(){}

void Tau::SetCharge( const double& charge_In){
  fParticle::SetCharge( charge_In);
  if (abs(charge)!=1) cout<<"ERROR: Tau.cpp: tau charge set to "<<charge<<"."<<endl;
}

void Tau::SetPdgID( const int& pdgID_In){
  if (abs(pdgID_In)!=15) cout<<"ERROR: Tau.cpp: trying to set to tau pdgId to"<<pdgID_In<<"."<<endl;
  
  if ( charge == 1 && pdgID_In == -15) {
    pdgID=-15;
  }
  else if(charge == -1 && pdgID_In == 15){
    pdgID=15;
  }
  else{
    cout<<"ERROR: Tau.cpp: inconsistency in tau charge and tau pdgID."<<endl;
  }
}
void Tau::Set(const int maptotree_In, const LorentzM momentum_In, const int& charge_In, const double& iso_In){


  LorentzM copy = momentum_In;
  LorentzM* pmom = &copy;
  AnalysisObject::Set( maptotree_In, pmom);
  SetCharge( charge_In);
  if ( charge == 1) {
    SetPdgID(-15);
  }
  else if(charge == -1){
    SetPdgID(15);
  }
  SetIso( iso_In);
}
void Tau::Set(const int maptotree_In, const LorentzM momentum_In, const int& charge_In, const double& iso_In, 
	      const double& dz_In){
  Set(maptotree_In, momentum_In, charge_In, iso_In);
  SetDz( dz_In);
}
void Tau::Set(const int maptotree_In, const LorentzM momentum_In, const int& pdgID_In, const int& charge_In,
	      const double& iso_In, const double& dz_In){
  Set(maptotree_In, momentum_In, charge_In, iso_In, dz_In);
  SetPdgID( pdgID_In);
}
void Tau::Set(const int maptotree_In, const LorentzM momentum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In, const map< string, bool>& id_In){
  Set( maptotree_In, momentum_In, pdgID_In, charge_In, iso_In, dz_In);
  id = id_In;

}
