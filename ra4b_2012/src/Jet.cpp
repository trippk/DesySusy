
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "Jet.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

map<const char*, map<const char*, double> >* Jet::pbJetWP=0;



double Jet::BJetDisc(const char* key){return bJetDisc[key];};

string Jet::GenFlavor()      {return genFlavor;};
bool   Jet::IsMatch()        {return isMatch;};
double Jet::ScaleCorrFactor(){return scaleCorrFactor;};
string Jet::Type()           {return type;};

bool   Jet::IsBJet(const char* key, double disc_cut){
  if( find(allBTags.begin(),allBTags.end(),key) == allBTags.end() ) return bJetDisc[key]>disc_cut;
  return false;
};

bool   Jet::IsBJet(const char* key, const char* WP){
  if (pbJetWP==0) {
    std::cout<<"BTagging WP table not set!"<<std::endl;
    return false;
    }
  if( find(allBTags.begin(),allBTags.end(),key) == allBTags.end() ) return this->IsBJet(key,(*pbJetWP)[key][WP]);
  return false;
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

void Jet::SetBJetDisc(const char* key, double value){
  if( find(allBTags.begin(),allBTags.end(),key) == allBTags.end() ) allBTags.push_back(key);
  bJetDisc[key]=value;
};


//map<const char*, map<const char*, double> > Jet::bjetWP;


void Jet::SetWP(const char* cme, map<const char*, map<const char*, double> >* bJetWP){
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
  }
  else if(cme=="7TeV"){
    (*pbJetWP)["TCHE"]["Loose"] = 1.7;
    (*pbJetWP)["TCHE"]["Medium"]= 3.3;
    (*pbJetWP)["TCHE"]["Tight"] =10.2;

    (*pbJetWP)["TCHP"]["Loose"] =1.19;
    (*pbJetWP)["TCHP"]["Medium"]=1.93;
    (*pbJetWP)["TCHP"]["Tight"] =3.41;

    (*pbJetWP)["CVS"]["Loose"] =0.244;
    (*pbJetWP)["CVS"]["Medium"]=0.679;
    (*pbJetWP)["CVS"]["Tight"] =0.898;

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
