#include "makeOsLepPairs.h"

void makeOsLepPairs(const std::set<diLepEvent_t>& firedDiLepTrigs, const std::vector<Muon*>& MuVec, const std::vector<Electron*> & ElVec, std::list<osPair> & osPairs) {

  osPairs.clear();

  //Create lists of positive and negative leptons, for each flavour
  std::list<Particle*> pMuList;
  std::list<Particle*> nMuList;
  std::list<Particle*> pElList;
  std::list<Particle*> nElList;
  //Populate the lists
  for (int iMu = 0; iMu < MuVec.size() ; iMu++ ) {
    if (MuVec.at(iMu)->Charge() > 0) pMuList.push_back( (Particle*) MuVec.at(iMu) );
    else nMuList.push_back( (Particle*) MuVec.at(iMu) );
  }
  for (int iEle = 0; iEle < ElVec.size() ; iEle++ ) {
    if (ElVec.at(iEle)->Charge() > 0) pElList.push_back( (Particle*) ElVec.at(iEle) );
    else nElList.push_back( (Particle*) ElVec.at(iEle) );
  }
  
  //Order each list by Pt. First entry in list has highest Pt.
  pMuList.sort(compare_Particle_Pt);
  nMuList.sort(compare_Particle_Pt);
  pElList.sort(compare_Particle_Pt);
  nElList.sort(compare_Particle_Pt);

  //For each fired-trigger type, diMu, diEl, MuEG, add the corresponding lepton Pt pair.
  for (std::set<diLepEvent_t>::const_iterator iTrig = firedDiLepTrigs.begin(); iTrig != firedDiLepTrigs.end() ; iTrig++ ) {
    
    switch(*iTrig) {
    case MUMU:
      addOsPair(osPairs, nMuList, pMuList);
      break;
    case ELEL:
      addOsPair(osPairs, nElList, pElList);
      break;
    case MUEL:
      addOsPair(osPairs, nElList, pMuList);
      addOsPair(osPairs, nMuList, pElList);
      break;
    default:
      cout << "ERROR!" << endl;
    }

  }

  //Sort the list of osPairs, so that the first entry contains the highest Pt pair
  osPairs.sort(compare_osPair_PtScalarSum);

  return;
}

void addOsPair(std::list<osPair> & osPairs, const std::list<Particle*> &nList, const std::list<Particle*> &pList) {
  if (nList.size() > 0 && pList.size() > 0) {
    osPairs.push_back( make_pair( nList.front(), pList.front() ) );
  }
  return;
}

bool compare_osPair_PtScalarSum(osPair p1, osPair p2) {

  double pT1 = p1.first->Pt() + p1.second->Pt();
  double pT2 = p2.first->Pt() + p2.second->Pt();

  if ( pT1 > pT2 ) return true;
  else false;
}

