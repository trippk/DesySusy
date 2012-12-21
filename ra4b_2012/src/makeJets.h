#ifndef makeJets_h
#define makeJets_h

#include "NtupleTools2_h.h"
#include "Jet.h"
#include "Muon.h"
#include "Electron.h"
#include "TRandom3.h"
#include "JetMonitor.h"

vector<Jet> makeAllJets(EasyChain* tree);
void rescaleJER(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jerSF_err, JetMonitor * pJetM);
float getJerSF(float eta, float err_factor);
float getJetRes(double pT, double eta);
void rescaleJES(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jesSF_err);
void matchJets( const vector<Jet*> & recoJets, const vector<LorentzM> & genJets, vector<int> & recoToGen, vector<int> & genToReco);

void makeGoodJets(EasyChain* tree, vector<Jet*>& AllJets, vector<Jet*>& goodJets, CutSet* flow_in=0 );
void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon>& Muons, vector<Electron>& Electrons, CutSet* flow_in=0);
void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon*>& Muons, vector<Electron*>& Electrons, CutSet* flow_in=0);
#endif
