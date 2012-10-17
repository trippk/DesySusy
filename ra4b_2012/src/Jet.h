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
  vector<string> allBTags;
  map<string, double>  bJetDisc;
  static map<string, map<string, double> >* pbJetWP;
  double correctionUncertainty_UP;
  double correctionUncertainty_DOWN;
  double jetPt_ShiftedUP;
  
 public:
  Jet(){
    genFlavor="";
    isMatch=0;
    scaleCorrFactor=0.;
    type = "";
    bJetDisc.clear();
    correctionUncertainty_UP=99;
    correctionUncertainty_DOWN=99;
    //jetPt_ShiftedUP=0;
    //jetPt_ShiftedDOWN=0;
  }
  Jet(int maptotree_In, LorentzM  *momuntum_In, double scaleCorrFactor_In=1., string type_In=""){
    //this->Jet();
    genFlavor="";
    isMatch=0;
    scaleCorrFactor=0.;
    type = "";
    bJetDisc.clear();
    correctionUncertainty_UP=99;
    correctionUncertainty_DOWN=99;
    this->Set(maptotree_In,momuntum_In,scaleCorrFactor_In,type_In);
  }

  ~Jet(){}

  double BJetDisc(string key);
  string GenFlavor();
  bool   IsMatch();
  double ScaleCorrFactor();
  string Type();
  bool IsBJet(string key="CSV", double disc_cut=0.679);
  bool IsBJet(string key="CSV", string WP="Medium");
  //  bool IsBJet(char* key="CSV", char* WP);


  void SetGenFlavor(string genFlavor_In);
  void SetIsMatch(bool isMatch_In);
  void SetScaleCorrFactor(double scr_In);
  void SetType(string type_In);
  void SetBJetDisc(string key, double value);
  static void SetWP(string cme="8TeV", map<string, map<string, double> >* bJetWP=0);
  virtual void Set(int maptotree_In, LorentzM * momuntum_In, double scaleCorrFactor_In=1., string type_In="");
  //  virtual void Set(int maptotree_In, LorentzM momuntum_In, double scaleCorrFactor_In=1., string type_In="");
  static map<string, map<string, double> > GetbJetWP();

  void SetCorrectionUncertainty(string name, double value);
  double GetCorrectionUncertainty(string name);

  double GetJetPt_Shifted(string name);


};

#endif
