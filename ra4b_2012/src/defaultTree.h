#ifndef defaultTree_h
#define defaultTree_h

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
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

class defaultTree: public subTree {
 public:

  defaultTree();
  defaultTree(TFile*, TString);

  void Constructor();

  void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, LorentzM& met);
  void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, double& met);
  virtual void Write();
  void SetTDir(TString);
  void SetTFile(TFile*);
protected:
  

  void SetToZero(vector<double>* vec);
  TTree* mytree;
  TDirectory* dir;
  TFile* tfile;
   mt2w_bisect::mt2w_interface mt2w_calc;
  
  int event;
  int run;
  double MET;
  double HT;
  double MHT;
  double Y;
  double MTEle;
  double MTMu;
  double MT2;
  double MT2WEle;
  double MT2WMu;
  double weight;
  double PUWeight;
  int nJets;
   
   vector<double> el;
   vector<double> mu;
   vector<double> jet1;
   vector<double> jet2;
   vector<double> jet3;
   vector<double> jet4;
   vector<double> jet5;
   vector<double> jet6;
   vector<double> jet7;
   
 };

#endif
