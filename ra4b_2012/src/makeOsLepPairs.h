#ifndef makeOsLepPairs_h
#define makeOsLepPairs_h

#include <vector>
#include <list>
#include <string>
#include <set>

#include "Muon.h"
#include "Electron.h"
#include "triggers_RA4b.h"

typedef std::pair<Particle*,Particle*> osPair;

void makeOsLepPairs(const std::set<diLepEvent_t>& firedDiLepTrigs, const std::vector<Muon*>& MuVec, const std::vector<Electron*> & ElVec, std::list<osPair> & osPairs);
void addOsPair(std::list<osPair> & osPairs, const std::list<Particle*> & nList, const std::list<Particle*> & pList);
bool compare_osPair_PtScalarSum(osPair p1, osPair p2) ;
#endif
