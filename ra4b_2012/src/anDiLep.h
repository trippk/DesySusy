#ifndef anDiLep_h
#define anDiLep_h

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <utility>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TMath.h"
#include "subTree.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "mt2w_interface.h"

#include "mTtools/MT2/mt2_bisect.h"

class anDiLep : public subTree {
 private:

   void AllocateMemory();///Allocate memory for internal members
   void DeallocateMemory();///Deallocate memory for internal members
   void SetToZero();///Initialises the internal members

   void SetBranchesRead();///Sets the branches for reading. A tree must be given to read in constructor
   void SetBranchesWrite();///Sets the branches for writing. Done if no tree is givn to constructor

   void getOsLeptonPair(const std::vector<LorentzM> * leptons, const std::vector<int> * leptonsQ, std::vector<LorentzM> & osPair);
   
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
   int nvtx;

   double mY; //Mass of produced sparticle, used in scans.
   double mLsp; //Mass of the LSP

   std::vector<LorentzM> * el; //el pt
   std::vector<int> * elQ; //el charge
   std::vector<LorentzM> * mu; //mu pt
   std::vector<int> * muQ; //mu charge

   std::vector<LorentzM> * jets; //jet pt
   std::vector<double> *  bjetdisc;
   int nbjets;
   std::vector<bool> * isbjet;

   LorentzM *   vMET_raw;
   LorentzM *   vMET_type1;
   
   //--------------------------

 public:

   anDiLep(TDirectory * dirIn = 0);
   anDiLep(TTree * treeToReadIn);
   ~anDiLep();

    //Adds information to the tree
   virtual void Fill(EventInfo* info, EasyChain* tree, std::vector<Muon*> & muons_in, std::vector<Electron*> & electrons_in, std::vector<Jet*> & jets_in, std::vector<GenJet*> & genjets_in, LorentzM& met_in) {
     Fill(info, tree, muons_in, electrons_in, jets_in, met_in, met_in);
   }
   //Adds information to the tree, including different met types
   void Fill(EventInfo* info, EasyChain* tree, std::vector<Muon*> & muons_in, std::vector<Electron*> & electrons_in, std::vector<Jet*> & jets_in, LorentzM& met_raw_in, LorentzM& met_type1_in); 

   virtual void Write(); //Writes the tree to the file

   long getEntries() {return nEntries;} //Returns the total number of entries in tree to be read
   bool Read(long getEntry = -1); //Reads in an entry from treeToRead. Will read in getEntry if greater than equal to zero. Returns true is read successful, false otherwise.
 
   void getOsMuPair(std::vector<LorentzM> & muPair); //Returns four vectors for an OS pair of muons (formed from the highest Pt OS pair). The first muon is negative, the second positive.
   void getOsElPair(std::vector<LorentzM> & elPair); //Returns four vectors for an OS pair of electrons (formed from the highest Pt OS pair). The first electron is negative, the second positive.
   void getOsSfPair(std::vector<LorentzM> & ossfPair); //Returns the OSSF pair with the highest sum Pt.

   double getHT(); //Returns HT
   //double getMET(); //Returns MET
   void getMETv(LorentzM & met, const std::string & metName); //Returns MET vector

   double getMT(const LorentzM & vis, const LorentzM & inv, double visMass, double invMass);

   void getJets(std::vector<LorentzM> & jetsOut);
   void getTaggedJets(std::vector<LorentzM> & jetsOut);
   void getUntaggedJets(std::vector<LorentzM> & jetsOut);

   void getMuons(std::vector<LorentzM> & muOut);
   void getElectrons(std::vector<LorentzM> & elOut);
   
   double getMT2W(const LorentzM & lepton, const std::vector<LorentzM> & taggedJets, const std::vector<LorentzM> & untaggedJets, const LorentzM & mpt);

   double getMT2(const LorentzM & lep1, const LorentzM & lep2, const LorentzM & mpt);

   double getWeight();
   int getNtags();

   int getNumVtx() {return nvtx;}

   double getMY() const {return mY;}
   double getMLSP() const {return mLsp;}
   bool isSusyScan() const {return (mY >= 0. && mLsp >= 0.);}
   bool isScanPoint(double mY_in, double mLsp_in) const;

   bool isData() {return run != 1;}
};


#endif
