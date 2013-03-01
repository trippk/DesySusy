#ifndef SUSY_DESY_MUON
#define SUSY_DESY_MUON
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

/* #include "DataFormats/HLTReco/interface/TriggerEvent.h" */
/* #include "DataFormats/Common/interface/TriggerResults.h" */
/* #include "FWCore/Common/interface/TriggerNames.h" */
/* #include "HLTrigger/HLTcore/interface/HLTConfigProvider.h" */

using namespace std;

class SusyDESY_Muon : public edm::EDProducer {
public:
     explicit SusyDESY_Muon(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();

     const std::string Prefix,Suffix;
     const edm::InputTag PatMuons;
};

#endif







