#ifndef SUSY_DESY_MODULE
#define SUSY_DESY_MODULE
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH1D.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

/* #include "DataFormats/HLTReco/interface/TriggerEvent.h" */
/* #include "DataFormats/Common/interface/TriggerResults.h" */
/* #include "FWCore/Common/interface/TriggerNames.h" */
/* #include "HLTrigger/HLTcore/interface/HLTConfigProvider.h" */


using namespace std;

class SusyDESY_Electrons : public edm::EDProducer {
public:
     explicit SusyDESY_Electrons(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
     const std::string Prefix,Suffix;
     const edm::InputTag PatElectrons;
     const edm::InputTag conversionsInputTag_;
     const edm::InputTag beamSpotInputTag_;
};

class SusyDESY_Muons : public edm::EDProducer {
public:
     explicit SusyDESY_Muons(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
     const std::string Prefix,Suffix;
     const edm::InputTag PatMuons;
};


class SusyDESY_Trigger : public edm::EDProducer {
public:
     explicit SusyDESY_Trigger(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
     const std::string Prefix,Suffix;
     
     void printMatchingInfo(edm::Event& iEvent, ostream& os=cout);
     template <typename T>
     void printVector(TString name, vector<T> v, Int_t length=30, ostream& os=cout);
     template <typename T>
     void printVectorSize(TString name, vector<T> v, Int_t length=30, ostream& os=cout); 
     
     
     //const edm::InputTag TriggerRes;
     edm::InputTag inputTag; 
     edm::InputTag muons_;
     edm::InputTag electrons_;
};


class SusyDESY_PU : public edm::EDProducer {
public:
     explicit SusyDESY_PU(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
  // const std::string Prefix,Suffix;
     const edm::InputTag PileUp;
  TH1F * pu;
};


#endif







