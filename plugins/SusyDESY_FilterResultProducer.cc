// -*- C++ -*-
//
// Package:    SusyDESY_FilterResultProducer
// Class:      SusyDESY_FilterResultProducer
// 
/**\class SusyDESY_FilterResultProducer SusyDESY_FilterResultProducer.cc SUSYBSMAnalysis/DesySusy/plugins/SusyDESY_FilterResultProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Hannes Schettler,,,uni-hamburg
//         Created:  Mon Mar 19 11:55:58 CET 2012
// $Id: SusyDESY_FilterResultProducer.cc,v 1.1 2012/06/08 08:39:51 schettle Exp $
//
//

#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_FilterResultProducer.h" 


SusyDESY_FilterResultProducerOneLepton::SusyDESY_FilterResultProducerOneLepton(const edm::ParameterSet& iConfig):
  Prescale          ( iConfig.getParameter<int>             (  "Prescale"         ) ),
  StdMuCollection   ( iConfig.getParameter<vector<string> > (  "StdMuCollection"  ) ),
  AltMuCollection   ( iConfig.getParameter<vector<string> > (  "AltMuCollection"  ) ),
  StdElCollection   ( iConfig.getParameter<vector<string> > (  "StdElCollection"  ) ),
  AltElCollection   ( iConfig.getParameter<vector<string> > (  "AltElCollection"  ) ),
  StdMuCollectionPF ( iConfig.getParameter<vector<string> > (  "StdMuCollectionPF") ),
  AltMuCollectionPF ( iConfig.getParameter<vector<string> > (  "AltMuCollectionPF") ),
  StdElCollectionPF ( iConfig.getParameter<vector<string> > (  "StdElCollectionPF") ),
  AltElCollectionPF ( iConfig.getParameter<vector<string> > (  "AltElCollectionPF") )
{
 
  produces < int > ("DESYfilterPassedOneLepton");

}


SusyDESY_FilterResultProducerOneLepton::~SusyDESY_FilterResultProducerOneLepton()
{
}

void
SusyDESY_FilterResultProducerOneLepton::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

//   collGetter TriggerGetter;
//   TriggerGetter.setStdCollStrings("susycaftriggers","","");
//   edm::Handle<std::map<std::string,bool> > triggered;
//   TriggerGetter.getCollection(iEvent, triggered, "triggered");

//   cout<<"Trigger:"<<endl;
//   for(std::map<std::string,bool>::const_iterator trig=triggered->begin(); trig!=triggered->end(); ++trig) {
//     if( trig->second )
//       cout<<" "<<trig->first<<"      "<<trig->second<<endl;
//   }

  collGetter MuGetter;
  MuGetter.setStdCollStrings( StdMuCollection );
  MuGetter.setAltCollStrings( AltMuCollection );
  
  edm::Handle<std::vector< LorentzVector > >  MuonP4;
  MuGetter.getCollection(iEvent, MuonP4, "P4");

//    edm::Handle<std::vector< float > > MuEcalIso, MuHcalIso, MuTrackIso;
//    MuGetter.getCollection( iEvent, MuEcalIso , "EcalIso"  );   
//    MuGetter.getCollection( iEvent, MuHcalIso , "HcalIso"  );
//    MuGetter.getCollection( iEvent, MuTrackIso, "TrackIso" );

  unsigned nGoodMuons=0;
  for(size_t mu = 0; mu<MuonP4->size(); mu++) {
    if( fabs(MuonP4->at(mu).eta()) >  2.5  ) continue;
    if( MuonP4->at(mu).pt()        < 10.   ) continue;
//      double RelIso_max20 = (MuEcalIso->at(mu)+MuHcalIso->at(mu)+MuTrackIso->at(mu))/std::max((float)20.,(float)MuonColl->at(mu).Pt());
//      if( RelIso_max20                 >   .15 ) continue;
     
    ++nGoodMuons;
  }

  collGetter MuGetterPF;
  MuGetterPF.setStdCollStrings( StdMuCollectionPF );
  MuGetterPF.setAltCollStrings( AltMuCollectionPF );
  
  edm::Handle<std::vector< LorentzVector > >  MuonP4PF;
  MuGetterPF.getCollection(iEvent, MuonP4PF, "P4");

  unsigned nGoodMuonsPF=0;
  for(size_t mu = 0; mu<MuonP4PF->size(); mu++) {
    if( fabs(MuonP4PF->at(mu).eta()) >  2.5  ) continue;
    if( MuonP4PF->at(mu).pt()        < 10.   ) continue;
     
    ++nGoodMuonsPF;
  }


  collGetter ElGetter;
  ElGetter.setStdCollStrings( StdElCollection );
  ElGetter.setAltCollStrings( AltElCollection );

  edm::Handle<std::vector< LorentzVector > >  ElectronP4;
  ElGetter.getCollection(iEvent, ElectronP4, "P4");

  unsigned nGoodElectrons=0;
  for(size_t el = 0; el<ElectronP4->size(); el++) {
    if( fabs(ElectronP4->at(el).eta()) >  2.5 ) continue;
    if( ElectronP4->at(el).pt()        < 10.  ) continue;

    ++nGoodElectrons;
  }

  collGetter ElGetterPF;
  ElGetterPF.setStdCollStrings( StdElCollectionPF );
  ElGetterPF.setAltCollStrings( AltElCollectionPF );

  edm::Handle<std::vector< LorentzVector > >  ElectronP4PF;
  ElGetterPF.getCollection(iEvent, ElectronP4PF, "P4");

  unsigned nGoodElectronsPF=0;
  for(size_t el = 0; el<ElectronP4PF->size(); el++) {
    if( fabs(ElectronP4PF->at(el).eta()) >  2.5 ) continue;
    if( ElectronP4PF->at(el).pt()        < 10.  ) continue;

    ++nGoodElectronsPF;
  }


  int filter=0;

  double random = rnd->Rndm();

  if( max(nGoodMuons,nGoodMuonsPF) + max(nGoodElectrons,nGoodElectronsPF) >= 1 )
    filter=1;
  else if(random < 1./double(Prescale) )
    filter=Prescale;

  std::auto_ptr<int> filterPassed (new int(filter));
  iEvent.put(filterPassed, "DESYfilterPassedOneLepton");

}

void 
SusyDESY_FilterResultProducerOneLepton::beginJob()
{
   rnd = new TRandom3();
}



