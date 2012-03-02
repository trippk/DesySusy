#include <iostream>
#include "TROOT.h"
#include "TH1.h"
#include "TH2D.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TTree.h"
#include "TPaveStats.h"
#include <algorithm>

#include "plotSet.h"

using namespace std;


double plotSet::global_event_weight=1.;

/////
/////   plot handling
/////

TH1* plotSet::Get(TString h) {
  for (map<TString, vector<TH1*> >::iterator ci=pTitleToHistos.begin(); ci!=pTitleToHistos.end(); ++ci) {
    for (size_t hi=0; hi<ci->second.size(); ++hi) {
      if (ci->second.at(hi)->GetName() == h)
	return ci->second.at(hi);
    }
  }
  cout<<"plotSet::Get: histogram "<<h<<" not found!"<<endl;
  TH1F* null = new TH1F("null","null",10,0,10);
  return null;
}

bool plotSet::isNewCanvas(TString c) {
  return (pTitleToHistos.find(c) == pTitleToHistos.end());
}

bool plotSet::isNewHisto(TString h, TString c) {
  if( isNewCanvas(c) ) return true;
  for( size_t i=0; i<pTitleToHistos[c].size(); ++i )
    if( pTitleToHistos[c].at(i)->GetTitle() == h ) return false;
  return true;
}

void plotSet::addPlot(TString name, TString can, int bins, double low, double up, double value, double weight) {

  if(isNewCanvas(can)) { // create new canvas
    TH1D* tmpH = new TH1D(name,can,bins,low,up);
    tmpH->Fill( value, weight );
    vector<TH1*> tmpV;
    tmpV.push_back( tmpH );
    pTitleToHistos[can] = tmpV;
  }
  else {
    int histNo = -1;
    for( size_t h = 0; h < pTitleToHistos[can].size() && histNo == -1; ++h)
      if( pTitleToHistos[can][h]->GetName() == name )
	histNo = h;

    if(histNo==-1) { // create new histogram and add to existing canvas
      TH1D* tmpH = new TH1D(name,can,bins,low,up);
      tmpH->Fill(value, weight);
      pTitleToHistos[can].push_back(tmpH);
    }
    else // fill existing histogram
      pTitleToHistos[can][histNo]->Fill(value, weight);
  }
}

void plotSet::addPlot(TH1* hist, TString histN, TString can) {
  hist->SetName(histN);
  hist->SetTitle(can);
  if(isNewCanvas(can)) { // create new canvas
    vector<TH1*> tmpV;
    tmpV.push_back( hist );
    pTitleToHistos[can] = tmpV;
  }
  else // add to existing canvas
    pTitleToHistos[can].push_back(hist);
}

void plotSet::addPlot(TString name, TString can,
		      int xbins, double xlow, double xup, double xvalue, 
		      int ybins, double ylow, double yup, double yvalue, double weight ) {

  if(isNewCanvas(can)) { // create new canvas
    TH2D* tmpH = new TH2D(name,can,xbins,xlow,xup,ybins,ylow,yup);
    tmpH->Fill( xvalue , yvalue, weight );
    vector<TH1*> tmpV;
    tmpV.push_back( tmpH );
    pTitleToHistos[can] = tmpV;
  }
  else {
    int histNo = -1;
    for( size_t h = 0; h < pTitleToHistos[can].size() && histNo == -1; ++h)
      if( pTitleToHistos[can][h]->GetName() == name )
	histNo = h;

    if(histNo==-1) { // create new histogram and add to existing canvas
      TH2D* tmpH = new TH2D(name,can,xbins,xlow,xup,ybins,ylow,yup);
      tmpH->Fill( xvalue, yvalue, weight );
      pTitleToHistos[can].push_back(tmpH);
    }
    else // fill existing histogram
      ((TH2D*) pTitleToHistos[can][histNo])->Fill( xvalue, yvalue, weight );
  }
}

void plotSet::SetAxesTitles(TString c, TString xt, TString yt) {

  if( pTitleToXaxisTitle.find(c) != pTitleToXaxisTitle.end() ) {
    if( pTitleToXaxisTitle[c] != xt ) {
      cout<<"Title for x axis in canvas "<<c<<" overwritten:"<<endl;
      cout<<"  "<<pTitleToXaxisTitle[c]<<"  -->  "<<xt<<endl;
    }
  }
  pTitleToXaxisTitle[c] = xt;

  if( pTitleToYaxisTitle.find(c) != pTitleToYaxisTitle.end() ) {
    if( pTitleToYaxisTitle[c] != yt ) {
      cout<<"Title for y axis in canvas "<<c<<" overwritten:"<<endl;
      cout<<"  "<<pTitleToYaxisTitle[c]<<"  -->  "<<yt<<endl;
    }
  }
  pTitleToYaxisTitle[c] = yt;
}

float plotSet::hist_maximum(TH1* h) { const int i = h->GetMaximumBin();   return h->GetBinContent(i) + h->GetBinError(i);}

float plotSet::hist_minimum(TH1* h) { 

  int min_bin(h->GetMaximumBin());
  for(int x=0; x<=h->GetNbinsX()+1; x++) {
    for(int y=0; y<=h->GetNbinsY(); y++) {
      int bin = h->GetBin(x,y);
      if( h->GetBinContent(bin) !=0 && h->GetBinContent(bin) < h->GetBinContent(min_bin) )
        min_bin = bin;
    }
  }
  return h->GetBinContent(min_bin) - h->GetBinError(min_bin);
}


/////
/////   tree handling
/////

// template<typename T>
// void plotSet::addLeaf(TString TreeName, TString BranchName, T value) {
//   bool isNewTree = true;
//   for(size_t tn=0; tn<tNames.size() && isNewTree; ++tn)
//     if( TreeName == tNames.at(tn) )
//       isNewTree = false;
//   if(isNewTree) {
//     TTree* ntr = new TTree(TreeName,TreeName);
//     tNames.push_back(TreeName);
//     tNameToTree[TreeName]=ntr;
//     tNameToIsFilled[TreeName]=false;
//   }

//   if( GetValueMap(value)[TreeName].find(BranchName) == GetValueMap(value)[TreeName].end() ) {
//     if( tNameToIsFilled[TreeName] ) cout<<" --- ------- ---"<<endl
// 				    <<" --- WARNING --- You are adding branch \'"
// 				    <<BranchName<<"\' to tree \'"<<TreeName
// 				    <<"\' which has already been filled in another loop! You will get nonsense numbers!"
// 				    <<endl<<" --- ------- ---"<<endl;
//     T valuePointer;
//     GetValueMap(value)[TreeName][BranchName] = valuePointer;
//     tNameToTree[TreeName]->Branch(BranchName, &GetValueMap(value)[TreeName][BranchName]);
//     tNameTobNameToNotFilledValue[TreeName][BranchName] = false;
//   }
//   else if( tNameTobNameToNotFilledValue[TreeName][BranchName] )
//     fillTree(TreeName);

//   GetValueMap(value)[TreeName][BranchName] = value;
//   tNameTobNameToNotFilledValue[TreeName][BranchName] = true;
// }

bool plotSet::fillTree(TString tn) {

  bool allToBeFilled = true;
  for(map<TString, bool>::iterator av=tNameTobNameToNotFilledValue[tn].begin(); av!=tNameTobNameToNotFilledValue[tn].end(); ++av) {
    if( !av->second ) allToBeFilled=false;
    av->second=false;
  }
  if(allToBeFilled) {
    tNameToTree[tn]->Fill();
    tNameToIsFilled[tn]=true;
  }
  return allToBeFilled;
}

/////
/////   print and save
/////

void plotSet::printAll(TString style) {
  gROOT->SetStyle("Plain");
  for( map<TString, vector<TH1*> >::iterator c = pTitleToHistos.begin(); c != pTitleToHistos.end(); ++c ) {
    print(c);
  }
}

void plotSet::savePSFile(TString Fname) {
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  int last = (int)pTitleToHistos.size() - 1;
  int count=0;
  for( map<TString, vector<TH1*> >::iterator c = pTitleToHistos.begin(); c != pTitleToHistos.end(); ++c ) {
    TString lFname = Fname;
    if     ( count == 0 && count != last ) lFname += "(";
    else if(               count == last ) lFname += ")";
    cout<<c->first<<"  "<<lFname<<endl;
    print(c)->Print(lFname);
    ++count;
  }
}

void plotSet::setTFile(TFile* f) {
  TDirectory* mainDir=0;
  if(f!=0)       mainDir = f->GetDirectory("/");
  if(mainDir!=0) tdir = mainDir->mkdir(Name);
  if(tdir==0) { 
    cout<<"plotSet::setTFile: cannot create directory in file!"<<endl;
    exit(0);
  }
  tfile=f;

}

void plotSet::saveRootFile(TString DSname) {

  //TString fname = DSname;
  //fname += "_";
  //fname += Name;
  //fname += ".root";
  //TFile *outputFile = new TFile(fname, "RECREATE");
  TDirectory* keep = gDirectory->GetDirectory("");
  tdir->cd();
  //save histograms
  for( map<TString, vector<TH1*> >::iterator c = pTitleToHistos.begin(); c != pTitleToHistos.end(); ++c )
    for( size_t h = 0; h < c->second.size(); ++h ) {
      if( pTitleToXaxisTitle.find(c->first) != pTitleToXaxisTitle.end() )
	c->second.at(h)->GetXaxis()->SetTitle( pTitleToXaxisTitle[c->first] );
      if( pTitleToYaxisTitle.find(c->first) != pTitleToYaxisTitle.end() )
	c->second.at(h)->GetYaxis()->SetTitle( pTitleToYaxisTitle[c->first] );
      c->second.at(h)->Write();
    }

  //save trees
  for(size_t tr=0; tr<tNames.size(); ++tr) {
    fillTree(tNames.at(tr));
    tNameToTree[tNames.at(tr)]->Write();
  }
  //outputFile->Close();
  //outputFile->Delete();

  // go back to original dir
  keep->cd();
}

TCanvas* plotSet::print( map<TString, vector<TH1*> >::iterator c ) {

  TCanvas* tmpC = new TCanvas( c->first, c->first, 700, 600 );
  pTitleToCanvas[c->first] = tmpC;
  tmpC->cd();
  float legSize = .04 + (.04 * min((int)c->second.size(),4));
  TLegend *leg = new TLegend( .4, .995-legSize, .8, .995 );
  leg->SetTextFont(42);
  leg->SetFillColor(0);
  leg->SetLineColor(1);

  if( c->second.size()==0 ) return 0;

  TH1* nu = (TH1*) c->second.at(0)->Clone("Axes");
  nu->Reset();
  nu->SetStats(false);
  nu->Draw();
  nu->GetXaxis()->SetTitleFont(42);
  nu->GetYaxis()->SetTitleFont(42);
  nu->GetXaxis()->SetLabelFont(42);
  nu->GetYaxis()->SetLabelFont(42);

  gPad->SetMargin(.08, .2, .1, legSize+.005);

  double minRange,  maxRange;
  float statPos = 0.995;
  for( size_t h = 0; h < c->second.size(); ++h ) {
    if( h==0 || maxRange < hist_maximum(c->second.at(h)) )
      maxRange = hist_maximum(c->second.at(h));
    if( h==0 || minRange < hist_minimum(c->second.at(h)) )
      minRange = hist_minimum(c->second.at(h));
    c->second.at(h)->SetLineWidth(2);
    c->second.at(h)->SetLineColor( h+1 );

    if( c->second.at(h)->IsA()->InheritsFrom("TH2") ) {
      c->second.at(h)->Draw("COL sames");
      //leg->AddEntry(c->second.at(h),c->second.at(h)->GetName(),"f");
    } else {
      c->second.at(h)->Draw("sames");
      leg->AddEntry(c->second.at(h),c->second.at(h)->GetName(),"l");
    }

    gPad->Update();
    TPaveStats *st = (TPaveStats*)c->second.at(h)->FindObject("stats");
    if(st) {
      float statSize = st->GetY2NDC()-st->GetY1NDC();
      st->SetY2NDC(statPos);
      statPos -= statSize;
      st->SetY1NDC(statPos);
      st->SetX1NDC(.8);
      st->SetX2NDC(.995);
    }
    
  }

  double addRange = (maxRange-minRange)/10.;
  nu->SetMinimum(minRange>=0. ? 0 : minRange-addRange);
  nu->SetMaximum(maxRange+addRange);

  gPad->RedrawAxis();
  leg->Draw();

  if( pTitleToXaxisTitle.find(c->first) != pTitleToXaxisTitle.end() )
    nu->GetXaxis()->SetTitle( pTitleToXaxisTitle[c->first] );
  if( pTitleToYaxisTitle.find(c->first) != pTitleToYaxisTitle.end() )
    nu->GetYaxis()->SetTitle( pTitleToYaxisTitle[c->first] );

  return tmpC;
}
