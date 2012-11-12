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
    {
      btag->Fill(fabs(Eta),Pt); 
      bpttag->Fill(Pt);
    }
  else if (matchtype == "c-matched")
    {
      ctag->Fill(fabs(Eta),Pt); 
      cpttag->Fill(Pt);
    }
  else 
    {
      ltag->Fill(fabs(Eta),Pt); 
      lpttag->Fill(Pt);
    }
}

void TagEff::fillmatch(double Pt, double Eta)
{
  if(matchtype=="b-matched")
    {
      bmatched->Fill( fabs(Eta) , Pt );
      bptmatched->Fill(Pt);
    }
  
  else if (matchtype == "c-matched") 
    {
      cmatched->Fill(fabs(Eta),Pt);
      cptmatched->Fill(Pt);
    }
  else 
    {
      lmatched->Fill(fabs(Eta),Pt);
      lptmatched->Fill(Pt);
    }
}

void TagEff::finalize(){
   if (!status) 
     {
       btag->Write();
       ctag->Write();
       ltag->Write();
       bmatched->Write();
       cmatched->Write();
       lmatched->Write();
       bpttag->Write();
       cpttag->Write();
       lpttag->Write(); 
       bptmatched->Write();
       cptmatched->Write();
       lptmatched->Write();
     }
   else
     {
       vectoTH1(weight, tagW);
       tagW->Write();
       //       beff->Write();
       //       ceff->Write();
       //       leff->Write();
       h0tag->Write();
       h1tag->Write();
       h2tag->Write();
       h3mtag->Write();
     }
}

int TagEff::JetLoop(vector<Jet*>& pureJets)
{
  
  //Filling the efficiencies, if they are already read (case status==1) apply Scaling factors
  int nbjet = 0;
  if(status) ApplySF(pureJets, &nbjet);
  else 
    {
      for(int njet = 0; njet < pureJets.size(); njet++)
	{
	  tagflavor(pureJets.at(njet)->GenFlavor());
	  fillmatch(pureJets.at(njet)->pt(),pureJets.at(njet)->eta()); //*****DONT FORGET TO CHANGE the input ord. of these funcs******************!!!!
	  if(pureJets.at(njet)->IsBJet("CSV","Medium")) //
	    {
	      nbjet++;
	      filltagmatch(pureJets.at(njet)->pt(), pureJets.at(njet)->eta());
	    }
	}
    }
  return nbjet;
}
//Apply scaling factors given by BTagPOG 
void TagEff::ApplySF(vector<Jet*>& pureJets, int *nbj)
{
  int binx=0;
  int biny=0;
  double sf=0.;
  double Pt=0.;
  vector<double> correctedEff;
  for(int njet = 0; njet < pureJets.size(); njet++)    
    {
      if(pureJets.at(njet)->IsBJet("CSV","Medium")) (*nbj)++;
      tagflavor(pureJets.at(njet)->GenFlavor());
      Pt=pureJets.at(njet)->pt();
      binx=(int)(fabs(pureJets.at(njet)->eta())*1.25)+1;//get the eta bin corresponds to this event
      biny=getptbin(Pt);//get the pt bin

      //sf = SF(Pt, fabs(pureJets.at(njet)->eta()), matchtype);
      sf=1.;
      if("b-matched"==matchtype)  {
	correctedEff.push_back((beff->GetBinContent(binx,biny))*sf); 
	bpttag->Fill(Pt,(beff->GetBinContent(binx,biny)));
	//	cout << " corr. eff: beff->GetBinContent(binx,biny) : " <<beff->GetBinContent(binx,biny) << endl;
      }
      else if("c-matched"==matchtype)  {
	correctedEff.push_back((ceff->GetBinContent(binx,biny))*sf); 
	//cout << " corr. eff: ceff->GetBinContent(binx,biny) : " <<ceff->GetBinContent(binx,biny) << endl;  
      }
      else 
	{
	  correctedEff.push_back((leff->GetBinContent(binx,biny))*sf);
	  lpttag->Fill(Pt,(beff->GetBinContent(binx,biny)));  
	  //cout << " corr. eff: leff->GetBinContent(binx,biny) : " <<leff->GetBinContent(binx,biny) << endl;}
	}
    }
   comb(correctedEff);//apply combinatorics
}

void TagEff::comb(vector<double > & eff){
  vector<double>  ones;               // like 1,1,1,1,1
  vector<double>  result;             //result of subtraction for example result = (1,1,1,1)-(e1,e2,e3,e4)
  vector<double>  weightIns;
  weightIns.assign(lasttagbin+1,0.);  //init. the vector to carry weights for the event
  int WTF=0;
  for(int numbjets=0 ; numbjets < lasttagbin  &&  eff.size()>0 ; numbjets ++)
    {
      if(0 == numbjets)
	{ 
	  result.clear();
	  ones.assign(eff.size(),1.); //
	  transform(ones.begin(),ones.end(),eff.begin(),back_inserter(result),std::minus<double>()); 
	  weightIns.at(numbjets)=weightIns.at(numbjets)+multIns(result);
	  if(weightIns.at(numbjets) <0.01) {cout << "error **************************" << endl;  WTF=1;}
	  continue;
	}       
      
      for(int bn=0;bn < eff.size() ; bn++){
	//here create vector of 1s
	ones.assign(eff.size(),1.);
	ones.at(bn)=0; 
	if(1 == numbjets) 
	  { 
	    result.clear();
	    transform(ones.begin(),ones.end(),eff.begin(),back_inserter(result),std::minus<double>()); 
	    weightIns.at(numbjets)=weightIns.at(numbjets)+multIns(result);   
	    continue;
	  }
	
	if(2 == numbjets) 
	  for(int b = bn+1; b < eff.size(); b++ && (ones.at(b-1)=1))
	    {
	      result.clear();
	      ones.at(b)=0; 
	      transform(ones.begin(),ones.end(),eff.begin(),back_inserter(result),std::minus<double>());
	      weightIns.at(numbjets)=weightIns.at(numbjets)+multIns(result);
	    }
      }
    }
  
  weightIns.at(lasttagbin)=1.;
  for(int indx=0; indx<lasttagbin; indx++)
    weightIns.at(lasttagbin)-=weightIns.at(indx);
  if(weightIns.at(lasttagbin) > 0.9) {cout << "CHECK THIS \n weightIns: ";  printvec(weightIns); cout << "lasttag bin is " << lasttagbin << "   and weight in the last tag bin is " << weightIns.at(lasttagbin) ; cout << "    eff: "; printvec(eff);}
  h0tag->Fill(weightIns.at(0));
  h1tag->Fill(weightIns.at(1));
  h2tag->Fill(weightIns.at(2));
  h3mtag->Fill(weightIns.at(3));

  transform(weight.begin(),weight.end(),weightIns.begin(),weight.begin(),plus<double>()); //sum with the other weights 

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
  
  double ptbin[]={40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670, 2000};
  //  double ptbin[]={40.,45., 50.,55., 60.,65., 70.,75., 80.,90.,100.,110., 120.,140., 160.,180., 210., 260., 320., 400., 500., 670., 2000.};
  for(i=0; i< sizeof(ptbin)/sizeof(ptbin[0]); i++)
    {
      if(Pt<ptbin[i]) break;
    } 
  return i;
}

void TagEff::getratio() //Get ratio for the efficiencies namely "matched & tagged jets/ matched jets"
{
  beff=(TH2D *)btag->Clone("beff");
  beff->SetTitle("matched & tagged jets/ matched jets");
  beff->Divide(bmatched);
  ceff=(TH2D *)ctag->Clone("ceff");
  ceff->SetTitle("matched & tagged jets/ matched jets");
  ceff->Divide(cmatched);
  leff=(TH2D *)ltag->Clone("leff");
  leff->SetTitle("matched & tagged jets/ matched jets");
  leff->Divide(lmatched);
}

void TagEff::tagflavor (int flavor) //current Jet class returns string however I modified it to return int
{
  if(5 == abs(flavor)) matchtype = "b-matched";
  else if(4 == abs(flavor)) matchtype = "c-matched";
  else matchtype = "light-matched";
}

void TagEff::vectoTH1 ( vector<double> & vec, TH1D * hist)
{
  for(int indx =0 ; indx < vec.size(); indx++)
    {
      hist->Fill(indx, vec.at(indx));
    }
}

double TagEff::SF( double pt, double eta, string match)
{
  double sf=0.;

  if("light-matched"==match) 
    {
      if(pt<670.)
	{
	  if(eta<0.8){sf=((1.06182+(0.000617034*pt))+(-1.5732e-06*(pt*pt)))+(3.02909e-10*(pt*(pt*pt)));}
	  else if (eta<1.6){sf=((1.111+(-9.64191e-06*pt))+(1.80811e-07*(pt*pt)))+(-5.44868e-10*(pt*(pt*pt)));}
	  else {sf=((1.08498+(-0.000701422*pt))+(3.43612e-06*(pt*pt)))+(-4.11794e-09*(pt*(pt*pt)));}
	}
      else sf= (1.04318+(0.000848162*pt)+(-2.5795e-06*(pt*pt))+1.64156e-09*(pt*(pt*pt)));
    }

    else sf=0.6981*((1.+(0.414063*pt))/(1.+(0.300155*pt))); 
    return sf;
}
