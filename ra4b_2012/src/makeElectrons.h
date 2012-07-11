#ifndef makeElectrons_h
#define makeElectrons_h

#include "NtupleTools2_h.h"
#include "Electron.h"

vector<Electron> makeAllElectrons(EasyChain* tree );
bool makeLooseElectrons(EasyChain* tree, vector<Electron>& AllElectrons, vector<Electron*>&  LooseElectrons);
bool makeTightElectrons(EasyChain* tree, vector<Electron>& AllElectrons, vector<Electron*>&  TightElectrons);
bool makeVetoElectrons(EasyChain* tree, vector<Electron>& AllElectrons,  vector<Electron*>&  VetoElectrons);
#endif
