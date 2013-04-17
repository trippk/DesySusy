#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "simpleJet.h"
#include "simplegenJet.h"
using namespace std;
using namespace ROOT::Math::VectorUtil;
ClassImp(simpleJet);


map<string, map<string, double> > simpleJet::bJetWP;


//=======CONSTRUCTORS=============//


simpleJet::simpleJet(){this->Initializer();}


simpleJet::simpleJet(const int maptotree_In, LorentzM const momentum_In, const double scaleCorrFactor_In, const string type_In){
  //CONSTRUCTOR WITH LORENTZM OBJECT
  //the LORENTZM lives inside the jet
  this->Initializer();
  this->Set(maptotree_In,momentum_In,scaleCorrFactor_In,type_In);
}

/*
simpleJet::simpleJet(simpleJet& copy){
  //COPY CONSTUCTOR WITH THE OBJECT
  if (bJetWP.size() == 0)SetWP();
  genFlavor=copy.genFlavor;
  isMatch=copy.isMatch;
  bJetDisc=copy.bJetDisc;
  correctionUncertainty_UP=copy.correctionUncertainty_UP;
  correctionUncertainty_DOWN=copy.correctionUncertainty_DOWN;
  matchedGenJet=copy.matchedGenJet;
  id=copy.id;
  scaleCorrFactor=copy.scaleCorrFactor;
  type=copy.type;
  maptotree=copy.maptotree;
  p4=copy.p4;
  pp4=&p4;

}
*/

simpleJet::~simpleJet(){}


void simpleJet::Initializer(){
  if (bJetWP.size() == 0)SetWP();
  genFlavor=0;
  isMatch=0;
  scaleCorrFactor=0.;
  type = "";
  bJetDisc.clear();
  correctionUncertainty_UP=99;
  correctionUncertainty_DOWN=99;

}


double simpleJet::BJetDisc(const string key) const {return bJetDisc.at(key);};
int    simpleJet::GenFlavor()const      {return genFlavor;};
bool   simpleJet::IsMatch()const        {return isMatch;};
double simpleJet::ScaleCorrFactor()const {return scaleCorrFactor;};
string simpleJet::Type()const           {return type;};


bool   simpleJet::IsBJet(const string key, const double disc_cut) const {
  //cout<<"calling is bjet with the disc_cut = "<<disc_cut<<endl;
  //Find disc value in bJetDisc
  map<string, double>::const_iterator itKey = bJetDisc.find(key);
  if (itKey == bJetDisc.end()) {
    std::cout<<"Btagging discriminator not found!"<<std::endl;
    return false;
  }
  else {
    //Compare with cut.
    return (itKey->second)>disc_cut;
  }

};


bool simpleJet::IsBJet(const string key, const string WP) const{

  //Find the disc cut in the bJetWP table
  map<string, map<string, double> >::const_iterator itKey = bJetWP.find(key);
  if (itKey != bJetWP.end()) {
    map<string, double>::const_iterator itWP = itKey->second.find(WP);
    //If here, found disc cut. Compare with disc value.

    if (itWP != itKey->second.end()) return IsBJet(key, itWP->second);
  }

  //If here the key or WP were not stored in bJetWP
  std::cout<<"BTagging WP not set!"<<std::endl;
  std::cout<<"using the key "<<key<<"of size"<<sizeof(key)<<endl;
  std::cout<<"WP given was "<<WP<<endl;
  std::cout<<(key=="CSV")<<" "<<(WP=="Medium")<<std::endl;
  return false;

};


void simpleJet::SetGenFlavor(const int genFlavor_In){
  genFlavor=genFlavor_In;
};
void simpleJet::SetIsMatch(const bool isMatch_In){
  isMatch=isMatch_In;
};
void simpleJet::SetScaleCorrFactor(const double scr_In){
  scaleCorrFactor=scr_In;
};
void simpleJet::SetType(const string type_In){
  type=type_In;
};

void simpleJet::SetBJetDisc(const string key, const double value){
  bJetDisc[key]=value;
};

//void simpleJet::SetWP(const string cme){
void simpleJet::SetWP(string cme){
  bJetWP.clear();

  if(cme=="8TeV"){
    cout << "simpleJet::SetWP >> Setting WP " << cme << endl;

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
    cout << "simpleJet::SetWP >> Setting WP " << cme << endl;

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
  else std::cout<<"simpleJet::SetWP >> ERROR : BTagging WP table not set!"<<std::endl;

};

void simpleJet::Set(const int maptotree_In, LorentzM pmomuntum_In, const double scaleCorrFactor_In, const string type_In){
  //set with lorentzm object
  simpleAnalysisObject::Set(maptotree_In, pmomuntum_In);
  //SET SCALE CORRECTION FACTOR
  scaleCorrFactor=scaleCorrFactor_In;
  //SET THE TYPE
  type=type_In;
}

map<string, map<string, double> > simpleJet::GetbJetWP(){return bJetWP;}


void simpleJet::SetCorrectionUncertainty(const string name, const double value){

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

double simpleJet::GetCorrectionUncertainty(const string name){
  
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


double simpleJet::GetJetPt_Shifted(const string name){

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


void simpleJet::SetPartner(int matchedGenJet_in){
  //matchedGenJet.reset(matchedGenJet_in);
  matchedGenJet=matchedGenJet_in;
}

int simpleJet::GetPartner(){return matchedGenJet;}
