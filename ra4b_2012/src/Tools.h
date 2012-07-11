#ifndef Tools_h
#define Tools_h

#include <vector>
#include "NtupleTools2_h.h"
#include "Muon.h"
#include "Electron.h"
#include "Jet.h"

using namespace std;

std::vector<LorentzM*> PointerMaker(std::vector<Muon*>* in );
std::vector<LorentzM*> PointerMaker(std::vector<Electron*>* in );
std::vector<LorentzM*> PointerMaker(std::vector<Jet*>* in );
std::vector<LorentzM*> PointerMaker(std::vector<LorentzM>* in );

#endif
