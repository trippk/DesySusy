#include "Tools.h"

float desy_tools::Consistency( LorentzM refP4, vector<LorentzM>& compP4) {
  
  float dR=0.;
  float dRclosest=20.;
  float dPt = 99999.;
  
  for (int iP4=0; iP4<compP4.size(); iP4++) {
    dR=ROOT::Math::VectorUtil::DeltaR( refP4, compP4.at(iP4));
    if (dR < dRclosest) {
      dRclosest = dR;
      dPt = fabs( refP4.Pt() - compP4.at(iP4).Pt());
    }
  }
  return dPt;
}

float desy_tools::Consistency( LorentzM refP4, EasyChain* tree, const char* compLabelP4) {
  
  vector<LorentzM>& compP4 = tree->Get(&compP4, compLabelP4);
  
  return desy_tools::Consistency( refP4, compP4);  
}

float desy_tools::Consistency( LorentzM refP4, int refCh, vector<LorentzM>& tmpCompP4, vector<int>& tmpCompCh) {
  
  vector<LorentzM> compP4;
    
  if (tmpCompP4.size() != tmpCompCh.size()){
    cout << "ERROR: desy_tools::Consistency: P4.size() != Ch.size()" << endl;
    return 1;
  }

  for (int iP4 = 0; iP4 < tmpCompP4.size(); iP4++) {
    if (tmpCompP4.at(iP4).Pt() > 10. && tmpCompCh.at(iP4) == refCh) compP4.push_back(tmpCompP4.at(iP4));
  }

  return desy_tools::Consistency( refP4, compP4);
}

float desy_tools::Consistency( LorentzM refP4, int refCh, EasyChain* tree, const char* compLabelP4, const char* compLabelCh) {
  
  vector<LorentzM>& tmpCompP4 = tree->Get(&tmpCompP4, compLabelP4);
  vector<int>&      tmpCompCh = tree->Get(&tmpCompCh, compLabelCh);
  
  vector<LorentzM> compP4;
  
  if (tmpCompP4.size() != tmpCompCh.size()){
    cout << "ERROR: desy_tools::Consistency: P4.size() != Ch.size()" << endl;
    return 1;
  }

  for (int iP4 = 0; iP4 < tmpCompP4.size(); iP4++) {
    if (tmpCompP4.at(iP4).Pt() > 10. && tmpCompCh.at(iP4) == refCh) compP4.push_back(tmpCompP4.at(iP4));
  }

  return desy_tools::Consistency( refP4, compP4);
}

bool desy_tools::CleaningByDR( LorentzM const & ref, vector<LorentzM*>& comp, float DR){

  for(int icomp=0; icomp<(int)comp.size(); ++icomp){
    if(DeltaR( ref, *comp.at(icomp)) < DR) return true;  
  }

  return false;
};

double desy_tools::HT( vector<Jet*>& jets){
  double HT=0;
  for (int ijet = 0; ijet<jets.size(); ijet++){
    HT += jets.at(ijet)->Pt();
  }
  return HT;
}
