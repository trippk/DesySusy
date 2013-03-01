#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_PileUp.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

SusyDESY_PileUp::SusyDESY_PileUp(const edm::ParameterSet& iConfig)
  :PileUp  (iConfig.getParameter<edm::InputTag>("PileUp"))
{
}

void SusyDESY_PileUp::beginJob(){
  edm::Service<TFileService> fs;
  pu= fs->make<TH1F>("pu","pu dist",100,0,100);
}

void SusyDESY_PileUp::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<float> pileup ;
  iEvent.getByLabel( PileUp, pileup );
  pu->Fill(*pileup.product());

}
