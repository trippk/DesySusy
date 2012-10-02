
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "Jet.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

map<string, map<string, double> >* Jet::pbJetWP=0;

double Jet::BJetDisc(string key){return bJetDisc[key];};

string Jet::GenFlavor()      {return genFlavor;};
bool   Jet::IsMatch()        {return isMatch;};
double Jet::ScaleCorrFactor(){return scaleCorrFactor;};
string Jet::Type()           {return type;};

bool   Jet::IsBJet(string key, double disc_cut){
  if( find(allBTags.begin(),allBTags.end(),key) == allBTags.end() ) {
    std::cout<<"Btagging discriminator not found!"<<std::endl;
    return false;
  }
  return bJetDisc[key]>disc_cut;
};

bool   Jet::IsBJet(string key, string WP){
  if(pbJetWP==0) {
    std::cout<<"BTagging WP table not set!"<<std::endl;
    return false;
  }
  
  //cout<<"in this jet "<<endl;
  //cout<<"the working point has been set to "<<Jet::GetbJetWP()["CSV"]["Medium"]<<endl;
  //cout<<"As a comparison, the pointer is"<<(*Jet::pbJetWP)["CSV"]["Medium"]<<endl;
  
  if((*Jet::pbJetWP)[key][WP]==0) {
    std::cout<<"the pointer "<<(*Jet::pbJetWP)[key][WP]<<endl;
    std::cout<<"BTagging WP not set!"<<std::endl;
    std::cout<<"using the key "<<key<<"of size"<<sizeof(key)<<endl;
    std::cout<<"WP given was "<<WP<<endl;
    std::cout<<(key=="CSV")<<" "<<(WP=="Medium")<<std::endl;
  return false;
  }
  return this->IsBJet(key,(*Jet::pbJetWP)[key][WP]);
};


void Jet::SetGenFlavor(string genFlavor_In){
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
  if( find(allBTags.begin(),allBTags.end(),key) == allBTags.end() ) allBTags.push_back(key);
  bJetDisc[key]=value;
};

void Jet::SetWP(string cme, map<string, map<string, double> >* bJetWP){
  pbJetWP=bJetWP;
  pbJetWP->clear();
  if(cme=="8TeV"){
    (*pbJetWP)["TCHP"]["Tight"] =3.41;

    (*pbJetWP)["CSV"]["Loose"] =0.244;
    (*pbJetWP)["CSV"]["Medium"]=0.679;
    (*pbJetWP)["CSV"]["Tight"] =0.898;

    (*pbJetWP)["JP"]["Loose"] =0.275;
    (*pbJetWP)["JP"]["Medium"]=0.545;
    (*pbJetWP)["JP"]["Tight"] =0.790;

    cout<<"here "<< (*pbJetWP)["CSV"]["Medium"]<<endl;
  }
  else if(cme=="7TeV"){
    (*pbJetWP)["TCHE"]["Loose"] = 1.7;
    (*pbJetWP)["TCHE"]["Medium"]= 3.3;
    (*pbJetWP)["TCHE"]["Tight"] =10.2;

    (*pbJetWP)["TCHP"]["Loose"] =1.19;
    (*pbJetWP)["TCHP"]["Medium"]=1.93;
    (*pbJetWP)["TCHP"]["Tight"] =3.41;

    (*pbJetWP)["CSV"]["Loose"] =0.244;
    (*pbJetWP)["CSV"]["Medium"]=0.679;
    (*pbJetWP)["CSV"]["Tight"] =0.898;

    (*pbJetWP)["JP"]["Loose"] =0.275;
    (*pbJetWP)["JP"]["Medium"]=0.545;
    (*pbJetWP)["JP"]["Tight"] =0.790;
  }
  else std::cout<<"BTagging WP table not set!"<<std::endl;

};

void Jet::Set(int maptotree_In, LorentzM * momuntum_In, double scaleCorrFactor_In, string type_In){

  //SET THE MAP TO THE TREE
  maptotree=maptotree_In;

  //SET THE FOURVECTOR
  pp4=momuntum_In;
  p4=*pp4;

  //SET SCALE CORRECTION FACTOR
  scaleCorrFactor=scaleCorrFactor_In;

  //SET THE TYPE
  type=type_In;

}


map<string, map<string, double> > Jet::GetbJetWP(){return *(Jet::pbJetWP);}



