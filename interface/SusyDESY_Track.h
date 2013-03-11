#ifndef SUSY_DESY_TRACK
#define SUSY_DESY_TRACK

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

class SusyDESY_Track : public edm::EDProducer {
public:
     explicit SusyDESY_Track (const edm::ParameterSet&);
     ~SusyDESY_Track();

private:
  //  virtual void beginJob() ;
  virtual void beginJob() ;
  virtual void beginRun(edm::Run&, const edm::EventSetup&) ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  float getFixGridRho(std::vector<float>& etabins,std::vector<float>& phibins);
  
  // ----------member data ---------------------------

  const std::string Prefix;
  const std::string Suffix;

  const edm::InputTag PfCandidatesTag;
  const edm::InputTag VertexTag;

  double dRcut;
  double dzcut;

  const reco::PFCandidateCollection *pfCandidates;

};

#endif

