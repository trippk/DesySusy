#ifndef SUSY_DESY_EVENTFILTERFROMLIST
#define SUSY_DESY_EVENTFILTERFROMLIST

#include <memory>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SUSYBSMAnalysis/DesySusy/interface/EventFilterFromListStandAlone.h"

//
// class declaration
//


using namespace std;
using namespace edm;

class SusyDESY_EventFilterFromList : public edm::EDFilter {
 public:
  explicit SusyDESY_EventFilterFromList(const edm::ParameterSet&);
  
 private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
  const std::string ListFile;
  EventFilterFromListStandAlone Filter;
  FileInPath ListFilePath;
  bool taggingMode;

};


#endif
