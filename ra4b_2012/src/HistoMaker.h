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

using namespace std;

#define delimiter " "
#define NMonitorJets 4

class HistoMaker {

 public:
  HistoMaker(const TString& pre="", const char* dlm=delimiter);
    ~HistoMaker(){
      if(autodump)  dumpToFile();
    };
  static double global_event_weight;
  
  static void setTFile(TFile *);
  void dumpToFile();
  void MakePlots( const TString&, vector<Muon*> muons, vector<Electron*> electrons, vector<Jet*> jets, LorentzM& met);
  bool autodump;
  
 protected:
  vector<TString> allCuts;
  TString prefix;
  TString delim;

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
  static TFile* tfile;
  static TDirectory* cplotdir;
  //TDirectory* tdir;

};

#endif
