#ifndef subTree_h
#define subTree_h

#include "TMath.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "NtupleTools2.h"
#include "EventInfo.h"

#include <iostream>

using namespace ROOT::Math::VectorUtil;

class subTree
 {
 public:
   virtual void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, vector<GenJet*>& genjets, LorentzM& met) = 0;
   virtual void Write() = 0;
 };

#endif
