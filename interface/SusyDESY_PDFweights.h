#ifndef SUSY_DESY_PDFWEIGHTS
#define SUSY_DESY_PDFWEIGHTS
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;

class SusyDESY_PDFweights : public edm::EDProducer {
public:
     explicit SusyDESY_PDFweights(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
     const std::string Prefix,Suffix;
     const edm::InputTag PDFset;
};

#endif
