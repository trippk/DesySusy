#ifndef anDiLep_h
#define anDiLep_h

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "TFile.h"
#include "TTree.h"
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
   
   mt2w_bisect::mt2w_interface mt2w_calc;

   TTree * treeToRead;
   TTree * treeToWrite;

   long nEntries; //Total number of entries in treeToRead
   long iEntry; //The next entry of treeToRead that will be read in ::Read
   
   /////////////////
   //Tree branches
   ////////////////

   int * event;
   int * run;
   double *  weight;
   double * PUWeight;

   std::vector<LorentzM> * el;
   std::vector<double> * elPt;
   std::vector<LorentzM> * mu;
   std::vector<double> * muPt;

   int * njets;
   std::vector<LorentzM> * jets;
   std::vector<double> *  jetsPt;
   std::vector<double> *  bjetdisc;
   int *  nbjets;
   std::vector<bool> * isbjet;

   LorentzM *   vMET;
   LorentzM *   vMHT;

   double   *   MET; 
   double   *   MHT;
   double   *   HT;
 
   double   *   elSig;
   double   *   muSig;
   double   *   METSig;
   double   *   MHTSig;

   double   *   mtEl;
   double   *   mtMu;

   double * mt2wEl;
   double * mt2wMu;
   
   //--------------------------

 public:

   anDiLep();
   anDiLep(TTree * treeToReadIn);
   ~anDiLep();

   virtual void Fill(EventInfo* info, EasyChain* tree, std::vector<Muon*> & muons_in, std::vector<Electron*> & electrons_in, std::vector<Jet*> & jets_in, LorentzM& met_in); //Adds information to the tree
   virtual void Write(); //Writes the tree to the file

   long getEntries() {return nEntries;} //Returns the total number of entries in tree to be read
   bool Read(long getEntry = -1); //Reads in an entry from treeToRead. Will read in getEntry if greater than equal to zero. Returns true is read successful, false otherwise.
 
   const std::vector<LorentzM> * getJets() {return jets;}

};


#endif
