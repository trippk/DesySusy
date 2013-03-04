#ifndef SUSY_DESY_SCAN
#define SUSY_DESY_SCAN

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "boost/regex.hpp"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_ScanPoint.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH2I.h"

class SusyDESY_Scan : public edm::EDProducer {
 public: 
  explicit SusyDESY_Scan(const edm::ParameterSet&);
  double convert(std::string);
 private:
  typedef std::map<std::string, double> DoubleParameterMap;
 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  const boost::regex scanFormat;
  const std::vector<std::string> scanPars;
  const bool debug;
  std::map<ScanPoint, DoubleParameterMap > additionalDoubleVars_;
  TH2I * scan;
  DoubleParameterMap additionalDoubleDefaultVars_;
};

#endif
