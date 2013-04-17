#ifndef objectsTree_h
#define objectsTree_h

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TTree.h"
#include "TMath.h"
#include "subTree.h"
#include "simpleElectron.h"
#include "simpleMuon.h"
#include "simpleJet.h"
//#include "simplegenJet.h"
#include "mt2w_interface.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>


using namespace std;
using namespace ROOT::Math::VectorUtil;

//class simpleJet;
//class simpleMuon;
//class simpleElectron;
//class simpleGenJet;

class objectsTree: public subTree {
 public:

  objectsTree();
  objectsTree(TFile*, TString);
  objectsTree(TFile*,TDirectory*);
  ~objectsTree(){
    //delete all the pointers
    delete(el);
    delete(mu);
    delete(Jets);
    //    delete(genJets);
  }
  //
  void Constructor();
  void Fill(EventInfo* info, EasyChain* tree, vector<simpleMuon> muons, vector<simpleElectron> electrons, vector<simpleJet> jets, LorentzM met);
  //void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, vector<GenJet*>& genJets,LorentzM& met);
  void Fill(EventInfo* info, EasyChain* tree, vector<simpleMuon*>& muons, vector<simpleElectron*>& electrons, vector<simpleJet*>& jets, LorentzM& met);
  void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, LorentzM& met);
  virtual void Write();
  void SetTDir(TString);
  void SetTDir(TDirectory*);
  void SetTFile(TFile*);
  void SetBranches();
protected:

  void SetToZero(vector<double>* vec);
  TTree* mytree;
  TDirectory* dir;
  TFile* tfile;
  mt2w_bisect::mt2w_interface mt2w_calc;
  //
  int event;
  int run;
  LorentzM PFmet;
  LorentzM* pPFmet;
  //
  vector<simpleElectron>* el;
  vector<simpleMuon>* mu;
  vector<simpleJet>* Jets;
  //  vector<simpleGenJet>* genJets;
};

#endif
