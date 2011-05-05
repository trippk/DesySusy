#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"

using namespace std;

// RA5 signal muon:
// global track normed chi2 <10
// inner track nhits > 10
// global track nhits > 10
// inner track<>beam spot <0.02
// global track  outer nhits > 0
// ECAL veto deposit <= 4
// Hcal veto deposit <= 6
// pt > 10/20
// good iso 0.15 - 10 pt
// this is used-> good iso 0.1  - 20 pt maxIso see below
// eta <= 2.4
bool muons_RA5(EasyChain* tree,vector<int>& goodNonIsoMuons, vector<double>& relIso){

	ConfigReader config;
	// static since we read only once
	static float mu_pt_min_low   = config.getFloat("mu_pt_min_low",  10);
	static float mu_pt_min_high  = config.getFloat("mu_pt_min_high", 20);
	static float mu_eta_max      = config.getDouble("mu_eta_max",   2.4);

	// fetch histograms defined in main_RA5.cpp
        static TH1F* hisomax = fetchHisto<TH1F>("hisomax");
        static TH1F* hiso    = fetchHisto<TH1F>("hiso");

	// the muon collection
	vector<LorentzM>& Muons = tree->Get(&Muons, "muonP4Pat");

	bool found_pt_min_high=false;
	int cntIso=0;
	for(int k=0;k<Muons.size();k++){

		// pt and eta cuts
		if(  Muons[k].pt() < mu_pt_min_low || fabs(Muons[k].eta()) > mu_eta_max ) continue;

		// track types
		vector<int>&      Mu_IsGlobal = tree->Get( &Mu_IsGlobal, "muonIsGlobalMuonPat" );
		vector<int>&      Mu_IsTracker= tree->Get( &Mu_IsTracker,"muonIsTrackerMuonPat");
		if(!Mu_IsGlobal[k]||!Mu_IsTracker[k])           continue;

		// #hits conditions
		vector<unsigned int>& Mu_InnerTrack_numValHits         = tree->Get( &Mu_InnerTrack_numValHits, "muonInnerTrackNumberOfValidHitsPat");
		vector<unsigned int>& Mu_GlobalTrack_numValTrackerHits = tree->Get( &Mu_GlobalTrack_numValTrackerHits,"DESYmuonGlobalTrackNumberOfValidTrackerHitsPat");
		vector<unsigned int>& Mu_GlobalTrack_numValOuterHits   = tree->Get( &Mu_GlobalTrack_numValOuterHits, "DESYmuonGlobalTrackNumberOfValidMuonHitsPat");
		if( Mu_InnerTrack_numValHits[k]         <= 10
		 || Mu_GlobalTrack_numValTrackerHits[k] <= 10
		 || Mu_GlobalTrack_numValOuterHits[k]   ==  0 ) continue;

		// beamspot distance
		vector<float>&   Mu_InnerTrack_Dxy_BS     = tree->Get( &Mu_InnerTrack_Dxy_BS,     "muonInnerTrackDxyBSPat"          );
		if(fabs(Mu_InnerTrack_Dxy_BS[k]) >= .02)        continue;

		// track chi2
		vector<float>&   Mu_GlobalTrack_normChi2  = tree->Get( &Mu_GlobalTrack_normChi2,  "muonGlobalTracknormalizedChi2Pat");
		if( Mu_GlobalTrack_normChi2[k] >= 10)           continue;

		// veto deposits
		vector<float>& Mu_IsolationR03emVetoEt   = tree->Get( &Mu_IsolationR03emVetoEt,  "DESYmuonIsolationR03emVetoEtPat"  );
		vector<float>& Mu_IsolationR03hadVetoEt  = tree->Get( &Mu_IsolationR03hadVetoEt, "DESYmuonIsolationR03hadVetoEtPat" );
		if (Mu_IsolationR03emVetoEt[k]         > 4.
		||  Mu_IsolationR03hadVetoEt[k]        > 6. )   continue;

		// relative isolation
		vector<float>&    Mu_ecalIso  = tree->Get( &Mu_ecalIso,  "muonEcalIsoPat" );
		vector<float>&    Mu_hcalIso  = tree->Get( &Mu_hcalIso,  "muonHcalIsoPat" );
		vector<float>&    Mu_trackIso = tree->Get( &Mu_trackIso, "muonTrackIsoPat");
		double reliso=(Mu_ecalIso[k]+Mu_hcalIso[k]+Mu_trackIso[k])/Muons[k].pt();
		double reliso_max20=(Mu_ecalIso[k]+Mu_hcalIso[k]+Mu_trackIso[k])/max(float(20.),Muons[k].pt());
		hiso->Fill(reliso);
		hisomax->Fill(reliso_max20);
		//vector<int>&      Mu_charge   = tree->Get( &Mu_charge,    "muonChargePat");
		goodNonIsoMuons.push_back(k);
		relIso.push_back(reliso_max20);
		if( Muons[k].pt() > mu_pt_min_high ) found_pt_min_high=true;
		if( reliso_max20<0.1 ) cntIso++;
	}
// this is correct
//	return cntIso >= 2 && found_pt_min_high;
// for testing:
	return Muons.size()>=2 &&found_pt_min_high;
};
