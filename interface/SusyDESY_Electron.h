#ifndef SUSY_DESY_ELECTRON
#define SUSY_DESY_ELECTRON

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace edm;

class SusyDESY_Electron : public edm::EDProducer {
public:
     explicit SusyDESY_Electron(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();

     const std::string Prefix,Suffix;
     const edm::InputTag PatElectrons;
     const edm::InputTag conversionsInputTag_;
     const edm::InputTag beamSpotInputTag_;
     const edm::InputTag rhoIsoInputTag_;
     const std::vector<edm::InputTag> isoValInputTags_;
};

#endif







