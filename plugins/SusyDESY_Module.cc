#include "SUSYBSMAnalysis/SusyDESY/interface/SusyDESY_Module.h"
#include "FWCore/Framework/interface/Event.h"

SusyDESY_Electrons::SusyDESY_Electrons(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PatElectrons( iConfig.getParameter<edm::InputTag> ("PatElectrons") )
{
  produces <bool>                 ( Prefix + "PatElectronsHandleValid" + Suffix );

  produces <std::vector<double> > ( Prefix + "SCrawEnergy"             + Suffix ); 
  produces <std::vector<double> > ( Prefix + "SCetaWidth"              + Suffix );
  produces <std::vector<double> > ( Prefix + "SCphiWidth"              + Suffix );
  produces <std::vector<double> > ( Prefix + "SCpreshowerEnergy"       + Suffix );

  //all variables you want to be added to the ntuple by your module
  produces <std::vector<double> > ( Prefix + "GenMatched"              + Suffix );
  produces <std::vector<double> > ( Prefix + "GenPdgId"                + Suffix );
  produces <std::vector<double> > ( Prefix + "GenStatus"               + Suffix );
}

void SusyDESY_Electrons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<bool>                 handleValid       ( new bool(false)           );

  std::auto_ptr<std::vector<double> > SCrawEnergy       ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > SCetaWidth        ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > SCphiWidth        ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > SCpreshowerEnergy ( new std::vector<double>() );

  //all variables you want to be added to the ntuple by your module

  std::auto_ptr<std::vector<double> > GenMatched         ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > GenPdgId           ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > GenStatus          ( new std::vector<double>() );

  edm::Handle<std::vector<pat::Electron> > ElColl;
  iEvent.getByLabel(PatElectrons, ElColl);

  if(ElColl.isValid()){
    *handleValid.get() = true;
    //you might do here something to get the variables you want to add, for example

    for(std::vector<pat::Electron>::const_iterator el = ElColl->begin(); el!=ElColl->end(); el++) {

      SCrawEnergy->push_back(el->superCluster()->rawEnergy());
      SCetaWidth->push_back(el->superCluster()->etaWidth());	
      SCphiWidth->push_back(el->superCluster()->phiWidth());
      SCpreshowerEnergy->push_back(el->superCluster()->preshowerEnergy());
      
      if(el->genLepton()){
	GenMatched->push_back(1);
	GenPdgId->push_back(el->genLepton()->pdgId());
	GenStatus->push_back(el->genLepton()->status());
      }
      else{
	GenMatched->push_back(0);
	GenPdgId->push_back(0);
	GenStatus->push_back(-1);
      }
      //cout<<"genMatched.back() = " << genMatched << endl;
      //cout<<"genPdgId.back()   = " << genPdgId.back() << endl;
      //cout<<"genStatus.back()  = " << genStatus.back() << endl;
      //cout<<endl;
    }
  }

  iEvent.put( handleValid       , Prefix + "PatElectronsHandleValid" + Suffix );

  iEvent.put( SCrawEnergy       , Prefix + "SCrawEnergy"             + Suffix );
  iEvent.put( SCetaWidth        , Prefix + "SCetaWidth"              + Suffix );
  iEvent.put( SCphiWidth        , Prefix + "SCphiWidth"              + Suffix );
  iEvent.put( SCpreshowerEnergy , Prefix + "SCpreshowerEnergy"       + Suffix );

  //all variables you want to be added to the ntuple by your module
  iEvent.put( GenMatched        , Prefix + "GenMatched"              + Suffix );
  iEvent.put( GenPdgId          , Prefix + "GenPdgId"                + Suffix );
  iEvent.put( GenStatus         , Prefix + "GenStatus"               + Suffix );
}

void SusyDESY_Electrons::beginJob(){}



SusyDESY_Muons::SusyDESY_Muons(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PatMuons( iConfig.getParameter<edm::InputTag> ("PatMuons") )
{
  produces <bool>                 ( Prefix + "PatMuonsHandleValid" + Suffix );
  //produces <std::vector<double> > ( Prefix + "PatMuonPT"           + Suffix );

  //all variables you want to be added to the ntuple by your module
}
void SusyDESY_Muons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<bool>                 handleValid ( new bool(false)           );
  //std::auto_ptr<std::vector<double> > muonPT      ( new std::vector<double>() );

  //all variables you want to be added to the ntuple by your module

  edm::Handle<std::vector<pat::Muon> > MuColl;
  iEvent.getByLabel(PatMuons, MuColl);

  if(MuColl.isValid()){
    *handleValid.get() = true;
    //you might do here something to get the variables you want to add, for example

//     for(std::vector<pat::Muon>::const_iterator el = MuColl->begin(); el!=MuColl->end(); el++) {
//       muonPT->push_back(el->pt());
//     }
  }
  iEvent.put( handleValid, Prefix + "PatMuonsHandleValid" + Suffix );
  //iEvent.put( muonPT     , Prefix + "PatMuonPT"           + Suffix );

  //all variables you want to be added to the ntuple by your module
}
void SusyDESY_Muons::beginJob(){}

