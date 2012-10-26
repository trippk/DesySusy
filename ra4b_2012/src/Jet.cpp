
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "Jet.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

map<string, map<string, double> > Jet::bJetWP;

double Jet::BJetDisc(string key){return bJetDisc[key];};

int    Jet::GenFlavor()      {return genFlavor;};
bool   Jet::IsMatch()        {return isMatch;};
double Jet::ScaleCorrFactor(){return scaleCorrFactor;};
string Jet::Type()           {return type;};

bool   Jet::IsBJet(string key, double disc_cut){

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

bool   Jet::IsBJet(string key, string WP){

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


void Jet::SetGenFlavor(int genFlavor_In){
  genFlavor=genFlavor_In;
};
void Jet::SetIsMatch(bool isMatch_In){
  isMatch=isMatch_In;
};
void Jet::SetScaleCorrFactor(double scr_In){
  scaleCorrFactor=scr_In;
};
void Jet::SetType(string type_In){
  type=type_In;
};

void Jet::SetBJetDisc(string key, double value){
  bJetDisc[key]=value;
};

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

void Jet::Set(int maptotree_In, LorentzM * pmomuntum_In, double scaleCorrFactor_In, string type_In){
  
  AnalysisObject::Set(maptotree_In, pmomuntum_In);

  //SET SCALE CORRECTION FACTOR
  scaleCorrFactor=scaleCorrFactor_In;

  //SET THE TYPE
  type=type_In;
}


/*
void Jet::Set(int maptotree_In, LorentzM  momuntum_In, double scaleCorrFactor_In, string type_In){

  //SET THE MAP TO THE TREE
  maptotree=maptotree_In;

  //SET THE FOURVECTOR

  p4=momuntum_In;
  pp4=&p4;
  //SET SCALE CORRECTION FACTOR
  scaleCorrFactor=scaleCorrFactor_In;

  //SET THE TYPE
  type=type_In;

}
*/



map<string, map<string, double> > Jet::GetbJetWP(){return Jet::bJetWP;}


void Jet::SetCorrectionUncertainty(string name, double value){

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

double Jet::GetCorrectionUncertainty(string name){
  
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


double Jet::GetJetPt_Shifted(string name){

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
