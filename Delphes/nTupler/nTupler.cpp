#include <iostream>
#include <utility>
#include <vector>

#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"

#include "TString.h"

#include "TH2.h"
#include "TH1F.h"
#include "TTree.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "classes/DelphesClasses.h"

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "ExRootAnalysis/ExRootResult.h"
#include "ExRootAnalysis/ExRootUtilities.h"

#include "nTuplerCode.h"

using namespace std;

//------------------------------------------------------------------------------

// Here you can put your analysis macro

#include "nTuplerCode.h"

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  char *appName = "nTupler";

  if(argc < 2)
  {
    cout << " Usage: " << appName << " input_file" << endl;
    cout << " input_file - input file in ROOT format ('Delphes' tree) - output filename (optionally)" << endl;
    return 1;
  }

  gROOT->SetBatch();

  int appargc = 1;
  char *appargv[] = {appName};
  TApplication app(appName, &appargc, appargv);

  TString inputFile(argv[1]);
  string outputFile = (string)argv[2];

//------------------------------------------------------------------------------

// Here you call your macro's main function 

  nTupler(inputFile,outputFile);

//------------------------------------------------------------------------------

}


