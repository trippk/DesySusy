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
#include <boost/shared_ptr.hpp>
#include "typedefs.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class defaultTree: public subTree {
 public:

  defaultTree();
  defaultTree(TFile*, TString);
  defaultTree(TFile*,TDirectory*);
  ~defaultTree(){
    //delete all the pointers
    delete(el);
    delete(mu);
    delete(Jets);
  }
  //
  void Constructor();
  //void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, double& met);
  void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, LorentzM& met);
  void Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Ptr_Jet>& jets_in, LorentzM& met_in);
  //
  virtual void Write();
  void SetTDir(TString);
  void SetTDir(TDirectory*);
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
  double PUWeight_up;
  double PUWeight_down;
  int NPV;



  vector<LorentzM>* el;
  vector<LorentzM>* mu;
  vector<LorentzM>* Jets;


 };

#endif
