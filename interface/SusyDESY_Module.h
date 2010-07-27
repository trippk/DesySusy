#ifndef SUSY_DESY_MODULE
#define SUSY_DESY_MODULE
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

//include whatever else is needed
using namespace std;

class SusyDESY_Electrons : public edm::EDProducer {
public:
     explicit SusyDESY_Electrons(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
     const std::string Prefix,Suffix;
     const edm::InputTag PatElectrons;
     //declare whatever you need to declare
};

class SusyDESY_Muons : public edm::EDProducer {
public:
     explicit SusyDESY_Muons(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
     const std::string Prefix,Suffix;
     const edm::InputTag PatMuons;
     //declare whatever you need to declare
};
#endif




