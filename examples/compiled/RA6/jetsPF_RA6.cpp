#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"

using namespace std;


bool jetsPF_RA6(EasyChain* tree, vector<unsigned>& selJet, CutSet& selCut) {

  ConfigReader config;

  static TString JetCollection = config.getTString( "JetCollection", "ak5JetPF" );//ak5JetPF2PAT
  static float jet_pt_min      = config.getFloat("jet_pt_min", 40);
  static float jet_eta_max     = config.getDouble("jet_eta_max",   3.);
  static bool  quick           = config.getBool("quick",false);

  TString JetP4name=JetCollection;
  JetP4name += "CorrectedP4Pat";
  vector<LorentzM>& JetsPF    = tree->Get( &JetsPF   , JetP4name );

  float HTPF = 0; 
  for( unsigned jet=0; jet<JetsPF.size(); ++jet ) {
    TString ptCut = "pt>";
    ptCut += jet_pt_min;
    if( !selCut.keepIf( ptCut          , JetsPF.at(jet).pt()         >  jet_pt_min     ) && quick ) continue;
    TString etaCut = "|eta|<";
    etaCut += jet_eta_max;
    if( !selCut.keepIf( etaCut         , fabs(JetsPF.at(jet).eta())  <  jet_eta_max    ) && quick ) continue;

    TString JetIDname=JetCollection;
    JetIDname += "PFJetIDloosePat";
    vector<int>&      Jets_IDloosePF            = tree->Get( &Jets_IDloosePF,  JetIDname );
    if( !selCut.keepIf( "looseID"      , Jets_IDloosePF.at(jet)                        ) && quick ) continue;


    if( quick || selCut.applyCuts(ptCut+" "+etaCut+" looseID" ) )
      selJet.push_back(jet);

  }// jet loop


}//main

