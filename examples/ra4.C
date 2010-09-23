// Usage:  root -q -b ra4.C+

#include <set>
#include <map>
#include <vector>
#include <iostream>
#include "TFile.h"
#include "Math/VectorUtil.h"
#include "TStopwatch.h"
#include "NtupleTools2.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

// whether we run on MC or data
const bool data(false);

void ra4(){

	EasyChain* tree = new EasyChain("/susyTree/tree");
	TStopwatch t;
	t.Start();
	const string dir="/scratch/hh/current/cms/user/mstein/ntuples/LM1/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/V00-13-06";
	AllRootFilesIn(dir,tree,"ls");
	t.Stop();
	cout<<tree->GetNtrees()<<" files read. Real: "<<t.RealTime()<<" CPU: "<<t.CpuTime()<<endl;
	int N=tree->GetEntries();

	set<int> runs;

	// preselection counts
	int cntTec=0;       //Number of evts - not(36 or 37 or 38 or 39) || 0 for data
	int cntHlt=0;       //Mumber of evts ... - HLT
	int cntVer=0;       //Number of evts ... - good vertex 
	int cntHBN=0;       //Number of evts ... - HBHEnoiseFilter
	int cntPur=0;       //Number of evts ... - PKAM filter

	// main selection counts
	int cntTightMu=0;   //Number of evts ... - exactly 1 tight Muon within cuts - see bellow
	int cntNoLooseMu=0; //Number of evts ... - no further loose muon
	int cntNoLooseEl=0; //Number of evts ... - no electron
	int cnt4jets=0;     //Number of evts ... - at least 4 jets
	int cntMET=0;       //Number of evts ... - MET > 100

	TStopwatch timer;
	timer.Start();
	for(int i=0;i<N;++i){
//	for(int i=0;i<100;++i){

		tree->GetEntry(i);

		unsigned   run = tree->Get(run, "run");
		runs.insert(run);

		//technical trigger bits
		vector<int>& L1tech  = tree->Get(&L1tech,"l1techbits");
		if(data) if(!L1tech[0]) continue;
		if(L1tech[36]||L1tech[37]||L1tech[38]||L1tech[39]) continue;
		cntTec++;

		//HLT trigger
		map<string,bool>& HLTtrigger = tree->Get(&HLTtrigger, "triggered");
		if( !HLTtrigger["HLT_Mu9"] ) continue;
		cntHlt++;

		// at least 1 good vertex
		vector<XYZPointD>& Vertices       = tree->Get( &Vertices,      "vertexPosition");
		vector<int>&       Vertex_isFake  = tree->Get( &Vertex_isFake, "vertexIsFake"  );
		vector<double>&    Vertex_ndof    = tree->Get( &Vertex_ndof,   "vertexNdof"    );
		vector<int>&       Vertex_nTracks = tree->Get( &Vertex_nTracks,"vertexNtrks"   );
		unsigned vx;
		for( vx=0; vx < Vertices.size(); ++vx ) {
			if( Vertex_isFake[vx] ) 	   continue;
			if( fabs(Vertices[vx].z()) > 15 )  continue; // 24 data
			if( Vertices[vx].rho() >  2 )      continue;
			if( Vertex_ndof[vx] <=   4 )       continue;
			break;
		}
		if(vx==Vertices.size()) continue;
		cntVer++;

		//HBHEnoiseFilter
		bool hbheNoiseFilterResult =  tree->Get(hbheNoiseFilterResult,"hbheNoiseFilterResult");
		if(!hbheNoiseFilterResult) continue;
		cntHBN++;

		// purity ratio
		// require at least 0.25 fraction to be high purity (PKAM event removal)  if n_tracks > 10
		// sum all eta ranges
		int   NEtaLT0p9AllTracks         = tree->Get(NEtaLT0p9AllTracks,    "tracksNEtaLT0p9AllTracks");
		int   NEta0p9to1p5AllTracks      = tree->Get(NEta0p9to1p5AllTracks, "tracksNEta0p9to1p5AllTracks");
		int   NEtaGT1p5AllTracks         = tree->Get(NEtaGT1p5AllTracks,    "tracksNEtaGT1p5AllTracks");
		float NallTracks = NEtaLT0p9AllTracks+NEta0p9to1p5AllTracks+NEtaGT1p5AllTracks;
		if(NallTracks==0) continue; // no use if there is no track at all
		int NEtaLT0p9HighPurityTracks    = tree->Get(NEtaLT0p9HighPurityTracks,    "tracksNEtaLT0p9HighPurityTracks");
		int NEta0p9to1p5HighPurityTracks = tree->Get(NEta0p9to1p5HighPurityTracks, "tracksNEta0p9to1p5HighPurityTracks");
		int NEtaGT1p5HighPurityTracks    = tree->Get(NEtaGT1p5HighPurityTracks,    "tracksNEtaGT1p5HighPurityTracks");
		float puriRatio  = (NEtaLT0p9HighPurityTracks+NEta0p9to1p5HighPurityTracks+NEtaGT1p5HighPurityTracks)/NallTracks; 
		if( puriRatio < 0.25 ) continue;
		cntPur++;

		//Muon what else
		vector<LorentzV>& Muons = tree->Get(&Muons, "muonP4Pat");
		if(Muons.size()==0) continue;

		// isolations
		vector<float>&      muEcalIso  = tree->Get(&muEcalIso,  "muonEcalIsoPat");
		vector<float>&      muHcalIso  = tree->Get(&muHcalIso,  "muonHcalIsoPat");
		vector<float>&     muTrackIso  = tree->Get(&muTrackIso, "muonTrackIsoPat");
		vector<double>&           dxy  = tree->Get(&dxy,        "muonGlobalTrackDxyPat"); // equals d0
		// muonID etc.
		vector<int>& muidTight = tree->Get(&muidTight, "muonIDGlobalMuonPromptTightPat");
		vector<int>& isGlobal  = tree->Get(&muidTight, "muonIsGlobalMuonPat");
		vector<int>& ValidHits = tree->Get(&ValidHits, "muonInnerTrackNumberOfValidHitsPat"); // should be muon->globalTrack()-> hitPattern().numberOfValidTrackerHits() 
		//
		// tight: mu_pt>=20, |mu_eta|<=2.1, relIso<0.05, d0<0.2 (should that be muonGlobalTrackDxyBSPat)
		//        no calo jet within R<0.3
		// loose: mu_pt>=10, |mu_eta|<=2.5, relIso<0.2
		//
		int loose(0),tight(0);
		vector<unsigned> candidates;
		for(unsigned k=0;k<Muons.size();k++) {

			if( Muons[k].pt() < 10 || fabs(Muons[k].eta()) > 2.5 ) continue; // neither veto nor tight muon

			double relIso=(muEcalIso[k]+muHcalIso[k]+muTrackIso[k])/Muons[k].pt();
			if( muidTight[k] && Muons[k].pt() >= 20 && fabs(Muons[k].eta()) <= 2.1
			    && relIso<0.05 && fabs(dxy[k])<0.02 && ValidHits[k]>=11)
				candidates.push_back(k);
			else if ( isGlobal[k] && relIso<0.2 ) {
				loose++;
//				break; // do not use this for the comparision of the number of tight muons
				       // although the final (not the tight count) result stays the same and it is a bit faster
			}
		}
		vector<LorentzV>*    ak5CaloJets=0;
		vector<int>* ak5JetJetIDloosePat=0;
		//deltaR(Muon,jet)>0.3
		for(unsigned i=0;i<candidates.size();i++){
			// now we ask for the jets
			if(ak5CaloJets==0) {
				ak5JetJetIDloosePat = tree->Get(&ak5JetJetIDloosePat,"ak5JetJetIDloosePat");
				ak5CaloJets         = tree->Get(&ak5CaloJets,"ak5JetCorrectedP4Pat");
			}
			vector<LorentzV>&  Jets(*ak5CaloJets);
			vector<int>&      JetID(*ak5JetJetIDloosePat);
			unsigned k;
			for(k=0;k<Jets.size();k++){
				if(Jets[k].pt()<30 || fabs(Jets[k].eta())>2.4 || JetID[k]==0) continue; // 14093
//				if(Jets[k].pt()<30 || fabs(Jets[k].eta())>2.4 ) continue; // 14052
				if(DeltaR(Jets[k],Muons[candidates[i]])<=0.3) break;
			}
			if(k==Jets.size()) tight++;
		}
		if( tight==1 ) cntTightMu++;
		if( tight==0 || tight+loose>1 ) continue;
		cntNoLooseMu++;

		//veto electron
		vector<LorentzV>& Electrons = tree->Get(&Electrons,  "electronP4Pat");
		vector<float>&    elEcalIso = tree->Get(&elEcalIso,  "electronEcalIsoPat");
		vector<float>&    elHcalIso = tree->Get(&elHcalIso,  "electronHcalIsoPat");
		vector<float>&   elTrackIso = tree->Get(&elTrackIso, "electronTrackIsoPat");
		unsigned ie;
		for(ie=0;ie<Electrons.size();ie++){
			if( Electrons[ie].pt()<15 || fabs(Electrons[ie].eta())>2.5) continue;
			double relIso=(elEcalIso[ie]+elHcalIso[ie]+elTrackIso[ie])/Electrons[ie].pt();
			if(relIso<0.2) break;
		}
		if(ie<Electrons.size()) continue;
		cntNoLooseEl++;

		//jets, n>=4
		vector<LorentzV>&  Jets(*ak5CaloJets);
		vector<int>&      JetID(*ak5JetJetIDloosePat);
		unsigned jetCnt=0;
		unsigned j;
		for(j=0;j<Jets.size();j++){
			if(Jets[j].pt()<30 || fabs(Jets[j].eta())>2.4 || JetID[j]==0) continue;
			if(++jetCnt == 5) break; //4: 3797, 5: 1926
		}
		if(j==Jets.size()) continue;
		cnt4jets++;
		
		//MET wo ist PAT MET?
		//metP4AK5 
		LorentzV* metP4Calo = tree->Get(&metP4Calo,"metP4Calo");
		if(metP4Calo->Et()<=100) continue;
		cntMET++;
		progress();
	}
	timer.Stop();
	cout<<"Real time: "<<timer.RealTime()<<" CPU:"<<timer.CpuTime()<<endl;
	cout<<"Runs: ";
	for(set<int>::iterator it=runs.begin();it!=runs.end();++it) cout<<(*it)<<" ";cout<<endl;

	cout<<N<<" Tot events"<<endl<<endl;
	cout<<cntTec<<" Number of evts after with tech. bits "<<100./N*cntTec<<endl;
	cout<<cntHlt<<" Number of ... + HLT "<<100./N*cntHlt<<endl;
	cout<<cntVer<<" Number of ... + at least 1 good vertex "<<100./N*cntVer<<endl;
	cout<<cntHBN<<" Number of ... + HBHEnoiseFilter "<<100./N*cntHBN<<endl;
	cout<<cntPur<<" Number of ... + PKAM "<<100./N*cntPur<<endl;
	cout<<endl;
	cout<<cntTightMu  <<" Number of ... + exactly 1 tight Muon within cuts "<<100./N*cntTightMu<<endl;
	cout<<cntNoLooseMu<<" Number of ... + no further loose muon "<<100./N*cntNoLooseMu<<endl;
	cout<<cntNoLooseEl<<" Number of ... + no electron "<<100./N*cntNoLooseEl<<endl;
	cout<<cnt4jets    <<" Number of ... + at least 4 jets "<<100./N*cnt4jets<<endl;
	cout<<cntMET      <<" Number of ... + MET > 100 "<<100./N*cntMET<<endl;

}
