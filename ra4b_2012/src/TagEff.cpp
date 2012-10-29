#include <functional>
#include <algorithm>
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <TString.h>
#include "AnalysisObject.h"
#include "TagEff.h"
#include "Jet.h"
#include "TH2D.h"
#include <csignal>


using namespace std;
using namespace ROOT::Math::VectorUtil;

void TagEff::filltagmatch(double Pt, double Eta)
{
  if(matchtype=="b-matched")
    btag->Fill(fabs(Eta),Pt);
  else if (matchtype == "c-matched")
    ctag->Fill(fabs(Eta),Pt);
  else ltag->Fill(fabs(Eta),Pt);
}

void TagEff::fillmatch(double Pt, double Eta)
{
  if(matchtype=="b-matched")
    bmatched->Fill(fabs(Eta),Pt);
  
  else if (matchtype == "c-matched") 
    cmatched->Fill(fabs(Eta),Pt);
  
  else lmatched->Fill(fabs(Eta),Pt);  
}

void TagEff::finalize(){
   if (!status) 
  {
      //      ftageff = TFile::Open("btageff.root","RECREATE");
      btag->Write();
      ctag->Write();
      ltag->Write();
      bmatched->Write();
      cmatched->Write();
      lmatched->Write();
      }
}

int TagEff::JetLoop(vector<Jet*>& pureJets)
{
  
  //Filling the efficiencies, if they are already read (case status==1) apply Scaling factors
  int nbjet = 0;
  if(status) ApplySF(pureJets);
  else 
    {
      for(int njet = 0; njet < pureJets.size(); njet++){
	tagflavor(pureJets.at(njet)->GenFlavor());
	fillmatch(pureJets.at(njet)->pt(),pureJets.at(njet)->eta()); //*****DONT FORGET TO CHANGE the input ord. of these funcs******************!!!!
	if(pureJets.at(njet)->IsBJet("CSV","Medium")) //
	  {
	    nbjet++;
	    filltagmatch(pureJets.at(njet)->pt(),pureJets.at(njet)->eta());
	  }
      }
    }
  return nbjet;
}
//Apply scaling factors given by BTagPOG 
void TagEff::ApplySF(vector<Jet*>& pureJets)
{
  //  if(!status)  { cout<<" ***efficiency info not found** "<< endl;  raise (SIGABRT);}
  getratio();
  int binx=0;
  int biny=0;

  double sf=0.;
  double Pt=0.;
  vector<double> correctedEff;
   for(int njet = 0; njet < pureJets.size(); njet++)    
    {
      tagflavor(pureJets.at(njet)->GenFlavor());
      Pt=pureJets.at(njet)->pt();
      binx=(int)(fabs(pureJets.at(njet)->eta())*1.25)+1;//get the eta bin corresponds to this event
      biny=getptbin(Pt);//get the pt bin
      if("light-matched"==matchtype) sf= ((1.04318+(0.000848162*Pt))+(-2.5795e-06*(Pt*Pt)))+(1.64156e-09*(Pt*(Pt*Pt)));//CSVM SF->I will write a method for this one as well -> "CVSM""light || c&b"
      else sf=0.6981*((1.+(0.414063*Pt))/(1.+(0.300155*Pt))); 
      if("b-matched"==matchtype)  correctedEff.push_back((btag3->GetBinContent(binx,biny))*sf);
      else if("c-matched"==matchtype)  correctedEff.push_back((ctag3->GetBinContent(binx,biny))*sf);
      else correctedEff.push_back((ltag3->GetBinContent(binx,biny))*sf);
    }
  // cout << "correctedEff   " ; printvec(correctedEff)
   comb(correctedEff);//apply combinatorics
}
//to see the combinations, comment in prinvec(..) s and cout s
void TagEff::comb(vector<double > & eff){
  vector<double>   ones;// like 1,1,1,1,1
  vector<double>   result;//result of subtraction for example result = (1,1,1,1)-(e1,e2,e3,e4)
  vector<double> weightIns;
  weightIns.assign(lasttagbin+1,0.);//init. the vector to carry weights for the event
  //cout << "check ini " ; printvec (weightIns);
  
  for(int numbjets=0 ; numbjets < lasttagbin  &&  eff.size()>0 ; numbjets ++)
    {
      if(0 == numbjets)
	{ 
	 
	  ones.assign(eff.size(),1.);
	  transform(ones.begin(),ones.end(),eff.begin(),back_inserter(result),std::minus<double>()); 
	  //  cout << numbjets;    
	  weightIns.at(numbjets)=weightIns.at(numbjets)+multIns(result); //sum up the weights 
	  // printvec(ones);
	  //  cout <<" result 0 " ; printvec(result);
	  continue;
	}       
      
      for(int bn=0;bn < eff.size() ; bn++){
	//here create vector of 1s
	ones.assign(eff.size(),1.);
	ones.at(bn)=0; 
	if(1 == numbjets) 
	  { 
	    result.clear();
	    // cout << numbjets; 
	    // printvec(ones); 
	    transform(ones.begin(),ones.end(),eff.begin(),back_inserter(result),std::minus<double>()); 
	    weightIns.at(numbjets)=weightIns.at(numbjets)+multIns(result);   
	    // cout <<" result  1" ; printvec(result);
	    continue;
	  }
	
	if(2 == numbjets) 
	  for(int b = bn+1; b < eff.size(); b++ && (ones.at(b-1)=1))
	    {
	      result.clear();
	      ones.at(b)=0; 
	      //cout << numbjets; 
	      // printvec(ones); 
	      transform(ones.begin(),ones.end(),eff.begin(),back_inserter(result),std::minus<double>());
	      weightIns.at(numbjets)=weightIns.at(numbjets)+multIns(result);
	      //cout <<" result  2"; printvec(result);
	    }
      }
    }
  weightIns.at(lasttagbin)=1.;
  for(int indx=0; indx<lasttagbin; indx++)
    weightIns.at(lasttagbin)-=weightIns.at(indx);
  //  cout << "before (weight)"; printvec(weight);
  // cout << "add this (weightins)"; printvec(weightIns);
  transform(weight.begin(),weight.end(),weightIns.begin(),weight.begin(),plus<double>()); //sum with the other weights 
  // cout << "and result is" ; printvec(weight);
}

double TagEff::multIns ( vector<double > & instance)
{
  double mult=instance.at(0);
  for(int i=1; i<instance.size(); i++){
    mult*=instance.at(i);
  }
  return fabs(mult);
}

vector<double>& TagEff::getWeight()
{
  return weight;
}

void TagEff::printvec( vector<double > & result)
{
  for(int i=0; i <result.size(); i++)
    {cout << "   "  <<  result.at(i) << "  " ;}
  cout<<endl;
}

int TagEff::getptbin(double Pt)
{
  int i;
  
  double ptbin[]={40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670, 840};
  for(i=0; i< sizeof(ptbin)/sizeof(ptbin[0]); i++)
    {
      if(Pt<ptbin[i]) break;
    } 
  return i;
}

void TagEff::getratio()
{
  btag3=(TH2D *)btag->Clone("btag3");
  btag3->SetTitle("matched & tagged jets/ matched jets");
  btag3->Divide(bmatched);
  ctag3=(TH2D *)ctag->Clone("ctag3");
  ctag3->SetTitle("matched & tagged jets/ matched jets");
  ctag3->Divide(cmatched);
  ltag3=(TH2D *)ltag->Clone("ltag3");
  ltag3->SetTitle("matched & tagged jets/ matched jets");
  ltag3->Divide(lmatched);
}

void TagEff::tagflavor (int flavor) //current Jet class returns string however I modified it to return int
{
  if(5 == abs(flavor)) matchtype = "b-matched";
  else if(3 == abs(flavor)) matchtype = "c-matched";
  else matchtype = "light-matched";
}

