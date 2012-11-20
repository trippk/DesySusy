#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Module.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

SusyDESY_Electrons::SusyDESY_Electrons(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PatElectrons( iConfig.getParameter<edm::InputTag> ("PatElectrons") ),
    conversionsInputTag_( iConfig.getParameter<edm::InputTag> ("conversionsInputTag") ),
    beamSpotInputTag_( iConfig.getParameter<edm::InputTag> ("beamSpotInputTag") )
{
  produces <bool>                 ( Prefix + "PatElectronsHandleValid" + Suffix );

  produces <std::vector<float> > ( Prefix + "SuperClusterEnergy"          + Suffix ); 
//   produces <std::vector<math::XYZPoint> > ( Prefix +"SuperClusterPosition"+ Suffix ); 
//   produces <std::vector<float> > ( Prefix + "SuperClusterRawEnergy"       + Suffix ); 
//   produces <std::vector<float> > ( Prefix + "SuperClusterEtaWidth"        + Suffix );
//   produces <std::vector<float> > ( Prefix + "SuperClusterPhiWidth"        + Suffix );
//   produces <std::vector<float> > ( Prefix + "SuperClusterPreshowerEnergy" + Suffix );

  produces <std::vector<bool> > ( Prefix + "GenMatched" + Suffix );
  produces <std::vector<int> >  ( Prefix + "GenPdgId"   + Suffix );
  produces <std::vector<int> >  ( Prefix + "GenStatus"  + Suffix );

  if(Suffix == "Pat") {
    produces <std::vector<float> > ( Prefix + "SimpleEleId95relIso" + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId90relIso" + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId85relIso" + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId80relIso" + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId70relIso" + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId60relIso" + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId95cIso"   + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId90cIso"   + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId85cIso"   + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId80cIso"   + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId70cIso"   + Suffix );
    produces <std::vector<float> > ( Prefix + "SimpleEleId60cIso"   + Suffix );
  }

  produces <std::vector<bool> > ( Prefix + "IsGsfCtfChargeConsistent"      + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsGsfCtfScPixChargeConsistent" + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsGsfScPixChargeConsistent"    + Suffix );

  produces <std::vector<bool> > ( Prefix + "EcalDrivenSeed"    + Suffix );
  produces <std::vector<bool> > ( Prefix + "TrackerDrivenSeed" + Suffix );

  produces <std::vector<bool> > ( Prefix + "IsEB"  + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsEE"  + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsGap" + Suffix );

  produces <std::vector<int> >  ( Prefix + "IdTriggerTight" + Suffix);
  produces <std::vector<bool> > ( Prefix + "HasMatchedConversions" + Suffix );
//   produces <std::vector<> > ( Prefix + ""   + Suffix );
}

void SusyDESY_Electrons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<bool>                 handleValid       ( new bool(false)           );

  std::auto_ptr<std::vector<float> > SCenergy           ( new std::vector<float>() );
//   std::auto_ptr<std::vector<math::XYZPoint> > SCposition( new std::vector<math::XYZPoint>() );
//   std::auto_ptr<std::vector<float> > SCrawEnergy        ( new std::vector<float>() );
//   std::auto_ptr<std::vector<float> > SCetaWidth         ( new std::vector<float>() );
//   std::auto_ptr<std::vector<float> > SCphiWidth         ( new std::vector<float>() );
//   std::auto_ptr<std::vector<float> > SCpreshowerEnergy  ( new std::vector<float>() );

  std::auto_ptr<std::vector<bool> > GenMatched         ( new std::vector<bool>() );
  std::auto_ptr<std::vector<int> >  GenPdgId           ( new std::vector<int>()  );
  std::auto_ptr<std::vector<int> >  GenStatus          ( new std::vector<int>()  );

  std::auto_ptr<std::vector<float> > simpleEleId95relIso ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId90relIso ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId85relIso ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId80relIso ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId70relIso ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId60relIso ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId95cIso   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId90cIso   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId85cIso   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId80cIso   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId70cIso   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > simpleEleId60cIso   ( new std::vector<float>() );

  std::auto_ptr<std::vector<bool> > isGsfCtfChargeConsistent      ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > isGsfCtfScPixChargeConsistent ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > isGsfScPixChargeConsistent    ( new std::vector<bool>() );

  std::auto_ptr<std::vector<bool> > EcalDrivenSeed    ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > TrackerDrivenSeed ( new std::vector<bool>() );

  std::auto_ptr<std::vector<bool> > IsEB  ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > IsEE  ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > IsGap ( new std::vector<bool>() );

  std::auto_ptr<std::vector<int> > idTrigTight(  new std::vector<int>() );

  std::auto_ptr<std::vector<bool> > matchedConv(  new std::vector<bool>() );

  edm::Handle<reco::ConversionCollection> conversions_h;
  iEvent.getByLabel(conversionsInputTag_, conversions_h);

  edm::Handle<reco::BeamSpot> beamspot_h;
  iEvent.getByLabel(beamSpotInputTag_, beamspot_h);
  const reco::BeamSpot &beamSpot = *(beamspot_h.product());

  edm::Handle<std::vector<pat::Electron> > ElColl;
  iEvent.getByLabel(PatElectrons, ElColl);

  if(ElColl.isValid()){
    *handleValid.get() = true;
    
    for(std::vector<pat::Electron>::const_iterator el = ElColl->begin(); el!=ElColl->end(); el++) {

      SCenergy         ->push_back(el->superCluster()->energy()         );
//       SCposition       ->push_back(el->superCluster()->position()       );
//       SCrawEnergy      ->push_back(el->superCluster()->rawEnergy()      );
//       SCetaWidth       ->push_back(el->superCluster()->etaWidth()       );	
//       SCphiWidth       ->push_back(el->superCluster()->phiWidth()       );
//       SCpreshowerEnergy->push_back(el->superCluster()->preshowerEnergy());
      

      if(Suffix == "Pat") {
	simpleEleId95relIso->push_back(el->electronID("simpleEleId95relIso"));
	simpleEleId90relIso->push_back(el->electronID("simpleEleId90relIso"));
	simpleEleId85relIso->push_back(el->electronID("simpleEleId85relIso"));
	simpleEleId80relIso->push_back(el->electronID("simpleEleId80relIso"));
	simpleEleId70relIso->push_back(el->electronID("simpleEleId70relIso"));
	simpleEleId60relIso->push_back(el->electronID("simpleEleId60relIso"));
	simpleEleId95cIso  ->push_back(el->electronID("simpleEleId95cIso"  ));
	simpleEleId90cIso  ->push_back(el->electronID("simpleEleId90cIso"  ));
	simpleEleId85cIso  ->push_back(el->electronID("simpleEleId85cIso"  ));
	simpleEleId80cIso  ->push_back(el->electronID("simpleEleId80cIso"  ));
	simpleEleId70cIso  ->push_back(el->electronID("simpleEleId70cIso"  ));
	simpleEleId60cIso  ->push_back(el->electronID("simpleEleId60cIso"  ));
      }
      
      isGsfCtfChargeConsistent     ->push_back(el->isGsfCtfChargeConsistent()     );
      isGsfCtfScPixChargeConsistent->push_back(el->isGsfCtfScPixChargeConsistent());
      isGsfScPixChargeConsistent   ->push_back(el->isGsfScPixChargeConsistent()   );

      EcalDrivenSeed   ->push_back(el->ecalDrivenSeed()   );
      TrackerDrivenSeed->push_back(el->trackerDrivenSeed());
      
      IsEB ->push_back(el->isEB() );
      IsEE ->push_back(el->isEE() );
      IsGap->push_back(el->isGap());
      
      if(el->genLepton()){
	GenMatched->push_back(true);
	GenPdgId->push_back(el->genLepton()->pdgId());
	GenStatus->push_back(el->genLepton()->status());
      }
      else{
	GenMatched->push_back(false);
	GenPdgId->push_back(0);
	GenStatus->push_back(-1);
      }

      bool trigtight = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERTIGHT, el->isEB(), 
							    el->pt(), //el->superCluster()->eta(),
							    el->deltaEtaSuperClusterTrackAtVtx(), el->deltaPhiSuperClusterTrackAtVtx(),
							    el->sigmaIetaIeta(), el->hadronicOverEm(),
							    el->dr03TkSumPt(), el->dr03EcalRecHitSumEt(), el->dr03HcalTowerSumEt());

      idTrigTight->push_back(trigtight);

      bool vtxFitConversion = ConversionTools::hasMatchedConversion((const reco::GsfElectron) *el, conversions_h, beamSpot.position());
      matchedConv->push_back(vtxFitConversion);

      //os<<"genMatched.back() = " << genMatched << endl;
      //os<<"genPdgId.back()   = " << genPdgId.back() << endl;
      //os<<"genStatus.back()  = " << genStatus.back() << endl;
      //os<<endl;
    }
  }

  iEvent.put( handleValid       , Prefix + "PatElectronsHandleValid" + Suffix );

  iEvent.put( SCenergy          , Prefix + "SuperClusterEnergy"          + Suffix );
//   iEvent.put( SCposition        , Prefix + "SuperClusterPosition"        + Suffix );
//   iEvent.put( SCrawEnergy       , Prefix + "SuperClusterRawEnergy"       + Suffix );
//   iEvent.put( SCetaWidth        , Prefix + "SuperClusterEtaWidth"        + Suffix );
//   iEvent.put( SCphiWidth        , Prefix + "SuperClusterPhiWidth"        + Suffix );
//   iEvent.put( SCpreshowerEnergy , Prefix + "SuperClusterPreshowerEnergy" + Suffix );

  iEvent.put( GenMatched        , Prefix + "GenMatched"              + Suffix );
  iEvent.put( GenPdgId          , Prefix + "GenPdgId"                + Suffix );
  iEvent.put( GenStatus         , Prefix + "GenStatus"               + Suffix );

  if(Suffix == "Pat") {
    iEvent.put( simpleEleId95relIso, Prefix + "SimpleEleId95relIso"+ Suffix );
    iEvent.put( simpleEleId90relIso, Prefix + "SimpleEleId90relIso"+ Suffix );
    iEvent.put( simpleEleId85relIso, Prefix + "SimpleEleId85relIso"+ Suffix );
    iEvent.put( simpleEleId80relIso, Prefix + "SimpleEleId80relIso"+ Suffix );
    iEvent.put( simpleEleId70relIso, Prefix + "SimpleEleId70relIso"+ Suffix );
    iEvent.put( simpleEleId60relIso, Prefix + "SimpleEleId60relIso"+ Suffix );
    iEvent.put( simpleEleId95cIso  , Prefix + "SimpleEleId95cIso"  + Suffix );
    iEvent.put( simpleEleId90cIso  , Prefix + "SimpleEleId90cIso"  + Suffix );
    iEvent.put( simpleEleId85cIso  , Prefix + "SimpleEleId85cIso"  + Suffix );
    iEvent.put( simpleEleId80cIso  , Prefix + "SimpleEleId80cIso"  + Suffix );
    iEvent.put( simpleEleId70cIso  , Prefix + "SimpleEleId70cIso"  + Suffix );
    iEvent.put( simpleEleId60cIso  , Prefix + "SimpleEleId60cIso"  + Suffix );
  }

  iEvent.put( isGsfCtfChargeConsistent     , Prefix + "IsGsfCtfChargeConsistent"      + Suffix );
  iEvent.put( isGsfCtfScPixChargeConsistent, Prefix + "IsGsfCtfScPixChargeConsistent" + Suffix );
  iEvent.put( isGsfScPixChargeConsistent   , Prefix + "IsGsfScPixChargeConsistent"    + Suffix ); 

  iEvent.put( EcalDrivenSeed   , Prefix + "EcalDrivenSeed"     + Suffix );
  iEvent.put( TrackerDrivenSeed, Prefix + "TrackerDrivenSeed"  + Suffix );

  iEvent.put( IsEB , Prefix + "IsEB"  + Suffix );
  iEvent.put( IsEE , Prefix + "IsEE"  + Suffix );
  iEvent.put( IsGap, Prefix + "IsGap" + Suffix );

  iEvent.put( idTrigTight, Prefix + "IdTriggerTight" + Suffix);

  iEvent.put( matchedConv, Prefix + "HasMatchedConversions" + Suffix);

}

void SusyDESY_Electrons::beginJob(){}


/////////////////////
////MUONS////////////
/////////////////////

SusyDESY_Muons::SusyDESY_Muons(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PatMuons( iConfig.getParameter<edm::InputTag> ("PatMuons") )
{
  produces <bool>                ( Prefix + "PatMuonsHandleValid"                 + Suffix );

  produces <std::vector<float> > ( Prefix + "HcalIsoDep"                          + Suffix );
  produces <std::vector<float> > ( Prefix + "EcalIsoDep"                          + Suffix );
  produces <std::vector<float> > ( Prefix + "TrackD0"                             + Suffix );
  produces <std::vector<float> > ( Prefix + "InnerTrackPtError"                   + Suffix );
  produces <std::vector<float> > ( Prefix + "IsolationR03hoVetoEt"                + Suffix );

  produces <std::vector<float> > (  Prefix + "PfIsolationR03ChgHadPt" + Suffix);
  produces <std::vector<float> > (  Prefix + "PfIsolationR03ChgParPt" + Suffix);
  produces <std::vector<float> > (  Prefix + "PfIsolationR03NeuHadEt" + Suffix);
  produces <std::vector<float> > (  Prefix + "PfIsolationR03GamEt" + Suffix);
  produces <std::vector<float> > (  Prefix + "PfIsolationR03NeuHadHiThrEt" + Suffix);
  produces <std::vector<float> > (  Prefix + "PfIsolationR03GamHiThrEt" + Suffix);
  produces <std::vector<float> > (  Prefix + "PfIsolationR03PUPt" + Suffix);
  produces <std::vector<float> > (  Prefix + "PfIsolationR03DeltaBCorrected" + Suffix);
}

void SusyDESY_Muons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<bool>                handleValid      ( new bool(false)          );

  std::auto_ptr<std::vector<float> > muonEcalIsoDep   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > muonHcalIsoDep   ( new std::vector<float>() );
  std::auto_ptr<std::vector<int> >   globalTrackNHVTH ( new std::vector<int>()   );
  std::auto_ptr<std::vector<float> > muonTrackD0      ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > innerTrackPtErr  ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > iso03hoVetoEt  ( new std::vector<float>() );

  std::auto_ptr<std::vector<float> >  pfIsolationR03ChgHadPt	( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >  pfIsolationR03ChgParPt	( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >  pfIsolationR03NeuHadEt	( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >  pfIsolationR03GamEt	( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >  pfIsolationR03NeuHadHiThrEt	( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >  pfIsolationR03GamHiThrEt	( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >  pfIsolationR03PUPt	( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >  pfIsolationR03DeltaB	( new std::vector<float>() ) ;

  edm::Handle<std::vector<pat::Muon> > MuColl;
  iEvent.getByLabel(PatMuons, MuColl);

  if(MuColl.isValid()){
    *handleValid.get() = true;

    for(std::vector<pat::Muon>::const_iterator mu = MuColl->begin(); mu!=MuColl->end(); mu++) {

      if(mu->ecalIsoDeposit() && mu->hcalIsoDeposit()){//this is needed to avoid the code seg faulting in 341 - NM 05/01/10
	muonEcalIsoDep->push_back(mu->ecalIsoDeposit()->candEnergy());
	muonHcalIsoDep->push_back(mu->hcalIsoDeposit()->candEnergy());
      }
      else {
	muonEcalIsoDep->push_back(-1);
	muonHcalIsoDep->push_back(-1);
      }

      bool tracker= mu->innerTrack().isNonnull();
      muonTrackD0->push_back( tracker ? mu->track()->d0() : 999999999. );
      innerTrackPtErr->push_back( tracker ? mu->innerTrack()->ptError() : -1);
      iso03hoVetoEt ->push_back(mu->isolationR03().hoVetoEt );

      {
	const reco::MuonPFIsolation& pfIso03 = mu->pfIsolationR03();
	float sumChgHadPt = pfIso03.sumChargedHadronPt;
	float sumNeuHadEt = pfIso03.sumNeutralHadronEt;
	float sumGamEt    = pfIso03.sumPhotonEt;
	float sumPUPt     = pfIso03.sumPUPt;
	float isoDelB     = (sumChgHadPt + std::max(0., (sumNeuHadEt + sumGamEt - 0.5*sumPUPt)))/(mu->p4()).Pt();

	pfIsolationR03ChgHadPt->push_back(sumChgHadPt);
	pfIsolationR03ChgParPt->push_back(pfIso03.sumChargedParticlePt);

	pfIsolationR03GamEt->push_back(sumGamEt);
	pfIsolationR03GamHiThrEt->push_back(pfIso03.sumPhotonEtHighThreshold);

	pfIsolationR03NeuHadEt->push_back(sumNeuHadEt);
	pfIsolationR03NeuHadHiThrEt->push_back(pfIso03.sumNeutralHadronEtHighThreshold);
	pfIsolationR03PUPt->push_back(sumPUPt);
	pfIsolationR03DeltaB->push_back(isoDelB);
      }


    }
  }

  iEvent.put( handleValid, Prefix + "PatMuonsHandleValid" + Suffix );

  iEvent.put( muonEcalIsoDep  , Prefix + "EcalIsoDep"           + Suffix );
  iEvent.put( muonHcalIsoDep  , Prefix + "HcalIsoDep"           + Suffix );
  iEvent.put( muonTrackD0     , Prefix + "TrackD0"              + Suffix );
  iEvent.put( innerTrackPtErr , Prefix + "InnerTrackPtError"    + Suffix );
  iEvent.put( iso03hoVetoEt   , Prefix + "IsolationR03hoVetoEt" + Suffix );

  iEvent.put( pfIsolationR03ChgHadPt, Prefix + "PfIsolationR03ChgHadPt" + Suffix);
  iEvent.put( pfIsolationR03ChgParPt, Prefix + "PfIsolationR03ChgParPt" + Suffix);
  iEvent.put( pfIsolationR03NeuHadEt, Prefix + "PfIsolationR03NeuHadEt" + Suffix);
  iEvent.put( pfIsolationR03GamEt, Prefix + "PfIsolationR03GamEt" + Suffix);
  iEvent.put( pfIsolationR03NeuHadHiThrEt, Prefix + "PfIsolationR03NeuHadHiThrEt" + Suffix);
  iEvent.put( pfIsolationR03GamHiThrEt, Prefix + "PfIsolationR03GamHiThrEt" + Suffix);
  iEvent.put( pfIsolationR03PUPt, Prefix + "PfIsolationR03PUPt" + Suffix);
  iEvent.put( pfIsolationR03DeltaB, Prefix + "PfIsolationR03DeltaBCorrected" + Suffix);
}

void SusyDESY_Muons::beginJob(){}


/////////////////////////////////////
////TRIGGER//////////////////////////
/////////////////////////////////////

SusyDESY_Trigger::SusyDESY_Trigger(const edm::ParameterSet& iConfig)
  : Prefix    (iConfig.getParameter<string> ("Prefix") ),
    Suffix    (iConfig.getParameter<string> ("Suffix") ),
    inputTag  (iConfig.getParameter<edm::InputTag>("inputTag")),
    muons_    (iConfig.getParameter<edm::InputTag>("muons")),
    electrons_(iConfig.getParameter<edm::InputTag>("electrons"))
{
  produces <bool>                               ( Prefix + "HandleValid"  + Suffix );
  produces <std::map<std::string,std::string> > ( Prefix + "NameMap"      + Suffix );

  produces <std::vector<std::string> > (Prefix + "MuMatchedTriggerFilter"      + Suffix);
  produces <std::vector<std::string> > (Prefix + "ElMatchedTriggerFilter"      + Suffix);
}


void SusyDESY_Trigger::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  //---------------------------------- get the trigger path info
  std::auto_ptr<bool>                               handleValid ( new bool(false)                        );
  std::auto_ptr<std::map<std::string,std::string> > nameMap     ( new std::map<std::string,std::string>  );

  edm::Handle<std::map<std::string,bool> >  triggered;
  iEvent.getByLabel( inputTag, triggered );

  if(triggered.isValid()) {
    *handleValid.get() = true;

    for(std::map<std::string,bool>::const_iterator trig=triggered->begin(); trig!=triggered->end(); ++trig) {
      std::string tn = trig->first;
      size_t usc = tn.find_last_of("_");
      if( tn.substr(usc+1)[0]=='v' && atoi(tn.substr(usc+2).c_str()) > 0 )
	(*nameMap)[tn.substr(0,usc)] = tn;
      else 
	(*nameMap)[tn              ] = tn;
    }
  }
  iEvent.put( handleValid , Prefix + "HandleValid" + Suffix );
  iEvent.put( nameMap     , Prefix + "NameMap"     + Suffix );
  
  //---------------------------------- get the trigger filter info which can be matched to muons and electrons
  //this is needed for trigger efficiency studies of single trigger legs
  //cout<<"iEvent = " << iEvent.id().event() << endl;
  //edm::Handle<std::vector<pat::Muon> > muons;
  //edm::Handle<std::vector<pat::Electron> > electrons;
  //iEvent.getByLabel(electrons_, electrons);
  //iEvent.getByLabel(muons_, muons);
  //static int counter=-1;
  //cout<<"======================================================================= event: " << setw(6) << ++counter << "   (" << setw(2)<< muons->size() << "|"<< setw(2) << electrons->size() << ")" << endl; 
  //printMatchingInfo(iEvent);
  
  edm::Handle<std::vector<pat::Muon> > muons;
  edm::Handle<std::vector<pat::Electron> > electrons;
  iEvent.getByLabel(muons_, muons);
  iEvent.getByLabel(electrons_, electrons);
  std::auto_ptr<std::vector<std::string> > MuMatchedTriggerFilter (new std::vector<std::string>());
  std::auto_ptr<std::vector<std::string> > ElMatchedTriggerFilter (new std::vector<std::string>());
  //static int counter=-1;
  //cout<<"======================================================================= event: " << setw(6) << ++counter << "   (" << setw(2)<< muons->size() << "|"<< setw(2) << electrons->size() << ")" << endl; 
  //---------------------------------- loop over muons
  for(int i=0,N=(int)muons->size(); i<N; ++i){
    string matchedTriggerFilters="";
    if(muons->at(i).triggerObjectMatches().size()==1){
      vector<string> filterLabels = muons->at(i).triggerObjectMatches().begin()->filterLabels();
      for(Int_t j=0,M=filterLabels.size(); j<M; ++j){
	matchedTriggerFilters+=filterLabels[j];
	if(j<M-1) matchedTriggerFilters+=";";
      }
    }
    //cout<<"------------------> matchedTriggerFilters["<<i<<"] = " << matchedTriggerFilters << endl;
    MuMatchedTriggerFilter->push_back(matchedTriggerFilters);
  }
  //---------------------------------- loop over electrons
  for(int i=0,N=(int)electrons->size(); i<N; ++i){
    string matchedTriggerFilters="";
    if(electrons->at(i).triggerObjectMatches().size()==1){
      vector<string> filterLabels = electrons->at(i).triggerObjectMatches().begin()->filterLabels();
      for(Int_t j=0,M=filterLabels.size(); j<M; ++j){
	matchedTriggerFilters+=filterLabels[j];
	if(j<M-1) matchedTriggerFilters+=";";
      }
    }
    //cout<<"------------------> matchedTriggerFilters["<<i<<"] = " << matchedTriggerFilters << endl;
    ElMatchedTriggerFilter->push_back(matchedTriggerFilters);
  }
  iEvent.put(MuMatchedTriggerFilter, Prefix + "MuMatchedTriggerFilter"      + Suffix);
  iEvent.put(ElMatchedTriggerFilter, Prefix + "ElMatchedTriggerFilter"      + Suffix);
}


void SusyDESY_Trigger::beginJob(){}


void SusyDESY_Trigger::printMatchingInfo(edm::Event& iEvent, ostream& os){
  edm::Handle<std::vector<pat::Muon> > muons;
  edm::Handle<std::vector<pat::Electron> > electrons;
  iEvent.getByLabel(electrons_, electrons);
  iEvent.getByLabel(muons_, muons);
    
  os<<endl;
  os<<"//////////////////////////////////////////////////////////////////////////////"<<endl;
  os<<"No. of Muons | electrons: " << muons->size() << " | " << electrons->size() << endl;
  os<<"//////////////////////////////////////////////////////////////////////////////"<<endl;
  os<<endl;
  //--------------------- Muons
  if(muons->size()) os<<"==================================================================== muons: " << muons->size() << endl;
  for(int i=0,N=(int)muons->size(); i<N; ++i){
    os<<"------------------------------ muon no: " << i << endl;
    os<<"muons->at("<<i<<").charge()                      = " << muons->at(i).charge() << endl;
    os<<"muons->at("<<i<<").p4()                          = " << muons->at(i).p4();
    if(muons->at(i).triggerObjectMatches().size() ==1) os << " <--> " <<muons->at(i).triggerObjectMatches().begin()->p4();
    os<<endl;
    os<<"muons->at("<<i<<").triggerObjectMatches().size() = " << muons->at(i).triggerObjectMatches().size() << endl;
    if(muons->at(i).triggerObjectMatches().size() ==1){
      os<<"muons->at("<<i<<").triggerObjectMatches().begin():" << endl;
      os<<setw(60)<<"collection() |"
      <<setw(20)<<"conditionNames |"
      <<setw(15)<<"filterLabels |"
      <<setw(15)<<"pathNames"
      <<endl;
      os<<setw(60)<< muons->at(i).triggerObjectMatches().begin()->collection()
      <<setw(20)<< muons->at(i).triggerObjectMatches().begin()->conditionNames().size()
      <<setw(15)<<muons->at(i).triggerObjectMatches().begin()->filterLabels().size()
      <<setw(15)<<muons->at(i).triggerObjectMatches().begin()->pathNames().size()
      <<endl
      <<endl;
      printVector<string>("conditionNames", muons->at(i).triggerObjectMatches().begin()->conditionNames());
      printVector<string>("filterLabels", muons->at(i).triggerObjectMatches().begin()->filterLabels());
      printVector<string>("pathNames", muons->at(i).triggerObjectMatches().begin()->pathNames());
      printVector<int>("triggerObjectTypes", muons->at(i).triggerObjectMatches().begin()->triggerObjectTypes());   
    }
    os<<endl;
  }
  //--------------------- Eletrons
  if(electrons->size()) os<<"==================================================================== electrons: " << electrons->size() << endl;
  for(int i=0,N=(int)electrons->size(); i<N; ++i){
    os<<"------------------------------ electron no: " << i << endl;
    os<<"electrons->at("<<i<<").charge()                      = " << electrons->at(i).charge() << endl;
    os<<"electrons->at("<<i<<").p4()                          = " << electrons->at(i).p4();
    if(electrons->at(i).triggerObjectMatches().size() ==1) os << " <--> " << electrons->at(i).triggerObjectMatches().begin()->p4();
    os<<endl;    
    os<<"electrons->at("<<i<<").triggerObjectMatches().size() = " << electrons->at(i).triggerObjectMatches().size() << endl;
    if(electrons->at(i).triggerObjectMatches().size() ==1){
      os<<"electrons->at("<<i<<").triggerObjectMatches().begin():" << endl;
      os<<setw(60)<<"collection() |"
      <<setw(20)<<"conditionNames |"
      <<setw(15)<<"filterLabels |"
      <<setw(15)<<"pathNames"
      <<endl;
      os<<setw(60)<< electrons->at(i).triggerObjectMatches().begin()->collection()
      <<setw(20)<< electrons->at(i).triggerObjectMatches().begin()->conditionNames().size()
      <<setw(15)<<electrons->at(i).triggerObjectMatches().begin()->filterLabels().size()
      <<setw(15)<<electrons->at(i).triggerObjectMatches().begin()->pathNames().size()
      <<endl
      <<endl;
      printVector<string>("conditionNames", electrons->at(i).triggerObjectMatches().begin()->conditionNames());
      printVector<string>("filterLabels", electrons->at(i).triggerObjectMatches().begin()->filterLabels());
      printVector<string>("pathNames", electrons->at(i).triggerObjectMatches().begin()->pathNames());
      printVector<int>("triggerObjectTypes", electrons->at(i).triggerObjectMatches().begin()->triggerObjectTypes());
    }
  }
}


template <typename T>
void SusyDESY_Trigger::printVector(TString name, vector<T> v, Int_t length, ostream& os){
  printVectorSize(name,v);
  if(v.size()>0){
    for(Int_t i=0,N=v.size(); i<N; ++i){
      TString temp_name = name;
      temp_name += "[";
      temp_name += i;
      temp_name += "] = ";
      os << setw(length) << temp_name << v[i] << endl;
    }
    os<<endl;
  }
}


template <typename T>
void SusyDESY_Trigger::printVectorSize(TString name, vector<T> v, Int_t length, ostream& os){
  name += ".size() = ";
  os << setw(length) << name << v.size();
  os << endl;
}




SusyDESY_PU::SusyDESY_PU(const edm::ParameterSet& iConfig)
  :PileUp  (iConfig.getParameter<edm::InputTag>("PileUp"))
{
}

void SusyDESY_PU::beginJob(){
  edm::Service<TFileService> fs;
  pu= fs->make<TH1F>("pu","pu dist",100,0,100);
}

void SusyDESY_PU::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<float> pileup ;
  iEvent.getByLabel( PileUp, pileup );
  pu->Fill(*pileup.product());

}



