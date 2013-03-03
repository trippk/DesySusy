#ifndef SUSY_DESY_TAU
#define SUSY_DESY_TAU

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include <string>
#include <vector>
#include <iostream>
#include "TMath.h"
using namespace std;

template< typename T >
class SusyDESY_Tau : public edm::EDProducer {
 public: 
 explicit SusyDESY_Tau(const edm::ParameterSet&);
 private:
  void initTemplate();
  void initRECO();
  void initPAT();
  void produce(edm::Event &, const edm::EventSetup & );
  void produceTemplate(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void produceRECO(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void producePAT(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);

  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > LorentzM;
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  const std::vector<std::string> TauIds;
  std::vector<std::string> availableIds;
  std::vector<std::string> missingIds;
};

template< typename T >
SusyDESY_Tau<T>::SusyDESY_Tau(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix")),
  TauIds ( iConfig.getParameter<std::vector<std::string> >  ( "TauIds") )
{
  
  initTemplate();
}

template< typename T >
void SusyDESY_Tau<T>::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<T> > collection;
  iEvent.getByLabel(inputTag,collection);

  produceTemplate(iEvent, iSetup, collection);
}

template<typename T>
void SusyDESY_Tau<T>::initRECO()
{
  produces <bool> (Prefix + "HandleValid" + Suffix);
  produces <std::vector<LorentzM> > ( Prefix + "P4" + Suffix );
  produces <std::vector<int> > (  Prefix + "Charge" + Suffix);
  produces <std::vector<math::XYZPoint> > (Prefix + "Vertex" + Suffix);

  produces <std::vector<int> > (Prefix + "NumSigTrks" + Suffix);
  produces <std::vector<int> > (Prefix + "NumIsoTrks" + Suffix);
  produces <std::vector<int> > (Prefix + "NumSigPFNeutHadrCands" + Suffix);  

  produces <std::vector<double> > (Prefix + "SumPtIsoPFNeutCands" + Suffix);
  produces <std::vector<double> > (Prefix + "NeutCandsTotEnergy" + Suffix);
  produces <std::vector<double> > (Prefix + "NeutCandsHoverHPlusE" + Suffix);
  
}

template< typename T >
void SusyDESY_Tau<T>::initPAT()
{

  produces <std::vector<double> > (Prefix + "TrkIso" + Suffix);
  produces <std::vector<double> >(Prefix + "EcalIso" + Suffix);
  produces <std::vector<double> >(Prefix + "HcalIso" +Suffix);
  produces <std::vector<double> >(Prefix + "CaloIso" + Suffix);

  produces <std::vector<float> > (Prefix + "ParticleIso" + Suffix);
  produces <std::vector<float> > (Prefix + "ChargedHadronIso" + Suffix);
  produces <std::vector<float> > (Prefix + "NeutralHadronIso" + Suffix);
  produces <std::vector<float> > (Prefix + "PhotonIso" + Suffix);

  for(std::vector<std::string>::const_iterator tauid = TauIds.begin(); tauid!=TauIds.end(); tauid++)
    produces <std::vector<float> > (Prefix + "TauId" + *tauid + Suffix );

}

template< typename T >
void SusyDESY_Tau<T>::
produceRECO(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& collection) {

  std::auto_ptr<bool> isHandleValid (new bool(collection.isValid()) );
  std::auto_ptr<std::vector<LorentzM> > p4 ( new std::vector<LorentzM>() );
  std::auto_ptr<std::vector<int> >  charge   ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<math::XYZPoint> > vertex (new std::vector<math::XYZPoint>() );

  std::auto_ptr<std::vector<int> > numSigTrks (new std::vector<int>() );
  std::auto_ptr<std::vector<int> > numIsoTrks (new std::vector<int>() );
  std::auto_ptr<std::vector<int> > numSigPFNeuHadCands (new std::vector<int> () );
  std::auto_ptr<std::vector<double> > SumPtIsoTrks (new std::vector<double> () );
  std::auto_ptr<std::vector<double> > NeutCandstotE (new std::vector<double> () );
  std::auto_ptr<std::vector<double> > NeutCandsHoHplusE (new std::vector<double> () );
  std::auto_ptr<std::vector<int> > againstele  (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > againstmu (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bydecay  (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bylooseisolation (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bymediumisolation  (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bytightisolation (new std::vector<int> () );
  
  if(collection.isValid()){
    uint itau=0;
    for(typename std::vector<T>::const_iterator it = collection->begin(); it!=collection->end(); it++) {
      LorentzM tau_p4( it->p4().pt(), it->p4().eta(), it->p4().phi(), it->p4().M());
      p4->push_back(tau_p4);
      charge->push_back(it->charge());
      
      bool pfVertex = (it->signalPFChargedHadrCands().size() && 
		       it->signalPFChargedHadrCands().begin()->isNonnull() &&
		       (*it->signalPFChargedHadrCands().begin())->trackRef().isNonnull() );
      vertex->push_back( !pfVertex ? it->vertex() : (*it->signalPFChargedHadrCands().begin())->trackRef()->vertex() );
         
      numSigTrks->push_back(it->signalTracks().size());
      numIsoTrks->push_back((it->isolationTracks()).size());
      numSigPFNeuHadCands->push_back((it->signalPFNeutrHadrCands()).size());
      double sumptisotrks=0.0;
      for(reco::track_iterator isotrk = (it->isolationTracks()).begin(); isotrk!=(it->isolationTracks()).end(); isotrk++){
	sumptisotrks+=(*isotrk)->pt();
      }

      SumPtIsoTrks->push_back(sumptisotrks);

      double ecaltauen = 0.0, hcaltauen = 0.0;

      if(it->signalPFNeutrHadrCands().isAvailable()){
	for(reco::PFCandidateRefVector::const_iterator sigNCands = (it->signalPFNeutrHadrCands()).begin(); sigNCands!=(it->signalPFNeutrHadrCands()).end(); sigNCands++){
	  ecaltauen += (*sigNCands)->ecalEnergy();
	  hcaltauen += (*sigNCands)->hcalEnergy();
	}
      }
      NeutCandstotE->push_back(ecaltauen+hcaltauen);
      if(ecaltauen+hcaltauen>0.0){
	NeutCandsHoHplusE->push_back(hcaltauen/(hcaltauen+ecaltauen));
      }
      
      itau++;
    } 
    
  }
  
  iEvent.put(isHandleValid, Prefix + "HandleValid" + Suffix);
  iEvent.put( p4,  Prefix + "P4" + Suffix );
  iEvent.put(charge,  Prefix + "Charge" + Suffix);
  iEvent.put(vertex ,Prefix + "Vertex" + Suffix);
  iEvent.put(numSigTrks, Prefix + "NumSigTrks" + Suffix);
  iEvent.put(numIsoTrks, Prefix + "NumIsoTrks" + Suffix);
  iEvent.put(numSigPFNeuHadCands, Prefix + "NumSigPFNeutHadrCands" + Suffix);  
  iEvent.put(SumPtIsoTrks, Prefix + "SumPtIsoPFNeutCands" + Suffix);
  iEvent.put(NeutCandstotE, Prefix + "NeutCandsTotEnergy" + Suffix);
  iEvent.put(NeutCandsHoHplusE, Prefix + "NeutCandsHoverHPlusE" + Suffix);

}
  
  // extra information stored for PAT data
template< typename T >
void SusyDESY_Tau<T>::
producePAT(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& collection) {
  
  std::auto_ptr<std::vector<double> > trkiso (new std::vector<double>());
  std::auto_ptr<std::vector<double> > ecaliso (new std::vector<double>());
  std::auto_ptr<std::vector<double> > hcaliso (new std::vector<double>());
  std::auto_ptr<std::vector<double> > caloiso (new std::vector<double>());
  
  std::auto_ptr<std::vector<float> > partIso (new std::vector<float>() );
  std::auto_ptr<std::vector<float> > charHadIso (new std::vector<float>() );
  std::auto_ptr<std::vector<float> > neutHadIso (new std::vector<float>() );
  std::auto_ptr<std::vector<float> > photIso (new std::vector<float>() );
  
  
  std::map<std::string, std::vector<float> *> TauDiscs; //fill map with pointers rather than auto_ptr
  for(std::vector<std::string>::const_iterator tauid = TauIds.begin(); tauid!=TauIds.end(); tauid++)
    TauDiscs[*tauid] = new std::vector<float>();

  int count = 0;
  if( collection.isValid()) {
    for(std::vector<pat::Tau>::const_iterator it = collection->begin(); it!=collection->end(); it++, count++){
      
      trkiso->push_back(it->trackIso());
      ecaliso->push_back(it->ecalIso());
      hcaliso->push_back(it->hcalIso());
      caloiso->push_back(it->caloIso());
      
      partIso->push_back(it->particleIso());
      charHadIso->push_back(it->chargedHadronIso());
      neutHadIso->push_back(it->neutralHadronIso());
      photIso->push_back(it->photonIso());

      //std::cout << "tau " << count << ": pt = " << it->pt() << "; eta = " << it->eta() <<"; ";
      //std::cout << "byLooseCombinedIsolationDeltaBetaCorr " << it->tauID("byLooseCombinedIsolationDeltaBetaCorr") << "; ";
      //std::cout << "ByDecayModeFinding " << it->tauID("decayModeFinding") << "."<< std::endl;

      for(std::vector<std::string>::const_iterator tauid = TauIds.begin(); tauid!=TauIds.end(); tauid++)
	TauDiscs[*tauid]->push_back(it->tauID(*tauid));
      
    }
  }

  //std::cout<<std::endl;
  
  iEvent.put(trkiso, Prefix + "TrkIso" + Suffix);
  iEvent.put(ecaliso, Prefix + "EcalIso" + Suffix);
  iEvent.put(hcaliso, Prefix + "HcalIso"+ Suffix);
  iEvent.put(caloiso, Prefix + "CaloIso" + Suffix);
  iEvent.put(partIso, Prefix + "ParticleIso" + Suffix);
  iEvent.put(charHadIso, Prefix + "ChargedHadronIso" + Suffix);
  iEvent.put(neutHadIso, Prefix + "NeutralHadronIso" + Suffix);
  iEvent.put(photIso, Prefix + "PhotonIso" + Suffix);
  for(std::map<std::string, std::vector<float> *>::iterator tauid = TauDiscs.begin(); tauid != TauDiscs.end(); tauid++)
    iEvent.put(std::auto_ptr<std::vector<float> >(tauid->second), Prefix + "TauId" + tauid->first + Suffix);
  
}

#endif
  
