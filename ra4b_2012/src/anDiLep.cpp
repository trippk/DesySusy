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

anDiLep::anDiLep(TTree * treeToReadIn) : treeToRead(treeToReadIn), treeToWrite(0), dir(0), nEntries(0), iEntry(0), event(0), run(0), mY(-1.), mLsp(-1.), weight(0), PUWeight(0), nvtx(-1), el(0), elQ(0), mu(0), muQ(0), jets(0), bjetdisc(0), nbjets(0), isbjet(0), vMET_raw(0), vMET_type1(0) {

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

  vMET_raw = new LorentzM();
  vMET_type1 = new LorentzM();

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

  if (vMET_raw) {
    delete vMET_raw;
    vMET_raw = 0;
  }

  if (vMET_type1) {
    delete vMET_type1;
    vMET_type1 = 0;
  }

  return;
}


void anDiLep::SetToZero(){

  event=0;
  run=0;
  weight=0.0;
  PUWeight=0.0;
  nvtx=-1;
  mY=-1.;
  mLsp=-1.;

  el->clear();
  elQ->clear();
  mu->clear();
  muQ->clear();
    
  jets->clear();
  bjetdisc->clear();
  nbjets=0;
  isbjet->clear();
  
  vMET_raw->SetPxPyPzE(0.,0.,0.,0.);
  vMET_type1->SetPxPyPzE(0.,0.,0.,0.);
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
  treeToWrite->Branch("nvtx",&nvtx,"nvtx/I");
  treeToWrite->Branch("mY"  ,&mY  ,"mY/D");
  treeToWrite->Branch("mLsp",&mLsp,"mLsp/D");

  treeToWrite->Branch("el","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &el);
  treeToWrite->Branch("elQ","std::vector<int>", &elQ);
  treeToWrite->Branch("mu","std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &mu);
  treeToWrite->Branch("muQ","std::vector<int>", &muQ);


  treeToWrite->Branch("jets", "std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >", &jets);
  treeToWrite->Branch("bjetdisc", &bjetdisc);
  treeToWrite->Branch("nbjets",&nbjets, "nbjets/I");
  treeToWrite->Branch("isbjet",&isbjet);

  treeToWrite->Branch("vMET_raw",vMET_raw);
  treeToWrite->Branch("vMET_type1",vMET_type1);

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
  treeToRead->SetBranchAddress("nvtx",&nvtx);
  treeToRead->SetBranchAddress("mY",&mY);
  treeToRead->SetBranchAddress("mLsp",&mLsp);
  
  treeToRead->SetBranchAddress("el",&el);
  treeToRead->SetBranchAddress("mu",&mu);
  treeToRead->SetBranchAddress("elQ",&elQ);
  treeToRead->SetBranchAddress("muQ",&muQ);

  treeToRead->SetBranchAddress("jets", &jets);
  treeToRead->SetBranchAddress("bjetdisc", &bjetdisc);
  treeToRead->SetBranchAddress("nbjets",&nbjets);
  treeToRead->SetBranchAddress("isbjet",&isbjet);

  treeToRead->SetBranchAddress("vMET_raw",&vMET_raw);
  treeToRead->SetBranchAddress("vMET_type1",&vMET_type1);

  return;
}

void anDiLep::Fill(EventInfo* info, EasyChain* tree, std::vector<Muon*> & muons_in, std::vector<Electron*> & electrons_in, std::vector<Jet*> & jets_in, LorentzM& met_raw_in, LorentzM& met_type1_in) {

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
  nvtx = info->PUInter;
  mY = info->mY;
  mLsp = info->mLsp;

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

  *vMET_raw   = met_raw_in;
  *vMET_type1 = met_type1_in;

  treeToWrite->Fill();

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

// double anDiLep::getMET() {
//   if (vMET == 0) return 0.;
//   return vMET->Pt();
// }

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

void anDiLep::getMETv(LorentzM & met, const std::string & metName) {
  if (metName == "raw") {
    if (vMET_raw) met = *vMET_raw; 
    else cout << "anDiLep::getMETv >> ERROR : vMET pointer not set!" << endl;
  }
  else if (metName == "type1") {
    if (vMET_type1) met = *vMET_type1; 
    else cout << "anDiLep::getMETv >> ERROR : vMET pointer not set!" << endl;
  }
  else {
    cout << "anDiLep::getMETv >> ERROR : Unknown MET type: " << metName << endl;
  }
  return;
}


void anDiLep::getJets(std::vector<LorentzM> & jetsOut) {
  jetsOut.clear();
  if (jets == 0) return;
  jetsOut = *jets;
  return;
}
void anDiLep::getTaggedJets(std::vector<LorentzM> & jetsOut) {
  jetsOut.clear();
  if (jets == 0 || isbjet == 0) {
    cout << "anDiLep::getTaggedJets >> jets or isbjet not set!" << endl;
    return;
  }
  if (jets->size() != isbjet->size() ) {
    cout << "anDiLep::getTaggedJets >> jets and isbjet vectors are different sizes!" << endl;
    return;
  }

  for (int iJet = 0; iJet < jets->size() ; iJet++ ) {
    if (isbjet->at(iJet)) jetsOut.push_back( jets->at(iJet) );
  }

  return;
}
void anDiLep::getUntaggedJets(std::vector<LorentzM> & jetsOut) {
  jetsOut.clear();
  if (jets == 0 || isbjet == 0) {
    cout << "anDiLep::getUntaggedJets >> jets or isbjet not set!" << endl;
    return;
  }
  if (jets->size() != isbjet->size() ) {
    cout << "anDiLep::getUntaggedJets >> jets and isbjet vectors are different sizes!" << endl;
    return;
  }

  for (int iJet = 0; iJet < jets->size() ; iJet++ ) {
    if (!isbjet->at(iJet)) jetsOut.push_back( jets->at(iJet) );
  }

  return;
}

void anDiLep::getMuons(std::vector<LorentzM> & muOut) {
  muOut.clear();
  if (mu == 0) return;
  muOut = *mu;
  return;
}
void anDiLep::getElectrons(std::vector<LorentzM> & elOut) {
  elOut.clear();
  if (el == 0) return;
  elOut = *el;
  return;
}



double anDiLep::getMT2W (const LorentzM & lepton, const std::vector<LorentzM> & taggedJets, const std::vector<LorentzM> & untaggedJets, const LorentzM & mpt) {

  //Create the possible jet pairs
  typedef std::vector<std::pair<const LorentzM*, const LorentzM*> > jetPairs_t;
  jetPairs_t jetPairs; //First jet in the pair is attached to the lepton

  typedef std::vector<LorentzM> jet_t;
  jet_t::const_iterator jetIt1;
  jet_t::const_iterator jetIt2;

  //Method depends upon the number of tagged jets
  if (taggedJets.size() >= 2) {
    //If more than two tagged jets are found, use all pairs of tagged jets to
    //assign the jets in forming MT2W.
    //Assume ignorance about which b-jet in the pair belongs to the lepton,
    //i.e. consider both possibilities
    for (jetIt1 = taggedJets.begin() ; jetIt1 != taggedJets.end() ; jetIt1++ ) {
      for (jetIt2 = jetIt1 + 1; jetIt2 != taggedJets.end() ; jetIt2++ ) {
	jetPairs.push_back(make_pair( &*jetIt1, &*jetIt2 )); 
	jetPairs.push_back(make_pair( &*jetIt2, &*jetIt1 ));
      }
    }

  }
  else if (taggedJets.size() == 1) {
    //If only one tagged jet, assign it to one of the jets in MT2W. 
    //For the remaining jet, take into account ALL possible untagged jets.
    //Again assume ignorance about whether the 
    jetIt1 = taggedJets.begin();
    for (jetIt2 = untaggedJets.begin(); jetIt2 != untaggedJets.end() ; jetIt2++ ) {
	jetPairs.push_back(make_pair( &*jetIt1, &*jetIt2));
	jetPairs.push_back(make_pair( &*jetIt2, &*jetIt1));
    }
    
  }
  else {
    //If no tagged jets, consider all pairs of untagged jets
    for (jetIt1 = untaggedJets.begin() ; jetIt1 != untaggedJets.end() ; jetIt1++ ) {
      for (jetIt2 = jetIt1 + 1; jetIt2 != untaggedJets.end() ; jetIt2++ ) {
	jetPairs.push_back(make_pair( &*jetIt1, &*jetIt2));
	jetPairs.push_back(make_pair( &*jetIt2, &*jetIt1));
      }
    }

  }

  //Create instance of the mt2w class
  double mt2w_upper_bound = 500.0;
  double mt2w_error_value = 499.0;
  double mt2w_scan_step = 0.5;
  mt2w_bisect::mt2w mt2w_calc(mt2w_upper_bound, mt2w_error_value, mt2w_scan_step);

  double mt2w_min = mt2w_upper_bound;
  //For each jet pair calculate MT2W. Find the smallest possible value.
  for (jetPairs_t::const_iterator jetPair = jetPairs.begin() ; jetPair != jetPairs.end(); jetPair++) {
    mt2w_calc.set_momenta(lepton.E(), lepton.Px(), lepton.Py(), lepton.Pz(),
			  jetPair->first->E(), jetPair->first->Px(), jetPair->first->Py(), jetPair->first->Pz(),
			  jetPair->second->E(), jetPair->second->Px(), jetPair->second->Py(), jetPair->second->Pz(),
			  mpt.Px(), mpt.Py()
			  );
    double mt2w = mt2w_calc.get_mt2w();
    if (mt2w <= mt2w_min) mt2w_min = mt2w;
  }

  return mt2w_min;  
}

double anDiLep::getMT2(const LorentzM & p1, const LorentzM & p2, const LorentzM & mpt) {

  mt2_bisect::mt2 mt2_calc;
  double p10[3] = { p1.M(), p1.Px(), p1.Py() };
  double p20[3] = { p2.M(), p2.Px(), p2.Py() };
  double pmiss[3] = {0., mpt.Px(), mpt.Py()};
  mt2_calc.set_momenta(p10, p20, pmiss);
  mt2_calc.set_mn(0.);
  
  return mt2_calc.get_mt2();

}


bool anDiLep::isScanPoint(double mY_in, double mLsp_in) const {
  
  const double TOL = 0.01;

  if (  (fabs(mY - mY_in) < TOL) && (fabs(mLsp - mLsp_in) < TOL )  ) return true;
  else return false;
}

