#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <fstream>

void subTreeAnalyzer(TString filepath, TString outfilepath="out.root", TString InDir="undef",TString SampleName="undef", TString SubSample="undef", TString Estimation="undef", TString Tail="undef", double TotalScale=1.0){

 
  //INPUT FILE
  TFile* infile = new TFile(filepath,"READ");
  if (!infile->IsOpen()){
    std::cout<<"not open"<<std::endl;
  }else{
    std::cout<<"it has been opened"<<std::endl;
  }

  //OUTPUT FILE
  TFile *outfile =new TFile(outfilepath,"RECREATE");
  outfile->cd();
  ofstream events;
  events.open("events.dat");
  

  //
  //GET THE TREE
  TTree* tree= (TTree*)infile->Get("subTree");
  //
  //
  //
  std::cout<<"what the fuck "<<std::endl;
  //
  std::cout<<"the entries: "<<tree->GetEntries()<<endl;
  //
  //GET THE BRANCHES
  int Run=0;
  tree->SetBranchAddress("Run",&Run);
  int Event=0;
  tree->SetBranchAddress("Event",&Event);
  double MET;
  tree->SetBranchAddress("MET",&MET);
  double HT;
  tree->SetBranchAddress("HT",&HT);
  double MHT;
  tree->SetBranchAddress("MHT",&MHT);
  double Y;
  tree->SetBranchAddress("Y",&Y);
  double MT;
  tree->SetBranchAddress("MT",&MT);
  double MT2;
  tree->SetBranchAddress("MT2",&MT2);
  double MT2WEle;
  tree->SetBranchAddress("MT2WEle",&MT2WEle);
  double MT2WMu;
  tree->SetBranchAddress("MT2WMu",&MT2WMu);
  double EventWeight;
  tree->SetBranchAddress("Weight",&EventWeight);
  int NJets;
  tree->SetBranchAddress("NJets",&NJets);
  vector< double >* treeElectron=0;
  tree->SetBranchAddress("treeEl",&treeElectron);
  vector< double >* treeMuon=0;
  tree->SetBranchAddress("treeMu",&treeMuon);
  vector<double>* jet1=0;
  tree->SetBranchAddress("jet1",&jet1);
  vector<double>* jet2=0;
  tree->SetBranchAddress("jet2",&jet2);
  vector<double>* jet3=0;
  tree->SetBranchAddress("jet3",&jet3);
  vector<double>* jet4=0;
  tree->SetBranchAddress("jet4",&jet4);
  vector<double>* jet5=0;
  tree->SetBranchAddress("jet5",&jet5);
  vector<double>* jet6=0;
  tree->SetBranchAddress("jet6",&jet6);
  vector<double>* jet7=0;
  tree->SetBranchAddress("jet7",&jet7);


  //=========CONSTANTS================
  double Ymax=5.5;
  double Ymin=2.5;
  double HTmax=650;
  double HTmin=375;


  //=========HISTOGRAMS========================//
  TH1D* hMET = new TH1D("MET","missing ET",50,50.0,300.0);
  TH1D* hHT = new TH1D("HT","HT",50,100.0,2000.0);
  TH1D* hAllJetsPt = new TH1D("AllJetsPt", "the pt of all jets",50,0.0,500.0);
  TH1D* RegionA = new TH1D("RegionA", "Number of entries in A",1,0.5,1.5);
  TH1D* RegionB = new TH1D("RegionB", "Number of entries in B",1,0.5,1.5);
  TH1D* RegionC = new TH1D("RegionC", "Number of entries in C",1,0.5,1.5);
  TH1D* RegionD = new TH1D("RegionD", "Number of entries in D",1,0.5,1.5);
  
  TH2D* HT_Y= new TH2D("HT_Y","HT vs Y",50,HTmin,2000.,50,Ymin,40.);  
  TH1D* hMT2WEle = new TH1D("MT2WEle","MT2W electrons",50,0.0,500.0);
  TH1D* hMT2WMu = new TH1D("MT2WMu","MT2W Muons",50,0.0,500.0);

  int EventMAX=0;
  int RunMAX=0;


  //===============EVENT LOOP =========================================
  int N=tree->GetEntries();
  std::cout<<"looping over "<<N<<" entries"<<std::endl;
  for (int i=0;i<N;++i){
    //std::cout<<" currently at "<<i<<std::endl;
    tree->GetEntry(i);


    events<<Run<<" "<<Event<<std::endl;
    vector< vector <double > > Jets;
    Jets.push_back(*jet1);
    Jets.push_back(*jet2);
    Jets.push_back(*jet3);
    Jets.push_back(*jet4);
    Jets.push_back(*jet5);
    Jets.push_back(*jet6);
    Jets.push_back(*jet7);
  
    double AllJetsPt=0;
    for (int k=0; k<7;++k){
      if(Jets.at(k).at(0)>0.0){
	hAllJetsPt->Fill(Jets.at(k).at(0),EventWeight);
      }
    }



    //=======A B C D===================

    const char* Region;
    if (  HT < HTmax ){
      Region="A";
      if ( Y > Ymax)Region="C";
    }else{
      Region="B";
      if( Y > Ymax)Region="D";
    }
    if( Region=="A"){
      RegionA->Fill(1,EventWeight);
    }else if(Region=="B"){
      RegionB->Fill(1,EventWeight);

    }else if(Region=="C"){
      RegionC->Fill(1,EventWeight);

    }else if(Region=="D"){
      RegionD->Fill(1,EventWeight);
    }


    hMET->Fill(MET,EventWeight);
    hHT->Fill(HT,EventWeight);    
    HT_Y->Fill(HT,Y,EventWeight);
    hMT2WEle->Fill(MT2WEle,EventWeight);
    hMT2WMu->Fill(MT2WMu,EventWeight);


    //LAST EVENT
    if (Run > RunMAX){
      RunMAX=Run;
      if (Event> EventMAX){
	EventMAX = Event;
      }
    }



    //==============================
    /*if (Run > 193621){
      cout<<"exiting "<<endl;
      continue;
    }
    
    if(Run == 193621){
      if(Event>1715774414){
	cout<<"exiting "<<endl;
	continue;
      }
      }*/
    //==============================


  }//EVENT LOOP

  events.close();
  std::cout<<"the last recorded event is"<<std::endl;
  std::cout<<" RUN  : "<<RunMAX<<std::endl;
  std::cout<<" EVENT: "<<EventMAX<<std::endl;
  //
  //
  //WRITE THE HISTOGRAMS
  TListIter* list = new TListIter(gDirectory->GetList()); //-->iterates over list of objects in memory
  TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
  while(nobj){
    
    if(nobj->IsA()->InheritsFrom("TH1")){
      nobj->Write();
      //nobj->Write("",TObject::kOverwrite) -->Overwrites the histogram if alredy present
    }
    nobj=list->Next();   //nobj point now to the next object
  }
 

}

