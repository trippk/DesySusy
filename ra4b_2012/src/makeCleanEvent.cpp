#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "makeCleanEvent.h"
#include "CutFlow.h" 
#include "evtqual_RA4b.h"
#include "cschalo_RA4b.h"
#include "trackingFailure_RA4b.h"
using namespace std;





bool makeCleanEvent(EasyChain* tree, CutSet* globalFlow){


  extern bool pcp;

  ConfigReader config;
  bool quick=config.getBool("quick_CleanEvents","true");
  bool OK=false;
    
  //====================================================================
  bool  hcalLaserEventFilterFlag   = tree->Get( hcalLaserEventFilterFlag,    "hcalLaserEventFilterFlag"  );
  OK= hcalLaserEventFilterFlag;
  if( !globalFlow->keepIf( "hcalLaserFilter"        , OK     ) && quick ) return false;;    
  //====================================================================


  //====================================================================
  bool eeBadSCPassed = tree->Get( eeBadSCPassed, "eeBadScFilterFlag"   );
  OK=eeBadSCPassed;
  if( !globalFlow->keepIf("eeBadSCFilter"     , OK) && quick ) return false;;
  //====================================================================



  //====================================================================
  // SCRAPING VETO
  //====================================================================
  unsigned int nTracksAll=tree->Get(nTracksAll,"nTracksAll");
  unsigned int nTracksHighPurity=tree->Get(nTracksHighPurity,"nTracksHighPurity");

  OK=true;
  if(pcp)cout<<"check point trackpurity"<<endl;
  if(nTracksAll>10){
    OK=false;
    if(double(nTracksHighPurity)/nTracksAll > 0.25)OK=true;
  }

  if(  !globalFlow->keepIf("Scraping_Veto", OK ) && quick ) return false;;
  if(pcp)cout<<"pure tracks passed"<<endl;
  //====================================================================    



  //====================================================================    
  // check event quality
  //====================================================================    
  //it only checks hbheNoiseFilterResult
  OK = evtqual_RA4b(tree);
  if(pcp)cout<<"check point calling event quality"<<endl;
  if( !globalFlow->keepIf("HBHE", OK)          && quick ) return false;;
  if(pcp)cout<<"noise passed"<<endl;
  //====================================================================





  //====================================================================    
  // check CSCHALO
  //====================================================================    
  //it only checks hbheNoiseFilterResult
  OK = cschalo_RA4b(tree);
  if( !globalFlow->keepIf("CSC_HALO", OK)          && quick ) return false;;
  //====================================================================




  //====================================================================    
  // check trackingFailure
  //====================================================================    
  //it only checks hbheNoiseFilterResult
  OK = trackingFailure_RA4b(tree);
  if( !globalFlow->keepIf("trackingFailure", OK)          && quick ) return false;;
  //====================================================================




  //====================================================================    
  // check ECAL_TP
  //====================================================================    
  //as of CMSSW427_patch7, RA2TPfilter_cff.py and EcalDeadCellEventFilter_cfi do the 
  //same.
  bool ECAL_TP = tree->Get(ECAL_TP,"ecalDeadCellTPFilterFlag");
  OK=ECAL_TP;
  if( !globalFlow->keepIf("ECAL_TP", OK)          && quick ) return false;;
  //====================================================================
 
  return true;

}
