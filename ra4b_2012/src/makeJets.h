#ifndef makeJets_h
#define makeJets_h

#include "NtupleTools2_h.h"
#include "Jet.h"
#include "Muon.h"
#include "Electron.h"

vector<Jet> makeAllJets(EasyChain* tree);
void makeGoodJets(EasyChain* tree, vector<Jet*>& AllJets, vector<Jet*>& goodJets );
void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon>& Muons, vector<Electron>& Electrons);
void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon*>& Muons, vector<Electron*>& Electrons);
#endif
