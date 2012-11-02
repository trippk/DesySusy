#include "anDiLep.h"

using std::cout;
using std::endl;
using std::cerr;

anDiLep::anDiLep(TDirectory * dirIn) : treeToRead(0), treeToWrite(0), nEntries(0), iEntry(0) {
  if (dirIn) {
    dir = dirIn;
    dir->cd();
  }
  else dir = gDirectory;

  cout << "anDiLep::anDiLep >> Creating in dir: " << dir->GetName() << endl;
  treeToWrite = new TTree("subTree", "a subTree");   ///Create a new tree to write.
  AllocateMemory();
  SetBranchesWrite();
}

anDiLep::anDiLep(TTree * treeToReadIn) : treeToRead(treeToReadIn), treeToWrite(0), dir(0), nEntries(0), iEntry(0), event(0), run(0), weight(0), PUWeight(0), el(0), elQ(0), mu(0), muQ(0), jets(0), bjetdisc(0), nbjets(0), isbjet(0), vMET(0) {

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
  
  el = new std::vector<LorentzM>();
  elQ = new std::vector<int>();
  mu = new std::vector<LorentzM>();
  muQ = new std::vector<int>();
  
  jets = new std::vector<LorentzM>();
  bjetdisc = new std::vector<double>();
  isbjet = new std::vector<bool>();

  vMET = new LorentzM();

  //Ensure that SumW2 is called and declare histograms
  TH1::SetDefaultSumw2(true);
  h_Mll = new TH1D("Mll", "Invariant mass of lepton pair;Mll;N", 300, 0., 300.);

  return;
}

void anDiLep::DeallocateMemory() {

  cout << "anDiLep::Decallocate mem!" << endl;

  if (el) {
    delete el;
    el = 0;
  }
  if (elQ) {
    delete elQ;
    elQ = 0;
  }
  if (mu) {
    delete mu;
    mu = 0;
  }
  if (muQ) {
    delete muQ;
    muQ = 0;
  }
  if (jets) {
    delete jets;
    jets = 0;
  }
  if (bjetdisc) {
    delete bjetdisc;
    bjetdisc = 0;
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

  event=0;
  run=0;
  weight=0.0;
  PUWeight=0.0;
  
  el->clear();
  elQ->clear();
  mu->clear();
  muQ->clear();
    
  jets->clear();
  bjetdisc->clear();
  nbjets=0;
  isbjet->clear();
  
  vMET->SetPxPyPzE(0.,0.,0.,0.);
}

void anDiLep::SetBranchesWrite() {

  if (!treeToWrite) {
    cerr << "anDiLep::SetBranchesWrite >> ERROR : treeToWrite not set!" << endl;
    return;
  }

  treeToWrite->Branch("Event",&event,"event/I");
  treeToWrite->Branch("Run",&run,"run/I");
  treeToWrite->Branch("Weight",&weight,"weight/D");
  treeToWrite->Branch("PUWeight",&PUWeight,"PUWeight/D");

  treeToWrite->Branch("el","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &el);
  treeToWrite->Branch("elQ","std::vector<int>", &elQ);
  treeToWrite->Branch("mu","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &mu);
  treeToWrite->Branch("muQ","std::vector<int>", &muQ);


  treeToWrite->Branch("jets", "std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &jets);
  treeToWrite->Branch("bjetdisc", &bjetdisc);
  treeToWrite->Branch("nbjets",&nbjets, "nbjets/I");
  treeToWrite->Branch("isbjet",&isbjet);

  treeToWrite->Branch("vMET",vMET);

  return;
}

void anDiLep::SetBranchesRead() {

  if (!treeToRead) {
    cerr << "anDiLep::SetBranchesRead >> ERROR : treeToRead not set!" << endl;
    return;
  }

  treeToRead->SetBranchAddress("Event",&event);
  treeToRead->SetBranchAddress("Run",&run);
  treeToRead->SetBranchAddress("Weight", &weight );
  treeToRead->SetBranchAddress("PUWeight",&PUWeight);
  
  treeToRead->SetBranchAddress("el",&el);
  treeToRead->SetBranchAddress("mu",&mu);
  treeToRead->SetBranchAddress("elQ",&elQ);
  treeToRead->SetBranchAddress("muQ",&muQ);

  treeToRead->SetBranchAddress("jets", &jets);
  treeToRead->SetBranchAddress("bjetdisc", &bjetdisc);
  treeToRead->SetBranchAddress("nbjets",&nbjets);
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

  event = info->Event;
  run = info->Run;    
  weight = info->EventWeight;
  PUWeight = info->PUWeight;

  for ( int iel = 0; iel < electrons_in.size(); iel++) {
    el->push_back( electrons_in.at(iel)->P4() );
    elQ->push_back( electrons_in.at(iel)->Charge() );
  }
  for ( int imu = 0; imu < muons_in.size(); imu++) {
    mu->push_back( muons_in.at(imu)->P4() );
    muQ->push_back( muons_in.at(imu)->Charge() );
  }
  for (int ijet=0; ijet<jets_in.size() ; ijet++) {
    jets->push_back(jets_in.at(ijet)->P4());
    
    bjetdisc->push_back(jets_in.at(ijet)->BJetDisc("CSV"));
    if ( jets_in.at(ijet)->IsBJet("CSV","Medium") ) {
      isbjet->push_back(true);
      nbjets+=1;
    }
    else isbjet->push_back(false);
  }

  *vMET = met_in;

  treeToWrite->Fill();

  std::vector<LorentzM> ossfPair;
  getOsSfPair(ossfPair);

  //Fill the histogram
  if (ossfPair.size() == 2) {
    LorentzM pll = ossfPair.at(0);
    pll = pll + ossfPair.at(1);
    h_Mll->Fill(pll.M(), weight);
  }

  return;
}

void anDiLep::Write(){
  
  if (!dir) {
    cout << "anDiLep::Write >> No dir is set! Cannot write!" << endl;
    return; 
  }

  dir->cd(); //Set current directory to the original directory, before write.
  
  if (treeToWrite) {
    cout << "anDiLep::Write >> WRITING TREE!" << endl;
    treeToWrite->Write();
  }
  if (h_Mll) {
    h_Mll->Write();
    cout << "anDiLep::Write >> WRITING HIST!" << endl;
  }
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


void anDiLep::getOsLeptonPair(const std::vector<LorentzM> * leptons, const std::vector<int> * leptonsQ, std::vector<LorentzM> & osPair) {

    osPair.clear();
    
    //Sense checks
    if (leptons == 0 || leptonsQ == 0) return;
    if (leptons->size() < 2) return;
    if (leptons->size() != leptonsQ->size() ) return;

    //Find highest pt lepton
    int highestPtLep = -1;
    double highestPt = -1.;

    for (int iLep = 0; iLep < leptons->size(); iLep++) {
      if (leptons->at(iLep).Pt() > highestPt ) {
	highestPt = leptons->at(iLep).Pt();
	highestPtLep = iLep;
      }
    }

    if (highestPtLep < 0) {
      cout << "anDiLep::getOsLeptonPair >> ERROR. Could not find highest Pt lepton." << endl;
      return;
    }
    //Now consider the leptons with the opposite sign to the highest pt lepton
    //Find the lepton in this set with highest Pt

    double highestPtOs = -1.;
    int highestPtOsLep = -1;
    const int highestPtCharge = leptonsQ->at(highestPtLep);

    for (int iLep = 0; iLep < leptons->size(); iLep++) {

      if (highestPtCharge * leptonsQ->at(iLep) >= 0 ) continue;

      if (leptons->at(iLep).Pt() > highestPtOs ) {
	highestPtOs = leptons->at(iLep).Pt();
	highestPtOsLep = iLep;
      }
    }

    //Order the lepton four-vectors by charge. Put negative charge (i.e. particle, not anti-particle) first.

    if (highestPtLep >= 0 && highestPtOsLep >= 0) {
      if (highestPtCharge < 0) {
	osPair.push_back(leptons->at(highestPtLep));
	osPair.push_back(leptons->at(highestPtOsLep));
      }
      else {
	osPair.push_back(leptons->at(highestPtOsLep));
	osPair.push_back(leptons->at(highestPtLep));
      }
    }


    return;
}

void anDiLep::getOsMuPair(std::vector<LorentzM> & muPair) {
  getOsLeptonPair(mu, muQ, muPair);
  return;
}
void anDiLep::getOsElPair(std::vector<LorentzM> & elPair) {
  getOsLeptonPair(el, elQ, elPair);
  return;
}
void anDiLep::getOsSfPair(std::vector<LorentzM> & ossfPair) {
  ossfPair.clear();

  std::vector<LorentzM> muOSPair;
  getOsMuPair(muOSPair);

  std::vector<LorentzM> elOSPair;
  getOsElPair(elOSPair);
  



  std::vector<LorentzM> * highestPtPair = 0;
  if ( muOSPair.size() == 2 && elOSPair.size() == 0 ) {
    highestPtPair = &muOSPair;
  }
  else if ( muOSPair.size() == 0 && elOSPair.size() == 2 ) {
    highestPtPair = &elOSPair;
  }
  else if (muOSPair.size() == 2 && elOSPair.size() == 2) { 
    //Pick the pair with the highest scalar sum of Pt
    double muSumPt = 0.;
    for (int iMu = 0; iMu < muOSPair.size() ; iMu++) {
      muSumPt += muOSPair.at(iMu).Pt();
    }
    double elSumPt = 0.;
    for (int iEl = 0; iEl < elOSPair.size() ; iEl++) {
      elSumPt += elOSPair.at(iEl).Pt();
    }

    if (muSumPt > elSumPt) {
      highestPtPair = &muOSPair;
    }
    else highestPtPair = &elOSPair;

  }
  else {
    return;
  }

  ossfPair = *highestPtPair;

  return;
}

double anDiLep::getHT() {

  if (jets == 0) return 0.;

  double HT = 0.;
  for (int iJet = 0; iJet < jets->size() ; iJet++) {
    HT += jets->at(iJet).Pt();
  }

  return HT;
}

double anDiLep::getMET() {
  if (vMET == 0) return 0.;
  return vMET->Pt();
}

double anDiLep::getMT(const LorentzM & vis, const LorentzM & inv, double visMass, double invMass) {
  double MTsq = 0.;

  double eT_vis = sqrt(visMass*visMass + vis.Pt()*vis.Pt());
  double eT_inv = sqrt(invMass*invMass + inv.Pt()*inv.Pt());

  MTsq  = visMass*visMass;
  MTsq += invMass*invMass;
  MTsq += 2.*eT_vis*eT_inv;
  MTsq += -2.*vis.Pt()*inv.Pt()*cos(vis.Phi() - inv.Phi());

  if (MTsq > 0) return sqrt(MTsq);

  return 0.;
}

double anDiLep::getWeight() {
  return weight;
}

int anDiLep::getNtags() {
  return nbjets;
}

void anDiLep::getMETv(LorentzM & met) {
  if (vMET) {
    met = *vMET; 
  }
  else {
    cout << "anDiLep::getMETv >> ERROR : vMET pointer not set!" << endl;
  }
  return;
}
