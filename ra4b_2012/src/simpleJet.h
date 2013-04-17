#ifndef simpleJet_h
#define simpleJet_h
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "simpleAnalysisObject.h"
#include "simplegenJet.h"
using namespace std;
using namespace ROOT::Math::VectorUtil;
//FORWARD declaration of GenJet to avoid infinite recursion
//class simpleGenJet;



class Jet;
class simpleJet: public simpleAnalysisObject {
  
 protected:
  int      genFlavor;
  bool     isMatch;
  double   scaleCorrFactor;
  string   type;
  
  //simpleGenJet* matchedGenJet;
  int matchedGenJet;


  //int matchedGenJet;


  static map<string, map<string, double> > bJetWP;
  map<string, double>  bJetDisc;

  double correctionUncertainty_UP;
  double correctionUncertainty_DOWN;
  double jetPt_ShiftedUP;
  
  friend class Jet;
 public:
  map<string, map<string, double> > GetbJetWP();

  simpleJet();
  simpleJet(const int maptotree_In, LorentzM const momentum_In, const double scaleCorrFactor_In=1., const string type_In="");
  //simpleJet(simpleJet& copy);
  ~simpleJet();

  double BJetDisc(const string key) const;
  int GenFlavor() const;
  int GetPartner();
  void Initializer();
  bool IsMatch() const;
  bool IsBJet(const string key="CSV", const double disc_cut=0.679) const ;
  bool IsBJet(const string key, const string WP) const;

  double ScaleCorrFactor()const;
  virtual void Set(const int maptotree_In, LorentzM momentum_In, const double scaleCorrFactor_In=1., const string type_In="");
  void SetGenFlavor(const int genFlavor_In);
  void SetIsMatch(const bool isMatch_In);
  void SetScaleCorrFactor(const double scr_In);
  void SetType(const string type_In);
  void SetBJetDisc(const string key, const double value);
  static void SetWP(string cme="8TeV");
  void SetCorrectionUncertainty(const string name, double const value);
  double GetCorrectionUncertainty(const string name);
  double GetJetPt_Shifted(const string name);
  void SetPartner(int GenJet_in);
  string Type()const;

  ClassDef(simpleJet,1)
};


#endif
