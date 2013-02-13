#ifndef systematics_RA4b_h
#define systematics_RA4b_h
#include <map>
#include <string>
#include "Jet.h"
#include <vector>
#include "./CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "./CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CutFlow.h"
#include "subTree.h"
#include "subTreeFactory.h"
#include "defaultTree.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"
#include <boost/shared_ptr.hpp>


using namespace ROOT::Math::VectorUtil;
typedef boost::shared_ptr<Jet> Ptr_Jet;
//
class Systematics{

private:

  std::map<std::string,bool> SysMap;
  bool Enabled;
  std::map<std::string, CutSet*> sysFlow;
  std::map<std::string, defaultTree*> sysDefaultTree;
  std::map<std::string, std::string > treeJetCollection;

  //this should be cleared once per event loop
  std::map<std::string, std::vector<Ptr_Jet> >  sysJet;
  std::map<std::string, double> sysMET;
  std::map<std::string, Ptr_LorentzM> sysMETVector;  
  std::map<std::string, double> sysHT;
  std::map<std::string, int> sysNumberOfbTags;

public:

  Systematics(){   
    Enabled=false;
  }
  ~Systematics(){
    if(Enabled){
      cout<<"removing the instance of systematics"<<endl;
      this->Reset();
      //typedef map<string,bool>::iterator map_it;
      //      for (map_it iter=this->GetSysMap().begin(); iter != this->GetSysMap().end(); iter++){
      //if(iter->second){    
      //	  GetsysDefaultTree(iter->first)->~defaultTree();
      //}
      //
      for (int i=0;i<this->vsrc.size();++i){
	delete(this->vsrc.at(i));
      }
      delete(this->total);
    }
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


  void SetsysJet(std::string,std::vector<Ptr_Jet>);
  std::vector<Ptr_Jet>& GetsysJet(std::string);
  
  void SetsysMET(std::string, double);
  double& GetsysMET(std::string);
  void SetsysMETVector(std::string, double px, double py);
  void SetsysMETVector(std::string name, LorentzM* vec_in);
  //
  Ptr_LorentzM GetsysMETVector(std::string);

  void SetsysHT(std::string, double);
  double& GetsysHT(std::string);
  CutSet* GetsysFlow(std::string);
  void CreatesysFlow(std::string);
  void AddTree(std::string, TFile* treefile);
  defaultTree* GetsysDefaultTree(std::string);
  
  //  JetCorrectionUncertainty* unc;
  JetCorrectionUncertainty* total;
  vector<JetCorrectionUncertainty*> vsrc;  

  void SetTreeJetCollection(std::string name1, std::string name2);
  std::vector<Ptr_Jet>& GetTreeJetCollection(std::string);

  int NumberOfbTags(std::string name){return sysNumberOfbTags[name];}

  void CalculateNumberOfbTags(std::string name, std::string jetname);



  
};

#endif
