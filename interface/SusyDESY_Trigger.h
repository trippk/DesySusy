#ifndef SUSY_DESY_TRIGGER
#define SUSY_DESY_TRIGGER
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

class SusyDESY_Trigger : public edm::EDProducer {
public:
     explicit SusyDESY_Trigger(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();
     const std::string Prefix,Suffix;
     //const edm::InputTag TriggerRes;


     //enum DataSource { NOT_APPLICABLE, STREAM, DATASET }; 
     edm::InputTag         inputTag; 
/*       std::string           sourceName;  */
/*       DataSource            sourceType;  */
/*       HLTConfigProvider     hltConfig;  */
/*       edm::InputTag         tag_;  */
/*       //int                   run_;  */

/*       std::vector<std::string>    dataSource;  */

/*       void getDataSource() { */
/* 	dataSource.clear(); */
/* 	if (sourceType == NOT_APPLICABLE) return; */
	
/* 	if (sourceType == STREAM) { */
/* 	  //unsigned int  index   = hltConfig.streamIndex(sourceName); */
/* 	  dataSource    = hltConfig.streamContent(sourceName); */
/* 	} */
/* 	else { */
/* 	  //unsigned int  index   = hltConfig.datasetIndex(sourceName); */
/* 	  dataSource    = hltConfig.datasetContent(sourceName); */
/* 	} */
/*       } */
      
};


#endif







