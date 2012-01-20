#ifndef SUSY_DESY_TOTALKINEMATICS
#define SUSY_DESY_TOTALKINEMATICS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Particle.h"

using namespace std;

class SusyDESY_TotalKinematicsFilterResult : public edm::EDProducer 
{
  public:
  explicit SusyDESY_TotalKinematicsFilterResult(const edm::ParameterSet&);
  ~SusyDESY_TotalKinematicsFilterResult();

  private:
  void produce(edm::Event&, const edm::EventSetup&);
  void beginJob();
  // ----------member data ---------------------------

  const std::string Prefix,Suffix;
  const edm::InputTag src_;
  double tolerance_;
  bool verbose_;

};



#endif
