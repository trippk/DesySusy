// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Track.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > LorentzM;
typedef math::XYZPoint Point;
using namespace reco;
using namespace edm;
using namespace std;


SusyDESY_Track::SusyDESY_Track(const edm::ParameterSet& iConfig):
  Prefix( iConfig.getParameter<string> ("Prefix") ),
  Suffix( iConfig.getParameter<string> ("Suffix") ),
  PfCandidatesTag( iConfig.getParameter<InputTag> ("PfCandidatesTag") ),
  VertexTag( iConfig.getParameter<InputTag>        ("VertexInputTag") ),
  dRcut ( iConfig.getParameter<double> ("dR_ConeSize") ),
  dzcut ( iConfig.getParameter<double> ("dz_CutValue") )
{
  produces<vector<LorentzM> > ( Prefix + "P4"     + Suffix );
  produces<vector<int> >      ( Prefix + "Chg"    + Suffix );
  produces<vector<int> >      ( Prefix + "PdgId"  + Suffix );
  produces<vector<float> >    ( Prefix + "Dz"     + Suffix );
  produces<vector<float> >    ( Prefix + "TrkIso" + Suffix );

}

SusyDESY_Track::~SusyDESY_Track() 
{

}

void  SusyDESY_Track::beginRun(edm::Run&, const edm::EventSetup& es) {}
void  SusyDESY_Track::beginJob() {}
void  SusyDESY_Track::endJob()   {}

// ------------ method called to produce the data  ------------

void SusyDESY_Track::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  auto_ptr<vector<LorentzM> > p4     ( new vector<LorentzM> );
  auto_ptr<vector<int>   >    chg    ( new vector<int>  );
  auto_ptr<vector<int>   >    pdgId  ( new vector<int>  );
  auto_ptr<vector<float> >    dzpv   ( new vector<float>);
  auto_ptr<vector<float> >    trkiso ( new vector<float>);

  //---------------------------------
  // get PFCandidate collection
  //---------------------------------
  
  Handle<PFCandidateCollection> pfCandidatesHandle;
  iEvent.getByLabel(PfCandidatesTag, pfCandidatesHandle);
  pfCandidates  = pfCandidatesHandle.product();

  //---------------------------------
  // get Vertex Collection
  //---------------------------------
  
  Handle<reco::VertexCollection> vertex_h;
  iEvent.getByLabel(VertexTag, vertex_h);
  const reco::VertexCollection *vertices = vertex_h.product();

  //-----------------------------------
  // Find 1st good vertex
  //-----------------------------------


  VertexCollection::const_iterator firstGoodVertex = vertices->end();

  int firstGoodVertexIdx = 0;
  for (VertexCollection::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx, ++firstGoodVertexIdx) {
    if (  !vtx->isFake() && vtx->ndof()>=4. && vtx->position().Rho()<=2.0 && fabs(vtx->position().Z())<=24.0) {
      firstGoodVertex = vtx;
      break;
    }
  }

  if ( !(firstGoodVertex==vertices->end()) ) {
    for( PFCandidateCollection::const_iterator pf_it = pfCandidates->begin(); pf_it != pfCandidates->end(); pf_it++ ) {
      
      LorentzM pf_p4( pf_it->p4().pt(), pf_it->p4().eta(), pf_it->p4().phi(), pf_it->p4().M());
      
      //do not store neutral particles
      if (pf_it->charge() == 0) continue;

      //----------------------------------------------------------------------------
      // now loop over other PFCandidates in the event to calculate trackIsolation
      //----------------------------------------------------------------------------
      
      float iso = 0.0;
      
      for( PFCandidateCollection::const_iterator pf_other = pfCandidates->begin(); pf_other != pfCandidates->end(); pf_other++ ) {
	
	// don't count the PFCandidate in its own isolation sum	
	if( pf_it == pf_other ) continue;
	
	// require the PFCandidate to be charged
	if( pf_other->charge() == 0 ) continue;

	// if the PFCandidate is an electron or a muon, do not include other electrons and muons in the isolation calculation
	if ( abs(pf_it->pdgId())==11 || abs(pf_it->pdgId())==13 ) {
	  if ( abs(pf_other->pdgId())==11 || abs(pf_other->pdgId())==13 ) continue;
	}
	
	// cut on dR between the PFCandidates
	float dR = deltaR(pf_it->eta(), pf_it->phi(), pf_other->eta(), pf_other->phi());
	if( dR > dRcut ) continue;
	
	// cut on the PFCandidate dz, use gsfTrack for electrons
	float dz_other = 100;

	if ( pf_other->trackRef().isNonnull())
	  dz_other = pf_other->trackRef()->dz( firstGoodVertex->position() );
	if ( abs(pf_other->pdgId())==11 && pf_other->gsfElectronRef().isNonnull() && pf_other->gsfTrackRef().isNonnull())
	  dz_other = pf_other->gsfTrackRef()->dz( firstGoodVertex->position() );
	if( fabs(dz_other) > dzcut ) continue;
	
	iso += pf_other->pt();
      }
  
      // calculate the dz of this candidate
      float dz_it = 100; 
      
      if ( pf_it->trackRef().isNonnull())
	dz_it = pf_it->trackRef()->dz( firstGoodVertex->position() );
      if (abs(pf_it->pdgId())==11 && pf_it->gsfElectronRef().isNonnull() && pf_it->gsfTrackRef().isNonnull())
	dz_it = pf_it->gsfTrackRef()->dz( firstGoodVertex->position() );
      


      //-------------------------------------------------------------------------------------
      // store PFCandidate info
      //-------------------------------------------------------------------------------------
     
      p4    ->push_back( pf_p4);
      chg   ->push_back( pf_it->charge());
      pdgId ->push_back( pf_it->pdgId());
      dzpv  ->push_back( dz_it);
      trkiso->push_back( iso);
    }
  }
    
  // put trkiso and dz values back into event
  iEvent.put(p4    , Prefix + "P4"     + Suffix );
  iEvent.put(chg   , Prefix + "Chg"    + Suffix );
  iEvent.put(pdgId , Prefix + "PdgId"  + Suffix );
  iEvent.put(dzpv  , Prefix + "Dz"     + Suffix );
  iEvent.put(trkiso, Prefix + "TrkIso" + Suffix );

}
  

