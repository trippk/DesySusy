#ifndef makeSystematics_h
#define makeSystematics_h
#include "JetCorrectionUncertainty.h"
//#include "Jet.h"

void rescaleJets(EasyChain*, vector<Jet*>&,Systematics& );
void rescaleMET(EasyChain*, vector<Jet*>&,Systematics&,string );
void rescaleHT(vector<Jet*>&,Systematics&, const string );
void rescaleClusters(vector<Jet*>&,Systematics&);
void JECUncertainty(Jet* , Systematics&   );
#endif
