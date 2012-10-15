#include "anDiLep.h"

using std::cout;
using std::endl;
using std::cerr;

anDiLep::anDiLep() : treeToRead(0), treeToWrite(0) {

  treeToWrite = new TTree("subTree", "a subTree");   ///Create a new tree to write.

  AllocateMemory();

  SetBranchesWrite();
}

anDiLep::anDiLep(TTree * treeToReadIn) : treeToRead(treeToReadIn), treeToWrite(0) {

  SetToZero();   //Initialise

  SetBranchesRead();
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
  elPt = new std::vector<double>();
  mu = new std::vector<LorentzM>();
  muPt = new std::vector<double>();
  
  njets = new int(0);
  jets = new std::vector<LorentzM>();
  jetsPt = new std::vector<double>();
  bjetdisc = new std::vector<double>();
  nbjets = new int(0);
  isbjet = new std::vector<bool>();

  vMET = new LorentzM();
  vMHT = new LorentzM();

  MET     = new double(0.);
  MHT	= new double(0.);
  HT	= new double(0.);

  elSig	= new double(0.);
  muSig	= new double(0.);
  METSig	= new double(0.);
  MHTSig	= new double(0.);

  mtEl	= new double(0.);
  mtMu	= new double(0.);

  mt2wEl	= new double(0.);
  mt2wMu	= new double(0.);

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
  if (elPt) {
    delete elPt;
    elPt = 0;
  }
  if (mu) {
    delete mu;
    mu = 0;
  }
  if (muPt) {
    delete muPt;
    muPt = 0;
  }

  if (njets) {
    delete njets;
    njets = 0;
  }
  if (jets) {
    delete jets;
    jets = 0;
  }
  if (jetsPt) {
    delete jetsPt;
    jetsPt = 0;
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
  if (vMHT) {
    delete vMHT;
    vMHT = 0;
  }


  if (MET) {
    delete MET;
    MET = 0;
  }
  if (MHT) {
    delete MHT;
    MHT = 0;
  }
  if (HT) {
    delete HT;
    HT = 0;
  }

  if (elSig) {
    delete elSig;
    elSig = 0;
  }
  if (muSig) {
    delete muSig;
    muSig = 0;
  }
  if (METSig) {
    delete METSig;
    METSig = 0;
  }
  if (MHTSig) {
    delete MHTSig;
    MHTSig = 0;
  }


  if (mtEl) {
    delete mtEl;
    mtEl = 0;
  }
  if (mtMu) {
    delete mtMu;
    mtMu = 0;
  }
  if (mt2wEl) {
    delete mt2wEl;
    mt2wEl = 0;
  }
  if (mt2wMu) {
    delete mt2wMu;
    mt2wMu = 0;
  }

  return;
}


void anDiLep::SetToZero(){

  *event=0;
  *run=0;
  *weight=0.0;
  *PUWeight=0.0;
  
  el->clear();
  elPt->clear();
  mu->clear();
  muPt->clear();
  
  *njets=0;
  jets->clear();
  jetsPt->clear();
  bjetdisc->clear();
  *nbjets=0;
  isbjet->clear();
  
  vMET->SetPxPyPzE(0.,0.,0.,0.);
  *MET=0.;
  vMHT->SetPxPyPzE(0.,0.,0.,0.);
  *MHT=0.;
  *HT=0.;

  *METSig = 0.;
  *MHTSig = 0.;

  *mtEl=0.;
  *mtMu=0.;

  *mt2wEl=0.;
  *mt2wMu=0.;
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
  treeToWrite->Branch("elPt",elPt);
  treeToWrite->Branch("mu","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &mu);
  treeToWrite->Branch("muPt",muPt);

  treeToWrite->Branch("njets",njets,"njets/I");
  treeToWrite->Branch("jets", "std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &jets);
  treeToWrite->Branch("jetsPt", &jetsPt);
  treeToWrite->Branch("bjetdisc", &bjetdisc);
  treeToWrite->Branch("nbjets",nbjets, "nbjets/I");
  treeToWrite->Branch("isbjet",&isbjet);

  treeToWrite->Branch("vMET",vMET);
  treeToWrite->Branch("MET",MET,"MET/D");
  treeToWrite->Branch("vMHT", vMHT);
  treeToWrite->Branch("MHT",MHT,"MHT/D");
  treeToWrite->Branch("HT",HT,"HT/D");

  treeToWrite->Branch("elSig",elSig,"elSig/D");
  treeToWrite->Branch("muSig",muSig,"muSig/D");
  treeToWrite->Branch("METSig",METSig,"METSig/D");
  treeToWrite->Branch("MHTSig",MHTSig,"MHTSig/D");

  treeToWrite->Branch("mtEl",mtEl,"mtEl/D");
  treeToWrite->Branch("mtMu",mtMu,"mtMu/D");				

  treeToWrite->Branch("mt2wEl",mt2wEl,"mt2wEl/D");
  treeToWrite->Branch("mt2wMu",mt2wMu,"mt2wMu/D");

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
  treeToRead->SetBranchAddress("elPt",&elPt);
  treeToRead->SetBranchAddress("mu",&mu);
  treeToRead->SetBranchAddress("muPt",&muPt);

  treeToRead->SetBranchAddress("njets",njets);
  treeToRead->SetBranchAddress("jets", &jets);
  treeToRead->SetBranchAddress("jetsPt", &jetsPt);
  treeToRead->SetBranchAddress("bjetdisc", &bjetdisc);
  treeToRead->SetBranchAddress("nbjets",nbjets);
  treeToRead->SetBranchAddress("isbjet",&isbjet);

  treeToRead->SetBranchAddress("vMET",&vMET);
  treeToRead->SetBranchAddress("vMHT", &vMHT);

  treeToRead->SetBranchAddress("MET",MET);
  treeToRead->SetBranchAddress("MHT",MHT);
  treeToRead->SetBranchAddress("HT",HT  );

  treeToRead->SetBranchAddress("elSig",elSig  );
  treeToRead->SetBranchAddress("muSig",muSig  );
  treeToRead->SetBranchAddress("METSig",METSig);
  treeToRead->SetBranchAddress("MHTSig",MHTSig);

  treeToRead->SetBranchAddress("mtEl",mtEl);
  treeToRead->SetBranchAddress("mtMu",mtMu);

  treeToRead->SetBranchAddress("mt2wEl",mt2wEl);
  treeToRead->SetBranchAddress("mt2wMu",mt2wMu);

  return;
}

void anDiLep::Fill(EventInfo* info, EasyChain* tree, std::vector<Muon*> & muons_in, std::vector<Electron*> & electrons_in, std::vector<Jet*> & jets_in, LorentzM& met_in) {

  if (!treeToWrite) {
    return;
  }

  this->SetToZero();

  *event = info->Event;
  *run = info->Run;    
  *weight = info->EventWeight;
  *PUWeight = info->PUWeight;

  for ( int iel = 0; iel < electrons_in.size(); iel++) {
    el->push_back( electrons_in.at(iel)->P4() );
    elPt->push_back( electrons_in.at(iel)->Pt() );
  }
  for ( int imu = 0; imu < muons_in.size(); imu++) {
    mu->push_back( muons_in.at(imu)->P4() );
    muPt->push_back( muons_in.at(imu)->Pt() );
  }

  *njets=jets_in.size();

  double HTx=0;
  double HTy=0;

  for (int ijet=0; ijet<jets_in.size() ; ijet++) {
    *HT+=jets_in.at(ijet)->Pt();
    HTx+=jets_in.at(ijet)->p4.Px();
    HTy+=jets_in.at(ijet)->p4.Py();
    jets->push_back(jets_in.at(ijet)->P4());
    jetsPt->push_back(jets_in.at(ijet)->Pt());
    
    bjetdisc->push_back(jets_in.at(ijet)->BJetDisc("CSV"));
    if ( jets_in.at(ijet)->IsBJet("CSV","Medium") ) {
      isbjet->push_back(true);
      *nbjets+=1;
    }
    else isbjet->push_back(false);
  }

  *vMET = met_in;
  *MET=met_in.Et();
  vMHT->SetPxPyPzE(HTx,HTy,0.,0.);
  *MHT=vMHT->Pt();

  *elSig = 0./sqrt(*HT);
  *muSig = 0./sqrt(*HT);
  *METSig = *MET/sqrt(*HT);
  *MHTSig = *MHT/sqrt(*HT);

  // mt2wEl=mt2w_calc.get_mt2w( electrons_in, jets_in, met_in);
//   mt2wMu=mt2w_calc.get_mt2w( muons_in, jets_in, met_in);

  treeToWrite->Fill();

  return;
}

void anDiLep::Write(){
  if (treeToWrite) treeToWrite->Write();
};

