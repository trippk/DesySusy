//#include "subTree.h"
#include "trigStudyTree.h"
#include "string.h"
#include "TFile.h"

using namespace std;


trigStudyTree::trigStudyTree(){
  tfile=0;
  dir=0;
  this->Constructor();
}


trigStudyTree::trigStudyTree(TFile* treefile, TString indir){
  tfile=treefile;
  tfile->cd();
  //cout<<"the tfile is "<<tfile<<endl;
  //cout<<"going to create the directory "<<indir<<endl;
  this->SetTDir(indir);
  //cout<<"I got the dir "<<dir<<endl;
  dir->cd();
  this->Constructor();
}


void trigStudyTree::Constructor(){
  initLeafs();
  
  mytree = new TTree("trigStudyTree","a subTree");  
  //muonP4 = new std::vector<LorentzM>();
  mytree->Branch("DESYtriggerElMatchedTriggerFilter",&DESYtriggerElMatchedTriggerFilter);
  mytree->Branch("DESYtriggerMuMatchedTriggerFilter",&DESYtriggerMuMatchedTriggerFilter);  
  mytree->Branch("DESYtriggerNameMap",&DESYtriggerNameMap);
  mytree->Branch("electronCharge",&electronCharge);
  mytree->Branch("electronP4", &electronP4);
  mytree->Branch("Event",&event,"event/I");
  mytree->Branch("HT",&HT,"HT/D");
  mytree->Branch("jetP4", &jetP4);
  mytree->Branch("MET",&MET,"MET/D");
  mytree->Branch("muonCharge",&muonCharge);
  mytree->Branch("muonP4", &muonP4);
  mytree->Branch("prescaled",&prescaled);
  mytree->Branch("PUInter",&PUInter);
  mytree->Branch("PUWeight",&PUWeight,"PUWeight/D");
  mytree->Branch("Run",&run,"run/I");
  mytree->Branch("triggered",&triggered);
  mytree->Branch("Weight",&weight,"weight/D");
}


// void trigStudyTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, LorentzM& met) {

void trigStudyTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Ptr_Jet>& jets, LorentzM& met) {
  vector<Jet*> newJets;
  for (int iel=0; iel<jets.size();++iel){
    newJets.push_back(jets.at(iel).get());
  }
  //
  Fill(info,tree,muons,electrons,newJets,met);
  //
}




void trigStudyTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons, vector<Electron*>& electrons, vector<Jet*>& jets, LorentzM& met) {
  //initialize the variables values; e.g. clear vectors
  initLeafs();
  DESYtriggerElMatchedTriggerFilter = info->ElMatchedTriggerFilter;
  DESYtriggerMuMatchedTriggerFilter = info->MuMatchedTriggerFilter;
  DESYtriggerNameMap                = info->TriggerMap;
  event                             = info->Event;
  MET                               = met.Et();
  prescaled                         = info->HLTprescaled;
  PUInter                           = info->PUInter;
  PUWeight                          = info->PUWeight;
  run                               = info->Run;
  triggered                         = info->HLTtrigger;
  weight                            = info->EventWeight;
  
  for(Int_t i=0,N=muons.size(); i<N; ++i){
    muonCharge.push_back(muons.at(i)->Charge());
    muonP4.push_back(muons.at(i)->P4());
  }
  for(Int_t i=0,N=electrons.size(); i<N; ++i){
    electronCharge.push_back(electrons.at(i)->Charge());
    electronP4.push_back(electrons.at(i)->P4());
  }
  for(Int_t i=0,N=jets.size(); i<N; ++i){
    jetP4.push_back(jets.at(i)->P4());
    HT+=jets.at(i)->Pt();
  }

  mytree->Fill();
};


void trigStudyTree::initLeafs(){
  DESYtriggerElMatchedTriggerFilter.clear();
  DESYtriggerMuMatchedTriggerFilter.clear();
  DESYtriggerNameMap               .clear();
  electronCharge                   .clear();
  electronP4                       .clear();
  event                            = 0;
  HT                               = 0.;
  jetP4                            .clear();
  MET                              = 0.;
  muonCharge                       .clear();
  muonP4                           .clear();
  prescaled                        .clear();
  PUInter                          = 0;
  PUWeight                         = 0.;
  run                              = 0;
  triggered                        .clear();
  weight                           = 0.;
};


void trigStudyTree::Write(){
  //  cout<<"at the moment of writing, dir is "<<dir<<endl;
  //  cout<<"dir name "<<dir->GetName()<<endl;
  //  cout<<"and the tree going to be written is "<<mytree<<endl;
  dir->cd();
  mytree->Write();
};


void trigStudyTree::SetTFile(TFile* infile){
  tfile=infile;
}


void trigStudyTree::SetTDir(TString newdir){
  //
  //
  //  cout<<"tokenizing "<<newdir<<endl;
  //
  //
  TObjArray* arr=newdir.Tokenize("/");
  TIter next(arr);
  TString* token;
  arr->Print();
  Int_t numsep=arr->GetEntries();
  
  TObjString* tok;
  //  cout<<"numsep "<<numsep<<endl;

  //if the path is simply "./"
  if(newdir==(TString)"./"){
    dir=(TDirectory*)tfile;
  }
  else{

    TDirectory* cdir=tfile;
    TString newpath="";
    if(numsep<2){cout<<"BAD FORM FOR NEW DIR. ERROR. DIR="<< newdir<<endl;}
    
    for(Int_t i=1; i<numsep;++i){
      
      tok=(TObjString*)(*arr)[i];    
      TString newsubdir=tok->GetName();
      newpath=newpath+newsubdir+(TString)+"/";
      //      cout<<"making the dir "<<newsubdir<<endl;
      bool okmkdir=cdir->mkdir(newsubdir);
      //      cout<<"result of mkdir is "<<okmkdir<<endl;
      //      cout<<"the current path is "<<newpath<<endl;
      if (!okmkdir){
	//======IT EXISTS, SO CD INTO IT AND REPEAT
	//	cout<<"so now I'm here"<<endl;
	//directory aleady exists!, go to it
	//	cout<<"trying to get "<<(TString)tok->GetName()<<endl;
	cdir=(TDirectory*)cdir->Get((TString)tok->GetName());
	//	cout<<"the directory already existed. Now I'm in "<<cdir->GetName()<<endl;
      }
      else{
	//IT DIDN'T EXIST (it does now), RETURN THE NEW DIR AND REPEAT
	cdir=(TDirectory*)cdir->Get((TString)newsubdir);
      }
    }
    
    //==========END OF THE LOOP, SET THE dir TO THE CURRENT DIR
    dir=cdir;
  }
}


