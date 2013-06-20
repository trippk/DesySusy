#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "makeTracks.h"
#include "Particle.h"
#include "Math/VectorUtil.h"

//======================================================
// make All Tracks
//======================================================

vector<fParticle> makeAllTracks(EasyChain* tree){

  double iso = 0.;
  bool isLepton = false;
  double deltaR = 999.;
  
  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  vector<fParticle>  allTracks;
  fParticle          dummyTrack;
  vector<LorentzM>& p4      = tree->Get( &p4,     "DESYtracksP4PF");
  vector<int>&      charge  = tree->Get( &charge, "DESYtracksChgPF");
  vector<int>&      pdgID   = tree->Get( &charge, "DESYtracksPdgIdPF");
  vector<float>&    dz      = tree->Get( &dz,     "DESYtracksDzPF");
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE TRACKS
  //=============================================

  for (int itrack=0; itrack<(int)p4.size(); itrack++ ) {
    
    if( p4.at(itrack).pt() <   5.)  continue;
    if( dz.at(itrack)      >   0.1) continue;
    if( charge.at(itrack)  ==  0)   continue;
    
    dummyTrack.Set( itrack, p4.at(itrack), charge.at(itrack), -1.);
    dummyTrack.SetPdgID( pdgID.at(itrack));
    dummyTrack.SetDz ( dz.at(itrack));

    iso = 0.;
    isLepton = abs( dummyTrack.PdgID())==11 || abs( dummyTrack.PdgID())==13;
    
    for (int jtrack=0; jtrack<(int)p4.size(); jtrack++ ) {
      
      if (itrack == jtrack) continue;

      if ( dz.at(jtrack) >= 0.1) continue;
      
      if ( isLepton) {
	if ( abs( pdgID.at(jtrack))==11 || abs( pdgID.at(jtrack))==13) continue;
      }

      deltaR= ROOT::Math::VectorUtil::DeltaR( dummyTrack.P4(), p4.at(jtrack));
      if ( deltaR >= 0.3) continue;
      
      iso += p4.at(jtrack).pt();
    }
    
    dummyTrack.SetIso( iso);

    allTracks.push_back(dummyTrack);
  }
  //=end=loop=over=tracks===============================================

  return allTracks;
}

// This filter has been sincronized and tested
// Parameters of this filter are hard coded. Please don't change them

void IsoTracksV4 ( vector<fParticle>& allTracks) {

  float iso = 0.;
  bool isLepton = false;
  float deltaR = 0.;
  
  vector<fParticle> tmpTracks;

  for (int itrack=0; itrack<(int)allTracks.size(); itrack++ ) {

    iso = 0.;
    isLepton = abs( allTracks.at(itrack).PdgID())==11 || abs( allTracks.at(itrack).PdgID())==13;

    if ( allTracks.at(itrack).Pt() > 10.) continue;

    for (int jtrack=0; jtrack<(int)allTracks.size(); jtrack++ ) {
      
      if (itrack == jtrack) continue;

      if (allTracks.at(jtrack).Dz() >= 0.1) continue;
      
      if ( isLepton) {
	if ( abs( allTracks.at(jtrack).PdgID())==11 || abs( allTracks.at(jtrack).PdgID())==13) continue;
      }

      deltaR= ROOT::Math::VectorUtil::DeltaR( allTracks.at(itrack).P4(), allTracks.at(jtrack).P4());
      if ( deltaR >= 0.3) continue;

      iso += allTracks.at(jtrack).pt();
    } 

    allTracks.at(itrack).SetIso( iso);
    tmpTracks.push_back( allTracks.at(itrack));
  }

  allTracks = tmpTracks;
  return;
}


bool IsoTrackVetoV4 ( fParticle lepton, vector<fParticle>& allTracks){

  fParticle* track;
  float deltaR = 0.;

  for (int itrack=0; itrack<(int)allTracks.size(); itrack++ ){
   
    track = &( allTracks.at(itrack));
    
    if ( track->Charge() == 0) continue;
    
    deltaR= ROOT::Math::VectorUtil::DeltaR( lepton.P4(), track->P4());
    if ( deltaR <= 0.4) continue;

    if ( abs( track->PdgID())==11 || abs( track->PdgID())==13) {
      
      if ( track->pt() <= 5.) continue;
      
      if ( track->RelIso() > 0.2) continue;

      return true;
    }    
    else {
      
      if ( track->Charge() * lepton.Charge() > 0.) continue;  
      
      if ( track->pt() <= 10.) continue;
      
      if ( track->RelIso() > 0.1) continue;
  
      return true;
    }
  }
  
  return false;
}



bool IsoTrackVetoV4 ( Particle* lepton, vector<fParticle>& allTracks){

  fParticle* track;
  float deltaR = 0.;


  if (lepton==0){
    cout<<"the pointer lepton is null!!!"<<endl;
  }
  for (int itrack=0; itrack<(int)allTracks.size(); itrack++ ){
   
    track = &( allTracks.at(itrack));
    
    if ( track->Charge() == 0) continue;
    
    deltaR= ROOT::Math::VectorUtil::DeltaR( lepton->P4(), track->P4());
    if ( deltaR <= 0.4) continue;

    if ( abs( track->PdgID())==11 || abs( track->PdgID())==13) {
      
      if ( track->pt() <= 5.) continue;
      
      if ( track->RelIso() > 0.2) continue;

      return true;
    }    
    else {
      
      if ( track->Charge() * lepton->Charge() > 0.) continue;  
      
      if ( track->pt() <= 10.) continue;
      
      if ( track->RelIso() > 0.1) continue;
  
      return true;
    }
  }
  
  return false;
}
