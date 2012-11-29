#ifndef triggers_RA4b_h
#define triggers_RA4b_h

#include <map>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"

#include <set>


bool triggers_RA4b(EasyChain* tree, vector<const char*>&, double&, CutSet* flow_in = 0);

//Added for handling diLeptonTriggers
enum diLepEvent_t {FAIL=-1,MUMU, MUEL, ELEL};
diLepEvent_t getDiLepTrigType(const std::string&);
bool triggerFired(EasyChain* tree, const std::string& triggername);
void getDiLepTriggers(EasyChain* tree, std::map<std::string,bool>& triggerMap, std::set<diLepEvent_t>& firedDiLepType );
bool isMuEGPlateau(const std::map<std::string, bool>& trigsFiredMap, double MuPt, double ElPt);

#endif
