#include "mt2w_bisect.h"
#include "mt2w_interface.h"

mt2w_bisect::mt2w_interface::mt2w_interface(){
  
  //pl[4] = {123.92, 33.5126, 28.4367, 115.864};
  //pb1[4] = {633.838, -4.35958, -18.0347, 633.549};
  //pb2[4] = {53.8194, 46.0444, -0.864168, 27.4517};
  //pmiss[3] = { 0., -75.1975, -9.53793};
  
  //mt2w_event.set_momenta(pl,pb1,pb2,pmiss);
      
  //cout << " mt2w = " << mt2w_event.get_mt2w() << endl;
}

/*
double mt2w_bisect::mt2w_interface::get_mt2w(Muon* lepton, vector<Jet*>& jets, LorentzM& met){

  this->set_momenta( lepton, jets[0], jets[1], met);
  
  mt2w_event.set_momenta(pl,pb1,pb2,pmiss);
  
  std::cout << " mt2w = " << mt2w_event.get_mt2w() << std::endl;
  return mt2w_event.get_mt2w();
}
*/

int mt2w_bisect::mt2w_interface::nBjets(vector<Jet*>& jets){
  int nbjets=0;
  for(int ijet = 0; ijet<jets.size(); ijet++){
    if (jets.at(ijet)->IsBJet("CSV","Medium")) nbjets++;
  }
  return nbjets;
}
/*
void mt2w_bisect::mt2w_interface::set_momenta(Muon* lepton, Jet* jet1, Jet* jet2, LorentzM& met){

   this->set_momentum(pl, lepton);
   this->set_momentum(pb1, jet1);
   this->set_momentum(pb2, jet2);
   pmiss[0]=0.;
   pmiss[1]=met.Px();
   pmiss[2]=met.Py();
 }

/*
void mt2w_bisect::mt2w_interface::set_momenta(vector<Muon*>& lepton, vector<Jet*>& jets, LorentzM& met){
   this->set_momenta( lepton[0], jets, met);
 }

 void mt2w_bisect::mt2w_interface::set_momenta(Muon& lepton, vector<Jet*>& jets, LorentzM& met){
   this->set_momuntum(pl, lepton);
   this->set_momuntum(pb1, jets[0]);
   this->set_momuntum(pb2, jets[1]);
   pmiss[0]=0.;
   pmiss[1]=met.Px();
   pmiss[2]=met.Py();
 }


void mt2w_bisect::mt2w_interface::set_momentum(double* v, Muon* fourVector){
   v[0]=fourVector->E();
   v[1]=fourVector->Px();
   v[2]=fourVector->Py();
   v[3]=fourVector->Pz();
}

void mt2w_bisect::mt2w_interface::set_momentum(double* v, Jet* fourVector){
   v[0]=fourVector->E();
   v[1]=fourVector->Px();
   v[2]=fourVector->Py();
   v[3]=fourVector->Pz();
}

  /*

double mt2w( vector<Muon*>& lepton, vector<Jet*>& jets, LorentzM& met) {

  mt2w_bisect::mt2w mt2w_event;

  



  std::cout<<"Let's see if it works"<<std::endl;
  return 2.4;

}
  */
