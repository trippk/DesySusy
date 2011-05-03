#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Module.h"
//#include "FWCore/Framework/interface/Event.h"
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
      innerTrackPLWM->push_back( tracker ? mu->innerTrack()->->hitPattern().pixelLayersWithMeasurement() : 0);

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

