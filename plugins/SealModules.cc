#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Module.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_PDFweights.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_TotalKinematicsFilter.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_FilterResultProducer.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Filter.h"

DEFINE_FWK_MODULE(SusyDESY_Electrons);
DEFINE_FWK_MODULE(SusyDESY_Muons);
DEFINE_FWK_MODULE(SusyDESY_Trigger);
DEFINE_FWK_MODULE(SusyDESY_PDFweights);
DEFINE_FWK_MODULE(SusyDESY_TotalKinematicsFilterResult);
DEFINE_FWK_MODULE(SusyDESY_FilterResultProducerOneLepton);
DEFINE_FWK_MODULE(SusyDESY_Filter);
DEFINE_FWK_MODULE(SusyDESY_PU);
