#include "HistoMaker.h"
#include "ConfigReader.h"

using namespace std;

HistoMaker::HistoMaker(const TString& pre, const char* dlm ):  delim(dlm), autodump(true), prefix(pre), tfile(0), cplotdir(0){

  //TH1::AddDirectory(kFALSE);
  TH1::SetDefaultSumw2(true);

};

double HistoMaker::global_event_weight = 1.;

void HistoMaker::setTFile(TFile *f){
	tfile=f;
	cplotdir = tfile->mkdir("ControlPlots");
};
void HistoMaker::setDir(TDirectory *f){
	cplotdir = f->mkdir("ControlPlots");
};

void HistoMaker::dumpToFile(){

//   TDirectory* keep = gDirectory->GetDirectory("");

//   TDirectory* mainDir=0;
//   if(tfile!=0)  mainDir = tfile->GetDirectory("/");

  // tdir->cd();

  if (cplotdir == 0) {
    cout << "HistoMaker::dumpToFile >> ERROR " << endl; 
  }
  else cplotdir->Write();
  
  /*
  for(unsigned c=0; c<allCuts.size(); ++c) {

    TString dirname=prefix;    
    for(Ssiz_t i=0;i<allCuts[c].Length();i++) dirname.Append( allCuts[c][i]==' ' ? '_' : allCuts[c][i] );

    TDirectory* tdir;
    if(mainDir!=0)  tdir = mainDir->mkdir(dirname);
    tdir->cd();
    
    MuPt[allCuts[c]]->Write();
    MuEta[allCuts[c]]->Write();
    
    ElePt[allCuts[c]]->Write();
    EleEta[allCuts[c]]->Write();

    NJets[allCuts[c]]->Write();
    PtAllJets[allCuts[c]]->Write();
    for(int i=0; i<4; ++i){
      (PtJet[i])[allCuts[c]]->Write();
    }
    
    NBJets[allCuts[c]]->Write();
    PtAllBJets[allCuts[c]]->Write();
    for(int i=0; i<4; ++i){
      (PtBJet[i])[allCuts[c]]->Write();
    }
    BDisc[allCuts[c]]->Write();
  
    HT[allCuts[c]]->Write();
    MHT[allCuts[c]]->Write();
    MET[allCuts[c]]->Write();
    YMET[allCuts[c]]->Write();
    
    HT_YMET[allCuts[c]]->Write();
    
    for (int idx=0; idx<4; idx++) {
      (NJets_ABCD[idx])[allCuts[c]]->Write();
      (PtAllJets_ABCD[idx])[allCuts[c]]->Write();
      (MET_ABCD[idx])[allCuts[c]]->Write();
      for(int i=0; i<NMonitorJets; i++) (PtJet_ABCD[i][idx])[allCuts[c]]->Write();
    }
    
    mainDir->cd();
  }
  keep->cd();
  */
}
  
void HistoMaker::MakePlots( const TString& cutName, vector<Muon*> muons, vector<Electron*> electrons, vector<Ptr_Jet> shared_ptr_jets, LorentzM& met){  
  
  //convert the shared pointers into normal pointers and call the old MakePlots
  vector<Jet*>jets;
  for (int ijet=0;ijet<(int)shared_ptr_jets.size();++ijet){
    Jet* dummypointer=shared_ptr_jets.at(ijet).get();
    jets.push_back(dummypointer);
  }
  this->MakePlots(cutName,muons,electrons,jets,met);
}
//
//
//
void HistoMaker::MakePlots( const TString& cutName, vector<Muon*> muons, vector<Electron*> electrons, vector<Jet*> jets, LorentzM& met){

  ConfigReader config;
  
  static float HT0 = config.getFloat("HT0", 375. );
  static float HT1 = config.getFloat("HT1", 650. );
  static float HT2 = config.getFloat("HT2", 650. );

  static float Y0 = config.getFloat("Y0", 3.25 );
  static float Y1 = config.getFloat("Y1", 5.5 );
  static float Y2 = config.getFloat("Y2", 5.5 );




  if( find(allCuts.begin(),allCuts.end(),cutName) == allCuts.end() ) {

    TDirectory* newdir=cplotdir->mkdir(cutName);
    newdir->cd();
  
    allCuts.push_back(cutName);
    
    TString hname;

    for(Ssiz_t i=0;i<cutName.Length();i++) hname.Append( cutName[i]==' ' ? '_' : cutName[i] );
    hname="";
    //cutName;

    mt2wEle[cutName] = new TH1D(hname+"mt2wEle","mt2wEle",100,0,500);
    mt2wMu[cutName]  = new TH1D(hname+"mt2wMu", "mt2wMu" ,100,0,500);

    MuPt[cutName] = new TH1D(hname+"MuPt","Pt of the Muon",50,0,500);
    MuEta[cutName] = new TH1D(hname+"MuEta","Pseudorapidity of the muon",60,-3.,3.);

    ElePt[cutName] = new TH1D(hname+"ElePt","Pt of the electron",50,0,500);
    EleEta[cutName] = new TH1D(hname+"ElEta","Pseudorapidity of the electron",60,-3.,3.);

    HT[cutName]= new TH1D(hname+"HT"," HT",250,0,2500);
    MET[cutName]= new TH1D(hname+"MET"," MET",100,0,1000);
    MHT[cutName]= new TH1D(hname+"MHT"," MHT",150,0,1500);
    YMET[cutName]= new TH1D(hname+"YMET"," YMET",80,0,40);



    NJets[cutName]= new TH1D(hname+"NJets","Number of Jets",15,0,15);
    PtAllJets[cutName]= new TH1D(hname+"PtAllJets","Pt of all the jets",150,0,1500);
    for(int i=0; i<NMonitorJets; i++) (PtJet[i])[cutName]= new TH1D(hname+"PtJet"+(long)i,"Pt of the Jet "+(long)i,50,0,500);

    NBJets[cutName]= new TH1D(hname+"NBJets","Number of b-tagged jets",15,0,15);
    PtAllBJets[cutName]= new TH1D(hname+"PtAllBJets","Pt of all the b-tagged jets",150,0,1500);
    for(int i=0; i<NMonitorJets; i++) (PtBJet[i])[cutName]= new TH1D(hname+"PtBJet"+(long)i,"Pt of the b-tagged jet "+(long)i,50,0,500);
    BDisc[cutName]= new TH1D(hname+"BDiscriminator","BTag Discriminator",50,-10,10);



    HT_YMET[cutName]= new TH2D(hname+"HT_YMET"," HT YMET scatter plot",100,0.,2500.,80,0.,40.);
     
    TString region[4]={"A","B","C","D"};
    for (int idx=0; idx<4; idx++) {
      (NJets_ABCD[idx])[cutName]= new TH1D(hname+"NJets"+(region[idx]),"Number of Jets in the region"+(region[idx]),15,0,15);
      (PtAllJets_ABCD[idx])[cutName]= new TH1D(hname+"PtAllJets"+(region[idx]),"Pt of all the jets in the region"+(region[idx]),150,0,1500);
      (MET_ABCD[idx])[cutName]= new TH1D(hname+"MET"+(region[idx]),"MET"+region[idx],100,0,1000);
      for(int i=0; i<NMonitorJets; i++) (PtJet_ABCD[i][idx])[cutName]= new TH1D(hname+"PtJet"+(long)i+region[idx],"PtJet"+(long)i+region[idx],50,0,500);
    }
    
  }

  //cout<<"EventWeight in HistoMaker"<<EventWeight<<" in "<<cutName<<endl;

  mt2wEle[cutName]->Fill(mt2w_calc.get_mt2w(electrons, jets, met),global_event_weight);
  mt2wMu[cutName] ->Fill(mt2w_calc.get_mt2w(    muons, jets, met),global_event_weight);

  for(int k=0;k< muons.size();++k){
    MuPt[cutName]->Fill(muons.at(k)->pt(),global_event_weight);
    MuEta[cutName]->Fill(muons.at(k)->Eta(),global_event_weight);
  }

  for(int k=0;k< electrons.size();++k){
    ElePt[cutName]->Fill(electrons.at(k)->pt(),global_event_weight);
    EleEta[cutName]->Fill(electrons.at(k)->Eta(),global_event_weight);
  }

  double hx=0.;
  double hy=0.;
  double ht=0.;
  int nBJets=0;
  
  for(int k=0;k< jets.size();++k){
    PtAllJets[cutName]->Fill(jets.at(k)->pt(),global_event_weight);
    if (k<NMonitorJets) (PtJet[k])[cutName]->Fill(jets.at(k)->pt(),global_event_weight);
    hx += jets.at(k)->P4().px();
    hy += jets.at(k)->P4().py();
    ht += jets.at(k)->pt();

    if(jets.at(k)->IsBJet("CSV","Medium") ) {
      nBJets++;
      PtAllBJets[cutName]->Fill(jets.at(k)->pt(),global_event_weight);
      BDisc[cutName]->Fill(jets.at(k)->BJetDisc("CSV"),global_event_weight);
      if (nBJets<NMonitorJets) (PtBJet[nBJets])[cutName]->Fill(jets.at(k)->pt(),global_event_weight);
    }
    
  }
  
  NJets[cutName]->Fill(jets.size(),global_event_weight);
  NBJets[cutName]->Fill(nBJets,global_event_weight);

  HT[cutName]->Fill(ht, global_event_weight);
  MHT[cutName]->Fill(sqrt(hx*hx+hy*hy),global_event_weight);
  MET[cutName]->Fill(met.pt(),global_event_weight);
  double ymet=met.pt()/sqrt(ht);
  YMET[cutName]->Fill(ymet, global_event_weight);

  HT_YMET[cutName]->Fill(ht,ymet, global_event_weight);

  int regionID=-1;

  bool A = ht >= HT0 && ht < HT1 && ymet >= Y0 && ymet < Y1;
  bool B = ht >= HT2 && ymet >= Y0 && ymet < Y1;
  bool C = ht >= HT0 && ht < HT1 && ymet >= Y2;
  bool D = ht >= HT2 && ymet >= Y2;

  if(A) regionID=0;
  if(B) regionID=1;
  if(C) regionID=2;
  if(D) regionID=3;
  
  if (regionID>-1) {
    (NJets_ABCD[regionID])[cutName]->Fill(jets.size(),global_event_weight);
    (MET_ABCD[regionID])[cutName]->Fill(met.pt(),global_event_weight);
    for(int k=0;k< jets.size();++k){
      (PtAllJets_ABCD[regionID])[cutName]->Fill(jets.at(k)->pt(),global_event_weight);
      if(k<NMonitorJets) (PtJet_ABCD[k][regionID])[cutName]->Fill(jets.at(k)->pt(),global_event_weight);
    }
  }
 

}
