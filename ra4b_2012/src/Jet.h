#ifndef Jet_h
#define Jet_h
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "AnalysisObject.h"
#include "genJet.h"
#include <boost/shared_ptr.hpp>
#include "typedefs.h"


using namespace std;
using namespace ROOT::Math::VectorUtil;
//FORWARD declaration of GenJet to avoid infinite recursion
class GenJet;

class Jet: public AnalysisObject {
  
 protected:
  int      genFlavor;
  bool     isMatch;
  double   scaleCorrFactor;
  string   type;
  
  //weak pointer to prevent circular reference
  WPtr_GenJet matchedGenJet;

  static map<string, map<string, double> > bJetWP;
  map<string, double>  bJetDisc;

  double correctionUncertainty_UP;
  double correctionUncertainty_DOWN;
  double jetPt_ShiftedUP;
  
 public:
  map<string, map<string, double> > GetbJetWP();

  Jet();
  Jet(const int maptotree_In, Ptr_LorentzM momentum_In, const double scaleCorrFactor_In=1., const string type_In="");
  Jet(const int maptotree_In, LorentzM* const momentum_In, const double scaleCorrFactor_In=1., const string type_In="");
  Jet(const int maptotree_In, LorentzM const momentum_In, const double scaleCorrFactor_In=1., const string type_In="");
  Jet(Ptr_Jet copy);
  Jet(Jet& copy);

  double BJetDisc(const string key) const;
  int GenFlavor() const;
  Ptr_GenJet GetPartner();
  void Initializer();
  bool   IsMatch() const;
  bool IsBJet(const string key="CSV", const double disc_cut=0.679) const ;
  bool IsBJet(const string key="CSV", const string WP="Medium") const;
  //  bool IsBJet(char* key="CSV", char* WP);
  double ScaleCorrFactor()const;
  virtual void Set(const int maptotree_In, LorentzM * const momentum_In, const double scaleCorrFactor_In=1., const string type_In="");
  virtual void Set(const int maptotree_In, Ptr_LorentzM momentum_In, const double scaleCorrFactor_In=1., const string type_In="");
  void SetGenFlavor(const int genFlavor_In);
  void SetIsMatch(const bool isMatch_In);
  void SetScaleCorrFactor(const double scr_In);
  void SetType(const string type_In);
  void SetBJetDisc(const string key, const double value);
  static void SetWP(string cme="8TeV");
  void SetCorrectionUncertainty(const string name, double const value);
  double GetCorrectionUncertainty(const string name);
  double GetJetPt_Shifted(const string name);
  void SetPartner(Ptr_GenJet GenJet_in);
  string Type()const;
};


#endif
