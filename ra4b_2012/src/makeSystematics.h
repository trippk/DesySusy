#ifndef makeSystematics_h
#define makeSystematics_h

#include "typedefs.h"

void ShiftJetEnergyScale(EasyChain*, Systematics&,vector<Ptr_Jet>&, vector<Muon*>&, vector<Electron*>&);
void ShiftClustersEnergyScale(EasyChain*, Systematics&,vector<Ptr_Jet>&,vector<Muon*>&, vector<Electron*>&);
void rescaleJets(EasyChain*, vector<Ptr_Jet>&,Systematics& );
void rescaleMET(EasyChain*, vector<Ptr_Jet>&,Systematics&,string );
void rescaleHT(vector<Ptr_Jet>&,Systematics&, const string );
void rescaleClusters(vector<Ptr_Jet>&,Systematics&); 
void JECUncertainty(Ptr_Jet , Systematics&, EasyChain*   );
void rescaleJetsJER(vector<Ptr_Jet>&, Systematics&);
double getJerSF(double,double);




/*
typedef boost::shared_ptr<Jet> Ptr_Jet;

template<typename __Jet__>
//void rescaleJets(EasyChain*, vector<Jet*>&,Systematics& );
void rescaleJets(EasyChain*, vector<__Jet__>&,Systematics& );
template void rescaleJets(EasyChain*, vector<Ptr_Jet>&,Systematics& );
template void rescaleJets(EasyChain*, vector<Jet*>&,Systematics& );

template<typename __Jet__>
void rescaleMET(EasyChain*, vector<__Jet__>&,Systematics&,string );
template void rescaleMET(EasyChain*, vector<Ptr_Jet>&,Systematics&,string );
template void rescaleMET(EasyChain*, vector<Jet*>&,Systematics&,string );
//void rescaleMET(EasyChain*, vector<Jet*>&,Systematics&,string );


template<typename __Jet__>
//void rescaleHT(vector<Jet*>&,Systematics&, const string );
void rescaleHT(vector<__Jet__>&,Systematics&, const string );
template void rescaleHT(vector<Jet*>&,Systematics&, const string );
template void rescaleHT(vector<Ptr_Jet>&,Systematics&, const string );

template<typename __Jet__>
//void rescaleClusters(vector<Jet*>&,Systematics&);
void rescaleClusters(vector<__Jet__>&,Systematics&);
template void rescaleClusters(vector<Jet*>&,Systematics&);
template void rescaleClusters(vector<Ptr_Jet>&,Systematics&);

template<typename __Jet__>
//void JECUncertainty(Jet* , Systematics&, EasyChain*   );
void JECUncertainty(__Jet__ , Systematics&, EasyChain*   );
template void JECUncertainty(Jet* , Systematics&, EasyChain*   );
template void JECUncertainty(Ptr_Jet , Systematics&, EasyChain*   );

template<typename __Jet__>
void rescaleJetsJER(vector<__Jet__>&, Systematics&);
template void rescaleJetsJER(vector<Ptr_Jet>&, Systematics&);
template void rescaleJetsJER(vector<Jet*>&,    Systematics&);

double getJerSF(double,double);

*/

#endif
