#ifndef event_h
#define event_h

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "eventselection.h"
#include "SampleInformation.h"
#include "makeElectrons.h"
#include "makeMuons.h"
#include "triggers_RA4b.h"
#include "triggers_RA4b_frequency.h"
#include "vertices_RA4b.h"
#include "metAndHT_RA4b.h"
#include "evtqual_RA4b.h"
#include "SetTriggers_RA4b.h"
#include "SetConditions_RA4b.h"
#include "cschalo_RA4b.h"
#include "trackingFailure_RA4b.h"
#include "THTools.h"
#include "TStopwatch.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "makeJets.h"
#include "HistoMaker.h"
#include "subTree.h"
#include "subTreeFactory.h"
#include "EventInfo.h"
#include "makeOsLepPairs.h"

extern bool pcp;

class event {
 public:
  event(EasyChain * tree_in);
  ~event();

  bool setSampleInformation();

  void setCutFlow(CutSet* cutSet_in) {globalflow = cutSet_in;}
  void setSubTree(subTree* subTree_in) {outTree = subTree_in;}
  void setControlPlots(HistoMaker* histMaker_in) {ControlPlots = histMaker_in;}

  void getEntry(int i);

  void doPU_RW();
  void createObjects();
  bool applyCuts(const std::vector<std::string> & cutVec);
  void fillTree();

 private:
  bool applyCut(const std::string & cutName);
  bool chkOsFlavTrigger();
  void resetAll();

  vector<const char*> triggernames;
  vector<double> PUmc;
  vector<double> PUdata;

  bool doControlPlots;
  CutSet * globalflow;
  HistoMaker * ControlPlots;
  subTree * outTree;

  EasyChain * tree;

  bool isSignalScanSample;
  float mY;
  float mLsp;

  bool isData;
  bool turntriggersoff;
  double PUWeight;
  double EventWeight;

  unsigned int EventNum;
  unsigned int RunNum;

  std::vector<Muon> Muons;
  std::vector<Muon*> LooseMuons;
  std::vector<Muon*> TightMuons;
  std::vector<Muon*> VetoMuons;

  std::vector<Electron>  Electrons;
  std::vector<Electron*> LooseElectrons;
  std::vector<Electron*> TightElectrons;
  std::vector<Electron*> VetoElectrons;
  std::vector<Electron*> CleanedTightElectrons;
  std::vector<Electron*> CleanedVetoElectrons;
  std::vector<Muon> TrkOrGlobalMuons;

  vector<Jet>  Jets;
  vector<Jet*> AllJets;
  vector<Jet*> GoodJets;
  vector<Jet*> CleanedJets;

  vector<Muon*> SignalMuons;
  vector<Muon*> WideMuons;

  vector<Electron*> SignalElectrons;
  vector<Electron*> WideElectrons;

  std::list<osPair> osPairs;
  std::map<std::string, bool> trigsFiredMap; //Used for diLep triggers
  std::set<diLepEvent_t> firedDiLepTypes; //Used for diLep

  double HT;
  LorentzM PFmetRaw;
  LorentzM PFmetType1;

};

#endif
