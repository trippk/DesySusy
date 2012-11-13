//A simple test program

#include "TTree.h"

#include "subTreeFactory.h"
#include "anDiLep.h"
#include <iostream>

using std::cout;
using std::endl;

bool pcp = false;
//double EventWeight = 1.0;
//bool checkthisevent = false;

void testWrite();
void testRead();

int main() {

  testWrite();
  testRead();

  return 0;
}

void testWrite() {

  cout << "Running write test" << endl;

  TFile f("testFile.root", "RECREATE");

  //Create some fake information, info, muons, electrons and jets.
  EventInfo info;
  std::vector<Muon*> muons;
  Muon mu1;
  Muon mu2;
  muons.push_back(&mu1);
  muons.push_back(&mu2);

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

    info.EventWeight = (float) i;

    LorentzM vec;
    vec.SetPxPyPzE((double) i,(double) i,(double) i,sqrt(3.)*(double) i);

    muons[0]->Set(-1, &vec, 1, 0.0);
    jets[0]->Set(-1, &vec);
    met.SetPxPyPzE((double) i,(double) i,(double) i,(double) i);

    vec.SetPxPyPzE((double) -i,(double) -i,(double) -i,sqrt(3.)*(double) i);
    muons[1]->Set(-1, &vec, 1, 0.0);

    analyser.Fill(&info, 0, muons, electrons, jets, met);
  }

  f.Write();
  f.Close();

  cout << "Test complete" << endl;

  return;
}
  
void testRead() {

  cout << "Running read test" << endl;

  TFile f("testFile.root", "READ");

  //Get the tree
  TTree * tree = (TTree*) f.Get("subTree");

  if (!tree) {
    cout << "Could not find the tree!" << endl;
    f.Close();
    return;
  }

  double mStop = 0.;
  tree->SetBranchAddress("mStop", &mStop);
  
  int run = -1;
  tree->SetBranchAddress("Run", &run);

  tree->GetEntry(1);

  cout << "I'm here and mStop is: " << mStop << endl;
  cout << "run is: " << run << endl;

  f.Close();

  cout << "Test complete" << endl;

  return;
}
