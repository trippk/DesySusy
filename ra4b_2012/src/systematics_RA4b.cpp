#include "systematics_RA4b.h"
#include <map>
#include <string>
#include <iostream>
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"
#include <boost/shared_ptr.hpp>
#include "typedefs.h"
using namespace std;
using namespace ROOT::Math::VectorUtil;


void Systematics::Reset(){

  //cout<<"inside of reset"<<endl;
  sysJet.clear();
  //cout<<"jets cleared"<<endl;
  sysMET.clear();
  //cout<<"sysmet cleared"<<endl;
  sysHT.clear();
  //cout<<"sysHT cleared"<<endl;
  passCuts.clear();
  //  cout<<"passcuts cleared"<<endl;
  sysMETVector.clear();
  //cout<<"sysmetvector cleared"<<endl;

}
  //================================================
  //typedef boost::shared_ptr<Jet> Ptr_Jet;
  

  //typedef map<string, vector<Jet*> >::iterator map_it;
  //prevent memory leaks of the Jet objects here:
  /*  for (map_it iter=sysJet.begin(); iter != sysJet.end(); ++iter){
    //cout<<"considering "<<iter->first<<endl;
    int pos=iter->first.find("_");
    if (pos==string::npos){
      //cout<<"    reseting "<<iter->first<<endl;
      for (int i=0 ; i< (int)(iter->second).size(); ++ i){
	//cout<<"reseting at "<<i<<" a pointer with adress "<<iter->second.at(i)<<endl;
	delete((iter->second).at(i));
      }
    }
  */
  //clear the vector
  // (iter->second).clear();

  

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


void Systematics::SetsysJet(string name, vector<Ptr_Jet> vec){
  sysJet[name]=vec;
}

vector<Ptr_Jet>& Systematics::GetsysJet(string name){
  return sysJet[name];
}

void Systematics::SetsysMET(string name, double newMET){
  sysMET[name]=newMET;
}
double& Systematics::GetsysMET(string name){
  return sysMET.at(name);
}
void Systematics::SetsysMETVector(string name, double px, double py){
  LorentzM* test =new LorentzM();
  test->SetPxPyPzE(px,py,0.,sqrt(px*px+py*py));
  Ptr_LorentzM dummypointer(test);
  sysMETVector[name]=dummypointer;
}
void Systematics::SetsysMETVector(string name, LorentzM* vec_in ){
  Ptr_LorentzM dummypointer(vec_in);
  sysMETVector[name]=dummypointer;
}
Ptr_LorentzM Systematics::GetsysMETVector(string name){
  return sysMETVector[name];
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

void Systematics::SetTreeJetCollection(string name, string jetcoll){
  treeJetCollection[name]=jetcoll;
}
std::string Systematics::GetTreeJetCollection(string name){
  return treeJetCollection.at(name);
}
