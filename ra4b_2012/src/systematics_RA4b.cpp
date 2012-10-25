#include "systematics_RA4b.h"
#include <map>
#include <string>
#include <iostream>
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"
using namespace std;
using namespace ROOT::Math::VectorUtil;


void Systematics::Reset(){


  //================================================
  typedef map<string, vector<Jet*> >::iterator map_it;
  //prevent memory leaks of the Jet objects here:
  for (map_it iter=sysJet.begin(); iter != sysJet.end(); ++iter){
    //cout<<"considering "<<iter->first<<endl;
    int pos=iter->first.find("_");
    if (pos==string::npos){
      //      cout<<"    reseting "<<iter->first<<endl;
      for (int i=0 ; i< (int)(iter->second).size(); ++ i){
	//      cout<<"reseting at "<<i<<" a pointer with adress "<<iter->second.at(i)<<endl;
	delete((iter->second).at(i));
      }
    }
    //clear the vector
    (iter->second).clear();
  }


  //delete the lorentz vectors
  typedef map<string, LorentzM*>::iterator l_it;
  for(l_it iter =sysMEVector.begin(); iter != sysMEVector.end(); ++iter){
    delete(iter->second);
  }
  //now clear the maps
  sysJet.clear();
  sysMET.clear();
  sysHT.clear();
  passCuts.clear();
  sysMEVector.clear();
  
}

map<string,bool>& Systematics::GetSysMap(){
  return SysMap;
}

bool Systematics::DoJetUp(){
  return SysMap["jetup"];
}
bool Systematics::DoJetDown(){return SysMap["jetdown"];}

void Systematics::AddUncertainty(string sysname, bool sysval){

  SysMap[sysname]=sysval;
  if(!Enabled)Enabled=true;

  //==CREATE A NEW CUTFLOW
  this->CreatesysFlow(sysname);

}

void Systematics::AddUncertainty(string sysname, TFile* treefile, bool sysval ){


  this->AddUncertainty(sysname,sysval);

  //==CREATE A NEW TREE
  this->AddTree(sysname,treefile);
}

void Systematics::AddTree(string sysname, TFile* treefile){
  
  sysDefaultTree[sysname]=(defaultTree*)subTreeFactory::NewTree((TString)"default",treefile,(TString)"Systematics/"+(TString)sysname);
  //  sysTree[sysname]->SetTFile(treefile);
  //  sysTree[sysname]->SetTDir("Systematics/"+sysname);
}


void Systematics::Print(){
  cout<<"SYSTEMATIC UNCERTAINTIES: ON"<<endl;


  cout<<"SYSTEMATIC UNCERTAINTIES: OFF"<<endl;
}

bool Systematics::IsEnabled(){return Enabled;}


void Systematics::SetsysJet(string name, vector<Jet*> vec){
  sysJet[name]=vec;
}

vector<Jet*>& Systematics::GetsysJet(string name){
  return sysJet[name];
}

void Systematics::SetsysMET(string name, double newMET){
  sysMET[name]=newMET;
}
double& Systematics::GetsysMET(string name){
  return sysMET[name];
}
void Systematics::SetsysMEVector(string name, double px, double py){
  
  sysMEVector[name]= new LorentzM(px,py,0, sqrt(px*px+py*py));
}

LorentzM* Systematics::GetsysMEVector(string name){
  return sysMEVector[name];
}

void Systematics::SetsysHT(string name, double newHT){
  sysHT[name]=newHT;
}
double& Systematics::GetsysHT(string name){
  return sysHT[name];
}

CutSet* Systematics::GetsysFlow(string name){
  return sysFlow[name];
}
void Systematics::CreatesysFlow(string name){
  sysFlow[name]=new CutSet((TString)name);
}

defaultTree* Systematics::GetsysDefaultTree(string name){
  return sysDefaultTree.at(name);
}
