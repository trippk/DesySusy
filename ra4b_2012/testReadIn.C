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
  }

  return 0;
}
