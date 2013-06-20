#ifndef makeTaus_h
#define makeTaus_h

#include "NtupleTools2_h.h"
#include "Particle.h"
#include "Muon.h"
#include "Electron.h"
#include "Tau.h"
#include "TRandom3.h"

vector<Tau> makeAllTaus( EasyChain* tree);
void makeCleanTaus( vector<Tau>& allTaus, vector<Tau*>& cleanTaus, vector<Muon*>& muons, vector<Electron*>& electrons);
bool makeVetoTaus( EasyChain* tree, vector<Tau>& allTaus,vector<Tau*>& vetoTaus, Particle* selectedLepton);
#endif
