//#include "subTree.h"
#include "defaultTree.h"


defaultTree::defaultTree(){
     
     mytree = new TTree("subTree","a subTree");

     event=0;
     run=0;
     MET=0.0;
     HT=0.0;
     MHT=0.0;
     Y=0.0;
     MT=0.0;
     MT2=0.0;
     MT2WEle=0.0;
     MT2WMu=0.0;
     weight=0.0;
     PUWeight=0.0;
     nJets=0;
  
     vector<double> dummyLepton(4,0.);
     el=dummyLepton;
     mu=dummyLepton;  

     static int jet_ncomp=5;
     vector<double> dummyJet(jet_ncomp,0.);
     jet1=dummyJet;
     jet2=dummyJet;
     jet3=dummyJet;
     jet4=dummyJet;
     jet5=dummyJet;
     jet6=dummyJet;
     jet7=dummyJet;

     mytree->Branch("Event",&event,"event/I");
     mytree->Branch("Run",&run,"run/I");
     mytree->Branch("MET",&MET,"MET/D");
     mytree->Branch("HT",&HT,"HT/D");
     mytree->Branch("MHT",&MHT,"MHT/D");
     mytree->Branch("Y",&Y,"Y/D");
     mytree->Branch("MT",&MT,"MT/D");
     mytree->Branch("MT2",&MT2,"MT2/D");
     mytree->Branch("MT2WEle",&MT2WEle,"MT2WEle/D");
     mytree->Branch("MT2WMu",&MT2WMu,"MT2WMu/D");
     mytree->Branch("Weight",&weight,"weight/D");
     mytree->Branch("PUWeight",&PUWeight,"PUWeight/D");
     mytree->Branch("treeEl",&el);
     mytree->Branch("treeMu",&mu);
     mytree->Branch("NJets",&nJets,"nJets/I");
     mytree->Branch("jet1",&jet1);
     mytree->Branch("jet2",&jet2);
     mytree->Branch("jet3",&jet3);
     mytree->Branch("jet4",&jet4);
     mytree->Branch("jet5",&jet5);
     mytree->Branch("jet6",&jet6);
     mytree->Branch("jet7",&jet7);
}

void defaultTree::Write(){
  mytree->Write();
};

void defaultTree::Fill( EventInfo* info, EasyChain* tree, vector<Muon*> muons, vector<Electron*> electrons, vector<Jet*> jets, LorentzM& met) {
  

  event = info->Event;
  run = info->Run;    
  MET=met.Et();
  weight=info->EventWeight;
  PUWeight=info->PUWeight;

  double HTx=0;
  double HTy=0;
  HT=0;
  for(int ijet=0;ijet<(int)jets.size();++ijet){
    HT+=jets.at(ijet)->Pt();
    HTx+=jets.at(ijet)->p4.Px();
    HTy+=jets.at(ijet)->p4.Py();
  }
  //cout<<"the number of jets is "<<jets.size()<<endl;
  //cout<<"and the resulting HT is "<<HT<<endl;
    
  MHT=sqrt(HTx*HTx + HTy*HTy);
  
  Y = met.Et() / sqrt(HT);

  if (electrons.size() != 0){
    el[0]=electrons.at(0)->Pt();
    el[1]=electrons.at(0)->Eta();
    el[2]=electrons.at(0)->Phi();
    el[3]=electrons.at(0)->p4.M();
  }
  else {this->SetToZero(&el);}

  if (muons.size() != 0){
    mu[0]=muons.at(0)->Pt();
    mu[1]=muons.at(0)->Eta();
    mu[2]=muons.at(0)->Phi();
    mu[3]=muons.at(0)->p4.M();
  }
  else {this->SetToZero(&mu);}

  jet1[0]=jets.at(0)->Pt();
  jet1[1]=jets.at(0)->Eta();
  jet1[2]=jets.at(0)->Phi();
  jet1[3]=jets.at(0)->p4.M();
  jet1[4]=jets.at(0)->BJetDisc("CSV");
  
  jet2[0]=jets.at(1)->Pt();
  jet2[1]=jets.at(1)->Eta();
  jet2[2]=jets.at(1)->Phi();
  jet2[3]=jets.at(1)->p4.M();
  jet2[4]=jets.at(1)->BJetDisc("CSV");
  
  jet3[0]=jets.at(2)->Pt();
  jet3[1]=jets.at(2)->Eta();
  jet3[2]=jets.at(2)->Phi();
  jet3[3]=jets.at(2)->p4.M();
  jet3[4]=jets.at(2)->BJetDisc("CSV");

  if (jets.size()>3){
    jet4[0]=jets.at(3)->Pt();
    jet4[1]=jets.at(3)->Eta();
    jet4[2]=jets.at(3)->Phi();
    jet4[3]=jets.at(3)->p4.M();
    jet4[4]=jets.at(3)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet4);}

  if (jets.size()>4){
    jet5[0]=jets.at(4)->Pt();
    jet5[1]=jets.at(4)->Eta();
    jet5[2]=jets.at(4)->Phi();
    jet5[3]=jets.at(4)->p4.M();
    jet5[4]=jets.at(4)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet5);}

  if( jets.size()>5){
    jet6[0]=jets.at(5)->Pt();
    jet6[1]=jets.at(5)->Eta();
    jet6[2]=jets.at(5)->Phi();
    jet6[3]=jets.at(5)->p4.M();
    jet7[4]=jets.at(5)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet6);}

  if( jets.size()>6){
    jet7[0]=jets.at(6)->Pt();
    jet7[1]=jets.at(6)->Eta();
    jet7[2]=jets.at(6)->Phi();
    jet7[3]=jets.at(6)->p4.M();
    jet7[4]=jets.at(6)->BJetDisc("CSV");
  }
  else {this->SetToZero(&jet7);}

  MT2WEle=mt2w_calc.get_mt2w( electrons, jets, met);
  MT2WMu=mt2w_calc.get_mt2w( muons, jets, met);

  mytree->Fill();
};

void defaultTree::SetToZero(vector<double>* vec){

  for(int i=0; i<vec->size(); i++){
    vec->at(i)=0.;
  }
  
};
