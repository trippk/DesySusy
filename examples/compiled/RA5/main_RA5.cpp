// incomplete!
// for RA5 in 2010 see https://twiki.cern.ch/twiki/bin/view/CMS/SameSignDilepton2010AnalysisObjects
//                     https://twiki.cern.ch/twiki/bin/viewauth/CMS/SameSignDilepton2010#Object_Selection
#include <map>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "THTools.h"
#include "muons_RA5.h"
#include "triggers_RA5.h"
#include "vertices_RA5.h"
#include "jets_RA5.h"
#include "metAndHT_RA5.h"
#include "evtqual_RA5.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

int main(int argc, char** argv){


	ConfigReader config("config_RA5.txt",argc,argv);

	// the main tree
	EasyChain* tree = new EasyChain("/susyTree/tree");

	// input file name(s) - a single file/directory or a list of names
	TString filename   = config.getTString("filename");

	// add file(s) or folder(s)
	int f = tree->AddSmart(filename);

	// output file name - if not given create one
	TString outname = config.getTString("outname",tree->GetUniqeName());

	// open output file
	TFile *outfile = TFile::Open(outname,"RECREATE");
	outfile->cd();

	// set output root file for cut flow (to be done!)
	CutSet::setTFile(outfile);
	
	// histograms for muons_RA5.cpp
	autoSavePtr<TH1F> hisomax = new TH1F("hisomax","RelIso max 20",50,0,6);
	autoSavePtr<TH1F>    hiso = new TH1F("hiso","RelIso",50,0,6);

	// if quick is true we continue with the next event
	// at the first condition that is not fullfilled
	bool quick=config.getBool("quick",false);

	// the main cut flow
	CutSet globalFlow("global flow");

	int N = tree->GetEntries();

	bool OK=false;
	for(int i=0;i<N;++i){

		//time and progress counter
		timer();

		tree->GetEntry(i);

		// for speed: check the cut with the highest rejection power first

		// check muons
		vector<int>    goodNonIsoMuons;
		vector<double> relIso;
		// OK if 2 good muons with pt>20/10 and relIso<0.1
		// but goodNonIsoMuons and relIso are filled with all good muons pt>10 and no iso cut
		OK = muons_RA5(tree,goodNonIsoMuons,relIso);
		if(  !globalFlow.keepIf("at_least_2_muons", OK )     && quick ) continue;

		// check triggers
		OK = triggers_RA5(tree);
		if( !globalFlow.keepIf("triggers", OK )              && quick ) continue;

		// check event quality
		OK = evtqual_RA5(tree);
		if( !globalFlow.keepIf("event_quality", OK )         && quick ) continue;

		// check vertices
		vector<int> goodVert;
		OK = vertices_RA5(tree,goodVert);
		if(  !globalFlow.keepIf("at_least_1_vertex", OK )    && quick ) continue;

		// check jets
		vector<int> goodJets;
		OK = jets_RA5(tree,goodJets);
		if(  !globalFlow.keepIf("at_least_2_good_jets", OK ) && quick ) continue;

		// check delta_R to muons
		int jetCnt=0;
		vector<LorentzM>&  Jets = tree->Get(&Jets, "ak5JetPFCorrectedP4Pat");
		vector<LorentzM>& Muons = tree->Get(&Muons, "muonP4Pat");
		for(int j=0;j<goodJets.size();++j){
			bool keep=true;
			for(int m=0;m<goodNonIsoMuons.size();++m){
				if( relIso[m] >= 0.1 ) continue;
				if( DeltaR(Jets[goodJets[j]],Muons[goodNonIsoMuons[m]]) > 0.4){
					keep=false;
					break;
				}
			}
			if(keep) jetCnt++;
		}
		if(  !globalFlow.keepIf("jets_muons_deltaR", jetCnt>=2 )    && quick ) continue;

		//if( !quick && !globalFlow.applyCuts("triggers event_quality at_least_1_vertex at_least_2_good_jets") ) continue ;


		// check MET and HT
		OK = metAndHT_RA5(tree);
		if(  !globalFlow.keepIf("MET_HT", OK ) && quick ) continue;

		// check muoncharge
		vector<int>&      Mu_charge   = tree->Get( &Mu_charge,    "muonChargePat");
		// to be continued ...
	}

	config.printUsed();

	globalFlow.printAll();
        globalFlow.dumpToHist();
 
	return 0;
}

