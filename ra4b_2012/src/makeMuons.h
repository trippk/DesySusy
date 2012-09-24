#ifndef makeMuons_h
#define makeMuons_h

#include "NtupleTools2_h.h"
#include "Muon.h"

vector<Muon> makeAllMuons(EasyChain* tree );
bool makeSoftMuons(EasyChain* tree, vector<Muon>& Muons, vector<Muon*>& SoftMuons);
bool makeLooseMuons(EasyChain* tree, vector<Muon>& Muons, vector<Muon*>& LooseMuons );
bool makeTightMuons(EasyChain* tree, vector<Muon>& Muons, vector<Muon*>& TightMuons);
bool makeVetoMuons(EasyChain* tree,  vector<Muon>& Muons, vector<Muon*>& VetoMuons);
float Consistency( LorentzM vRef, EasyChain* tree, const char* name);
#endif
