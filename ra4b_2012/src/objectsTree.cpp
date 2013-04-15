//#include "subTree.h"
#include "defaultTree.h"
#include "string.h"
#include "TFile.h"
#include "objectsTree.h"
#include "simpleElectron.h"
#include "simpleMuon.h"
#include "simpleJet.h"

using namespace std;


objectsTree::objectsTree(){

  tfile=0;
  dir=0;
  this->Constructor();

}

objectsTree::objectsTree(TFile* treefile, TString indir){

  tfile=treefile;
  tfile->cd();
  //cout<<"the tfile is "<<tfile<<endl;
  //cout<<"going to create the directory "<<indir<<endl;
  this->SetTDir(indir);
  //cout<<"I got the dir "<<dir<<endl;
  dir->cd();
  this->Constructor();
}


objectsTree::objectsTree(TFile* treefile, TDirectory* indir){
  tfile=treefile;
  //
  tfile->cd();
  this->SetTDir(indir);
  dir->cd();
  this->Constructor();
}


void objectsTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Jet*>& jets_in, LorentzM& PFmet){
    vector<simpleJet> mySimpleJets;
    for (int ijet=0;ijet<jets_in.size();++ijet){
      mySimpleJets.push_back(jets_in.at(ijet)->makeSimpleJet());
    }
    vector<simpleMuon> mySimpleMuons;
    for (int imu=0;imu<muons_in.size();++imu){
      mySimpleMuons.push_back(muons_in.at(imu)->makeSimpleMuon());
    }
    vector<simpleElectron> mySimpleElectrons;
    for (int iel=0;iel<electrons_in.size();++iel){
      mySimpleElectrons.push_back(electrons_in.at(iel)->makeSimpleElectron());
    }

    this->Fill(info,tree,mySimpleMuons,mySimpleElectrons,mySimpleJets,PFmet);
}

void objectsTree::Fill(EventInfo* info, EasyChain* tree, vector<simpleMuon*>& muons_in, vector<simpleElectron*>& electrons_in, vector<simpleJet*>& jets_in, LorentzM& PFmet){
  
  cout<<"pass objects, not pointers!"<<endl;
  vector<simpleJet> sJets;
  vector<simpleMuon> sMuons;
  vector<simpleElectron> sElectrons;

  for (int ijet=0;ijet<jets_in.size();++ijet){
    sJets.push_back(*(jets_in.at(ijet)));
  }
  for (int imu=0;imu<muons_in.size();++imu){
    sMuons.push_back(*(muons_in.at(imu)));
  }
  for (int iel=0;iel<electrons_in.size();++iel){
    sElectrons.push_back(*(electrons_in.at(iel)));
  }

  this->Fill(info,tree,sMuons,sElectrons,sJets,PFmet);

}





void objectsTree::Fill(EventInfo* info, EasyChain* tree, vector<simpleMuon>& muons_in, vector<simpleElectron>& electrons_in, vector<simpleJet>& jets_in, LorentzM& PFmet){

  if(el != 0){ el->clear();}
  // else{ el=new vector<simpleElectron>;  }
  if(mu != 0){mu->clear();}
  //  else{ mu=new vector<simpleMuon>;  }
  if(Jets!= 0){Jets->clear();}
  //else{ Jets=new vector<simpleJet>;  }
  
  pPFmet=&PFmet;
  double MET=PFmet.Pt();
  //MET=0;

  event = info->Event;
  run = info->Run;    
  el=&electrons_in;
  Jets=&jets_in;
  mu=&muons_in;

  mytree->Fill();
  //weight=info->EventWeight;
  //PUWeight=info->PUWeight;
  //PUWeight_up=info->PUWeight_up;
  //PUWeight_down=info->PUWeight_down;
  //NPV=info->PUInter;
  //NBtags=info->NBtags;
  //cout<<"receiving "<<NBtags<<endl;

  //HT=0;
  //double HTx=0;
  //double HTy=0;

//   for(int ijet=0;ijet<(int)jets_in.size();++ijet){
//     HT+=jets_in.at(ijet)->Pt();
//     HTx+=jets_in.at(ijet)->P4().Px();
//     HTy+=jets_in.at(ijet)->P4().Py();
//   }

//   MHT=sqrt(HTx*HTx + HTy*HTy);
  
//   Y = MET / sqrt(HT);

  
  //cout<<"jets size"<<jets_in.size()<<endl;
  //  for(int ijet=0;ijet<jets_in.size();++ijet){
  //Jets->push_back(*jets_in.at(ijet)); 
    //cout<<"filling "<<jets_in.at(ijet)->Pt()<<endl;
    //cout<<"and getting "<<Jets->at(ijet)->
  //}
  //for(int imu=0;imu<muons_in.size();++imu){
  //    mu->push_back(muons_in.at(imu));
  //  }
  //  for(int iel=0;iel<electrons_in.size();++iel){ 
  //    el->push_back(electrons_in.at(iel));  
  //}

  //MT2WEle=mt2w_calc.get_mt2w(electrons_in, jets_in, met);
  //MT2WMu=mt2w_calc.get_mt2w(muons_in, jets_in, met);

  //cout<<"in the default tree, mt2WMu is "<<MT2WMu<<endl;
  //cout<<"the value of met is "<<met.Pt()<<endl;
  //cout<<"the jet collection has size "<<jets_in.size()<<endl;
  //mt2wmu_intree->Fill(MT2WMu);
  //

  //TRANSVERSE MASS
//   MTEle=0;
//   double pt1=0.;
//   double pt2=0.;
//   for (unsigned int iel=0; iel<electrons_in.size();++iel){
//     pt1=electrons_in.at(iel)->Pt();
//     pt2=met.pt();
//     MTEle=sqrt( 2*pt1*pt2 * (1 - cos(DeltaPhi(electrons_in.at(iel)->P4(), met))));
//     break;
//   }
//   //
//   //
//   //
//   MTMu=0;
//   for (unsigned int imu=0; imu<muons_in.size();++imu){
//     pt1=muons_in.at(imu)->Pt();
//     pt2=met.pt();
//     MTMu=sqrt( 2*pt1*pt2 * (1 - cos(DeltaPhi(muons_in.at(imu)->P4(), met))));
//     break;
//   }

  //cout<<"filling! an eta of "<<pPFmet->Eta()<<endl;
  //cout<<"comparing with before "<<PFmet.Eta()<<endl;


}



// void objectsTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Ptr_Jet>& jets_in, LorentzM& PFmet){
//   //  cout<<"filling"<<endl;

//   //cout<<"calling Fill with shared_ptr"<<endl;
//   //cout<<"met is "<<met.Et()<<endl;
//   //copy the shared pointer to normal pointers
//   vector<Jet*> dummyJetCollection;
//   for (int ijet=0; ijet<jets_in.size();++ijet){
//     dummyJetCollection.push_back(jets_in.at(ijet).get());
//   }  
//   this->Fill(info,tree,muons_in,electrons_in,dummyJetCollection, met);
// }


void objectsTree::Constructor(){
  
  mytree = new TTree("subTree","a subTree");  
  //  cout<<"a new tree with in "<<mytree<<" was created "<<endl;
  event=0;
  run=0;
  //MET=0.0;
//   HT=0.0;
//   MHT=0.0;
//   Y=0.0;
//   MTEle=0.0;
//   MTMu=0.0;
//   MT2=0.0;
//   MT2WEle=0.0;
//   MT2WMu=0.0;
//   weight=0.0;
//   PUWeight=0.0;
//   PUWeight_up=0.0;
//   PUWeight_down=0.0;
//   NPV=0.0;
//  NBtags=0.0;
  //Jets = new vector<simpleJet>;
//  Jetset(new vector<LorentzM>);
  //el = new vector<simpleElectron>;
  //mu = new vector<simpleMuon>;
  //pPFmet = new LorentzM;
  Jets=0;
  el=0;
  mu=0;
  pPFmet=0;

//genJets=new vector<simpleGenJet>;
  this->SetBranches();
}


void objectsTree::Write(){
  cout<<"writing inthe directory "<<endl;
  dir->GetPath();
  dir->cd();
  mytree->Write();
};



void objectsTree::SetToZero(vector<double>* vec){
  for(int i=0; i<vec->size(); i++){
    vec->at(i)=0.;
  }
};

void objectsTree::SetTFile(TFile* infile){
  tfile=infile;
}

void objectsTree::SetTDir(TDirectory* newdir){
  dir=newdir;
}
void objectsTree::SetTDir(TString newdir){
  //
  //
  TObjArray* arr=newdir.Tokenize("/");
  TIter next(arr);
  TString* token;
  Int_t numsep=arr->GetEntries();
  
  TObjString* tok;

  //if the path is simply "./"
  if(newdir==(TString)"./"){
    dir=(TDirectory*)tfile;
  }
  else{
    TDirectory* cdir=tfile;
    TString newpath="";
    if(numsep<2){cout<<"BAD FORM FOR NEW DIR. ERROR. THE STRING IS"<<newdir<<endl;}
    
    for(Int_t i=1; i<numsep;++i){
      
      tok=(TObjString*)(*arr)[i];    
      TString newsubdir=tok->GetName();
      newpath=newpath+newsubdir+(TString)+"/";
      bool okmkdir=cdir->mkdir(newsubdir);
      if (!okmkdir){
	//======IT EXISTS, SO CD INTO IT AND REPEAT
	//directory aleady exists!, go to it
	cdir=(TDirectory*)cdir->Get((TString)tok->GetName());
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

void objectsTree::SetBranches(){
  mytree->Branch("Event",&event,"event/I");
  mytree->Branch("Run",&run,"run/I");
  mytree->Branch("Jets","std::vector<simpleJet>",&Jets);
  //mytree->Branch("Muons","std::vector<simpleMuon>",&mu);
  //mytree->Branch("Electrons","std::vector<simpleElectron>",&el);
  mytree->Branch("PFmet","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >",&pPFmet);
  //mytree->Branch("genJets","std::vector<simpleGenJet>",&genJets);

  //myTree->Branch("goodVertices",&
  //  mytree->Branch("Electrons","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >",&el);


  //  mytree->Branch("PUWeight",&PUWeight,"PUWeight/D");
  //  mytree->Branch("PUWeight_up",&PUWeight_up,"PUWeight_up/D");
  //  mytree->Branch("PUWeight_down",&PUWeight_down,"PUWeight_down/D");
  //mytree->Branch("NPV",&NPV,"NPV/I");
  //mytree->Branch("NBtags",&NBtags,"NBtags/I");

}  


