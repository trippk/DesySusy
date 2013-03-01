//-----------------------------------------------------------------------------------------
//
// Computation of the trackIsolation, for use with the isolated track veto 
// used for the stop quark search in the single lepton channel
// Author: Ben Hooberman
//
// For each PFCandidate above threshold minPt_PFCandidate store 4 quantities:
// pT of PFCandidate
// charge of PFCandidate
// dz of PFCandidate w.r.t. the 1st good vertex
// the trackIsolation value
//
// In the analysis, we veto any event containing IN ADDITION TO the selected lepton a charged PFCandidate with:
// pT > 10 GeV, dz < 0.05 cm, and trackIso/pT < 0.1
//
//-----------------------------------------------------------------------------------------

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

#include "SUSYBSMAnalysis/DesySusy/interface/TrackIsolationMaker.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"

//typedef math::XYZTLorentzVectorF LorentzVector;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > LorentzM;
typedef math::XYZPoint Point;
using namespace reco;
using namespace edm;
using namespace std;


TrackIsolationMaker::TrackIsolationMaker(const edm::ParameterSet& iConfig):
  Prefix( iConfig.getParameter<string> ("Prefix") ),
  Suffix( iConfig.getParameter<string> ("Suffix") ),
  PfCandidatesTag( iConfig.getParameter<InputTag> ("PfCandidatesTag") ),
  VertexTag( iConfig.getParameter<InputTag>        ("VertexInputTag") ),
  dRcut ( iConfig.getParameter<double> ("dR_ConeSize") ),              // dR value used to define the isolation cone                (default  0.3 )
  dzcut ( iConfig.getParameter<double> ("dz_CutValue") ),              // cut value for dz(trk,vtx) for track to include in iso sum (default  0.05)
  minPt ( iConfig.getParameter<double> ("minPt_PFCandidate") ),        // store PFCandidates with pt above this cut                 (default 10   )
  minLepPt ( iConfig.getParameter<double> ("minLepPt_PFCandidate") ),  // store Lepton PFCandidates with pt above this cut          (default  5   )
  minTrkIso ( iConfig.getParameter<double> ("minTrkIso_PFCandidate") ) // store PFCandidates with iso below this cut                (default  0.2 )
{
  produces<vector<LorentzM> > ( Prefix + "P4"     + Suffix );
  produces<vector<int> >      ( Prefix + "Chg"    + Suffix );
  produces<vector<int> >      ( Prefix + "PdgId"  + Suffix );
  produces<vector<float> >    ( Prefix + "Dz"     + Suffix );
  produces<vector<float> >    ( Prefix + "TrkIso" + Suffix );

}

TrackIsolationMaker::~TrackIsolationMaker() 
{

}

void  TrackIsolationMaker::beginRun(edm::Run&, const edm::EventSetup& es) {}
void  TrackIsolationMaker::beginJob() {}
void  TrackIsolationMaker::endJob()   {}

// ------------ method called to produce the data  ------------

void TrackIsolationMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  auto_ptr<vector<LorentzM> > p4     ( new vector<LorentzM> );
  auto_ptr<vector<int>   >    chg    ( new vector<int>  );
  auto_ptr<vector<int>   >    pdgId  ( new vector<int>  );
  auto_ptr<vector<float> >    dzpv   ( new vector<float>);
  auto_ptr<vector<float> >    trkiso ( new vector<float>);
  auto_ptr<vector<float> >    vuoto  ( new vector<float>);

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
      
      //-------------------------------------------------------------------------------------
      // only store PFCandidate values if charged and pt > minPt.
      //-------------------------------------------------------------------------------------

      LorentzM pf_p4( pf_it->p4().pt(), pf_it->p4().eta(), pf_it->p4().phi(), pf_it->p4().M());
      //std::cout<<"id="<<pf_it->pdgId()<<" pt="<<pf_p4.pt()<<" chg="<<pf_it->charge()<<std::endl;
      if ( abs(pf_it->pdgId()) == 11 || abs(pf_it->pdgId()) == 13 ) {
	if ( pf_it->pt() < minLepPt ) continue;
      }
      else if ( pf_it->pt() < minPt ) continue;
      
      if (pf_it->charge() == 0) continue;
      //std::cout<<"in the loop afetr the first cuts"<<std::endl;
      //----------------------------------------------------------------------------
      // now loop over other PFCandidates in the event to calculate trackIsolation
      //----------------------------------------------------------------------------
      
      float iso = 0.0;
      
      for( PFCandidateCollection::const_iterator pf_other = pfCandidates->begin(); pf_other != pfCandidates->end(); pf_other++ ) {
	
	// don't count the PFCandidate in its own isolation sum	
	if( pf_it == pf_other       ) continue;
	
	// require the PFCandidate to be charged
	if( pf_other->charge() == 0 ) continue;
	
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
	//cout<<"iso = "<<iso<<endl;
      }
  
      // calculate the dz of this candidate
      float dz_it = 100; //
      
      if ( pf_it->trackRef().isNonnull())
	dz_it = pf_it->trackRef()->dz( firstGoodVertex->position() );
      if (abs(pf_it->pdgId())==11 && pf_it->gsfElectronRef().isNonnull() && pf_it->gsfTrackRef().isNonnull())
	dz_it = pf_it->gsfTrackRef()->dz( firstGoodVertex->position() );
      
      //if (iso > minTrkIso) continue;


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
  
//define this as a plug-in
DEFINE_FWK_MODULE(TrackIsolationMaker);

