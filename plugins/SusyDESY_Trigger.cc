#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Trigger.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

SusyDESY_Trigger::SusyDESY_Trigger(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    inputTag  (iConfig.getParameter<edm::InputTag>("inputTag"))
{

  produces <bool>                               ( Prefix + "HandleValid"  + Suffix );
  produces <std::map<std::string,std::string> > ( Prefix + "NameMap"      + Suffix );

}


void SusyDESY_Trigger::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  std::auto_ptr<bool>                               handleValid ( new bool(false)                        );
  std::auto_ptr<std::map<std::string,std::string> > nameMap     ( new std::map<std::string,std::string>  );

  edm::Handle<std::map<std::string,bool> >  triggered;
  iEvent.getByLabel( inputTag, triggered );

  if(triggered.isValid()) {
    *handleValid.get() = true;

    for(std::map<std::string,bool>::const_iterator trig=triggered->begin(); trig!=triggered->end(); ++trig) {
      std::string tn = trig->first;
      size_t usc = tn.find_last_of("_");
      if( tn.substr(usc+1)[0]=='v' && atoi(tn.substr(usc+2).c_str()) > 0 )
	(*nameMap)[tn.substr(0,usc)] = tn;
      else 
	(*nameMap)[tn              ] = tn;
    }
  }
  iEvent.put( handleValid , Prefix + "HandleValid" + Suffix );
  iEvent.put( nameMap     , Prefix + "NameMap"     + Suffix );
}


void SusyDESY_Trigger::beginJob(){}
