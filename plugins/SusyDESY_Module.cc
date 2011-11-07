#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Module.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

SusyDESY_Electrons::SusyDESY_Electrons(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PatElectrons( iConfig.getParameter<edm::InputTag> ("PatElectrons") )
{
  produces <bool>                 ( Prefix + "PatElectronsHandleValid" + Suffix );

  produces <std::vector<float> > ( Prefix + "SuperClusterEnergy"          + Suffix ); 
  produces <std::vector<math::XYZPoint> > ( Prefix +"SuperClusterPosition"+ Suffix ); 
  produces <std::vector<float> > ( Prefix + "SuperClusterRawEnergy"       + Suffix ); 
  produces <std::vector<float> > ( Prefix + "SuperClusterEtaWidth"        + Suffix );
  produces <std::vector<float> > ( Prefix + "SuperClusterPhiWidth"        + Suffix );
  produces <std::vector<float> > ( Prefix + "SuperClusterPreshowerEnergy" + Suffix );

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
  produces <std::vector<bool> > ( Prefix + "ChargeInfoIsGsfCtfConsistent"      + Suffix );
  produces <std::vector<bool> > ( Prefix + "ChargeInfoIsGsfCtfScPixConsistent" + Suffix );
  produces <std::vector<bool> > ( Prefix + "ChargeInfoIsGsfScPixConsistent"    + Suffix );

  produces <std::vector<bool> > ( Prefix + "IsGsfCtfChargeConsistent"      + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsGsfCtfScPixChargeConsistent" + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsGsfScPixChargeConsistent"    + Suffix );

  produces <std::vector<int> > ( Prefix + "GsfTrackCharge"     + Suffix );
  produces <std::vector<int> > ( Prefix + "CtfTrackCharge"     + Suffix );
  produces <std::vector<int> > ( Prefix + "ScPixCharge"        + Suffix );

  produces <std::vector<bool> > ( Prefix + "EcalDrivenSeed"    + Suffix );
  produces <std::vector<bool> > ( Prefix + "TrackerDrivenSeed" + Suffix );

  produces <std::vector<bool> > ( Prefix + "IsEB"  + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsEE"  + Suffix );
  produces <std::vector<bool> > ( Prefix + "IsGap" + Suffix );


//   produces <std::vector<> > ( Prefix + ""   + Suffix );
}

void SusyDESY_Electrons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<bool>                 handleValid       ( new bool(false)           );

  std::auto_ptr<std::vector<float> > SCenergy           ( new std::vector<float>() );
  std::auto_ptr<std::vector<math::XYZPoint> > SCposition( new std::vector<math::XYZPoint>() );
  std::auto_ptr<std::vector<float> > SCrawEnergy        ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > SCetaWidth         ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > SCphiWidth         ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > SCpreshowerEnergy  ( new std::vector<float>() );

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

  std::auto_ptr<std::vector<bool> > chargeInfoIsGsfCtfConsistent      ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > chargeInfoIsGsfCtfScPixConsistent ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > chargeInfoIsGsfScPixConsistent    ( new std::vector<bool>() );

  std::auto_ptr<std::vector<bool> > isGsfCtfChargeConsistent      ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > isGsfCtfScPixChargeConsistent ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > isGsfScPixChargeConsistent    ( new std::vector<bool>() );

  std::auto_ptr<std::vector<int> > GsfTrackCharge         ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > CtfTrackCharge         ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > ScPixCharge            ( new std::vector<int>() );

  std::auto_ptr<std::vector<bool> > EcalDrivenSeed    ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > TrackerDrivenSeed ( new std::vector<bool>() );

  std::auto_ptr<std::vector<bool> > IsEB  ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > IsEE  ( new std::vector<bool>() );
  std::auto_ptr<std::vector<bool> > IsGap ( new std::vector<bool>() );

  edm::Handle<std::vector<pat::Electron> > ElColl;
  iEvent.getByLabel(PatElectrons, ElColl);

  if(ElColl.isValid()){
    *handleValid.get() = true;

    for(std::vector<pat::Electron>::const_iterator el = ElColl->begin(); el!=ElColl->end(); el++) {

      SCenergy         ->push_back(el->superCluster()->energy()         );
      SCposition       ->push_back(el->superCluster()->position()       );
      SCrawEnergy      ->push_back(el->superCluster()->rawEnergy()      );
      SCetaWidth       ->push_back(el->superCluster()->etaWidth()       );	
      SCphiWidth       ->push_back(el->superCluster()->phiWidth()       );
      SCpreshowerEnergy->push_back(el->superCluster()->preshowerEnergy());
      

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

      chargeInfoIsGsfCtfConsistent     ->push_back(el->chargeInfo().isGsfCtfConsistent     );
      chargeInfoIsGsfCtfScPixConsistent->push_back(el->chargeInfo().isGsfCtfScPixConsistent);
      chargeInfoIsGsfScPixConsistent   ->push_back(el->chargeInfo().isGsfScPixConsistent   );
      
      isGsfCtfChargeConsistent     ->push_back(el->isGsfCtfChargeConsistent()     );
      isGsfCtfScPixChargeConsistent->push_back(el->isGsfCtfScPixChargeConsistent());
      isGsfScPixChargeConsistent   ->push_back(el->isGsfScPixChargeConsistent()   );

      GsfTrackCharge    ->push_back(el->gsfTrack()->charge()          );
      if(el->closestCtfTrackRef().isNonnull())
	CtfTrackCharge    ->push_back(el->closestCtfTrackRef()->charge());
      else
	CtfTrackCharge    ->push_back(0);
      ScPixCharge       ->push_back(el->chargeInfo().scPixCharge      );

      //el->closestCtfTrack().ctfTrack;

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
      //cout<<"genMatched.back() = " << genMatched << endl;
      //cout<<"genPdgId.back()   = " << genPdgId.back() << endl;
      //cout<<"genStatus.back()  = " << genStatus.back() << endl;
      //cout<<endl;
    }
  }

  iEvent.put( handleValid       , Prefix + "PatElectronsHandleValid" + Suffix );

  iEvent.put( SCenergy          , Prefix + "SuperClusterEnergy"          + Suffix );
  iEvent.put( SCposition        , Prefix + "SuperClusterPosition"        + Suffix );
  iEvent.put( SCrawEnergy       , Prefix + "SuperClusterRawEnergy"       + Suffix );
  iEvent.put( SCetaWidth        , Prefix + "SuperClusterEtaWidth"        + Suffix );
  iEvent.put( SCphiWidth        , Prefix + "SuperClusterPhiWidth"        + Suffix );
  iEvent.put( SCpreshowerEnergy , Prefix + "SuperClusterPreshowerEnergy" + Suffix );

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

  iEvent.put( chargeInfoIsGsfCtfConsistent     , Prefix + "ChargeInfoIsGsfCtfConsistent"      + Suffix );
  iEvent.put( chargeInfoIsGsfCtfScPixConsistent, Prefix + "ChargeInfoIsGsfCtfScPixConsistent" + Suffix );
  iEvent.put( chargeInfoIsGsfScPixConsistent   , Prefix + "ChargeInfoIsGsfScPixConsistent"    + Suffix );

  iEvent.put( isGsfCtfChargeConsistent     , Prefix + "IsGsfCtfChargeConsistent"      + Suffix );
  iEvent.put( isGsfCtfScPixChargeConsistent, Prefix + "IsGsfCtfScPixChargeConsistent" + Suffix );
  iEvent.put( isGsfScPixChargeConsistent   , Prefix + "IsGsfScPixChargeConsistent"    + Suffix ); 

  iEvent.put( GsfTrackCharge, Prefix + "GsfTrackCharge"     + Suffix );
  iEvent.put( CtfTrackCharge, Prefix + "CtfTrackCharge"     + Suffix );
  iEvent.put( ScPixCharge   , Prefix + "ScPixCharge"        + Suffix );

  iEvent.put( EcalDrivenSeed   , Prefix + "EcalDrivenSeed"     + Suffix );
  iEvent.put( TrackerDrivenSeed, Prefix + "TrackerDrivenSeed"  + Suffix );

  iEvent.put( IsEB , Prefix + "IsEB"  + Suffix );
  iEvent.put( IsEE , Prefix + "IsEE"  + Suffix );
  iEvent.put( IsGap, Prefix + "IsGap" + Suffix );
}

void SusyDESY_Electrons::beginJob(){}




SusyDESY_Muons::SusyDESY_Muons(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PatMuons( iConfig.getParameter<edm::InputTag> ("PatMuons") )
{
  produces <bool>                ( Prefix + "PatMuonsHandleValid"                 + Suffix );

  produces <std::vector<float> > ( Prefix + "HcalIsoDep"                          + Suffix );
  produces <std::vector<float> > ( Prefix + "EcalIsoDep"                          + Suffix );

  produces <std::vector<int> >   ( Prefix + "GlobalTrackNumberOfValidTrackerHits" + Suffix );
  produces <std::vector<int> >   ( Prefix + "GlobalTrackNumberOfValidMuonHits"    + Suffix ); 
  produces <std::vector<float> > ( Prefix + "TrackD0"                             + Suffix );
  produces <std::vector<int> >   ( Prefix + "InnerTrackPixelLayersWithMeasurement"+ Suffix );

  produces <std::vector<float> > ( Prefix + "IsolationR03emVetoEt"  + Suffix );
  produces <std::vector<float> > ( Prefix + "IsolationR03hadVetoEt" + Suffix );
  produces <std::vector<float> > ( Prefix + "IsolationR03hoVetoEt"  + Suffix );
}

void SusyDESY_Muons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<bool>                handleValid      ( new bool(false)          );

  std::auto_ptr<std::vector<float> > muonEcalIsoDep   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > muonHcalIsoDep   ( new std::vector<float>() );

  std::auto_ptr<std::vector<int> >   globalTrackNHVTH ( new std::vector<int>()   );
  std::auto_ptr<std::vector<int> >   globalTrackNHVMH ( new std::vector<int>()   );
  std::auto_ptr<std::vector<float> > muonTrackD0      ( new std::vector<float>() );
  std::auto_ptr<std::vector<int> >   innerTrackPLWM   ( new std::vector<int>()   );

  std::auto_ptr<std::vector<float> > iso03emVetoEt  ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > iso03hadVetoEt ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > iso03hoVetoEt  ( new std::vector<float>() );

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
      bool global = mu->globalTrack().isNonnull();
      bool tracker= mu->innerTrack().isNonnull();
      globalTrackNHVTH->push_back( global ? mu->globalTrack()->hitPattern().numberOfValidTrackerHits() : 0);
      globalTrackNHVMH->push_back( global ? mu->globalTrack()->hitPattern().numberOfValidMuonHits ()   : 0);
      muonTrackD0->push_back( tracker ? mu->track()->d0() : 999999999. );
      innerTrackPLWM->push_back( tracker ? mu->innerTrack()->hitPattern().pixelLayersWithMeasurement() : 0);

      iso03emVetoEt ->push_back(mu->isolationR03().emVetoEt );
      iso03hadVetoEt->push_back(mu->isolationR03().hadVetoEt);
      iso03hoVetoEt ->push_back(mu->isolationR03().hoVetoEt );
    }
  }

  iEvent.put( handleValid, Prefix + "PatMuonsHandleValid" + Suffix );

  iEvent.put( muonEcalIsoDep  , Prefix + "EcalIsoDep"                               + Suffix );
  iEvent.put( muonHcalIsoDep  , Prefix + "HcalIsoDep"                               + Suffix );

  iEvent.put( muonTrackD0     , Prefix + "TrackD0"                                  + Suffix );
  iEvent.put( globalTrackNHVTH, Prefix + "GlobalTrackNumberOfValidTrackerHits"      + Suffix );
  iEvent.put( globalTrackNHVMH, Prefix + "GlobalTrackNumberOfValidMuonHits"         + Suffix );
  iEvent.put( innerTrackPLWM,   Prefix + "InnerTrackPixelLayersWithMeasurement"     + Suffix );

  iEvent.put( iso03emVetoEt  , Prefix + "IsolationR03emVetoEt"  + Suffix );
  iEvent.put( iso03hadVetoEt , Prefix + "IsolationR03hadVetoEt" + Suffix );
  iEvent.put( iso03hoVetoEt  , Prefix + "IsolationR03hoVetoEt"  + Suffix );
}

void SusyDESY_Muons::beginJob(){}


/////////////////////////////////////
////PILE UP INFO/////////////////////
/////////////////////////////////////

SusyDESY_PileUpInfo::SusyDESY_PileUpInfo(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PUinfo( iConfig.getParameter<edm::InputTag> ("PUinfo") )
{
  produces <bool>                ( Prefix + "PUinfoHandleValid"  + Suffix );
  produces <std::vector<int> >   ( Prefix + "BunchCrossing"       + Suffix );
  produces <std::vector<int> >   ( Prefix + "NumInteractions"    + Suffix );
  //produces <>                ( Prefix + ""                 + Suffix );
}

void SusyDESY_PileUpInfo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  std::auto_ptr<bool>                handleValid     ( new bool(false)       );
  std::auto_ptr<std::vector<int> >   bunchCrossing   ( new std::vector<int>  );
  std::auto_ptr<std::vector<int> >   NumInteractions ( new std::vector<int>  );
//   std::auto_ptr<std::vector<> >    ( new std::vector<>  );


  edm::Handle<std::vector< PileupSummaryInfo > >  PileUpInfo;
  iEvent.getByLabel( PUinfo, PileUpInfo);

  if(PileUpInfo.isValid()) {
    *handleValid.get() = true;

    for(std::vector<PileupSummaryInfo>::const_iterator PVI=PileUpInfo->begin(); PVI!=PileUpInfo->end(); ++PVI) {
      bunchCrossing  ->push_back( PVI->getBunchCrossing()      );
      NumInteractions->push_back( PVI->getPU_NumInteractions() );
    }
  }
  iEvent.put( handleValid    , Prefix + "PUinfoHandleValid" + Suffix );
  iEvent.put( bunchCrossing  , Prefix + "BunchCrossing"      + Suffix );
  iEvent.put( NumInteractions, Prefix + "NumInteractions"   + Suffix );
}

void SusyDESY_PileUpInfo::beginJob(){}


/////////////////////////////////////
////TRIGGER//////////////////////////
/////////////////////////////////////

SusyDESY_Trigger::SusyDESY_Trigger(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    inputTag  (iConfig.getParameter<edm::InputTag>("inputTag"))
  , sourceName(iConfig.getParameter<std::string>  ("SourceName"))
  , sourceType(NOT_APPLICABLE)
  , tag_( iConfig.getParameter<edm::InputTag>("TriggerEventInputTag")) 
				  //  , run_(-1)

{

  produces <bool>                               ( Prefix + "HandleValid"  + Suffix );
  produces <std::map<std::string,std::string> > ( Prefix + "NameMap"      + Suffix );

}


void SusyDESY_Trigger::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  std::auto_ptr<bool>                               handleValid ( new bool(false)                        );
  std::auto_ptr<std::map<std::string,std::string> > nameMap     ( new std::map<std::string,std::string>  );

  if ( inputTag.process().empty() ) { 
    edm::Handle<trigger::TriggerEvent> temp;
    iEvent.getByLabel( tag_, temp );
    if( temp.isValid() ) { inputTag = edm::InputTag( inputTag.label(), inputTag.instance(), temp.provenance()->processName() ); }
    else std::cout<<"  not valid  "<<std::endl;
  }

  bool  hltChanged = false;
  if (!hltConfig.init(iEvent.getRun(), iSetup, inputTag.process(), hltChanged) ) {
    std::cout << "HLT config initialization error with process name \"" << inputTag.process() << "\"."<<std::endl;
  } else if ( hltConfig.size() < 1 ) {
    std::cout << "HLT config has zero size."<<std::endl;
  }
  getDataSource();


  edm::Handle<edm::TriggerResults >  Triggers;
  iEvent.getByLabel( inputTag, Triggers );

  if(Triggers.isValid()) {
    *handleValid.get() = true;
    const edm::TriggerNames& names = iEvent.triggerNames(*Triggers);

    for(unsigned i=0; i < Triggers->size(); i++) {
      if (dataSource.empty() || std::find(dataSource.begin(), dataSource.end(), names.triggerName(i)) != dataSource.end()) {

	std::string tn = names.triggerName(i);
	size_t usc = tn.find_last_of("_");
	if( tn.substr(usc+1)[0]=='v' && atoi(tn.substr(usc+2).c_str()) > 0 )
	  (*nameMap)[tn.substr(0,usc)] = tn;
	else 
	  (*nameMap)[tn              ] = tn;
      }
    }
  }
  iEvent.put( handleValid , Prefix + "HandleValid" + Suffix );
  iEvent.put( nameMap     , Prefix + "NameMap"     + Suffix );
}


void SusyDESY_Trigger::beginJob(){}


/////////////////////////////////////
////PDF WEIGHTS//////////////////////
/////////////////////////////////////

SusyDESY_PDFweights::SusyDESY_PDFweights(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PDFset( iConfig.getParameter<edm::InputTag> ("PDFset") )
{
    produces <bool>                 ( Prefix + "PDFsetHandleValid"  + Suffix );
    produces <std::vector<double> > ( Prefix + "weights"            + Suffix );
  //produces <>                ( Prefix + ""                 + Suffix );
}

void SusyDESY_PDFweights::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  std::auto_ptr<bool>                 handleValid ( new bool(false)         );
  std::auto_ptr<std::vector<double> > _weights ( new std::vector<double>() );
//   std::auto_ptr<std::vector<> >    ( new std::vector<>  );


  edm::Handle<std::vector< double > >  PDFweights;
  iEvent.getByLabel( PDFset, PDFweights);

  if(PDFweights.isValid()) {
    *handleValid.get() = true;
    
    std::vector<double> weights = (*PDFweights);
    std::cout << "Event weight for central PDF:" << weights[0] << std::endl;
    _weights->push_back(weights[0]);
    unsigned int nmembers = weights.size();
    for (unsigned int j=1; j<nmembers; j+=2) {
      std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
      std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
      _weights->push_back(weights[j]);
      _weights->push_back(weights[j+1]);
    }

  }
  iEvent.put( handleValid , Prefix + "PDFsetHandleValid" + Suffix );
  iEvent.put( _weights    , Prefix + "weights"           + Suffix );
}

void SusyDESY_PDFweights::beginJob(){//std::cout<<"______XXXXXXXXXXXXXXXX_____"<<std::endl;
}
