#ifndef makeMuons_h
#define makeMuons_h

#include "NtupleTools2_h.h"
#include "Muon.h"
#include "TRandom3.h"

vector<Muon> makeAllMuons(EasyChain* tree );
vector<Muon> makeTrkOrGlobalMuons(EasyChain* tree );
bool makeSoftMuons(EasyChain* tree, vector<Muon>& Muons, vector<Muon*>& SoftMuons);
bool makeLooseMuons(EasyChain* tree, vector<Muon>& Muons, vector<Muon*>& LooseMuons, CutSet* flow_in = 0 );
bool makeTightMuons(EasyChain* tree, vector<Muon>& Muons, vector<Muon*>& TightMuons, CutSet* flow_in = 0);
bool makeVetoMuons(EasyChain* tree,  vector<Muon>& Muons, vector<Muon*>& VetoMuons, CutSet* flow_in = 0);
float Consistency( LorentzM vRef, EasyChain* tree, const char* name);

void rescaleMUR(EasyChain* tree, vector<Muon*>&TightMuons, LorentzM& metCorr, float murErr);
float getMurSF(float err);
void rescaleMUR_simple(EasyChain* tree, vector<Muon*>&TightMuons, LorentzM& metCorr, float murErr);
void makeSimpleMuonCollection(vector<std::string>&,vector<simpleMuon>& ,vector<simpleMuon*>&);
void makeSimpleMuonCollection(vector<std::string>&,vector<simpleMuon*>&,vector<simpleMuon*>&);


#endif
