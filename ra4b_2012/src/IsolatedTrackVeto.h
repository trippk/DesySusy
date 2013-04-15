////////////////////////////////////////////////////////////////
// IsolatedTrackVeto                                          //
// author: Francesco Costanza (francesco.costanza@desy.de)    //
// it returns true if the events contains an isolated track   //
// not compatible with the Signal Lepton                      //
////////////////////////////////////////////////////////////////

#ifndef IsolatedTrackVeto_h
#define IsolatedTrackVeto_h

#include "TMath.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "Muon.h"
#include "Electron.h"

#include <iostream>
#include <vector>

using namespace ROOT::Math::VectorUtil;
using namespace std;

template <class T>
bool IsolatedTrackVeto ( T* lepton, EasyChain* tree) {

  bool flagLep = false;
  bool flagHad = false;

  ConfigReader config;
  
  static float DZVrtxMax  = config.getFloat("TrackVeto_DZVrtxMax", 0.1 ); 
  static float DRLepMin  = config.getFloat("TrackVeto_DRLepMin",  0.4 ); 
  static float LepPtMin  = config.getFloat("TrackVeto_LepPtMin",  5. );  
  static float HadPtMin  = config.getFloat("TrackVeto_HadPtMin", 10. ); 
  static float LepRelIsoCut  = config.getFloat("TrackVeto_LepRelIsoCut",  .2 );  
  static float HadRelIsoCut  = config.getFloat("TrackVeto_HadRelIsoCut",  .1 ); 

  vector<LorentzM>& tracks = tree->Get(&tracks, "DESYtracksP4PF");
  vector<int>&      charge = tree->Get(&charge, "DESYtracksChgPF");
  vector<int>& trackspdgid = tree->Get(&charge, "DESYtracksPdgIdPF");
  vector<float>& trkiso    = tree->Get(&trkiso, "DESYtracksTrkIsoPF");
  vector<float>& dz        = tree->Get(&dz, "DESYtracksDzPF");
  
  float trkRelIsoLep = 99999.;
  float trkRelIsoHad = 99999.;

  for (unsigned itrk=0; itrk<tracks.size(); itrk++){
    
    float deltaR= ROOT::Math::VectorUtil::DeltaR( *lepton, tracks.at(itrk));
    if (deltaR < DRLepMin) continue;
    
    if (fabs(dz.at(itrk)) > DZVrtxMax) continue;
    
    if ( abs(trackspdgid.at(itrk)) == 11 || abs(trackspdgid.at(itrk)) == 13 ) {
      if ( tracks.at(itrk).pt() >  LepPtMin) {
	float reliso = trkiso.at(itrk)/tracks.at(itrk).pt();
	if ( reliso < trkRelIsoLep )
	  trkRelIsoLep = reliso;
      }
    }
    else if ( tracks.at(itrk).pt() > HadPtMin ) {
      if (charge.at(itrk) * lepton->Charge() > 0) continue;
      float reliso = trkiso.at(itrk)/tracks.at(itrk).pt();
      if ( reliso < trkRelIsoHad )
	trkRelIsoHad = reliso;
    }
  }  

  flagLep = trkRelIsoLep < LepRelIsoCut;
  flagHad = trkRelIsoHad < HadRelIsoCut;
  
  return flagLep || flagHad;

};
#endif
