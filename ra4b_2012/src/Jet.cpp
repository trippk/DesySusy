#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "Jet.h"
#include "genJet.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "typedefs.h"
#include "simpleJet.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

map<string, map<string, double> > Jet::bJetWP;


//=======CONSTRUCTORS=============//


Jet::Jet(){this->Initializer();}

Jet::Jet(const int maptotree_In, Ptr_LorentzM momentum_In, const double scaleCorrFactor_In, const string type_In){
  //CONSTRUCTOR WITH SHARED POINTER
  //the jet owns the lorentz vector
  this->Initializer();
  this->AnalysisObject::Set(maptotree_In,momentum_In);
  scaleCorrFactor=scaleCorrFactor_In;
  type=type_In;
}


Jet::Jet(const int maptotree_In, LorentzM* const momentum_In, const double scaleCorrFactor_In, const string type_In){
  //CONSTRUCTOR WITH NORMAL POINTER
  //the jet DOES NOT own the lorentz vector
  this->Initializer();
  this->Set(maptotree_In,momentum_In,scaleCorrFactor_In,type_In);
}


Jet::Jet(const int maptotree_In, LorentzM const momentum_In, const double scaleCorrFactor_In, const string type_In){
  //CONSTRUCTOR WITH LORENTZM OBJECT
  //the LORENTZM lives inside the jet
  this->Initializer();
  this->Set(maptotree_In,momentum_In,scaleCorrFactor_In,type_In);
}


//COPY CONSTUCTOR
Jet::Jet(Ptr_Jet copy){
  if (bJetWP.size() == 0)SetWP();
  this->AnalysisObject::CopyLorentzM(copy.get());
  this->copyJetStuff(copy.get());
}


Jet::Jet(Jet& copy){
  //COPY CONSTUCTOR WITH THE OBJECT
  if (bJetWP.size() == 0)SetWP();
  this->AnalysisObject::CopyLorentzM(&copy);
  this->copyJetStuff(copy);
}


void Jet::copyJetStuff(Jet* copy){
  maptotree=copy->maptotree;
  genFlavor=copy->genFlavor; 
  isMatch=copy->isMatch;
  bJetDisc=copy->bJetDisc;
  correctionUncertainty_UP=copy->correctionUncertainty_UP;
  correctionUncertainty_DOWN=copy->correctionUncertainty_DOWN;
  matchedGenJet=copy->matchedGenJet;
  id=copy->id;
  scaleCorrFactor=copy->scaleCorrFactor;
  type=copy->type;
}

void Jet::copyJetStuff(Jet& copy){
  this->copyJetStuff(&copy);
}




void Jet::Initializer(){
  if (bJetWP.size() == 0)SetWP();
  genFlavor=0;
  isMatch=0;
  scaleCorrFactor=0.;
  type = "";
  bJetDisc.clear();
  correctionUncertainty_UP=99;
  correctionUncertainty_DOWN=99;

}


double Jet::BJetDisc(const string key) const {return bJetDisc.at(key);};
int    Jet::GenFlavor()const      {return genFlavor;};
bool   Jet::IsMatch()const        {return isMatch;};
double Jet::ScaleCorrFactor()const {return scaleCorrFactor;};
string Jet::Type()const           {return type;};


bool   Jet::IsBJet(const string key, const double disc_cut) const {
  //cout<<"calling is bjet with the disc_cut = "<<disc_cut<<endl;
  //Find disc value in bJetDisc
  //cout<<"first"<<endl;
  map<string, double>::const_iterator itKey = bJetDisc.find(key);
  if (itKey == bJetDisc.end()) {
    std::cout<<"Btagging discriminator not found!"<<std::endl;
    cout<<"bJetDisc has size "<<bJetDisc.size()<<endl;
    cout<<"I was looking for "<<key<<endl;
    return false;
  }
  else {
    //Compare with cut.
    return (itKey->second)>disc_cut;
  }

};


bool Jet::IsBJet(const string key, const string WP) const{
  //Find the disc cut in the bJetWP table
  map<string, map<string, double> >::const_iterator itKey = bJetWP.find(key);
  if (itKey != bJetWP.end()) {
    map<string, double>::const_iterator itWP = itKey->second.find(WP);
    //cout<<"itWP found = "<<itWP->second<<endl;
    //If here, found disc cut. Compare with disc value.

    if (itWP != itKey->second.end()) return IsBJet(key, itWP->second);
  }else{
    cout<<"btagging itkey not found in bJetWP!"<<endl;
  }

  //If here the key or WP were not stored in bJetWP
  std::cout<<"BTagging WP not set!"<<std::endl;
  std::cout<<"using the key "<<key<<"of size"<<sizeof(key)<<endl;
  std::cout<<"WP given was "<<WP<<endl;
  std::cout<<(key=="CSV")<<" "<<(WP=="Medium")<<std::endl;
  return false;

};


void Jet::SetGenFlavor(const int genFlavor_In){
  genFlavor=genFlavor_In;
};
void Jet::SetIsMatch(const bool isMatch_In){
  isMatch=isMatch_In;
};
void Jet::SetScaleCorrFactor(const double scr_In){
  scaleCorrFactor=scr_In;
};
void Jet::SetType(const string type_In){
  type=type_In;
};

void Jet::SetBJetDisc(const string key, const double value){
  bJetDisc[key]=value;
};

//void Jet::SetWP(const string cme){
void Jet::SetWP(string cme){
  bJetWP.clear();

  if(cme=="8TeV"){
    cout << "Jet::SetWP >> Setting WP " << cme << endl;

    (bJetWP)["TCHP"]["Tight"] =3.41;

    (bJetWP)["CSV"]["Loose"] =0.244;
    (bJetWP)["CSV"]["Medium"]=0.679;
    (bJetWP)["CSV"]["Tight"] =0.898;

    (bJetWP)["JP"]["Loose"] =0.275;
    (bJetWP)["JP"]["Medium"]=0.545;
    (bJetWP)["JP"]["Tight"] =0.790;

    //cout<<"here "<< (bJetWP)["CSV"]["Medium"]<<endl;
  }
  else if(cme=="7TeV"){
    cout << "Jet::SetWP >> Setting WP " << cme << endl;

    (bJetWP)["TCHE"]["Loose"] = 1.7;
    (bJetWP)["TCHE"]["Medium"]= 3.3;
    (bJetWP)["TCHE"]["Tight"] =10.2;

    (bJetWP)["TCHP"]["Loose"] =1.19;
    (bJetWP)["TCHP"]["Medium"]=1.93;
    (bJetWP)["TCHP"]["Tight"] =3.41;

    (bJetWP)["CSV"]["Loose"] =0.244;
    (bJetWP)["CSV"]["Medium"]=0.679;
    (bJetWP)["CSV"]["Tight"] =0.898;

    (bJetWP)["JP"]["Loose"] =0.275;
    (bJetWP)["JP"]["Medium"]=0.545;
    (bJetWP)["JP"]["Tight"] =0.790;
  }
  else std::cout<<"Jet::SetWP >> ERROR : BTagging WP table not set!"<<std::endl;

};

void Jet::Set(const int maptotree_In, LorentzM * const pmomuntum_In, const double scaleCorrFactor_In, const string type_In){
  //set with normal pointer
  AnalysisObject::Set(maptotree_In, pmomuntum_In);
  //SET SCALE CORRECTION FACTOR
  scaleCorrFactor=scaleCorrFactor_In;
  //SET THE TYPE
  type=type_In;
}

void Jet::Set(const int maptotree_In, LorentzM pmomuntum_In, const double scaleCorrFactor_In, const string type_In){
  //set with lorentzm object
  AnalysisObject::Set(maptotree_In, pmomuntum_In);
  //SET SCALE CORRECTION FACTOR
  scaleCorrFactor=scaleCorrFactor_In;
  //SET THE TYPE
  type=type_In;
}
void Jet::Set(const int maptotree_In, Ptr_LorentzM pmomentum_In, const double scaleCorrFactor_In, const string type_In){
  //set with shared_ptr to lorentzM
  AnalysisObject::Set(maptotree_In, pmomentum_In);
  //SET SCALE CORRECTION FACTOR
  scaleCorrFactor=scaleCorrFactor_In;
  //SET THE TYPE
  type=type_In;
}



map<string, map<string, double> > Jet::GetbJetWP(){return bJetWP;}


void Jet::SetCorrectionUncertainty(const string name, const double value){

  if (name=="up" || name=="UP" || name == "Up"){
    correctionUncertainty_UP=value;
  }
  else if(name=="down" || name=="Down" || name == "DOWN"){
    correctionUncertainty_DOWN=value;
  }
  else{
    cout<<"from SetCorrectionUncertainty: WRONG NAME. ERROR"<<endl;
    correctionUncertainty_UP=10000000;
    correctionUncertainty_DOWN=10000000;
  }

}

double Jet::GetCorrectionUncertainty(const string name){
  
  if (name=="up" || name=="UP" || name == "Up"){
    return correctionUncertainty_UP;
  }
  else if(name=="down" || name=="Down" || name == "DOWN"){
    return correctionUncertainty_DOWN;
  }
  else{
    cout<<"from GetCorrectionUncertainty: WRONG NAME. ERROR"<<endl;
    return 10000000;
  }
}


double Jet::GetJetPt_Shifted(const string name){

 if (name=="up" || name=="UP" || name == "Up"){
   return this->Pt()*(1+correctionUncertainty_UP);
  }
  else if(name=="down" || name=="Down" || name == "DOWN"){
    return this->Pt()*(1-correctionUncertainty_DOWN);
  }
  else if(name=="noshift" || name=="central"){
    return this->Pt();
  }
  else{
    cout<<"from GetJetPt_Shifted: WRONG NAME. ERROR"<<endl;
    return -1;
  }


}


void Jet::SetPartner(Ptr_GenJet matchedGenJet_in){
  //matchedGenJet.reset(matchedGenJet_in);
  matchedGenJet=matchedGenJet_in;
}


//the lock returns a shared_ptr from a weak_ptr
Ptr_GenJet Jet::GetPartner(){return matchedGenJet.lock();}



simpleJet Jet::makeSimpleJet(){
  //it makes a simpleJet out of this
  
  simpleJet dummySimpleJet(maptotree,p4,scaleCorrFactor,type);
  dummySimpleJet.genFlavor=genFlavor;
  dummySimpleJet.isMatch=isMatch;
  dummySimpleJet.bJetDisc=bJetDisc;
  dummySimpleJet.correctionUncertainty_UP=correctionUncertainty_UP;
  dummySimpleJet.correctionUncertainty_DOWN=correctionUncertainty_DOWN;
  dummySimpleJet.jetPt_ShiftedUP=jetPt_ShiftedUP;
  dummySimpleJet.id=id;

  return dummySimpleJet;
}

