#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include <vector>

//check variables code is incomplete

bool jets_RA5(EasyChain* tree,vector<int>& goodJets){

	static ConfigReader config;
	// static since we read only once
	static float jet_pt_min  = config.getFloat("jet_pt_min",   30);
	static float jet_eta_max = config.getDouble("jet_eta_max",2.4);
	static bool  quick       = config.getBool("quick");

	// static to get the cut flow for all events
	static CutSet jetFlow("good jet selection");
	// print at end of program
	jetFlow.autoprint=true;

	vector<int>&       JetID = tree->Get(&JetID,"ak5JetPFPFJetIDloosePat");
	vector<LorentzM>&   Jets = tree->Get(&Jets, "ak5JetPF2PATCorrectedP4Pat");
	int cntJets=0;
	for(int k = 0; k<Jets.size(); k++){
		if( !jetFlow.keepIf("pt>=jet_min",              Jets[k].pt()   >= jet_pt_min  ) && quick) continue;
		if( !jetFlow.keepIf("eta<=eta_max",        fabs(Jets[k].eta()) <= jet_eta_max ) && quick) continue;
		if( !jetFlow.keepIf("ak5JetPFPFJetIDloosePat", JetID[k]!=0 )                    && quick) continue;
		goodJets.push_back(k);
	}

	return goodJets.size() >= 2;
}
//for b-jets
//vector<float>& TrkCountingHighEffB = tree->Get(&TrkCountingHighEffB,"ak5JetTrkCountingHighEffBJetTagsPat");
//vector<float>& TrkCountingHighPurB = tree->Get(&TrkCountingHighPurB,"ak5JetTrkCountingHighPurBJetTagsPat");
//if(TrkCountingHighEffB[k]>3.4)
                                                                                                
