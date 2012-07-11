//this class represents a  seleceted electron


//#include "Math/VectorUtil.h"
//#include "NtupleTools2.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include "selectedlepton.h"
#include "eventselection.h"
using namespace std;
using namespace ROOT::Math::VectorUtil;


LeptonCuts SetOfCuts::Leptons;
ElectronCuts SetOfCuts::TightElectrons;
ElectronCuts SetOfCuts::LooseElectrons;
ElectronCuts SetOfCuts::VetoElectrons;
ElectronCuts SetOfCuts::SignalElectrons;
ElectronCuts SetOfCuts::WideElectrons;
MuonCuts SetOfCuts::LooseMuons;
MuonCuts SetOfCuts::TightMuons;
MuonCuts SetOfCuts::VetoMuons;
MuonCuts SetOfCuts::SoftMuons;
MuonCuts SetOfCuts::SignalMuons;
MuonCuts SetOfCuts::WideMuons;
EventCuts SetOfCuts::Event;
JetCuts SetOfCuts::Jets;
LeptonPairCuts SetOfCuts::LeptonPairs;
