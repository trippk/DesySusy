#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"

using namespace std;


bool electrons_RA6(EasyChain* tree, vector<unsigned>& selEl, CutSet& selCut) {

  //cout<<"electrons"<<endl;

  ConfigReader config;

  static float el_pt_min_low   = config.getFloat("el_pt_min_low",  10);
  //static float el_pt_min_high  = config.getFloat("el_pt_min_high", 20);
  static float el_eta_max      = config.getDouble("el_eta_max",   2.5);
  static bool  quick           = config.getBool("quick"      ,false);


  vector<LorentzM>& Electrons = tree->Get(&Electrons, "electronP4Pat");

  for( unsigned el=0; el<Electrons.size(); ++el ) {
    TString etaCut ="|eta|<";
    etaCut+=el_eta_max;
    if( !selCut.keepIf(etaCut   , fabs(Electrons.at(el).eta()) < el_eta_max    ) && quick ) continue;
    TString ptCut ="pt>";
    ptCut+=el_pt_min_low;
    if( !selCut.keepIf(ptCut    , Electrons.at(el).pt()        > el_pt_min_low ) && quick ) continue;

    vector<int>&    idLoose        = tree->Get( &idLoose, "electronIdLoosePat" );
    if( !selCut.keepIf("IDloose", idLoose.at(el)                               ) && quick ) continue;

    if( quick || selCut.applyCuts("RA6 electron selection", ptCut+" "+etaCut+" IDloose" ))
      selEl.push_back(el);

  }// electron loop

  //cout<<"el end"<<endl;
}//main
