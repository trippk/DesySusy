#ifndef SUSY_DESY_JET
#define SUSY_DESY_JET
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

namespace SusyDESY_Tools{

  template<class T>
    bool isInCollection(const T& e, const std::vector<T>& ve) {
    typename std::vector<T>::const_iterator it(ve.begin()),end(ve.end());
    for(;it!=end;++it) 
      if( e.p4() == it->p4() ) return true;
    return false;
  }

}

using namespace std;

class SusyDESY_Jet : public edm::EDProducer {
public:
     explicit SusyDESY_Jet(const edm::ParameterSet&);
private:
     void produce( edm::Event &, const edm::EventSetup & );
     void beginJob();

     const std::string Prefix,Suffix;
     const edm::InputTag VertexTag;
     const edm::InputTag JetsTag;
     const edm::InputTag SelectedJetsTag;
     const std::string JECfile;
     const std::vector<std::string> JetsCorrections;
     const std::vector<edm::InputTag> PUJetIDdiscriminators;
     const std::vector<edm::InputTag> PUJetIDflags;
};

#endif







