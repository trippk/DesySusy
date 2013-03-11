#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Electron.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Muon.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Tau.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Jet.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Track.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Trigger.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_PileUp.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_PDFweights.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_TotalKinematicsFilter.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_FilterResultProducer.h"
#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Filter.h"

DEFINE_FWK_MODULE(SusyDESY_Electron);
DEFINE_FWK_MODULE(SusyDESY_Muon);
DEFINE_FWK_MODULE(SusyDESY_Jet);
typedef SusyDESY_Tau<reco::PFTau> SusyDESY_pfTau;
typedef SusyDESY_Tau<pat::Tau> SusyDESY_PatTau;
DEFINE_FWK_MODULE(SusyDESY_PatTau);
DEFINE_FWK_MODULE(SusyDESY_pfTau);

DEFINE_FWK_MODULE(SusyDESY_Trigger);
DEFINE_FWK_MODULE(SusyDESY_PDFweights);
DEFINE_FWK_MODULE(SusyDESY_TotalKinematicsFilterResult);
DEFINE_FWK_MODULE(SusyDESY_FilterResultProducerOneLepton);
DEFINE_FWK_MODULE(SusyDESY_Filter);
DEFINE_FWK_MODULE(SusyDESY_PileUp);
DEFINE_FWK_MODULE(SusyDESY_Track);
