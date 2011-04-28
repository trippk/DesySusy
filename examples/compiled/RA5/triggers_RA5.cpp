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

bool triggers_RA5(EasyChain* tree){

	static ConfigReader config;

	// default trigger - in case that triggerlist is not in config file
	config.getTString("triggerlist"," ");
	// number of different triggers in list
	int size = config.ListSize("triggerlist");

	if(size==0) return true; // no trigger check at all for empty (" ") triggerlist

	// check if any trigger from triggerlist is available
	map<string,bool>& HLTtrigger = tree->Get(&HLTtrigger, "triggered");

	if( HLTtrigger.size()==0 ) return false;

	// loop over triggers from triggerlist until any found
	for(int i=0;i<size;i++)
		if( HLTtrigger[ config.getString("triggerlist",i) ] ) return true;

	return false;
}

