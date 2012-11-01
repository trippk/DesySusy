int testReadIn() {

  //Load shared library
  int chk = gSystem->Load("libRA4b.so");
  if (chk != 0) {
    cout << "Problems loading shared lib"<< endl;
    return 1;
  }
  else cout << "Shared lib loaded"<< endl;

  //Load file with tree
  TFile f("testFile.root", "READ");
  if (f.IsZombie()) {
    cout << "Could not find file testFile.root" << endl;
    f.Close();
    return 1;
  }
  TTree * tree = (TTree*) f.Get("subTree");
  if (!tree) {
    cout << "Could not load subTree!" << endl;
    f.Close();
    return 1;
  }

  cout << "Creating anDiLep instance..."<< endl;
  anDiLep test(tree);
  cout << "...done" << endl;

  cout << "Found: " << test.getEntries() << " entries in the tree!" << endl;
  cout << "Looping over entries..." << endl;
  int iEntry = 0;
  while (test.Read()) {
    iEntry++;
    cout << "Got entry: " << iEntry << endl;

    const vector<LorentzM> * jets = test.getJets();
    if (jets) {
      cout << "Found " << jets->size() << " jets" << endl;
      for (int iJet = 0 ; iJet < jets->size() ; iJet++) {
	cout << "Jet: " << iJet << " Px: " << jets->at(iJet).Px()  << " Py: " << jets->at(iJet).Py()  << " Pz: " << jets->at(iJet).Pz()  << " E: " << jets->at(iJet).E() << endl;
      }
    }
    cout << "HT: " << test.getHT() << endl;
    cout << "MET: " << test.getMET() << endl;

    std::vector<LorentzM> ossfPair;
    test.getOsSfPair(ossfPair);
    
    TLorentzVector pll;
    if (ossfPair.size() == 2) {
      TLorentzVector temp(ossfPair[0].X(), ossfPair[0].Y(), ossfPair[0].Z(), ossfPair[0].T());
      pll  = temp;
      temp.SetXYZT(ossfPair[1].X(), ossfPair[1].Y(), ossfPair[1].Z(), ossfPair[1].T());
      pll += temp;
    }
    cout << "Mll: " << pll.M() << endl;


  }

  return 0;
}
