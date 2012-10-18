#include "anDiLep.h"

using std::cout;
using std::endl;
using std::cerr;

anDiLep::anDiLep() : treeToRead(0), treeToWrite(0), nEntries(0), iEntry(0) {
  treeToWrite = new TTree("subTree", "a subTree");   ///Create a new tree to write.
  AllocateMemory();
  SetBranchesWrite();
}

anDiLep::anDiLep(TTree * treeToReadIn) : treeToRead(treeToReadIn), treeToWrite(0), nEntries(0), iEntry(0), event(0), run(0), weight(0), PUWeight(0), el(0), mu(0), jets(0), bjetdisc(0), nbjets(0), isbjet(0), vMET(0) {

  SetBranchesRead();

  if (treeToRead) {
    nEntries = treeToRead->GetEntries();
  }

}

anDiLep::~anDiLep() {
  if (treeToWrite) {
    DeallocateMemory();
  }
}

void anDiLep::AllocateMemory() {
  
  event = new int(0);
  run = new int(0);
  weight = new double(0.);
  PUWeight = new double(0.);

  el = new std::vector<LorentzM>();
  mu = new std::vector<LorentzM>();
  
  jets = new std::vector<LorentzM>();
  bjetdisc = new std::vector<double>();
  nbjets = new int(0);
  isbjet = new std::vector<bool>();

  vMET = new LorentzM();

  return;
}

void anDiLep::DeallocateMemory() {

  if (event) {
    delete event;
    event = 0;
  }
  if (run) {
    delete run;
    run = 0;
  }
  if (weight) {
    delete weight;
    weight = 0;
  }
  if (PUWeight) {
    delete PUWeight;
    PUWeight = 0;
  }

  if (el) {
    delete el;
    el = 0;
  }
  if (mu) {
    delete mu;
    mu = 0;
  }
  if (jets) {
    delete jets;
    jets = 0;
  }
  if (bjetdisc) {
    delete bjetdisc;
    bjetdisc = 0;
  }
  if(nbjets) {
    delete nbjets;
    nbjets = 0;
  }
  if (isbjet) {
    delete isbjet;
    isbjet = 0;
  }

  if (vMET) {
    delete vMET;
    vMET = 0;
  }

  return;
}


void anDiLep::SetToZero(){

  *event=0;
  *run=0;
  *weight=0.0;
  *PUWeight=0.0;
  
  el->clear();
  mu->clear();
  
  jets->clear();
  bjetdisc->clear();
  *nbjets=0;
  isbjet->clear();
  
  vMET->SetPxPyPzE(0.,0.,0.,0.);
}

void anDiLep::SetBranchesWrite() {

  if (!treeToWrite) {
    cerr << "anDiLep::SetBranchesWrite >> ERROR : treeToWrite not set!" << endl;
    return;
  }

  treeToWrite->Branch("Event",event,"event/I");
  treeToWrite->Branch("Run",run,"run/I");
  treeToWrite->Branch("Weight",weight,"weight/D");
  treeToWrite->Branch("PUWeight",PUWeight,"PUWeight/D");

  treeToWrite->Branch("el","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &el);
  treeToWrite->Branch("mu","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &mu);

  treeToWrite->Branch("jets", "std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &jets);
  treeToWrite->Branch("bjetdisc", &bjetdisc);
  treeToWrite->Branch("nbjets",nbjets, "nbjets/I");
  treeToWrite->Branch("isbjet",&isbjet);

  treeToWrite->Branch("vMET",vMET);

  return;
}

void anDiLep::SetBranchesRead() {

  if (!treeToRead) {
    cerr << "anDiLep::SetBranchesRead >> ERROR : treeToRead not set!" << endl;
    return;
  }

  treeToRead->SetBranchAddress("Event",event);
  treeToRead->SetBranchAddress("Run",run);
  treeToRead->SetBranchAddress("Weight",weight);
  treeToRead->SetBranchAddress("PUWeight",PUWeight);
  
  treeToRead->SetBranchAddress("el",&el);
  treeToRead->SetBranchAddress("mu",&mu);

  treeToRead->SetBranchAddress("jets", &jets);
  treeToRead->SetBranchAddress("bjetdisc", &bjetdisc);
  treeToRead->SetBranchAddress("nbjets",nbjets);
  treeToRead->SetBranchAddress("isbjet",&isbjet);

  treeToRead->SetBranchAddress("vMET",&vMET);

  return;
}

void anDiLep::Fill(EventInfo* info, EasyChain* tree, std::vector<Muon*> & muons_in, std::vector<Electron*> & electrons_in, std::vector<Jet*> & jets_in, LorentzM& met_in) {

  if (!treeToWrite) {
    return;
  }

  if (!info) {
    cout << "anDiLep::Fill >> ERROR: info pointer NULL!" << endl;
    return;
  }

  this->SetToZero();


  *event = info->Event;
  *run = info->Run;    
  *weight = info->EventWeight;
  *PUWeight = info->PUWeight;

  for ( int iel = 0; iel < electrons_in.size(); iel++) {
    el->push_back( electrons_in.at(iel)->P4() );
  }
  for ( int imu = 0; imu < muons_in.size(); imu++) {
    mu->push_back( muons_in.at(imu)->P4() );
  }
  for (int ijet=0; ijet<jets_in.size() ; ijet++) {
    jets->push_back(jets_in.at(ijet)->P4());
    
    bjetdisc->push_back(jets_in.at(ijet)->BJetDisc("CSV"));
    if ( jets_in.at(ijet)->IsBJet("CSV","Medium") ) {
      isbjet->push_back(true);
      *nbjets+=1;
    }
    else isbjet->push_back(false);
  }

  *vMET = met_in;

  treeToWrite->Fill();

  return;
}

void anDiLep::Write(){
  if (treeToWrite) treeToWrite->Write();
};

bool anDiLep::Read(long getEntry) {

  if (!treeToRead) {
    return false;
  }

  long entryToGet = iEntry;

  if (getEntry < 0) iEntry++;
  else entryToGet = getEntry;

  if (entryToGet < 0 || entryToGet >= nEntries) {
    return false;
  }
  else {
    treeToRead->GetEntry(entryToGet);
    return true;
  }

}
