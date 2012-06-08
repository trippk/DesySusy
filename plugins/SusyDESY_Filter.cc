// -*- C++ -*-
//
// Package:    SusyDESY_Filter
// Class:      SusyDESY_Filter
// 
/**\class SusyDESY_Filter SusyDESY_Filter.cc SUSYBSMAnalysis/DesySusy/src/SusyDESY_Filter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Hannes Schettler,,,uni-hamburg
//         Created:  Fri Mar 16 15:32:54 CET 2012
// $Id$
//
//


#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Filter.h" 


SusyDESY_Filter::SusyDESY_Filter(const edm::ParameterSet& iConfig):
  FilterResult    ( iConfig.getParameter<edm::InputTag> (    "FilterResult") )
{
}


SusyDESY_Filter::~SusyDESY_Filter()
{
}


// ------------ method called on each new Event  ------------
bool
SusyDESY_Filter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //using namespace edm;

   edm::Handle< int > filterResult;
   iEvent.getByLabel( FilterResult, filterResult);

   return bool(*filterResult);
}



