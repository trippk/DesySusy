#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Muon.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"


SusyDESY_Muon::SusyDESY_Muon(const edm::ParameterSet& iConfig)
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

void SusyDESY_Muon::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

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

void SusyDESY_Muon::beginJob(){}
