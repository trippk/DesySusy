#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_PDFweights.h"

SusyDESY_PDFweights::SusyDESY_PDFweights(const edm::ParameterSet& iConfig)
  : Prefix( iConfig.getParameter<string> ("Prefix") ),
    Suffix( iConfig.getParameter<string> ("Suffix") ),
    PDFset( iConfig.getParameter<edm::InputTag> ("PDFset") )
{
    produces <bool>                 ( Prefix + "PDFsetHandleValid"  + Suffix );
    produces <std::vector<double> > ( Prefix + "weights"            + Suffix );
  //produces <>                ( Prefix + ""                 + Suffix );
}

void SusyDESY_PDFweights::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  std::auto_ptr<bool>                 handleValid ( new bool(false)         );
  std::auto_ptr<std::vector<double> > _weights ( new std::vector<double>() );
//   std::auto_ptr<std::vector<> >    ( new std::vector<>  );


  edm::Handle<std::vector< double > >  PDFweights;
  iEvent.getByLabel( PDFset, PDFweights);

  if(PDFweights.isValid()) {
    *handleValid.get() = true;
    
    std::vector<double> weights = (*PDFweights);
    //std::cout << "Event weight for central PDF:" << weights[0] << std::endl;
    _weights->push_back(weights[0]);
    unsigned int nmembers = weights.size();
    for (unsigned int j=1; j<nmembers; j+=2) {
      //std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
      //std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
      _weights->push_back(weights[j]);
      _weights->push_back(weights[j+1]);
    }

  }
  iEvent.put( handleValid , Prefix + "PDFsetHandleValid" + Suffix );
  iEvent.put( _weights    , Prefix + "weights"           + Suffix );
}

void SusyDESY_PDFweights::beginJob(){//std::cout<<"______XXXXXXXXXXXXXXXX_____"<<std::endl;
}
