#ifndef typedef_h
#define typedef_h

#include "NtupleTools2_h.h"
//#include "Electron.h"
//#include "Muon.h"
//#include "Jet.h"
//#include "genJet.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Jet;
class GenJet;
class Muon;
class Electron;

typedef boost::shared_ptr<LorentzM> Ptr_LorentzM;
typedef boost::shared_ptr<Jet> Ptr_Jet;
typedef boost::weak_ptr<Jet> WPtr_Jet;
typedef boost::shared_ptr<GenJet> Ptr_GenJet;
typedef boost::weak_ptr<GenJet> WPtr_GenJet;
typedef boost::shared_ptr<Muon> Ptr_Muon;
typedef boost::shared_ptr<Electron> Ptr_Electron;


#endif
