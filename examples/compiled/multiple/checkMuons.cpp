#include "NtupleTools2_h.h"

using namespace std;

int checkMuons(EasyChain* tree){
	vector<LorentzV>& Muons = tree->Get(&Muons, "muonP4Pat");
	return Muons.size();
};
