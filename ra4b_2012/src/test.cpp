//A simple test program

#include "anDiLep.h"
#include <iostream>

using std::cout;
using std::endl;

bool pcp = false;
//double EventWeight = 1.0;
//bool checkthisevent = false;

int main() {

  cout << "Running test" << endl;

  TFile f("testFile.root", "RECREATE");

  //Create some fake information, info, muons, electrons and jets.
  EventInfo info;
  std::vector<Muon*> muons;
  Muon mu;
  muons.push_back(&mu);

  std::vector<Electron*> electrons;

  std::vector<Jet*> jets;
  cout << "Creating first jet" << endl;
  Jet jet1;
  cout << "Setting Btag disc" << endl;
  jet1.SetBJetDisc("CSV", 0.01);
  cout << "...done" << endl;
  jets.push_back(&jet1);

  LorentzM met;

  //Create the new analyser
  anDiLep analyser;
  
  for (int i = 0; i < 10; i++) {
    cout << "Fill: " << i << endl;

    LorentzM vec;
    vec.SetPxPyPzE((double) i,(double) i,(double) i,(double) i);

    muons[0]->Set(-1, &vec, 1, 0.0);
    jets[0]->Set(-1, &vec);
    met.SetPxPyPzE((double) i,(double) i,(double) i,(double) i);

    analyser.Fill(&info, 0, muons, electrons, jets, met);
  }

  f.Write();
  f.Close();

  cout << "Test complete" << endl;

  return 0;
}


  
