#ifndef nTupler_h
#define nTupler_h
#include <string>
#include "Math/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
typedef ROOT::Math::LorentzVector< ROOT::Math::PtEtaPhiM4D<float> > LorentzM;
void nTupler(const char *inputFile, std::string outname = "subTree.root");
#endif
