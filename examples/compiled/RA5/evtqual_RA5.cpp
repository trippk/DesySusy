#include <map>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "muons_RA5.h"
#include "vertices_RA5.h"
#include "jets_RA5.h"

using namespace std;

// what else to check???
bool evtqual_RA5(EasyChain* tree){

        // check event quality
        // calo noise filter
	bool hbheNoiseFilterResult =  tree->Get(hbheNoiseFilterResult,"hbheNoiseFilterResult");

	return hbheNoiseFilterResult;
}

