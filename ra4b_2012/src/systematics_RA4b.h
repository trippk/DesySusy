#ifndef systematics_RA4b_h
#define systematics_RA4b_h
#include <map>
#include <string>
#include "Jet.h"
#include <vector>
#include "CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/SimpleJetCorrectionUncertainty.h"
#include "CutFlow.h"
#include "subTree.h"
#include "subTreeFactory.h"
#include "defaultTree.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"

using namespace ROOT::Math::VectorUtil;
//
class Systematics{

private:


  std::map<std::string,bool> SysMap;
  bool Enabled;
  std::map<std::string, CutSet*> sysFlow;
  std::map<std::string, defaultTree*> sysDefaultTree;

  //this should be cleared once per event loop
  std::map<std::string, std::vector<Jet*> >  sysJet;
  std::map<std::string, double> sysMET;
  std::map<std::string, LorentzM*> sysMEVector;  
  std::map<std::string, double> sysHT;

public:

  Systematics(){   
    Enabled=false;
  }

  std::map<std::string,bool>& GetSysMap();
  std::map<std::string, bool> passCuts;
  void Reset();
  bool DoJetUp();
  bool DoJetDown();
  void AddUncertainty(std::string,bool sysval=true);
  void AddUncertainty(std::string,TFile* treefile, bool sysval=true);
  void Print();
  bool IsEnabled();


  void SetsysJet(std::string,std::vector<Jet*>);
  std::vector<Jet*>& GetsysJet(std::string);
  
  void SetsysMET(std::string, double);
  double& GetsysMET(std::string);
  void SetsysMEVector(std::string, double px, double py);
  LorentzM* GetsysMEVector(std::string);

  void SetsysHT(std::string, double);
  double& GetsysHT(std::string);
  CutSet* GetsysFlow(std::string);
  void CreatesysFlow(std::string);
  void AddTree(std::string, TFile* treefile);
  defaultTree* GetsysDefaultTree(std::string);
  
  //  JetCorrectionUncertainty* unc;
  JetCorrectionUncertainty* total;
  vector<JetCorrectionUncertainty*> vsrc;  

};

#endif
