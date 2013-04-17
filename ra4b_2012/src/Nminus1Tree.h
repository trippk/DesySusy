#ifndef Nminus1Tree_h
#define Nminus1Tree_h

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "subTree.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "mt2w_interface.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;
using namespace ROOT::Math::VectorUtil;

namespace gentools{
std::vector<int> GetDaughters(int ParticleIndex, std::vector<int>* MotherIndex);
int Decay(int ParticleIndex, std::vector<int>* MotherIndex, std::vector<int>* PdgId, TString spazio);
TString IdToString(int id);
}


class Nminus1Tree: public subTree {
 public:

   Nminus1Tree();
   virtual void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Jet*>& jets_in, vector<GenJet*>& genjets_in, LorentzM& met_in);
   virtual void Write();
  protected:
   
   template <class T> void SetVector(LorentzED* v, T ref);
   void SetToZero();
   void FirstFill( EasyChain* tree);
   void FillScan( EasyChain* tree);

   TTree* mytree;
   mt2w_bisect::mt2w_interface mt2w_calc;

   int event;
   int run;

   double xs;
   int nEvents;
   double FE;
   double globalWeight;
   double PUWeight;
   double eventWeight;

   LorentzED* el;
   double elPt;
   LorentzED* mu;
   double muPt;

   double mt2wEl;
   double mt2wMu;

   LorentzED* jets[7];
   double    jetsPt[7];
   double    bjetdisc[7];
   int       njets;
   int       nbjets[3];
   bool      isbjet[7][3];

   LorentzED*   vMET;
   double      MET; 
   LorentzED*   vMHT;
   double      MHT;
   double      HT;
   
   double      elSig;
   double      muSig;
   double      METSig;
   double      MHTSig;

   double      mtEl;
   double      mtMu;

   map<TString, bool> scanBool;
   map<TString, int> scanInt;
   map<TString, float> scanFloat;
   map<TString, double> scanDouble;

   int nFill;
   int leptonsFromTop;

private:
	std::vector<int> GetDaughters(int ParticleIndex, std::vector<int>* MotherIndex);
	int Decay(int ParticleIndex, std::vector<int>* MotherIndex, std::vector<int>* PdgId, TString spazio);
	TString IdToString(int id);
};


template <class T>
void Nminus1Tree::SetVector(LorentzED* v, T ref) {

  v->SetPxPyPzE(ref->Px(),
		ref->Py(),
		ref->Pz(),
		ref->E());
}

#endif
