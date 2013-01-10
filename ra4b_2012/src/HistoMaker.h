#ifndef HistoMaker_h
#define HistoMaker_h

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include "TString.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2D.h"
#include "NtupleTools2_h.h"
#include "Tools.h"
#include "Muon.h"
#include "Electron.h"
#include "Jet.h"
#include "TFile.h"
#include "mt2w_interface.h"
#include <boost/shared_ptr.hpp>
#include "typedefs.h"
using namespace std;

#define delimiter " "
//#define NMonitorJets 4


class HistoMaker {

 public:
  HistoMaker(const TString& pre="", const char* dlm=delimiter);
  ~HistoMaker(){
    if(autodump)  dumpToFile();
  };
  static double global_event_weight;
  
  void setTFile(TFile *);
  void setDir(TDirectory *);

  void dumpToFile();
  void MakePlots( const TString&, vector<Muon*> muons, vector<Electron*> electrons, vector<Ptr_Jet> jets, LorentzM& met);
  void MakePlots( const TString&, vector<Muon*> muons, vector<Electron*> electrons, vector<Jet*> jets, LorentzM& met);
  bool autodump;
  
 protected:
  static const int NMonitorJets=4;

  vector<TString> allCuts;
  TString prefix;
  TString delim;

  mt2w_bisect::mt2w_interface mt2w_calc;

  map<TString, TH1D*> mt2wEle;
  map<TString, TH1D*> mt2wMu;

  map<TString, TH1D*> MuPt;
  map<TString, TH1D*> MuEta;

  map<TString, TH1D*> ElePt;
  map<TString, TH1D*> EleEta;

  map<TString, TH1D*> NJets;
  map<TString, TH1D*> PtAllJets;
  map<TString, TH1D*> PtJet[NMonitorJets];

  map<TString, TH1D*> NBJets;
  map<TString, TH1D*> PtAllBJets;
  map<TString, TH1D*> PtBJet[NMonitorJets];
  map<TString, TH1D*> BDisc;
  
  map<TString, TH1D*> HT;
  map<TString, TH1D*> MHT;
  map<TString, TH1D*> MET;
  map<TString, TH1D*> YMET;
  
  map<TString, TH2D*> HT_YMET;

  map<TString, TH1D*> MET_ABCD[4];
  map<TString, TH1D*> NJets_ABCD[4];
  map<TString, TH1D*> PtAllJets_ABCD[4];
  map<TString, TH1D*> PtJet_ABCD[NMonitorJets][4];

  // ROOT related
  TFile* tfile;
  TDirectory* cplotdir;
  //TDirectory* tdir;

};

#endif
