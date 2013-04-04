#ifndef makeSystematics_h
#define makeSystematics_h

#include "typedefs.h"

void ShiftJetEnergyScale(EasyChain*, Systematics&,vector<Ptr_Jet>&, vector<Muon*>&, vector<Electron*>&);
void ShiftClustersEnergyScale(EasyChain*, Systematics&,vector<Ptr_Jet>&,vector<Muon*>&, vector<Electron*>&);
void JetResolutionSmearing(EasyChain*, Systematics&,vector<Ptr_Jet>&,vector<Muon*>&, vector<Electron*>&);
void rescaleJets(EasyChain*, vector<Ptr_Jet>&,Systematics& );
void rescaleMET(EasyChain*, vector<Ptr_Jet>&,Systematics&,string );
void rescaleHT(vector<Ptr_Jet>&,Systematics&, const string );
void rescaleClusters(vector<Ptr_Jet>&,Systematics&); 
void JECUncertainty(Ptr_Jet , Systematics&, EasyChain*   );
void rescaleJetsJER(vector<Ptr_Jet>&, Systematics&);
double getJerSF(double,double);
void JetUncertaintyInitialize(Systematics&);

//double getJerSF(double,double);



#endif
