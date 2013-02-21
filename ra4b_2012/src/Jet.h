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
  //  static void SetWP(const string cme="8TeV");
  static void SetWP(string cme="8TeV");
  virtual void Set(const int maptotree_In, LorentzM * const momuntum_In, const double scaleCorrFactor_In=1., const string type_In="");
  virtual void Set(const int maptotree_In, Ptr_LorentzM momentum_In, const double scaleCorrFactor_In=1., const string type_In="");

  Jet(){
    if (bJetWP.size() == 0)SetWP();
    genFlavor=0;
    isMatch=0;
    scaleCorrFactor=0.;
    type = "";
    bJetDisc.clear();
    correctionUncertainty_UP=99;
    correctionUncertainty_DOWN=99;
    //jetPt_ShiftedUP=0;
    //jetPt_ShiftedDOWN=0;
  }

  //CONSTRUCTOR WITH SHARED POINTER
  //the jet owns the lorentz vector
  Jet(const int maptotree_In, Ptr_LorentzM momentum_In, const double scaleCorrFactor_In=1., const string type_In=""){
    if (bJetWP.size() == 0)SetWP();
    genFlavor=0;
    isMatch=0;
    bJetDisc.clear();
    correctionUncertainty_UP=99;
    correctionUncertainty_DOWN=99;
    //matchedGenJet=0;
    ownsP4=true;
    this->AnalysisObject::Set(maptotree_In,momentum_In);
    scaleCorrFactor=scaleCorrFactor_In;
    type=type_In;
  }

  //CONSTRUCTOR WITH NORMAL POINTER
  //the jet does not own the lorent vector
  Jet(const int maptotree_In, LorentzM * const momentum_In, const double scaleCorrFactor_In=1., const string type_In=""){
    //this->Jet();
    if (bJetWP.size() == 0)SetWP();
    genFlavor=0;
    isMatch=0;
    scaleCorrFactor=0.;
    type = "";
    //cout<<"second constructor"<<endl;
    bJetDisc.clear();
    correctionUncertainty_UP=99;
    correctionUncertainty_DOWN=99;
    this->Set(maptotree_In,momentum_In,scaleCorrFactor_In,type_In);
    //cout<<"the jet at "<<this<<" was constructed with the external pointer"<<endl;
  }


  //COPY CONSTUCTOR
  Jet(Ptr_Jet copy){
    if (bJetWP.size() == 0)SetWP();
    genFlavor=copy->genFlavor;
    isMatch=copy->isMatch;
    bJetDisc=copy->bJetDisc;
    correctionUncertainty_UP=copy->correctionUncertainty_UP;
    correctionUncertainty_DOWN=copy->correctionUncertainty_DOWN;
    matchedGenJet=copy->matchedGenJet;
    //p4=copy->p4;
    if(!copy->ownsP4){
      this->Set(copy->maptotree,copy->pp4_original,copy->scaleCorrFactor,copy->type);      
      ownsP4=false;
    }
    else{
      //the Jet owns the lorentz vector 
      this->AnalysisObject::Set(copy->maptotree,copy->owned_pp4_original);
      ownsP4=true;
      scaleCorrFactor=copy->scaleCorrFactor;
      type=copy->type;
    }
    id=copy->id;
    //cout<<"leaving the Jet* constructor"<<endl;  }
  }
  ~Jet(){}

  double BJetDisc(const string key) const;
  int GenFlavor() const;
  bool   IsMatch() const;
  double ScaleCorrFactor()const;
  string Type()const;
  bool IsBJet(const string key="CSV", const double disc_cut=0.679) const ;
  bool IsBJet(const string key="CSV", const string WP="Medium") const;
  //  bool IsBJet(char* key="CSV", char* WP);
  void SetGenFlavor(const int genFlavor_In);
  void SetIsMatch(const bool isMatch_In);
  void SetScaleCorrFactor(const double scr_In);
  void SetType(const string type_In);
  void SetBJetDisc(const string key, const double value);


  //  virtual void Set(int maptotree_In, LorentzM momuntum_In, double scaleCorrFactor_In=1., string type_In="");
  map<string, map<string, double> > GetbJetWP();
  void SetCorrectionUncertainty(const string name, double const value);
  double GetCorrectionUncertainty(const string name);
  double GetJetPt_Shifted(const string name);
  void SetPartner(Ptr_GenJet GenJet_in);
  Ptr_GenJet GetPartner();

};


#endif
