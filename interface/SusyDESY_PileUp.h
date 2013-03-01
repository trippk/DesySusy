#ifndef SUSY_DESY_PILEUP
#define SUSY_DESY_PILEUP
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH1D.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include <vector>

using namespace std;

class SusyDESY_PileUp : public edm::EDProducer {
 public:
  explicit SusyDESY_PileUp(const edm::ParameterSet&);
 private:
  void produce( edm::Event &, const edm::EventSetup & );
  void beginJob();
  const edm::InputTag PileUp;
  TH1F * pu;
};

#endif







