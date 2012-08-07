
#include "HistoMaker.h"
#include "ConfigReader.h"

using namespace std;

extern double EventWeight;

HistoMaker::HistoMaker(const TString& pre, const char* dlm ):  delim(dlm), autodump(true), prefix(pre){

  //TH1::AddDirectory(kFALSE);
  TH1::SetDefaultSumw2(true);

};

double HistoMaker::global_event_weight = 1.;
TFile* HistoMaker::tfile = 0;
TDirectory* HistoMaker::cplotdir = 0;

void HistoMaker::setTFile(TFile *f){
	tfile=f;
	cplotdir = tfile->mkdir("ControlPlots");

};

void HistoMaker::dumpToFile(){

  TDirectory* keep = gDirectory->GetDirectory("");

  TDirectory* mainDir=0;
  if(tfile!=0)  mainDir = tfile->GetDirectory("/");

  // tdir->cd();
  cplotdir->Write();
  
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
    
    
    MuPt[cutName] = new TH1D(hname+"MuPt","Pt of the Muon",50,0,500);
    MuEta[cutName] = new TH1D(hname+"MuEta","Pseudorapidity of the muon",60,-3.,3.);

    ElePt[cutName] = new TH1D(hname+"ElePt","Pt of the electron",50,0,500);
    EleEta[cutName] = new TH1D(hname+"ElEta","Pseudorapidity of the electron",60,-3.,3.);

    NJets[cutName]= new TH1D(hname+"NJets","Number of Jets",15,0,15);
    PtAllJets[cutName]= new TH1D(hname+"PtAllJets","Pt of all the jets",150,0,1500);
    for(int i=0; i<NMonitorJets; i++) (PtJet[i])[cutName]= new TH1D(hname+"PtJet"+(long)i,"Pt of the Jet "+(long)i,50,0,500);

    NBJets[cutName]= new TH1D(hname+"NBJets","Number of b-tagged jets",15,0,15);
    PtAllBJets[cutName]= new TH1D(hname+"PtAllBJets","Pt of all the b-tagged jets",150,0,1500);
    for(int i=0; i<NMonitorJets; i++) (PtBJet[i])[cutName]= new TH1D(hname+"PtBJet"+(long)i,"Pt of the b-tagged jet "+(long)i,50,0,500);
    BDisc[cutName]= new TH1D(hname+"BDiscriminator",cutName+"BTag Discriminator",50,-10,10);

    HT[cutName]= new TH1D(hname+"HT",cutName+" HT",250,0,2500);
    MHT[cutName]= new TH1D(hname+"MHT",cutName+" MHT",150,0,1500);
    MET[cutName]= new TH1D(hname+"MET",cutName+" MET",100,0,1000);
    YMET[cutName]= new TH1D(hname+"YMET",cutName+" YMET",80,0,40);

    HT_YMET[cutName]= new TH2D(hname+"HT_YMET",cutName+" HT YMET scatter plot",100,0.,2500.,80,0.,40.);
     
    TString region[4]={"A","B","C","D"};
    for (int idx=0; idx<4; idx++) {
      (NJets_ABCD[idx])[cutName]= new TH1D(hname+"NJets"+(region[idx]),cutName+"NJets"+(region[idx]),15,0,15);
      (PtAllJets_ABCD[idx])[cutName]= new TH1D(hname+"PtAllJets"+(region[idx]),cutName+"PtAllJets"+(region[idx]),150,0,1500);
      (MET_ABCD[idx])[cutName]= new TH1D(hname+"MET"+(region[idx]),cutName+"MET"+region[idx],100,0,1000);
      for(int i=0; i<NMonitorJets; i++) (PtJet_ABCD[i][idx])[cutName]= new TH1D(hname+"PtJet"+(long)i+region[idx],cutName+"PtJet"+(long)i+region[idx],50,0,500);
    }
    
  }

  //cout<<"EventWeight in HistoMaker"<<EventWeight<<" in "<<cutName<<endl;

  for(int k=0;k< muons.size();++k){
    MuPt[cutName]->Fill(muons.at(k)->pt(),EventWeight);
    MuEta[cutName]->Fill(muons.at(k)->Eta(),EventWeight);
  }

  for(int k=0;k< electrons.size();++k){
    ElePt[cutName]->Fill(electrons.at(k)->pt(),EventWeight);
    EleEta[cutName]->Fill(electrons.at(k)->Eta(),EventWeight);
  }

  double hx=0.;
  double hy=0.;
  double ht=0.;
  int nBJets=0;
  
  for(int k=0;k< jets.size();++k){
    PtAllJets[cutName]->Fill(jets.at(k)->pt(),EventWeight);
    if (k<NMonitorJets) (PtJet[k])[cutName]->Fill(jets.at(k)->pt(),EventWeight);
    hx += jets.at(k)->pP4()->px();
    hy += jets.at(k)->pP4()->py();
    ht += jets.at(k)->pt();

    if(jets.at(k)->IsBJet("CVS","Medium") ) {
      nBJets++;
      PtAllBJets[cutName]->Fill(jets.at(k)->pt(),EventWeight);
      BDisc[cutName]->Fill(jets.at(k)->BJetDisc("CVS"),EventWeight);
      if (nBJets<NMonitorJets) (PtBJet[nBJets])[cutName]->Fill(jets.at(k)->pt(),EventWeight);
    }
    
  }
  
  NJets[cutName]->Fill(jets.size(),EventWeight);
  NBJets[cutName]->Fill(nBJets);

  HT[cutName]->Fill(ht);
  MHT[cutName]->Fill(sqrt(hx*hx+hy*hy),EventWeight);
  MET[cutName]->Fill(met.pt(),EventWeight);
  double ymet=met.pt()/sqrt(ht);
  YMET[cutName]->Fill(ymet);

  HT_YMET[cutName]->Fill(ht,ymet);

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
    (NJets_ABCD[regionID])[cutName]->Fill(jets.size(),EventWeight);
    (MET_ABCD[regionID])[cutName]->Fill(met.pt(),EventWeight);
    for(int k=0;k< jets.size();++k){
      (PtAllJets_ABCD[regionID])[cutName]->Fill(jets.at(k)->pt(),EventWeight);
      if(k<NMonitorJets) (PtJet_ABCD[k][regionID])[cutName]->Fill(jets.at(k)->pt(),EventWeight);
    }
  }
 

}
