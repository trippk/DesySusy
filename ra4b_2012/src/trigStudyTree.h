#ifndef trigStudyTree_h
#define trigStudyTree_h

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TTree.h"
//#include "TMath.h"
#include "subTree.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include <Math/LorentzVector.h>

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "typedefs.h"

//typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >          LorentzM;

using namespace std;
using namespace ROOT::Math::VectorUtil;

class trigStudyTree: public subTree {
public:
  trigStudyTree();
  trigStudyTree(TFile*, TString);
  
  void Constructor();
  void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, LorentzM& met);
  void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Ptr_Jet>& jets, LorentzM& met);
  virtual void Write();
  void SetTDir(TString);
  void SetTFile(TFile*);
  
protected:
  void initLeafs();
  TTree* mytree;
  TDirectory* dir;
  TFile* tfile;
  
  //these are the leaves to write into a root file
  vector<string> DESYtriggerElMatchedTriggerFilter;
  vector<string> DESYtriggerMuMatchedTriggerFilter;
  map<string, string> DESYtriggerNameMap;
  vector<int> electronCharge;
  vector<LorentzM> electronP4;
  int event;
  double HT;
  vector<LorentzM> jetP4;
  double MET;
  vector<int> muonCharge;
  vector<LorentzM> muonP4;
  map<string, int> prescaled;
  int PUInter;
  double PUWeight;
  int run;
  map<string, bool> triggered;
  double weight;
  
  
};

#endif
