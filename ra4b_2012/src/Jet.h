#ifndef Jet_h
#define Jet_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "AnalysisObject.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class Jet: public AnalysisObject {
  
 protected:
  string   genFlavor;
  bool     isMatch;
  double   scaleCorrFactor;
  string   type;
  vector<const char*> allBTags;
  map<const char*, double>  bJetDisc;
  static map<const char*, map<const char*, double> >* pbJetWP;

 public:
  Jet(){
    pp4=0;
    genFlavor="";
    maptotree=0;
    allIDs.clear();
    id.clear();
    type = "";
    isMatch=0;
    scaleCorrFactor=0.;
    bJetDisc.clear();
  }

  ~Jet(){}

  double BJetDisc(const char* key);
  string GenFlavor();
  bool   IsMatch();
  double ScaleCorrFactor();
  string Type();
  bool IsBJet(const char* key="CSV", double disc_cut=0.679);
  bool IsBJet(const char* key="CSV", const char* WP="Medium");

  void SetGenFlavor(string genFlavor_In);
  void SetIsMatch(bool isMatch_In);
  void SetScaleCorrFactor(double scr_In);
  void SetType(string type_In);
  void SetBJetDisc(const char* key, double value);
  static void SetWP(const char* cme="8TeV", map<const char*, map<const char*, double> >* bJetWP=0);

  virtual void Set(int maptotree_In, LorentzM * momuntum_In, double scaleCorrFactor_In=1., string type_In="");

};

#endif
