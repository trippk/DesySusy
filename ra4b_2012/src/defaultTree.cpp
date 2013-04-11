//#include "subTree.h"
#include "defaultTree.h"
#include "string.h"
#include "TFile.h"
#include "boost/test/utils/foreach.hpp"
#include "boost/algorithm/string/iter_find.hpp"
#include "boost/algorithm/string.hpp"
#include <boost/shared_ptr.hpp>
#include "typedefs.h"


using namespace std;


defaultTree::defaultTree(){

  tfile=0;
  dir=0;
  this->Constructor();

}

defaultTree::defaultTree(TFile* treefile, TString indir){

  tfile=treefile;
  tfile->cd();
  //cout<<"the tfile is "<<tfile<<endl;
  //cout<<"going to create the directory "<<indir<<endl;
  this->SetTDir(indir);
  //cout<<"I got the dir "<<dir<<endl;
  dir->cd();
  this->Constructor();
}


defaultTree::defaultTree(TFile* treefile, TDirectory* indir){
  tfile=treefile;
  //
  tfile->cd();
  this->SetTDir(indir);
  dir->cd();
  this->Constructor();
}


void defaultTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Jet*>& jets_in, LorentzM& met){

  el->clear();
  mu->clear();
  Jets->clear();

  MET=met.Pt();
  //MET=0;

  event = info->Event;
  run = info->Run;    
  weight=info->EventWeight;
  PUWeight=info->PUWeight;
  PUWeight_up=info->PUWeight_up;
  PUWeight_down=info->PUWeight_down;
  NPV=info->PUInter;
  NBtags=info->NBtags;
  //cout<<"receiving "<<NBtags<<endl;

  HT=0;
  double HTx=0;
  double HTy=0;

  for(int ijet=0;ijet<(int)jets_in.size();++ijet){
    HT+=jets_in.at(ijet)->Pt();
    HTx+=jets_in.at(ijet)->P4().Px();
    HTy+=jets_in.at(ijet)->P4().Py();
  }

  MHT=sqrt(HTx*HTx + HTy*HTy);
  
  Y = MET / sqrt(HT);


  //cout<<"jets size"<<jets_in.size()<<endl;
  for(int ijet=0;ijet<jets_in.size();++ijet){
    Jets->push_back(jets_in.at(ijet)->P4()); 
  }
  for(int imu=0;imu<muons_in.size();++imu){
    mu->push_back(muons_in.at(imu)->P4());
  }
  for(int iel=0;iel<electrons_in.size();++iel){ 
    el->push_back(electrons_in.at(iel)->P4());  
  }

  MT2WEle=mt2w_calc.get_mt2w(electrons_in, jets_in, met);
  MT2WMu=mt2w_calc.get_mt2w(muons_in, jets_in, met);

  //cout<<"in the default tree, mt2WMu is "<<MT2WMu<<endl;
  //cout<<"the value of met is "<<met.Pt()<<endl;
  //cout<<"the jet collection has size "<<jets_in.size()<<endl;
  //mt2wmu_intree->Fill(MT2WMu);
  //

  //TRANSVERSE MASS
  MTEle=0;
  double pt1=0.;
  double pt2=0.;
  for (unsigned int iel=0; iel<electrons_in.size();++iel){
    pt1=electrons_in.at(iel)->Pt();
    pt2=met.pt();
    MTEle=sqrt( 2*pt1*pt2 * (1 - cos(DeltaPhi(electrons_in.at(iel)->P4(), met))));
    break;
  }
  //
  //
  //
  MTMu=0;
  for (unsigned int imu=0; imu<muons_in.size();++imu){
    pt1=muons_in.at(imu)->Pt();
    pt2=met.pt();
    MTMu=sqrt( 2*pt1*pt2 * (1 - cos(DeltaPhi(muons_in.at(imu)->P4(), met))));
    break;
  }

  mytree->Fill();

}

void defaultTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Ptr_Jet>& jets_in, LorentzM& met){
  //  cout<<"filling"<<endl;

  //cout<<"calling Fill with shared_ptr"<<endl;
  //cout<<"met is "<<met.Et()<<endl;
  //copy the shared pointer to normal pointers
  vector<Jet*> dummyJetCollection;
  for (int ijet=0; ijet<jets_in.size();++ijet){
    dummyJetCollection.push_back(jets_in.at(ijet).get());
  }  
  this->Fill(info,tree,muons_in,electrons_in,dummyJetCollection, met);
}



//void defaultTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Ptr_Jet>& jets_in, LorentzM* pmet){


//}


//void defaultTree::Fill(EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Ptr_Jet>& jets_in, LorentzM& met_in){
//  this->FillTemplate(info,tree,muons_in,electrons_in,jets_in,met_in);
//}

void defaultTree::Constructor(){
  
  mytree = new TTree("subTree","a subTree");  
  //  cout<<"a new tree with in "<<mytree<<" was created "<<endl;
  event=0;
  run=0;
  MET=0.0;
  HT=0.0;
  MHT=0.0;
  Y=0.0;
  MTEle=0.0;
  MTMu=0.0;
  MT2=0.0;
  MT2WEle=0.0;
  MT2WMu=0.0;
  weight=0.0;
  PUWeight=0.0;
  PUWeight_up=0.0;
  PUWeight_down=0.0;
  NPV=0.0;
  NBtags=0.0;
  Jets = new vector<LorentzM>;
//  Jetset(new vector<LorentzM>);
  el = new vector<LorentzM>;
  mu = new vector<LorentzM>;

  //  nJets=0;
  
  //  vector<double> dummyLepton(4,0.);
  //  el=dummyLepton;
  //  mu=dummyLepton;  

  //static int jet_ncomp=5;
  //vector<double> dummyJet(jet_ncomp,0.);
  //  jet1=dummyJet;
  //  jet2=dummyJet;
  //  jet3=dummyJet;
  //  jet4=dummyJet;
  //  jet5=dummyJet;
  //  jet6=dummyJet;
  //  jet7=dummyJet;

  //  cout<<"mytree pointer "<<mytree<<endl;
  mytree->Branch("Event",&event,"event/I");
  mytree->Branch("Run",&run,"run/I");
  mytree->Branch("MET",&MET,"MET/D");
  mytree->Branch("HT",&HT,"HT/D");
  mytree->Branch("MHT",&MHT,"MHT/D");
  mytree->Branch("Y",&Y,"Y/D");
  mytree->Branch("MTEle",&MTEle,"MTEle/D");
  mytree->Branch("MTMu",&MTMu,"MTMu/D");
  mytree->Branch("MT2",&MT2,"MT2/D");
  mytree->Branch("MT2WEle",&MT2WEle,"MT2WEle/D");
  mytree->Branch("MT2WMu",&MT2WMu,"MT2WMu/D");
  mytree->Branch("Weight",&weight,"weight/D");
  mytree->Branch("PUWeight",&PUWeight,"PUWeight/D");
  mytree->Branch("PUWeight_up",&PUWeight_up,"PUWeight_up/D");
  mytree->Branch("PUWeight_down",&PUWeight_down,"PUWeight_down/D");
  mytree->Branch("NPV",&NPV,"NPV/I");
  mytree->Branch("NBtags",&NBtags,"NBtags/I");
  //  mytree->Branch("NJets",&nJets,"nJets/I");
  mytree->Branch("Jets","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >",&Jets);
  mytree->Branch("Muons","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >",&mu);
  mytree->Branch("Electrons","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >",&el);
  
  //  mytree->Branch("treeEl",&el);
  //  mytree->Branch("treeMu",&mu);


  /*
  mytree->Branch("jet1",&jet1);
  mytree->Branch("jet2",&jet2);
  mytree->Branch("jet3",&jet3);
  mytree->Branch("jet4",&jet4);
  mytree->Branch("jet5",&jet5);
  mytree->Branch("jet6",&jet6);
  mytree->Branch("jet7",&jet7);
  */
  //  cout<<"at the end of the constructor, dir is "<<dir<<endl;
}


void defaultTree::Write(){
  //  cout<<"at the moment of writing, dir is "<<dir<<endl;
  //  cout<<"dir name "<<dir->GetName()<<endl;
  //  cout<<"and the tree going to be written is "<<mytree<<endl;
  dir->cd();
  mytree->Write();
};



void defaultTree::SetToZero(vector<double>* vec){

  for(int i=0; i<vec->size(); i++){
    vec->at(i)=0.;
  }
  
};

void defaultTree::SetTFile(TFile* infile){
  tfile=infile;
}

void defaultTree::SetTDir(TDirectory* newdir){
  dir=newdir;
}
void defaultTree::SetTDir(TString newdir){
  //
  //
  //  cout<<"tokenizing "<<newdir<<endl;
  //
  //
  
  TObjArray* arr=newdir.Tokenize("/");
  TIter next(arr);
  TString* token;
  //arr->Print();
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
    if(numsep<2){cout<<"BAD FORM FOR NEW DIR. ERROR"<<endl;}
    
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











/*

  if (electrons.size() != 0){
    el[0]=electrons.at(0)->Pt();
    el[1]=electrons.at(0)->Eta();
    el[2]=electrons.at(0)->Phi();
    el[3]=electrons.at(0)->P4().M();
  }
  else {this->SetToZero(&el);}

  if (muons.size() != 0){
    mu[0]=muons.at(0)->Pt();
    mu[1]=muons.at(0)->Eta();
    mu[2]=muons.at(0)->Phi();
    mu[3]=muons.at(0)->P4().M();
  }
  else {this->SetToZero(&mu);}

  if(jets.size() !=0){
    jet1[0]=jets.at(0)->Pt();
    jet1[1]=jets.at(0)->Eta();
    jet1[2]=jets.at(0)->Phi();
    jet1[3]=jets.at(0)->P4().M();
    jet1[4]=jets.at(0)->BJetDisc("CSV");
  }
  if (jets.size() >1){
    jet2[0]=jets.at(1)->Pt();
    jet2[1]=jets.at(1)->Eta();
    jet2[2]=jets.at(1)->Phi();
    jet2[3]=jets.at(1)->P4().M();
    jet2[4]=jets.at(1)->BJetDisc("CSV");
  }
  if(jets.size()>2){
    jet3[0]=jets.at(2)->Pt();
    jet3[1]=jets.at(2)->Eta();
    jet3[2]=jets.at(2)->Phi();
    jet3[3]=jets.at(2)->P4().M();
    jet3[4]=jets.at(2)->BJetDisc("CSV");
  }
  if (jets.size()>3){
    jet4[0]=jets.at(3)->Pt();
    jet4[1]=jets.at(3)->Eta();
    jet4[2]=jets.at(3)->Phi();
    jet4[3]=jets.at(3)->P4().M();
    jet4[4]=jets.at(3)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet4);}

  if (jets.size()>4){
    jet5[0]=jets.at(4)->Pt();
    jet5[1]=jets.at(4)->Eta();
    jet5[2]=jets.at(4)->Phi();
    jet5[3]=jets.at(4)->P4().M();
    jet5[4]=jets.at(4)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet5);}

  if( jets.size()>5){
    jet6[0]=jets.at(5)->Pt();
    jet6[1]=jets.at(5)->Eta();
    jet6[2]=jets.at(5)->Phi();
    jet6[3]=jets.at(5)->P4().M();
    jet7[4]=jets.at(5)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet6);}

  if( jets.size()>6){
    jet7[0]=jets.at(6)->Pt();
    jet7[1]=jets.at(6)->Eta();
    jet7[2]=jets.at(6)->Phi();
    jet7[3]=jets.at(6)->P4().M();
    jet7[4]=jets.at(6)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet7);}
*/




//=================================================================
//this is moved to the header file because it is a template class
//=================================================================

/*template<typename __Jet__>
void defaultTree::Fill( EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<__Jet__>& jets_in, LorentzM& met) {
  
  //  cout<<"filling"<<endl;
  
  el->clear();
  mu->clear();
  Jets->clear();

  MET=met.Pt();
  //MET=0;

  event = info->Event;
  run = info->Run;    
  weight=info->EventWeight;
  PUWeight=info->PUWeight;

  HT=0;
  double HTx=0;
  double HTy=0;

  for(int ijet=0;ijet<(int)jets_in.size();++ijet){
    HT+=jets_in.at(ijet)->Pt();
    HTx+=jets_in.at(ijet)->P4().Px();
    HTy+=jets_in.at(ijet)->P4().Py();
  }

  MHT=sqrt(HTx*HTx + HTy*HTy);
  
  Y = MET / sqrt(HT);


  //cout<<"jets size"<<jets_in.size()<<endl;
  for(int ijet=0;ijet<jets_in.size();++ijet){
    Jets->push_back(jets_in.at(ijet)->P4()); 
  }
  for(int imu=0;imu<muons_in.size();++imu){
    mu->push_back(muons_in.at(imu)->P4());
  }
  for(int iel=0;iel<electrons_in.size();++iel){ 
    el->push_back(electrons_in.at(iel)->P4());  
 }

  MT2WEle=mt2w_calc.get_mt2w(electrons_in, jets_in, met);
  MT2WMu=mt2w_calc.get_mt2w(muons_in, jets_in, met);

  //cout<<"in the default tree, mt2WMu is "<<MT2WMu<<endl;
  //mt2wmu_intree->Fill(MT2WMu);
  //

  //TRANSVERSE MASS
  MTEle=0;
  double pt1=0.;
  double pt2=0.;
  for (unsigned int iel=0; iel<electrons_in.size();++iel){
    pt1=electrons_in.at(iel)->Pt();
    pt2=met.pt();
    MTEle=sqrt( 2*pt1*pt2 * (1 - cos(DeltaPhi(electrons_in.at(iel)->P4(), met))));
    break;
  }
  //
  //
  //
  MTMu=0;
  for (unsigned int imu=0; imu<muons_in.size();++imu){
    pt1=muons_in.at(imu)->Pt();
    pt2=met.pt();
    MTMu=sqrt( 2*pt1*pt2 * (1 - cos(DeltaPhi(muons_in.at(imu)->P4(), met))));
    break;
  }

  mytree->Fill();

};

*/
