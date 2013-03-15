#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_EventFilterFromList.h"

SusyDESY_EventFilterFromList::SusyDESY_EventFilterFromList(const edm::ParameterSet& iConfig)
  :   ListFile    ( iConfig.getParameter<string>( "ListFile") ),
      taggingMode ( iConfig.getUntrackedParameter<bool>( "taggingMode", true) )
{
  ListFilePath = FileInPath(  ListFile );
  if ( ListFilePath.location() > 0 ) {
    Filter = EventFilterFromListStandAlone( ListFilePath.fullPath().c_str()  ); 
    if ( taggingMode ) produces<bool>();
  }
}

bool SusyDESY_EventFilterFromList::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  if ( ListFilePath.location() == 0 ) {
    cout<< "WARNING!! SusyDESY_EventFilterFromList could not find ListFile "<<ListFile<<"."<<endl;
    return true;
  }

  int flag = false;

  flag = Filter.filter( iEvent.id().run(), iEvent.id().luminosityBlock(), iEvent.id().event() );

  if ( !taggingMode ) 
    return flag;
  else {
    auto_ptr<bool> pOut( new bool(flag) );
    iEvent.put( pOut);
    return true;
  }
}

void SusyDESY_EventFilterFromList::beginJob() {}
