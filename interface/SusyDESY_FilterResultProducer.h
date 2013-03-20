#ifndef SUSY_DESY_FILTERRESULTPRODUCER
#define SUSY_DESY_FILTERRESULTPRODUCER

#include <memory>


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "TRandom3.h"

//
// class declaration
//

using namespace std;

class SusyDESY_FilterResultProducerOneLepton : public edm::EDProducer {
   public:
      explicit SusyDESY_FilterResultProducerOneLepton(const edm::ParameterSet&);
      ~SusyDESY_FilterResultProducerOneLepton();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);

      // ----------member data ---------------------------
      const int Prescale;
      const vector<string> StdMuCollection,   AltMuCollection;
      const vector<string> StdElCollection,   AltElCollection;
      const vector<string> StdMuCollectionPF, AltMuCollectionPF;
      const vector<string> StdElCollectionPF, AltElCollectionPF;
      TRandom3* rnd;

      // collGetter: meant to replace the 'iEvent.getByLabel(...)' in the code.
      // set string components of the variables names by
      //    setStdCollStrings and setAltCollStrings in the order
      //      1. name of producer (e.g. "susycafelectron" or "susydesyelectron")
      //      2. Prefix (e.g. "electron" or "DESYelectron")
      //      3. Suffix (e.g. "Pat" or "PF")
      //    or as vector.
      // get collection by
      //    getCollection with the three arguments:
      //      1. iEvent
      //      2. edm::Handle
      //      3. name of variable without prefix and suffix (e.g. "P4")
      class collGetter { 
      private:
	TString stdCollProd, stdCollPre, stdCollSuf;
	TString altCollProd, altCollPre, altCollSuf;

      public:
	void setStdCollStrings(TString prod, TString pre, TString suf) {
	  stdCollProd = prod;
	  stdCollPre  = pre;
	  stdCollSuf  = suf;
	}
	void setStdCollStrings(vector<string> str) {
	  if(str.size() != 3) {
	    cout<<"collGetter: not StdCollection set"<<endl;
	    return;
	  }
	  stdCollProd = str.at(0);
	  stdCollPre  = str.at(1);
	  stdCollSuf  = str.at(2);
	}
	void setAltCollStrings(TString prod, TString pre, TString suf) {
	  altCollProd = prod;
	  altCollPre  = pre;
	  altCollSuf  = suf;
	}
	void setAltCollStrings(vector<string> str) {
	  if(str.size() != 3) {
	    cout<<"collGetter: not AltCollection set"<<endl;
	    return;
	  }
	  altCollProd = str.at(0);
	  altCollPre  = str.at(1);
	  altCollSuf  = str.at(2);
	}
	template< typename T >
	  bool getCollection( edm::Event& iEvent, T& coll, TString name ) {
	  iEvent.getByLabel(edm::InputTag((stdCollProd+":"+stdCollPre+name+stdCollSuf).Data()), coll );
	  if( coll.isValid() ) return true;
	  iEvent.getByLabel(edm::InputTag((altCollProd+":"+altCollPre+name+altCollSuf).Data()), coll );
	  if( coll.isValid() ) return true;

	  cout<<"SusyDESY_FilterResultProducerOneLepton:"
	      << " both \'"<<stdCollProd<<":"<<stdCollPre<<name<<stdCollSuf
	      <<"\' and \'"<<altCollProd<<":"<<altCollPre<<name<<altCollSuf<<"\' not found."<<endl;
	  return false;
	}
      };

  typedef reco::Candidate::LorentzVector LorentzVector;
};


#endif
