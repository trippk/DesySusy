#ifndef anDiLep_h
#define anDiLep_h

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TMath.h"
#include "subTree.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "mt2w_interface.h"


class anDiLep : public subTree {
 private:

   void AllocateMemory();///Allocate memory for internal members
   void DeallocateMemory();///Deallocate memory for internal members
   void SetToZero();///Initialises the internal members

   void SetBranchesRead();///Sets the branches for reading. A tree must be given to read in constructor
   void SetBranchesWrite();///Sets the branches for writing. Done if no tree is givn to constructor

   void getOsLeptonPair(const std::vector<LorentzM> * leptons, const std::vector<int> * leptonsQ, std::vector<LorentzM> & osPair);
   
   mt2w_bisect::mt2w_interface mt2w_calc;

   TDirectory * dir; //dir in which tree and hists are created

   TTree * treeToRead;
   TTree * treeToWrite;

   long nEntries; //Total number of entries in treeToRead
   long iEntry; //The next entry of treeToRead that will be read in ::Read
   
   /////////////////
   //Tree branches
   ////////////////

   int event;
   int run;
   double weight;
   double PUWeight;

   std::vector<LorentzM> * el; //el pt
   std::vector<int> * elQ; //el charge
   std::vector<LorentzM> * mu; //mu pt
   std::vector<int> * muQ; //mu charge

   std::vector<LorentzM> * jets; //jet pt
   std::vector<double> *  bjetdisc;
   int nbjets;
   std::vector<bool> * isbjet;

   LorentzM *   vMET;
   
   //--------------------------


   /////////////////
   //Histograms
   /////////////////

   TH1D * h_Mll; //Stores the invariant mass of OSSF lepton pair

   //--------------------------

 public:

   anDiLep(TDirectory * dirIn = 0);
   anDiLep(TTree * treeToReadIn);
   ~anDiLep();

   virtual void Fill(EventInfo* info, EasyChain* tree, std::vector<Muon*> & muons_in, std::vector<Electron*> & electrons_in, std::vector<Jet*> & jets_in, LorentzM& met_in); //Adds information to the tree
   virtual void Write(); //Writes the tree to the file

   long getEntries() {return nEntries;} //Returns the total number of entries in tree to be read
   bool Read(long getEntry = -1); //Reads in an entry from treeToRead. Will read in getEntry if greater than equal to zero. Returns true is read successful, false otherwise.
 
   void getOsMuPair(std::vector<LorentzM> & muPair); //Returns four vectors for an OS pair of muons (formed from the highest Pt OS pair). The first muon is negative, the second positive.
   void getOsElPair(std::vector<LorentzM> & elPair); //Returns four vectors for an OS pair of electrons (formed from the highest Pt OS pair). The first electron is negative, the second positive.
   void getOsSfPair(std::vector<LorentzM> & ossfPair); //Returns the OSSF pair with the highest sum Pt.

   double getHT(); //Returns HT
   double getMET(); //Returns MET
   void getMETv(LorentzM & met); //Returns MET vector

   double getMT(const LorentzM & vis, const LorentzM & inv, double visMass, double invMass);

   void getJets(std::vector<LorentzM> & jetsOut);

   double getWeight();
   int getNtags();
};


#endif
