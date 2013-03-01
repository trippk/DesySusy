#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Electron.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

SusyDESY_Electron::SusyDESY_Electron(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PatElectrons( iConfig.getParameter<edm::InputTag> ("PatElectrons") ),
    conversionsInputTag_( iConfig.getParameter<edm::InputTag> ("conversionsInputTag") ),
    beamSpotInputTag_( iConfig.getParameter<edm::InputTag> ("beamSpotInputTag") ),
    rhoIsoInputTag_(iConfig.getParameter<edm::InputTag>("rhoIsoInputTag")),
    isoValInputTags_(iConfig.getParameter<std::vector<edm::InputTag> >("isoValInputTags"))
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

  produces <std::vector<float> > (Prefix + "ChargedHadronIso"      + Suffix);
  produces <std::vector<float> > (Prefix + "PhotonIso"             + Suffix);
  produces <std::vector<float> > (Prefix + "NeutralHadronIso"      + Suffix);
  produces <std::vector<float> > (Prefix + "Rho"                   + Suffix);
  produces <std::vector<float> > (Prefix + "EffArea"               + Suffix);
  produces <std::vector<float> > (Prefix + "CorrectedAbsIsolation" + Suffix);
  produces <std::vector<float> > (Prefix + "CorrectedRelIsolation" + Suffix);

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

void SusyDESY_Electron::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
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

  std::auto_ptr<std::vector<float> > chargedHadronIso      ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > photonIso             ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > neutralHadronIso      ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > rho                   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > effArea               ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > correctedAbsIsolation ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > correctedRelIsolation ( new std::vector<float>() );

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

  /*edm::Handle<PFCandidateCollection> pfCandidatesHandle;
  iEvent.getByLabel(pfCandidatesTag_, pfCandidatesHandle);
  pfCandidates  = pfCandidatesHandle.product();*/

  // rho for isolation
  edm::Handle<double> rhoIso_h;
  iEvent.getByLabel(rhoIsoInputTag_, rhoIso_h);
  double rhoIso = *(rhoIso_h.product());

  // iso deposits
  std::vector< edm::Handle< edm::ValueMap<double> > > isoVals(isoValInputTags_.size());
  for (size_t j = 0; j < isoValInputTags_.size(); ++j) {
    iEvent.getByLabel(isoValInputTags_[j], isoVals[j]);
  }

  /*IsoDepositVals electronIsoValPFId(3);
  iEvent.getByLabel("elPFIsoValueCharged03PFIdPFIso", electronIsoValPFId[0]);
  iEvent.getByLabel("elPFIsoValueGamma03PFIdPFIso", electronIsoValPFId[1]);
  iEvent.getByLabel("elPFIsoValueNeutral03PFIdPFIso", electronIsoValPFId[2]);*/

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

      edm::Ptr< reco::GsfElectron > gsfel = (edm::Ptr< reco::GsfElectron >) el->originalObjectRef();
      float iso_ch = (*(isoVals[0]))[gsfel];
      float iso_ph = (*(isoVals[1]))[gsfel];
      float iso_nh = (*(isoVals[2]))[gsfel];
      
      float effA = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03,  gsfel->superCluster()->eta(), ElectronEffectiveArea::kEleEAData2011);

      float rhoPrime = std::max(rhoIso, 0.0);
      float absoluteIsolation = iso_ch + std::max(iso_nh + iso_ph - rhoPrime * effA, (float)0.0);
      float relativeIsolation = absoluteIsolation / el->pt();

      chargedHadronIso     ->push_back(iso_ch);
      photonIso            ->push_back(iso_ph);
      neutralHadronIso     ->push_back(iso_nh);
      rho                  ->push_back(rhoIso);
      effArea              ->push_back(effA);
      correctedAbsIsolation->push_back(absoluteIsolation);
      correctedRelIsolation->push_back(relativeIsolation);

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

  iEvent.put( chargedHadronIso     , Prefix + "ChargedHadronIso"      + Suffix );
  iEvent.put( photonIso            , Prefix + "PhotonIso"             + Suffix );
  iEvent.put( neutralHadronIso     , Prefix + "NeutralHadronIso"      + Suffix );
  iEvent.put( rho                  , Prefix + "Rho"                   + Suffix );
  iEvent.put( effArea              , Prefix + "EffArea"               + Suffix );
  iEvent.put( correctedAbsIsolation, Prefix + "CorrectedAbsIsolation" + Suffix );
  iEvent.put( correctedRelIsolation, Prefix + "CorrectedRelIsolation" + Suffix );

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

void SusyDESY_Electron::beginJob(){}
