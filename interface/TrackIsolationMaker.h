// -*- C++ -*-
//
// Package:    NtupleMaker
// Class:      NtupleMaker
// 
/**\class NtupleMaker NtupleMaker.cc CMS2/NtupleMaker/src/NtupleMaker.cc

   Description: <one line class summary>

   Implementation:
   <Notes on implementation>
*/
#ifndef NTUPLEMAKER_TRACKISOLATIONMAKER_H
#define NTUPLEMAKER_TRACKISOLATIONMAKER_H

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/ParticleFlow/interface/PFPileUpAlgo.h"
#include "Math/VectorUtil.h"

//
// class decleration
//

class TrackIsolationMaker : public edm::EDProducer {
public:
     explicit TrackIsolationMaker (const edm::ParameterSet&);
     ~TrackIsolationMaker();

private:
  //  virtual void beginJob() ;
  virtual void beginJob() ;
  virtual void beginRun(edm::Run&, const edm::EventSetup&) ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  float getFixGridRho(std::vector<float>& etabins,std::vector<float>& phibins);
  
  // ----------member data ---------------------------
  double dRcut;
  double dzcut;
  double minPt;
  double minLepPt;
  double minTrkIso;

  const edm::InputTag PfCandidatesTag;
  const edm::InputTag VertexTag;

  const std::string Prefix;
  const std::string Suffix;

  const reco::PFCandidateCollection *pfCandidates;

};

#endif

