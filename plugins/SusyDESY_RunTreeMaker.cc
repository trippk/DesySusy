#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TTree.h>

#include <SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h>
#include <FWCore/Framework/interface/Run.h>


class DesySusy_RunTreeMaker : public edm::EDAnalyzer {
public:
  explicit DesySusy_RunTreeMaker(const edm::ParameterSet&);
  ~DesySusy_RunTreeMaker();
  
private:
  virtual void beginJob();
  virtual void endJob(){}
  virtual void analyze(const edm::Event&, const edm::EventSetup&){}
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);

  edm::Service<TFileService> fs;
  TTree *tree;

  const edm::InputTag inputTag;
  bool handleIsValid;
  double internalXsecVal;

};

DesySusy_RunTreeMaker::DesySusy_RunTreeMaker(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
{
}

DesySusy_RunTreeMaker::~DesySusy_RunTreeMaker()
{
}

void DesySusy_RunTreeMaker::beginJob()
{
  tree=fs->make<TTree>("tree", "");
  tree->Branch("handleIsValid",     &handleIsValid,   "handleIsValid/O"     );
  tree->Branch("internalXsecValue", &internalXsecVal, "internalXsecValue/D" );
}

void DesySusy_RunTreeMaker::beginRun(const edm::Run& iRun, const edm::EventSetup& eventSetup)
{

  edm::Handle<GenRunInfoProduct> genruninfo;  
  iRun.getByLabel( inputTag, genruninfo );

  handleIsValid=genruninfo.isValid();
  
  if (!handleIsValid) {
    handleIsValid   = false;
    internalXsecVal = -1;
  } else {
    handleIsValid     = true;
    internalXsecVal = genruninfo->internalXSec().value();
  }
  
  tree->Fill();
}

DEFINE_FWK_MODULE(DesySusy_RunTreeMaker);
