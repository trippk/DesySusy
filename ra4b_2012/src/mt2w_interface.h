#ifndef mt2w_interface_h
#define mt2w_interface_h

#include "NtupleTools2_h.h"
#include "Particle.h"
#include "Muon.h"
#include "Jet.h"
#include <vector>
#include "mt2w_bisect.h"

using namespace mt2w_bisect;
//typedef double momenta[2][4];

namespace mt2w_bisect{

  //typedef double momenta[2][4];

  struct momenta {
    double pb1[4];
    double pb2[4];
  };

  class mt2w_interface {
  
  protected:
    mt2w mt2w_event;

    double pl[4];     // El, plx, ply, plz,     (visible lepton)
    double pb1[4];    // Eb1, pb1x, pb1y, pb1z  (bottom on the same side as the visible lepton)
    double pb2[4];    // Eb2, pb2x, pb2y, pb2z  (other bottom, paired with the invisible W)
    double pmiss[3];  // <unused>, pmx, pmy     (missing pT)

    int nBjets(vector<Jet*>& jets);
    
    template <class T> void set_momentum(double* v, T* fourVector);
    //void set_momentum(double* v, Jet* fourVector);
    //void set_momenta(vector<Muon*>& lepton, Jet& jets, LorentzM& met);
    //void set_momenta(Muon& lepton, vector<Jet*>& jets, LorentzM& met);
    template <class T> void set_momenta(T* lepton, Jet* jet1, Jet* jet2, LorentzM& met);

  public:

    mt2w_interface();
    ~mt2w_interface(){};
    template <class T> double get_mt2w(vector<T*>& lepton, vector<Jet*>& jets, LorentzM& met);
    template <class T> double get_mt2w(T* lepton, vector<Jet*>& jets, LorentzM& met);
  };
}

template <class T>
double mt2w_bisect::mt2w_interface::get_mt2w(vector<T*>& lepton, vector<Jet*>& jets, LorentzM& met){
  if (lepton.size() == 0) return 0;
  return this->get_mt2w( lepton[0], jets, met);
}
template <class T>
double mt2w_bisect::mt2w_interface::get_mt2w(T* lepton, vector<Jet*>& jets, LorentzM& met){

  if (jets.size() < 3) return 0;

  this->set_momenta( lepton, jets[0], jets[1], met);

  int nbjets=0;
  std::vector<Jet*> bJets;
  std::vector<Jet*> noBjets;

  std::vector<momenta> momenta4mt2w;

  for(int ijet = 0; ijet<jets.size(); ijet++){
    if (jets.at(ijet)->IsBJet("CSV","Medium")) {
      //std::cout <<jets.at(ijet)->BJetDisc("CSV")<<std::endl;
      bJets.push_back(jets.at(ijet));
      nbjets++;
    }
    else if (noBjets.size()<3){
      noBjets.push_back(jets.at(ijet));
    }
  }

  if (nbjets == 0 || nbjets >= 3) {

    for (int i=0; i<3; i++) {
      for (int j=i+1; j<3; j++) {
	this->set_momentum(pb1, jets.at(i));
	this->set_momentum(pb2, jets.at(j));
	mt2w_bisect::momenta tmp;
	tmp.pb1[0]=pb1[0];
	tmp.pb1[1]=pb1[1];
	tmp.pb1[2]=pb1[2];
	tmp.pb1[3]=pb1[3];
	tmp.pb2[0]=pb2[0];
	tmp.pb2[1]=pb2[1];
	tmp.pb2[2]=pb2[2];
	tmp.pb2[3]=pb2[3];
	momenta4mt2w.push_back(tmp);
	tmp.pb1[0]=pb2[0];
	tmp.pb1[1]=pb2[1];
	tmp.pb1[2]=pb2[2];
	tmp.pb1[3]=pb2[3];
	tmp.pb2[0]=pb1[0];
	tmp.pb2[1]=pb1[1];
	tmp.pb2[2]=pb1[2];
	tmp.pb2[3]=pb1[3];
	momenta4mt2w.push_back(tmp);
      }
    }
  }
  
  if (nbjets == 1 ) {
    for (int i=0; i<2; i++) {
      if ( noBjets.at(i)->GetIndexInTree() == bJets.at(0)->GetIndexInTree()) continue;
      	this->set_momentum(pb1, bJets.at(0));
	this->set_momentum(pb2, noBjets.at(i));
	mt2w_bisect::momenta tmp;
	tmp.pb1[0]=pb1[0];
	tmp.pb1[1]=pb1[1];
	tmp.pb1[2]=pb1[2];
	tmp.pb1[3]=pb1[3];
	tmp.pb2[0]=pb2[0];
	tmp.pb2[1]=pb2[1];
	tmp.pb2[2]=pb2[2];
	tmp.pb2[3]=pb2[3];
	momenta4mt2w.push_back(tmp);
	tmp.pb1[0]=pb2[0];
	tmp.pb1[1]=pb2[1];
	tmp.pb1[2]=pb2[2];
	tmp.pb1[3]=pb2[3];
	tmp.pb2[0]=pb1[0];
	tmp.pb2[1]=pb1[1];
	tmp.pb2[2]=pb1[2];
	tmp.pb2[3]=pb1[3];
	momenta4mt2w.push_back(tmp);
    }  
  }
  
  if (nbjets == 2 ) {
    this->set_momentum(pb1, bJets.at(0));
    this->set_momentum(pb2, bJets.at(1));
    mt2w_bisect::momenta tmp;
	
    tmp.pb1[0]=pb1[0];
    tmp.pb1[1]=pb1[1];
    tmp.pb1[2]=pb1[2];
    tmp.pb1[3]=pb1[3];
    tmp.pb2[0]=pb2[0];
    tmp.pb2[1]=pb2[1];
    tmp.pb2[2]=pb2[2];
    tmp.pb2[3]=pb2[3];
    momenta4mt2w.push_back(tmp);
    tmp.pb1[0]=pb2[0];
    tmp.pb1[1]=pb2[1];
    tmp.pb1[2]=pb2[2];
    tmp.pb1[3]=pb2[3];
    tmp.pb2[0]=pb1[0];
    tmp.pb2[1]=pb1[1];
    tmp.pb2[2]=pb1[2];
    tmp.pb2[3]=pb1[3];
    momenta4mt2w.push_back(tmp); 
  }

  double minmt2w=99999.;

  for(int i=0; i<momenta4mt2w.size(); i++) {
    
    mt2w_event.set_momenta(pl,(momenta4mt2w.at(i)).pb1,(momenta4mt2w.at(i)).pb2,pmiss);
    
    //std::cout << " mt2w = " << mt2w_event.get_mt2w() << std::endl;
    

    if(minmt2w>mt2w_event.get_mt2w()) minmt2w=mt2w_event.get_mt2w();

  }

  //std::cout << "           min mt2w = " << (momenta4mt2w.at(0))[0][1] << std::endl;

  //if (nbjets==2) 
  //std::cout << "           min mt2w = " << minmt2w << std::endl;
  //if (nbjets==2)
  //std::cout << "           nbjets = " << nbjets << std::endl;
  return minmt2w;

}

template <class T>
void mt2w_bisect::mt2w_interface::set_momentum(double* v, T* fourVector){
   v[0]=fourVector->E();
   v[1]=fourVector->Px();
   v[2]=fourVector->Py();
   v[3]=fourVector->Pz();
}

template <class T>
void mt2w_bisect::mt2w_interface::set_momenta(T* lepton, Jet* jet1, Jet* jet2, LorentzM& met){

   this->set_momentum(pl, lepton);
   this->set_momentum(pb1, jet1);
   this->set_momentum(pb2, jet2);
   pmiss[0]=0.;
   pmiss[1]=met.Px();
   pmiss[2]=met.Py();
 }


#endif
