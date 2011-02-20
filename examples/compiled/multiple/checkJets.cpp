#include "NtupleTools2_h.h"

int checkJets(EasyChain* tree){

	vector<int>&       JetID = tree->Get(&JetID,"ak5JetJetIDloosePat");
	vector<LorentzV>&   Jets = tree->Get(&Jets,"ak5JetCorrectedP4Pat");
	int cntJets=0;
	for(int k = 0; k<Jets.size(); k++){
		if(Jets[k].pt()<30 || fabs(Jets[k].eta())>2.4 || JetID[k]==0) continue;
		cntJets++;
	}
	return cntJets;
}
