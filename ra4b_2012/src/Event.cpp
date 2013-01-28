#include "Event.h"

extern bool pcp;

event::event(EasyChain * tree_in) : tree(tree_in), ControlPlots(0), outTree(0), jetMon(0) {
  return;
}

event::~event() {
  return;
}


bool event::setSampleInformation() {

  if (tree == 0) {
    cout << "event::setSampleInformation >> ERROR tree is NULL."<< endl;
    return false;
  }

  ConfigReader config;

  //===========================================================================
  // SET THE INFORMATION ABOUT THE SAMPLE
  //===========================================================================
  SampleInformation mySampleInfo;

  //it HAS to be read from config.txt, otherwise the default is
  //undefined
  string WhatSample=config.getString("SampleName","undefined");
  string WhatSubSample=config.getString("SubSampleName","undefined");
  string WhatEstimation=config.getString("Estimation","undefined");
  string WhatTail=config.getString("Tail","undefined");
  mySampleInfo.SetInformation(WhatSample,WhatSubSample,WhatEstimation,WhatTail);
  //print it
  mySampleInfo.DumpInformation();

  //Here extract whether the sample is a signal scan, and extract mass point
  //from the subsample name.
  isSignalScanSample = false;
  if (WhatSample == "T2tt") {
    isSignalScanSample = true;
  }
  if (isSignalScanSample) {
    cout << "Using a signal scan sample: " << WhatSample << endl;
  }

  //===========================================================================
  // SET THE CONDITIONS FOR THE SAMPLE AND ESTIMATION
  //===========================================================================
  SetOfCuts myCuts;
  SetConditions_RA4b(mySampleInfo);
  //===========================================================================

  if(pcp)cout<<"going to set the triggers "<<endl;
  //===========================================================================
  // SET THE TRIGGERS
  //===========================================================================
  SetTriggers_RA4b(mySampleInfo,triggernames);
  //===========================================================================
  if(pcp)cout<<"out of set the triggers "<<endl;
  if(pcp)cout<<"check point 1"<<endl;

  isData=config.getBool("isData");

  //======================================================
  //WEIGHTS
  //======================================================
  //PU
  int nobinsmc=0;
  int nobinsdata=0;
  if(!isData){
    nobinsmc = config.getDouble("PU_"+WhatSample+"_"+WhatSubSample+"_mc",PUmc," ");
    nobinsdata = config.getDouble("PU_data",PUdata," ");
  } 
  if(nobinsmc!=nobinsdata && !isData){
    cout << "problem in pu inf in para_config - number of bins in MC PU dist is different than data!" << endl;
    return false;
  }

  doControlPlots=config.getBool("DoControlPlots",true); 

  //===========================================
  turntriggersoff=config.getBool("TurnTriggersOff",false);
  if(turntriggersoff){
    cout<<"-----------TURNTRIGGERSOFF IS true!!-----------"<<endl;
    if(isData){
      cout<<"--------NO TRIGGERS, NO DATA--------------------"<<endl;
      return false;
    }
  }
  //===========================================

  return true;
}


void event::resetAll() {

  EventNum = 0;
  RunNum = 0;

  mY = -1.;
  mLsp = -1.;

  EventWeight = 1.;
  PUWeight = 1.;

  nGoodVertices = -1;

  Muons.clear();
  LooseMuons.clear();
  TightMuons.clear();
  VetoMuons.clear();

  Electrons.clear();
  LooseElectrons.clear();
  TightElectrons.clear();
  VetoElectrons.clear();
  CleanedTightElectrons.clear();
  CleanedVetoElectrons.clear();
  TrkOrGlobalMuons.clear();

  Jets.clear();
  AllJets.clear();
  GoodJets.clear();
  CleanedJets.clear();

  SignalMuons.clear();
  WideMuons.clear();

  SignalElectrons.clear();
  WideElectrons.clear();

  osPairs.clear();
  trigsFiredMap.clear();
  firedDiLepTypes.clear();

  HT = 0.;
  PFmetRaw.SetPxPyPzE(0.,0.,0.,0.);
  PFmetType1.SetPxPyPzE(0.,0.,0.,0.);

  return;
}


void event::getEntry(int i) {
  resetAll();  

  tree->GetEntry(i);

  EventNum = tree->Get(EventNum,"event");
  RunNum   = tree->Get(RunNum,"run");    

  //If a signal scan, get the masses of the SUSY particles
  if (isSignalScanSample) {
    mY   = tree->Get( mY  , "susyScanmStop");
    mLsp = tree->Get( mLsp, "susyScanmLSP");
  }

  return;
}

void event::doPU_RW() {

  //==============================================
  // PILE UP RW
  //==============================================
  PUWeight=1.;
  if(!isData) {
    float PUnumInter    = tree->Get( PUnumInter, "pileupTrueNumInteractionsBX0");
    int relevantNumPU = (int) PUnumInter;
    if( relevantNumPU >= PUmc.size() ) {
      cout << "something wrong with the pile up info!!! - exceed max number of vertex:     " << PUmc.size() <<endl;
      return; 
    }
    else {
      PUWeight= PUdata.at( relevantNumPU )/PUmc.at( relevantNumPU );
    }
    EventWeight *= PUWeight;
  }
  CutSet::global_event_weight  = EventWeight;
  HistoMaker::global_event_weight = EventWeight; 
  //==============================================

  return;
}

void event::createObjects() {
  //           C R E A T E    O B J E C T S                            //

  //============================================
  //Make MET
  LorentzM& PFmetType1_r = tree->Get(&PFmetType1_r, "metP4TypeIPF");
  LorentzM& PFmetRaw_r = tree->Get(&PFmetRaw_r, "metP4PF");
  PFmetRaw = PFmetRaw_r;
  PFmetType1 = PFmetType1_r;
  //============================================


  //============================================
  // Make Muons
  Muons=makeAllMuons(tree);
  std::vector<Muon*> AllMuons;
  for (int imu=0;imu<(int)Muons.size();++imu){
    AllMuons.push_back(&Muons.at(imu));
  }

  if (!isData) {
    //Manipulate muons according to systematic options. Simultaneously correct MET.
    LorentzM metCorr;
    metCorr.SetPxPyPzE(0.,0.,0.,0.);

    if (sysOptions.murDo) {
      if (pcp) cout << "Rescaling muon resolution."<< endl;
      //rescaleMUR(tree, AllMuons, metCorr, sysOptions.murErr);
      rescaleMUR_simple(tree, AllMuons, metCorr, sysOptions.murErr);
    }

    //Update the met with the corrections
    if (pcp) cout << "Implementing met correction: METCORR=(" << metCorr.Px() << ","  << metCorr.Py() << ","  << metCorr.Pz() << ","  << metCorr.E() << ")" << endl;
    PFmetType1 += metCorr;
    PFmetRaw += metCorr;
  }

  makeTightMuons(tree,Muons,TightMuons, cutFlows.muonTight);
  makeLooseMuons(tree,Muons,LooseMuons, cutFlows.muonLoose);
  makeVetoMuons(tree,Muons,VetoMuons, cutFlows.muonVeto);
  //============================================

  //============================================
  // Make Electrons
  Electrons=makeAllElectrons(tree);
  makeTightElectrons(tree,Electrons,TightElectrons, cutFlows.electronTight);
  makeVetoElectrons(tree, Electrons,VetoElectrons, cutFlows.electronVeto);

  TrkOrGlobalMuons = makeTrkOrGlobalMuons(tree);
  makeCleanedElectrons(TightElectrons, CleanedTightElectrons, TrkOrGlobalMuons, cutFlows.electronTightCleaned);
  makeCleanedElectrons(VetoElectrons , CleanedVetoElectrons , TrkOrGlobalMuons, cutFlows.electronVetoCleaned);
  //============================================

  //============================================    
  // Make Jets
  Jets=makeAllJets(tree);
  for (int ijet=0;ijet<(int)Jets.size();++ijet){
    AllJets.push_back(&Jets.at(ijet));
  }

  if (!isData) {
    //Manipulate jets according to systematic options. Simultaneously correct MET.
    LorentzM metCorr;
    metCorr.SetPxPyPzE(0.,0.,0.,0.);

    if (sysOptions.jerDo) {
      if (pcp) cout << "Rescaling jet resolution."<< endl;

      //We need to clean the jet collection of leptons.
      vector<Jet*> jetsToSmear;
      makeCleanedJets( AllJets, jetsToSmear, TightMuons, CleanedTightElectrons);

      rescaleJER(tree, jetsToSmear, metCorr, sysOptions.jerErr, jetMon);
    }

    if (sysOptions.jesDo) {
      if (pcp) cout << "Rescaling jet energy."<< endl;

      //We need to clean the jet collection of leptons.
      vector<Jet*> jetsToSmear;
      makeCleanedJets( AllJets, jetsToSmear, TightMuons, CleanedTightElectrons);

      rescaleJES(tree, jetsToSmear, metCorr, sysOptions.jesErr);
    }

    //Update the met with the corrections
    if (pcp) cout << "Implementing met correction: METCORR=(" << metCorr.Px() << ","  << metCorr.Py() << ","  << metCorr.Pz() << ","  << metCorr.E() << ")" << endl;
    PFmetType1 += metCorr;
    PFmetRaw += metCorr;
  }

  makeGoodJets(tree,AllJets,GoodJets, cutFlows.jetsGood);
  makeCleanedJets( GoodJets, CleanedJets, TightMuons, CleanedTightElectrons, cutFlows.jetsCleaned); 

  HT=0.0;
  for(int ijet=0;ijet<(int)CleanedJets.size();++ijet){
    HT=HT+CleanedJets.at(ijet)->Pt();	
  }
  //============================================

  ConfigReader config;
  string SigMu = config.getString("SignalMuon_Selection","Tight");
  string WidMu = config.getString("WideMuon_Selection","Veto");
  string SigEl = config.getString("SignalElectron_Selection","Tight");
  string WidEl = config.getString("WideElectron_Selection","Veto");

  for (int imu=0; (int)imu<Muons.size();++imu){
    if (Muons.at(imu).IsID(SigMu) ){  
      SignalMuons.push_back(&Muons.at(imu));
    }
    else if (!Muons.at(imu).IsID(SigMu) && Muons.at(imu).IsID(WidMu) ){
      WideMuons.push_back(&Muons.at(imu) ) ;
    }
  }

  for (int imu=0; (int)imu<CleanedTightElectrons.size();++imu){
    if (CleanedTightElectrons.at(imu)->IsID("Tight")) {
      SignalElectrons.push_back(CleanedTightElectrons.at(imu));
    }
  }
  for (int imu=0; (int)imu<CleanedVetoElectrons.size();++imu){
    if ( !CleanedVetoElectrons.at(imu)->IsID("Tight") && CleanedVetoElectrons.at(imu)->IsID("Veto") ) {
      WideElectrons.push_back(CleanedVetoElectrons.at(imu));
    }
  }

  getDiLepTriggers(tree, trigsFiredMap, firedDiLepTypes);
  makeOsLepPairs(firedDiLepTypes, SignalMuons, SignalElectrons, osPairs);

  return;
}

bool event::applyCuts(const std::vector<std::string> & cutVec) {

  if (ControlPlots == 0) {
    cout << "event::applyCuts >> ERROR ControlPlots is NULL." << endl;
    return false;
  }
  if (cutFlows.globalflow == 0) {
    cout << "event::applyCuts >> ERROR globalflow is NULL." << endl;
    return false;
  }

  if (doControlPlots) ControlPlots->MakePlots("Before_CutFlow", TightMuons, CleanedTightElectrons, CleanedJets, PFmetType1); 

  bool passAllCuts = true;
  
  vector<Muon*> * muonsPlot = &TightMuons;
  vector<Electron*> * electronsPlot = &CleanedTightElectrons;
  vector<Jet*> * jetsPlot = &CleanedJets;

  std::vector<std::string>::const_iterator cutIt;
  for (cutIt = cutVec.begin() ; cutIt != cutVec.end() ; cutIt++ ) {
    bool passCut = applyCut(*cutIt, muonsPlot, electronsPlot, jetsPlot);
    cutFlows.globalflow->keepIf(*cutIt, passCut);
    passAllCuts = passAllCuts&&passCut;
    if (passAllCuts) {
      if (doControlPlots) ControlPlots->MakePlots(*cutIt, *muonsPlot, *electronsPlot, *jetsPlot, PFmetType1); 
    }
    else break;      
  }
  
  return passAllCuts;
}

bool event::applyCut(const string &cutName, vector<Muon*> * muonsPlot, vector<Electron*> * electronsPlot, vector<Jet*> * jetsPlot) {
  
  bool OK = false;

  muonsPlot = &TightMuons;
  electronsPlot = &CleanedTightElectrons;
  jetsPlot = &CleanedJets;

  if (cutName == "Triggers"){
    if (turntriggersoff) OK = true;
    else OK = triggers_RA4b(tree, triggernames,EventWeight, cutFlows.triggerflow);
  }
  else if (cutName == "hcalLaserFilter") {
    OK = tree->Get( OK,    "hcalLaserEventFilterFlag"  );
  }
  else if (cutName == "eeBadSCFilter"){
    OK = tree->Get( OK, "eeBadScFilterFlag"   );
  }
  else if (cutName == "Scraping_Veto"){
    unsigned int nTracksAll=tree->Get(nTracksAll,"nTracksAll");
    unsigned int nTracksHighPurity=tree->Get(nTracksHighPurity,"nTracksHighPurity");
    
    if(nTracksAll>10){
      if( ( (double) nTracksHighPurity)/ ( (double) nTracksAll ) > 0.25)OK=true;
    }
    else OK = true;
  }
  else if (cutName == "PV"){
    vector<int> goodVert;
    OK = vertices_RA4b(tree,goodVert);
    nGoodVertices = goodVert.size(); //Store the number of good vertices in the event
  }
  else if (cutName == "HBHE"){
    OK = evtqual_RA4b(tree);
  }
  else if (cutName == "CSC_HALO"){
    if (isSignalScanSample) OK=true;
    else OK = cschalo_RA4b(tree);
  }
  else if (cutName == "trackingFailure"){
    OK = trackingFailure_RA4b(tree);
  }
  else if (cutName == "ECAL_TP"){
    OK = tree->Get(OK,"ecalDeadCellTPFilterFlag");
  }
  else if (cutName == "One_single_lepton"){
    OK=SetOfCuts::Leptons.NUM.Examine((int)SignalElectrons.size()+(int)SignalMuons.size());
  }
  else if (cutName == "Jet_Cuts"){
    OK=SetOfCuts::Jets.NUM.Examine(CleanedJets.size());
    muonsPlot = &SignalMuons;
    electronsPlot = &SignalElectrons;
  }
  else if (cutName == "Signal_Muons"){
    OK=SetOfCuts::SignalMuons.NUM.Examine(SignalMuons.size());
    muonsPlot = &SignalMuons;
    electronsPlot = &SignalElectrons;
  }
  else if (cutName == "Signal_Electrons"){
    OK=SetOfCuts::SignalElectrons.NUM.Examine(SignalElectrons.size());
    muonsPlot = &SignalMuons;
    electronsPlot = &SignalElectrons;
  }
  else if (cutName == "Wide_Muons"){
    OK=SetOfCuts::WideMuons.NUM.Examine(WideMuons.size());
    muonsPlot = &WideMuons;
  }
  else if (cutName == "Wide_Electrons"){
    OK=SetOfCuts::WideElectrons.NUM.Examine(WideElectrons.size());
    electronsPlot = &WideElectrons;
  }
  else if (cutName == "HT"){
    OK=SetOfCuts::Event.HT.Examine(HT);
    muonsPlot = &SignalMuons;
    electronsPlot = &SignalElectrons;
  }
  else if (cutName == "MET"){
    OK=SetOfCuts::Event.MET.Examine(PFmetType1.pt());
    muonsPlot = &SignalMuons;
    electronsPlot = &SignalElectrons;
  }
  else if (cutName == "MET_RAW"){
    OK=SetOfCuts::Event.MET.Examine(PFmetRaw.pt());
    muonsPlot = &SignalMuons;
    electronsPlot = &SignalElectrons;
  }
  else if (cutName == "OS_FLAV_THRESH"){
    OK=chkOsFlavTrigger();
    muonsPlot = &SignalMuons;
    electronsPlot = &SignalElectrons;
  }
  else {
    cout << "Unknown cut: " << cutName << endl;
  }

  return OK;
}

void event::fillTree() {

  if (outTree == 0) {
    cout << "event::fillTree >> ERROR outTree pointer in NULL." << endl;
    return;
  }

  EventInfo info;
  info.Event=EventNum;
  info.Run=RunNum;
  info.EventWeight=EventWeight;
  info.PUWeight=PUWeight;
  info.mY=mY;
  info.mLsp=mLsp;
  info.PUInter=nGoodVertices;

  //=====================FILL THE TREES====================
  anDiLep * anDiLepTree = dynamic_cast<anDiLep*>(outTree);
  if (anDiLepTree == 0) {
    outTree->Fill( &info, tree, SignalMuons, SignalElectrons, CleanedJets, PFmetType1);
  }
  else {
    anDiLepTree->Fill( &info, tree, SignalMuons, SignalElectrons, CleanedJets, PFmetRaw, PFmetType1);
  }
	
  return;
}

bool event::chkOsFlavTrigger() {

  const bool isSyncExercise = false; //Set to true for comparison with Hannes

  //Look at first trigger in triggernames. Is this a MuMu, ElEl or MuEG *DATASET*?
  diLepEvent_t diLepEventType = getDiLepTrigType( triggernames.at(0) ); 
   
  bool osPairExists = false;
  bool osPairFlavorMatch = false;
  bool osPairTrigPlateau = false;
  
  if ( osPairs.size() > 0 ) {
    osPairExists = true;
      
    std::list<osPair>::const_iterator osPairIt;
    switch (diLepEventType) 
      {
      case MUMU:

	if (isSyncExercise) {
	  for (osPairIt = osPairs.begin() ; osPairIt != osPairs.end() ; osPairIt++ ) {
	    if (osPairIt->first->Flavor() == "mu" && osPairIt->second->Flavor() == "mu") {
	      osPairFlavorMatch = true;
	      if (osPairIt->first->Pt() > 20. || osPairIt->second->Pt() > 20.) {
		osPairTrigPlateau = true;
	      }
	    }
	  }
	}
	else {
	  if (osPairs.front().first->Flavor() == "mu" && osPairs.front().second->Flavor() == "mu") {
	    osPairFlavorMatch = true;
	    if (osPairs.front().first->Pt() > 20. || osPairs.front().second->Pt() > 20.) {
	      osPairTrigPlateau = true;
	      }
	  }
	}

	break;
      case ELEL:

	if (isSyncExercise) {
	  for (osPairIt = osPairs.begin() ; osPairIt != osPairs.end() ; osPairIt++ ) {
	    if (osPairIt->first->Flavor() == "el" && osPairIt->second->Flavor() == "el") {
	      osPairFlavorMatch = true;
	      if (osPairIt->first->Pt() > 20. || osPairIt->second->Pt() > 20.) {
		osPairTrigPlateau = true;
	      }
	    }
	  }
	}
	else {
	  if (osPairs.front().first->Flavor() == "el" && osPairs.front().second->Flavor() == "el") {
	    osPairFlavorMatch = true;
	    if (osPairs.front().first->Pt() > 20. || osPairs.front().second->Pt() > 20.) {
	      osPairTrigPlateau = true;
	    }
	  }
	}
	
	break;
      case MUEL:
	
	if (isSyncExercise) {
	  for (osPairIt = osPairs.begin() ; osPairIt != osPairs.end() ; osPairIt++ ) {
	    if ((osPairIt->first->Flavor() == "el" && osPairIt->second->Flavor() == "mu")||(osPairIt->first->Flavor() == "mu" && osPairIt->second->Flavor() == "el")) {
	      osPairFlavorMatch = true;
	      if (osPairIt->first->Pt() > 20. || osPairIt->second->Pt() > 20.) {
		osPairTrigPlateau = true;
	      }
	    }
	  }
	}
	else {
	  if (osPairs.front().first->Flavor() == "el" && osPairs.front().second->Flavor() == "mu") {
	    osPairFlavorMatch = true;
	    osPairTrigPlateau = isMuEGPlateau( trigsFiredMap, osPairs.front().second->Pt(), osPairs.front().first->Pt() );
	  }
	  else if ( osPairs.front().first->Flavor() == "mu" && osPairs.front().second->Flavor() == "el") {
	    osPairFlavorMatch = true;
	    osPairTrigPlateau = isMuEGPlateau( trigsFiredMap, osPairs.front().first->Pt(), osPairs.front().second->Pt() );
	  }
	}
	break;
      default:
	osPairFlavorMatch = false;
	osPairTrigPlateau = false;	  
      }
  }

  bool OK = osPairExists && osPairFlavorMatch && osPairTrigPlateau;
 
  return OK;
}
