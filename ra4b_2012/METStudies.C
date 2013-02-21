#include "TDirectory.h"
#include "TCanvas.h"
#include "TKey.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"
#include "THStack.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "systematicUncertainties.h"
#include "histoPack.h"



using namespace std;

//void systematicUncertainties(TString _defaultTreeFile, TString _upDownFile){
void METStudies(int intData, TString _defaultTreeFile, TString _upDownFile, TString _outFile, TString centralMode){


  cout<<"the central mode is "<<centralMode<<endl;

  void LoopAndPlot(TreeVariables&, HistoPack&, TTree*);

  //int lib=gSystem->Load("libRA4b.so");
  //if (lib != 0){
  //cout<<"the library did not load properly"<<endl;
  //}
  
  bool isData;
  if(intData !=0){
    isData=true;
  }else{
    isData=false;
  }

  //================INPUT FILES
  TFile* defaultTreeFile = new TFile(_defaultTreeFile,"READ");
  if (!defaultTreeFile->IsOpen())std::cout<<_defaultTreeFile<<" not open"<<std::endl;

  TFile* upDownFile;
  if(_upDownFile!="same"){
    upDownFile = new TFile(_upDownFile,"READ");
    if (!upDownFile->IsOpen())std::cout<<_upDownFile<<" not open"<<std::endl;
  }else{
    upDownFile = new TFile(_defaultTreeFile,"READ");
  }


  cout<<endl;
  cout<<endl;
  cout<<"THE INPUT FILE IS "<<_defaultTreeFile<<endl;
  cout<<endl;
  cout<<endl;


  //
  //================OUTPUT FILE
  //TString outfilepath="test_systematics.root";
  cout<<endl;
  cout<<"the outFileName is "<<_outFile<<endl;
  cout<<endl;
  TFile* outfile =new TFile(_outFile,"RECREATE");
  outfile->cd();



  //============GET THE NAME OF THE SYSTEMATICS PRESENT IN THE FILES
  vector<TString> sysNames;
  if(!isData){
    sysNames=*(GetSystematicNames(upDownFile));
  }
  //cout<<"size of sysnames = "<<sysNames.size()<<endl;
  


  //DEFAULT TREE
  TTree* defaultTree= (TTree*)defaultTreeFile->Get("subTree");
  cout<<"defaultTree pointer is "<<defaultTree<<endl;



  //HISTOS
  if(!isData){
    for (int iname=0; iname<(int)sysNames.size();++iname){
      //==========CREATE A DIR IN THE OUTPUTFILE
      
      cout<<"inside the systematic loop "<<endl;
      
      outfile->cd();
      TDirectory* outDir= (TDirectory*)outfile->mkdir(sysNames.at(iname));
      if(outDir==0){cout<<"the directory "<<outDir<<" already exists"<<endl;}
      else{outDir->cd();}


      HistoPack centralHistos("_central");
      HistoPack upHistos("_up");
      HistoPack downHistos("_down");

      //
      //
      //
      //===============GET THE TREES
      //
      //
      //=======UPTREE
      TString upPath="Systematics/"+sysNames.at(iname)+"up"+"/subTree";

      TTree* upTree= (TTree*)upDownFile->Get((TString)upPath);
      if (upTree==0){cout<<"did NOT get the upTree, ERROR"<<endl;}

      //=======DOWNTREE
      TString downPath="Systematics/"+sysNames.at(iname)+"down"+"/subTree";
      TTree* downTree= (TTree*)upDownFile->Get(downPath);
      if (downTree==0){cout<<"did NOT get the downTree, ERROR"<<endl;}

      //=======CENTRALTREE
      TreeVariables defaultVariables;
      TString centralPath="Systematics/"+sysNames.at(iname)+"central"+"/subTree";
      TTree* centralTree= (TTree*)upDownFile->Get(centralPath);

      if (centralTree==0){
	cout<<endl;
	cout<<"changing the centralTree to the defaultTree"<<endl;
	cout<<endl;
	//then the central tree is the default one
	centralTree=defaultTree;
      }

      if(centralMode=="default"){
	cout<<"changing the centralTree to the defaultTree"<<endl;	
	centralTree=defaultTree;
      }
      //
      //
      //
      //=======DEFINE THE BRANCHES AND GET THE VARIABLES
      TreeVariables upVariables;
      DefineBranches(upTree,upVariables);
      //
      TreeVariables downVariables;
      DefineBranches(downTree,downVariables);
      //
      TreeVariables centralVariables;
      DefineBranches(centralTree,centralVariables);
      //
      //

      //==============EVENT LOOP===============//
      int NMAX_central=0;
      int NMAX_up=0;
      int NMAX_down=0;


      NMAX_central=centralTree->GetEntries();
      NMAX_up=upTree->GetEntries();
      NMAX_down=downTree->GetEntries();
      cout<<endl;
      cout<<"increase in final events with UP variation"<<NMAX_up/double(NMAX_central) -1.0;
      cout<<endl;
      cout<<"increase in final events with DOWN variation"<<NMAX_down/double(NMAX_central) -1;
      cout<<endl;
      //cin.get();
      //
      //
      //cout<<"tree being used is "<<centralTree->GetName()<<endl;
      //cout<<"looping over "<<NMAX<<endl;
      //NMAX=1;
      
      LoopAndPlot(centralVariables,centralHistos,centralTree);
      LoopAndPlot(upVariables,upHistos,upTree);
      LoopAndPlot(downVariables,downHistos,downTree);


      cout<<"out of the last loop "<<endl;
      //myC=new TCanvas("c","c",800,600);
      //delete(centralTree);
      delete(upTree);
      delete(downTree);
   
      cout<<"writing the histograms "<<endl;
      outDir->Write();


      //ERASE ALL HISTOGRAMS FROM MEMORY
      TListIter* list = new TListIter(gDirectory->GetList()); //-->iterates over list of objects in memory
      TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
      while(nobj){

	if(nobj->IsA()->InheritsFrom("TH1")){
	  delete(nobj);
	  //nobj->Write("",TObject::kOverwrite) -->Overwrites the histogram if alredy present
	}
	//delete(nobj);
	//TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
	nobj=list->Next();   //nobj point now to the next object
      }
      delete(list);    
      
    }
    
  }
  
  else{


    //IF IT IS A DATA FILE
    outfile->cd();
    HistoPack dataHistos("");
    
    /*
    TH1D* MT2WMu= new TH1D("MT2WMu"," ",100,0.,500);    
    TH1D* MTMu= new TH1D("MTMu","central ",100,0.,500.);    
    TH1D* MT2WEl= new TH1D("MT2WEl","central ",100,0.,500.);    
    TH1D* MT2= new TH1D("MT2","central ",100,0.,500.);    
    TH1D* JetPt=new TH1D("JetPt","JetPt",30,20.0,500.0);
    TH1D* MET=new TH1D("MET","MET",100.,0,1000.0);
    TH1D* HT=new TH1D("HT","HT",20,100.0,1000.0);

    */

    TString dataPath="subTree";
    TTree* dataTree=(TTree*)defaultTreeFile->Get((TString)dataPath);
    if(!dataTree){cout<<"did NOT get the data tree, ERROR"<<endl;}
    //===============DEFINE THE BRANCHES
    TreeVariables dataVariables;
    DefineBranches(dataTree,dataVariables);
    
    //==============EVENT LOOP===============//
    LoopAndPlot(dataVariables,dataHistos,dataTree);
    cout<<"writing the data histos "<<endl;
    outfile->Write();

    //ERASE ALL HISTOGRAMS FROM MEMORY
    TListIter* list = new TListIter(gDirectory->GetList()); //-->iterates over list of objects in memory
    TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
    while(nobj){
      
      if(nobj->IsA()->InheritsFrom("TH1")){
	delete(nobj);
	//nobj->Write("",TObject::kOverwrite) -->Overwrites the histogram if alredy present
      }
      //delete(nobj);
      //TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
      nobj=list->Next();   //nobj point now to the next object
    }
    delete(list);    

  }









  /*
  //THE PILE UP UNCERTAINTY CAN BE DONE HERE
  centralTree= (TTree*)defaultTreeFile->Get("subTree");
  DefineBranches(centralTree,centralVariables);

  outfile->cd();
  TDirectory* outDir=(TDirectory*)outfile->mkdir("pileup");
  double upWeight=centralVariables.EventWeight*(1.05);
  double downWeight=centralVariables.EventWeight*(0.95);
  
  //==============EVENT LOOP===============//
  int NMAX=centralTree->GetEnstries();
  cout<<"looping over "<<NMAX<<endl;
  //NMAX=1;
  for (int iEvent=0; iEvent<NMAX ; ++iEvent){
  if (iEvent%10000 == 0)cout<<"events "<<iEvent<<endl;
  centralTree->GetEntry(iEvent);
  //========mt2wMu
  MT2WMu_up->Fill(centralVariables.MT2WMu,upWeight);
  MT2WMu_down->Fill(centralVariables.MT2WMu,downWeight);
  MT2WMu_central->Fill(centralVariables.MT2WMu,centralVariables.EventWeight);
      
  }
  */


  outfile->Close();
}
  
  

  
  
  
void DefineBranches(TTree* tree,TreeVariables& name){
    
  //GET THE BRANCHES
  tree->SetBranchAddress("Run",&name.Run);
  tree->SetBranchAddress("Jets",&name.Jets);
  tree->SetBranchAddress("Event",&name.Event);
  tree->SetBranchAddress("MET",&name.MET);
  tree->SetBranchAddress("HT",&name.HT);
  tree->SetBranchAddress("MHT",&name.MHT);
  tree->SetBranchAddress("Y",&name.Y);
  tree->SetBranchAddress("MTEle",&name.MTEle);
  tree->SetBranchAddress("MTMu",&name.MTMu);
  tree->SetBranchAddress("MT2",&name.MT2);
  tree->SetBranchAddress("MT2WEle",&name.MT2WEle);
  tree->SetBranchAddress("MT2WMu",&name.MT2WMu);
  tree->SetBranchAddress("Weight",&name.EventWeight);
  tree->SetBranchAddress("Electrons",&name.Electrons);
  tree->SetBranchAddress("Muons",&name.Muons);
  tree->SetBranchAddress("PUWeight",&name.PUWeight);
  tree->SetBranchAddress("PUWeight_up",&name.PUWeight_up);
  tree->SetBranchAddress("PUWeight_up",&name.PUWeight_down);
  tree->SetBranchAddress("NPV",&name.NPV);

}



vector<TString>* GetSystematicNames(TFile* upDownFile){

  vector<TString>* sysNames = new vector<TString>;
  
  TDirectory* sysDir = (TDirectory*)upDownFile->Get("Systematics");
  if(!sysDir)return sysNames;

  TListIter* list = new TListIter(sysDir->GetListOfKeys()); 
  TKey *nkey=(TKey*) list->Next(); //nkey now points to the first key
  TObject* nobj;
  while(nkey){
    
    nobj=(TObject*)sysDir->Get(nkey->GetName());
    if(nobj->IsA()->InheritsFrom("TDirectory")){
      TString st=(TString)nkey->GetName();
      if(st(st.Length()-2,st.Length()-1)=="up"){
	//cout<<"pushing back "<<st(0,st.Length()-4)<<endl;
	//cout<<"st = "<<st<<endl;
	sysNames->push_back(st(0,st.Length()-2));
      }
    }

    //delete the object in memory, 
    //we dont want it hanging in there
    delete(nobj);
    nkey=(TKey*)list->Next();   //nobj point now to the next object
  }
  cout<<"returning sysnames with size "<<sysNames->size()<<endl;
  return sysNames;
}


THStack* plotUpDownCentral(TH1D* central, TH1D* up, TH1D* down, TString psname){
  
  gPad->Clear();
  THStack* stack = new THStack("stack",central->GetName());
  central->SetFillColor(2);
  stack->Add(central);
  up->SetLineColor(1);
  stack->Add(up);
  down->SetLineColor(4);
  stack->Add(down);
  stack->Draw("nostack");
  gPad->Print(psname+".ps");
  return stack;
  //
}




void LoopAndPlot(TreeVariables& variables, HistoPack& histos, TTree* tree){
  double FillJetsPt(TH1D*, double, std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >* );


  cout<<"INSIDE LOOP AND PLOT"<<endl;
  int NEvents=tree->GetEntries();
  histos.TotalEvents->SetBinContent(1,(double)NEvents);

  cout<<"WITH NEVents = "<<NEvents<<endl;
  for(int iEvent=0;iEvent<NEvents;++iEvent){
    if (iEvent%50000 == 0)cout<<"events "<<iEvent<<endl;	
    
    tree->GetEntry(iEvent);
    double EW=variables.EventWeight;
    histos.MT2WMu->Fill(variables.MT2WMu, EW);
    histos.MTMu->Fill(variables.MTMu,EW);
    histos.MT2_MET->Fill(variables.MET,variables.MTMu);


    if(variables.MTMu>200.0){
      histos.counter->Fill(0.0, EW);
    }
    else if(variables.MTMu>150.0){
      histos.counter->Fill(1.0, EW);
    }
    else if(variables.MTMu>130.0){
      histos.counter->Fill(2.0, EW);
    }
    else if(variables.MTMu>120.0){
      histos.counter->Fill(3.0, EW);
    }


    

    double HT=0;
    for(int ijet=0; ijet<(int)variables.Jets->size();++ijet){
      
      histos.JetPt->Fill(variables.Jets->at(ijet).Pt(), EW);
      HT+=variables.Jets->at(ijet).Pt();
    }
    //cout<<""<<endl;
    histos.HT->Fill(HT,EW);
    histos.MET->Fill(variables.MET,EW);
    histos.NPV->Fill(variables.NPV,EW);
    //withouth weight
    histos.NPV_NoW->Fill(variables.NPV,1.0);


    //now in bins of number of primary vertices
    if (variables.NPV <10){
      histos.MTMu_PrimaryVtx.at(0)->Fill(variables.MTMu,EW);
      histos.MET_PrimaryVtx.at(0)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_PrimaryVtx.at(0),EW, variables.Jets);
    }else if(variables.NPV <30){
      histos.MTMu_PrimaryVtx.at(1)->Fill(variables.MTMu,EW);
      histos.MET_PrimaryVtx.at(1)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_PrimaryVtx.at(1),EW, variables.Jets);
    }else{
      histos.MTMu_PrimaryVtx.at(2)->Fill(variables.MTMu,EW);
      histos.MET_PrimaryVtx.at(2)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_PrimaryVtx.at(2),EW, variables.Jets);
    }      

    //now in bins of HT
    if (variables.HT <250.){
      histos.MTMu_HT.at(0)->Fill(variables.MTMu,EW);
      histos.MET_HT.at(0)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_HT.at(0),EW, variables.Jets);
    }else if(variables.HT <400){
      histos.MTMu_HT.at(1)->Fill(variables.MTMu,EW);
      histos.MET_HT.at(1)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_HT.at(1),EW, variables.Jets);
    }else{
      histos.MTMu_HT.at(2)->Fill(variables.MTMu,EW);
      histos.MET_HT.at(2)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_HT.at(2),EW, variables.Jets);
    }      

    //now in bins of MET
    if (variables.MET >150.){
      histos.MTMu_MET.at(2)->Fill(variables.MTMu,EW);
      histos.MET_MET.at(2)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_MET.at(2),EW, variables.Jets);
    }else if(variables.MET >100){
      histos.MTMu_MET.at(1)->Fill(variables.MTMu,EW);
      histos.MET_MET.at(1)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_MET.at(1),EW, variables.Jets);
    }else if(variables.MET>50){
      histos.MTMu_MET.at(0)->Fill(variables.MTMu,EW);
      histos.MET_MET.at(0)->Fill(variables.MET,EW);
      FillJetsPt(histos.JetPt_MET.at(0),EW, variables.Jets);
    } 

  }

  cout<<"By from loop and plot"<<endl;
}


double FillJetsPt(TH1D* histo, double Weight, std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >*  jets){
  double HT=0;
  for(int ijet=0; ijet<(int)jets->size();++ijet){
    histo->Fill(jets->at(ijet).Pt(), Weight);
    HT+=jets->at(ijet).Pt();
  }
  return HT;
}

