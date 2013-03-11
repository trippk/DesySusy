#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Jet.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

typedef std::vector <reco::PFCandidatePtr> constituents_type;
typedef std::vector <reco::PFCandidatePtr>::iterator constituents_iterator;

SusyDESY_Jet::SusyDESY_Jet(const edm::ParameterSet& iConfig)
  : Prefix               ( iConfig.getParameter<string> ("Prefix") ),
    Suffix               ( iConfig.getParameter<string> ("Suffix") ),
    VertexTag            ( iConfig.getParameter<edm::InputTag> ("VertexInputTag") ),
    JetsTag              ( iConfig.getParameter<edm::InputTag> ( "JetsTag") ),
    SelectedJetsTag      ( iConfig.getParameter<edm::InputTag> ( "SelectedJetsTag") ),
    JECfile              ( iConfig.getParameter<string>        (         "JECfile") ),
    JetsCorrections      ( iConfig.getParameter<std::vector<std::string> >  ( "JetCorrections") ),
    PUJetIDdiscriminators( iConfig.getParameter<std::vector<edm::InputTag> >( "Discriminators") ),
    PUJetIDflags         ( iConfig.getParameter<std::vector<edm::InputTag> >(          "Flags") )
{
  produces <std::vector<int> >   (Prefix + "Selected" + Suffix );
  
  produces <std::vector<float> > (Prefix + "JECuncertainty" + Suffix );
  for(std::vector<std::string>::const_iterator corr = JetsCorrections.begin(); corr!=JetsCorrections.end(); corr++)
    produces <std::vector<float> > (Prefix + *corr + "CorrFactor" + Suffix );

  /*produces <std::vector<float> > (Prefix + "PileUpId" + "dZ"          + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "nCharged"    + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "nNeutrals"   + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "nParticles"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "dR2Mean" + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "dRMean"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "frac01"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "frac02"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "frac03"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "frac04"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "frac05"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "frac06"  + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "frac07"  + Suffix );*/
  produces <std::vector<float> > (Prefix + "PileUpId" + "beta"            + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "betaStar"        + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "betaClassic"     + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "betaStarClassic" + Suffix );
  produces <std::vector<float> > (Prefix + "PileUpId" + "beta2" + Suffix );
  /*  produces <std::vector<float> > (Prefix + "PileUpId" + "ptD"  + Suffix );
      produces <std::vector<float> > (Prefix + "PileUpId" + "nvtx" + Suffix );*/

  for(std::vector<edm::InputTag>::const_iterator puid = PUJetIDdiscriminators.begin(); puid!=PUJetIDdiscriminators.end(); puid++) {
    produces <std::vector<float> > (Prefix + "PileUpId" + puid->instance() + Suffix );
    std::string tmp = puid->instance(); tmp.erase(tmp.find("Discriminant"));
    produces <std::vector<int> > (Prefix + "PileUpId" + tmp + "Loose"  + Suffix );
    produces <std::vector<int> > (Prefix + "PileUpId" + tmp + "Medium" + Suffix );
    produces <std::vector<int> > (Prefix + "PileUpId" + tmp + "Tight"  + Suffix );
  }
}

void SusyDESY_Jet::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<int> >   selected    ( new std::vector<int>()   );
  std::auto_ptr<std::vector<float> > uncertainty ( new std::vector<float>() );
  std::vector<std::auto_ptr<std::vector<float> > > corrections;
  for(std::vector<std::string>::const_iterator str = JetsCorrections.begin(); str!=JetsCorrections.end(); str++) {
    corrections.push_back( std::auto_ptr<std::vector<float> >(new std::vector<float>()) );
  }

  /*std::auto_ptr<std::vector<float> > dZ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> > nCharged( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > nNeutrals( new std::vector<float>() ); 
  std::auto_ptr<std::vector<float> > nParticles( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > dR2Mean( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > dRMean( new std::vector<float>() ); 
  std::auto_ptr<std::vector<float> > frac01( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > frac02( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > frac03( new std::vector<float>() ); 
  std::auto_ptr<std::vector<float> > frac04( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > frac05( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > frac06( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > frac07( new std::vector<float>() );*/
  std::auto_ptr<std::vector<float> > beta( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > betaStar( new std::vector<float>() ); 
  std::auto_ptr<std::vector<float> > betaClassic( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > betaStarClassic( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > beta2( new std::vector<float>() );
  /*std::auto_ptr<std::vector<float> > ptD( new std::vector<float>() ); 
    std::auto_ptr<std::vector<float> > nvtx( new std::vector<float>() );*/

  std::vector<std::auto_ptr<std::vector<float> > > puidDiscriminators;
  std::vector<std::auto_ptr<std::vector<int> > > puidLoose;
  std::vector<std::auto_ptr<std::vector<int> > > puidMedium;
  std::vector<std::auto_ptr<std::vector<int> > > puidTight;
  for( unsigned int ipuid = 0; ipuid < PUJetIDdiscriminators.size(); ipuid++) {
    puidDiscriminators.push_back( std::auto_ptr<std::vector<float> >(new std::vector<float>()) );
    puidLoose.push_back( std::auto_ptr<std::vector<int> >(new std::vector<int>()) );
    puidMedium.push_back( std::auto_ptr<std::vector<int> >(new std::vector<int>()) );
    puidTight.push_back( std::auto_ptr<std::vector<int> >(new std::vector<int>()) );
  }

  edm::FileInPath JECfilePath(JECfile);
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JECfilePath.fullPath().c_str());

  edm::Handle< edm::View<pat::Jet> > JetColl;
  iEvent.getByLabel(JetsTag, JetColl);

  edm::Handle<std::vector<pat::Jet> > selectedJetColl;
  iEvent.getByLabel(SelectedJetsTag, selectedJetColl);

  edm::Handle<edm::ValueMap<StoredPileupJetIdentifier> > puJetId;
  iEvent.getByLabel("puJetId",puJetId);

  std::vector<edm::Handle<edm::ValueMap<float> > > puJetIdMVAs;
  std::vector<edm::Handle<edm::ValueMap<int> > > puJetIdFlags;

  for (unsigned int ilabel = 0; ilabel < PUJetIDdiscriminators.size(); ilabel++) {
    edm::Handle<edm::ValueMap<float> > puJetIdMVA;
    iEvent.getByLabel( PUJetIDdiscriminators.at(ilabel), puJetIdMVA);
    puJetIdMVAs.push_back(puJetIdMVA);
    
    edm::Handle<edm::ValueMap<int> > puJetIdFlag;
    iEvent.getByLabel( PUJetIDflags.at(ilabel), puJetIdFlag);
    puJetIdFlags.push_back(puJetIdFlag);
  }

  //---------------------------------
  // get Vertex Collection
  //---------------------------------
  

  edm::Handle<reco::VertexCollection> vertex_h;
  iEvent.getByLabel(VertexTag, vertex_h);
  const reco::VertexCollection *vertices = vertex_h.product();

  reco::VertexCollection::const_iterator firstGoodVertex = vertices->end();

  int firstGoodVertexIdx = 0;
  for (reco::VertexCollection::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx, ++firstGoodVertexIdx) {
    if (  !vtx->isFake() && vtx->ndof()>=4. && vtx->position().Rho()<=2.0 && fabs(vtx->position().Z())<=24.0) {
      firstGoodVertex = vtx;
      break;
    }
  }

  
  if ( (!(firstGoodVertex==vertices->end())) && JetColl.isValid() ) {
      for( unsigned int ijet = 0; ijet < JetColl->size(); ijet++) {
      
      const pat::Jet & jet = JetColl->at(ijet);
      
      //Jet Selected
      selected->push_back( selectedJetColl.isValid() && SusyDESY_Tools::isInCollection( jet, *selectedJetColl) );
	  

      //Jet Energy Correction Factors      
      for(unsigned int icorr = 0; icorr < corrections.size(); icorr++)
	corrections.at(icorr)->push_back( jet.jecSetsAvailable() ? 
					  jet.energy() / jet.correctedJet(JetsCorrections.at(icorr)).energy() :
					  1.0 );
      
      //Jet Energy Correction Uncertainties      
      jecUnc->setJetEta(jet.eta());
      jecUnc->setJetPt(jet.pt()); // here you must use the CORRECTED jet pt
      double unc = 0.;
      try {unc = jecUnc->getUncertainty(true);} // sigh,,, they are still throwing exceptions
      catch (...) {
	std::cout << "JetCorrectionUncertainty::getUncertainty threw exception on jet with pt( " << jet.pt() << " ) and eta ( " << jet.eta() << " )." << std::endl;
      }
      uncertainty->push_back(unc);

      //PU JetId
      for (unsigned int ipuid = 0; ipuid < PUJetIDdiscriminators.size(); ipuid++) {
	
	float mva   = (*(puJetIdMVAs.at(ipuid)))[JetColl->refAt(ijet)];
	int  idflag = (*(puJetIdFlags.at(ipuid)))[JetColl->refAt(ijet)];
    
	puidDiscriminators.at(ipuid)->push_back(mva);

	if ( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose ) )
	  puidLoose.at(ipuid)->push_back(1);
	else puidLoose.at(ipuid)->push_back(0);
	
	if ( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kMedium ) )
	  puidMedium.at(ipuid)->push_back(1);
	else puidMedium.at(ipuid)->push_back(0);

	if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kTight ) )
	  puidTight.at(ipuid)->push_back(1);
	else puidTight.at(ipuid)->push_back(0);

	//if (jet.pt() > 30 )std::cout << "jet " << ijet << " pt " << jet.pt() << " eta " << jet.eta() << " PU JetID MVA " << mva << " " << PUJetIDdiscriminators.at(ipuid).label() << ":" << PUJetIDdiscriminators.at(ipuid).instance() << std::endl; 
      }

      /*dZ->push_back( ((*puJetId)[JetColl->refAt(ijet)]).dZ());
      nCharged->push_back( ((*puJetId)[JetColl->refAt(ijet)]).nCharged());  
      nNeutrals->push_back( ((*puJetId)[JetColl->refAt(ijet)]).nNeutrals());
      nParticles->push_back( ((*puJetId)[JetColl->refAt(ijet)]).nParticles());
      dR2Mean->push_back( ((*puJetId)[JetColl->refAt(ijet)]).dR2Mean());
      dRMean->push_back( ((*puJetId)[JetColl->refAt(ijet)]).dRMean());  
      frac01->push_back( ((*puJetId)[JetColl->refAt(ijet)]).frac01());  
      frac02->push_back( ((*puJetId)[JetColl->refAt(ijet)]).frac02()); 
      frac03->push_back( ((*puJetId)[JetColl->refAt(ijet)]).frac03()); 
      frac04->push_back( ((*puJetId)[JetColl->refAt(ijet)]).frac04()); 
      frac05->push_back( ((*puJetId)[JetColl->refAt(ijet)]).frac05()); 
      frac06->push_back( ((*puJetId)[JetColl->refAt(ijet)]).frac06()); 
      frac07->push_back( ((*puJetId)[JetColl->refAt(ijet)]).frac07()); */
      beta->push_back( ((*puJetId)[JetColl->refAt(ijet)]).beta());     
      betaStar->push_back( ((*puJetId)[JetColl->refAt(ijet)]).betaStar()); 
      betaClassic->push_back( ((*puJetId)[JetColl->refAt(ijet)]).betaClassic()); 
      betaStarClassic->push_back( ((*puJetId)[JetColl->refAt(ijet)]).betaStarClassic()); 
      /*ptD->push_back( ((*puJetId)[JetColl->refAt(ijet)]).ptD());   
	nvtx->push_back( ((*puJetId)[JetColl->refAt(ijet)]).nvtx());*/
      
      //beta2 definition:
      float pt_tot = 0.;
      float pt_vtx = 0.;
      constituents_type constituents = jet.getPFConstituents();
      for(constituents_iterator it=constituents.begin(); it!=constituents.end(); ++it) {
	reco::PFCandidatePtr & icand = *it;

	pt_tot += icand->pt() * icand->pt();

	if(  !(icand->trackRef().isNonnull()) || !(icand->trackRef().isAvailable()) ) continue;

	double dZ0 = icand->trackRef()->dz(firstGoodVertex->position());
	if ( fabs(dZ0) > 0.5 ) continue;
	pt_vtx += icand->pt() * icand->pt();
      }

      if (pt_tot > 0)
	beta2->push_back(pt_vtx/pt_tot);
      else
	beta2->push_back(0.);

    }
  }

  iEvent.put( selected,    Prefix + "Selected"       + Suffix );
  iEvent.put( uncertainty, Prefix + "JECuncertainty" + Suffix );
  for(unsigned int icorr = 0; icorr < corrections.size(); icorr++) 
    iEvent.put( corrections.at(icorr), Prefix + JetsCorrections.at(icorr) + "CorrFactor" + Suffix );

  /*iEvent.put( dZ, Prefix + "PileUpId" + "dZ"          + Suffix );
  iEvent.put(nCharged, Prefix + "PileUpId" + "nCharged"    + Suffix );
  iEvent.put(nNeutrals, Prefix + "PileUpId" + "nNeutrals"   + Suffix );
  iEvent.put(nParticles, Prefix + "PileUpId" + "nParticles"  + Suffix );
  iEvent.put(dR2Mean, Prefix + "PileUpId" + "dR2Mean" + Suffix );
  iEvent.put(dRMean, Prefix + "PileUpId" + "dRMean"  + Suffix );
  iEvent.put(frac01, Prefix + "PileUpId" + "frac01"  + Suffix );
  iEvent.put(frac02, Prefix + "PileUpId" + "frac02"  + Suffix );
  iEvent.put(frac03, Prefix + "PileUpId" + "frac03"  + Suffix );
  iEvent.put(frac04, Prefix + "PileUpId" + "frac04"  + Suffix );
  iEvent.put(frac05, Prefix + "PileUpId" + "frac05"  + Suffix );
  iEvent.put(frac06, Prefix + "PileUpId" + "frac06"  + Suffix );
  iEvent.put(frac07, Prefix + "PileUpId" + "frac07"  + Suffix );*/
  iEvent.put(beta,   Prefix + "PileUpId" + "beta"            + Suffix );
  iEvent.put(betaStar, Prefix + "PileUpId" + "betaStar"        + Suffix );
  iEvent.put(betaClassic, Prefix + "PileUpId" + "betaClassic"     + Suffix );
  iEvent.put(betaStarClassic, Prefix + "PileUpId" + "betaStarClassic" + Suffix );
  iEvent.put(beta2, Prefix + "PileUpId" + "beta2" + Suffix );
  /*iEvent.put(ptD, Prefix + "PileUpId" + "ptD"  + Suffix );
    iEvent.put(nvtx, Prefix + "PileUpId" + "nvtx" + Suffix );*/

  for(unsigned int ipuid = 0; ipuid < puidDiscriminators.size(); ipuid++) {
    iEvent.put( puidDiscriminators.at(ipuid), Prefix + "PileUpId" + PUJetIDdiscriminators.at(ipuid).instance() + Suffix );
    std::string tmp = PUJetIDdiscriminators.at(ipuid).instance(); tmp.erase(tmp.find("Discriminant"));
    iEvent.put( puidLoose.at(ipuid), Prefix + "PileUpId" + tmp + "Loose"  + Suffix );
    iEvent.put( puidMedium.at(ipuid), Prefix + "PileUpId" + tmp + "Medium"  + Suffix );
    iEvent.put( puidTight.at(ipuid), Prefix + "PileUpId" + tmp + "Tight"  + Suffix );
  }
}

void SusyDESY_Jet::beginJob(){}
