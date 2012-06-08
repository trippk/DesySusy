#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


class SusyDESY_Filter : public edm::EDFilter {
   public:
      explicit SusyDESY_Filter(const edm::ParameterSet&);
      ~SusyDESY_Filter();

  //static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
  //virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
  //virtual void endJob() ;
      
//       virtual bool beginRun(edm::Run&, edm::EventSetup const&);
//       virtual bool endRun(edm::Run&, edm::EventSetup const&);
//       virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
//       virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------

      const edm::InputTag FilterResult;

};
