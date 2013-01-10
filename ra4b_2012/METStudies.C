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




//void systematicUncertainties(TString _centralFile, TString _upDownFile){
void systematicUncertainties(){


  int lib=gSystem->Load("libRA4b.so");
  if (lib != 0){
    cout<<"the library did not load properly"<<endl;
  }
  
  //TString _centralFile="./TTJets/SUMMER12/TTJets_SUMMER12_RA4b-mu_NoTail_Tree.root";
  //TString _centralFile="TTJetsMG/Full-DR53/TTJetsMG_Full-DR53_RA4b-mu_NoTail_Tree.root";
  TString _centralFile="TTJets_SemiLeptMGDecaysFull_cfg-1_out_tree.root";
    //TString _centralFile="./SusyCAF_Tree_out_tree.root";
    TString _upDownFile="same";
  //================INPUT FILES
  TFile* centralFile = new TFile(_centralFile,"READ");
  if (!centralFile->IsOpen())std::cout<<_centralFile<<" not open"<<std::endl;

  TFile* upDownFile;
  if(_upDownFile!="same"){
    upDownFile = new TFile(_upDownFile,"READ");
    if (!upDownFile->IsOpen())std::cout<<_upDownFile<<" not open"<<std::endl;
  }else{
    upDownFile = new TFile(_centralFile,"READ");
  }



  //================OUTPUT FILE
  TString outfilepath="test_systematics.root";
  TFile* outfile =new TFile(outfilepath,"RECREATE");
  outfile->cd();
  //ofstream events;
  //  events.open("events.dat");
  




  //============GET THE NAME OF THE SYSTEMATICS PRESENT
  vector<TString> sysNames=*(GetSystematicNames(upDownFile));
  cout<<"size of sysnames = "<<sysNames.size()<<endl;
  //============FOR EVERY SYSTEMATIC, GET THE CORRESPONDING TREE
  TTree* centralTree= (TTree*)centralFile->Get("subTree");
  cout<<"centralTree pointer is "<<centralTree<<endl;
  TreeVariables centralVariables;
  DefineBranches(centralTree,centralVariables);



  //HISTOS

  TCanvas* myC;

  for (int iname=0; iname<(int)sysNames.size();++iname){
    //==========CREATE A DIR IN THE OUTPUTFILE
    outfile->cd();
    TDirectory* outDir= (TDirectory*)outfile->mkdir(sysNames.at(iname));
    if(outDir==0){cout<<"the directory "<<outDir<<" already exists"<<endl;}
    else{outDir->cd();}



    //===================HISTOGRAMS
    //map<string,TH1D*>
    TH1D* MT2WMu_up= new TH1D("MT2WMu_up","up ",100,0.,500);    
    TH1D* MT2WMu_down= new TH1D("MT2WMu_down","down ",100,0.,500);    
    TH1D* MT2WMu_central= new TH1D("MT2WMu_central","central ",100,0.,500.);    
    //TH1D* MT2WMu_upShift= new TH1D("MT2WMu_upShift","up Shift",100,-200.0,200);    
    //TH1D* MT2WMu_downShift= new TH1D("MT2WMu_downShift","down Shift",100,-200.0,200);
    TH1D* MT2WMu_Shift= new TH1D("MT2WMu","Shift",100,0.0,100);    
    TH1D* MT2WMu_RelShift= new TH1D("MT2WMu_Rel","Shift",50,-2.0,2.0);    

    TH1D* JetPt_up=new TH1D("JetPt_up","JetPt_up",30,20.0,500.0);
    TH1D* HT_up=new TH1D("HTup","HTup",20,100.0,1000.0);
    TH1D* JetPt_down=new TH1D("JetPt_down","JetPt_down",30,20.0,500.0);
    TH1D* HT_down=new TH1D("HTdown","HTdown",20,100.0,1000.0);
    TH1D* JetPt_central=new TH1D("JetPt_central","JetPt_central",30,20.0,500.0);
    TH1D* HT_central=new TH1D("HTcentral","HTcentral",20,100.0,1000.0);
    
    TH1D* MET_up=new TH1D("MET_up","MET_up",100.,0,1000.0);
    TH1D* MET_down=new TH1D("MET_down","MET_down",100.,0,1000.0);
    TH1D* MET_central=new TH1D("MET_central","MET_central",100.,0,1000.0);

    //
    //
    //
    //
    //===============GET THE TREES
    //UPTREE
    TString upPath="Systematics/"+sysNames.at(iname)+"up"+"/subTree";
    cout<<"upPath = "<<upPath<<endl;
    TTree* upTree= (TTree*)upDownFile->Get((TString)upPath);
    cout<<"the file is "<<upDownFile->GetName()<<endl;



    if (upTree==0){cout<<"did NOT get the upTree, ERROR"<<endl;}

    //DOWNTREE
    TString downPath="Systematics/"+sysNames.at(iname)+"down"+"/subTree";
    TTree* downTree= (TTree*)upDownFile->Get(downPath);
    if (downTree==0){cout<<"did NOT get the downTree, ERROR"<<endl;}
    //
    //
    //
    //===============DEFINE THE BRANCHES
    TreeVariables upVariables;
    DefineBranches(upTree,upVariables);
    //
    TreeVariables downVariables;
    DefineBranches(downTree,downVariables);
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
    cin.get();
    //
    //
    //cout<<"tree being used is "<<centralTree->GetName()<<endl;
    //cout<<"looping over "<<NMAX<<endl;
    //NMAX=1;



    for (int iEvent=0; iEvent<NMAX_up; ++iEvent){

      if (iEvent%100000 == 0)cout<<"events up"<<iEvent<<endl;

      upTree-> GetEntry(iEvent);
      MT2WMu_up->Fill(upVariables.MT2WMu,upVariables.EventWeight);

      double HTup=0;
      for (int ijet=0;(int)ijet<upVariables.Jets->size();++ijet){
	//cout<<"here5.4"<<" "<<ijet<<endl;
	JetPt_up->Fill(upVariables.Jets->at(ijet).Pt());
	HTup+=upVariables.Jets->at(ijet).Pt();
      }

      HT_up->Fill(HTup);
      double METup=0;
      MET_up->Fill(upVariables.MET,upVariables.EventWeight);
    }
    
    for (int iEvent=0; iEvent<NMAX_down; ++iEvent){
      if (iEvent%10000 == 0)cout<<"events down"<<iEvent<<endl;
      downTree->GetEntry(iEvent);
      MT2WMu_down->Fill(downVariables.MT2WMu,downVariables.EventWeight);
      double HTdown=0;

      for (int ijet=0;(int)ijet<downVariables.Jets->size();++ijet){
	double HT=0;
	JetPt_down->Fill(downVariables.Jets->at(ijet).Pt());
	HTdown+=downVariables.Jets->at(ijet).Pt();
      }

      HT_down->Fill(HTdown);
      double METdown=0;
      MET_down->Fill(downVariables.MET,downVariables.EventWeight);
    }






    //==================================================================
    for (int iEvent=0; iEvent<NMAX_central ; ++iEvent){

      if (iEvent%10000 == 0)cout<<"events central"<<iEvent<<endl;
      //cout<<"here"<<endl;
      //cout<<"here2"<<endl;
      upTree->GetEntry(iEvent);
      //cout<<"here3"<<endl;

      centralTree->GetEntry(iEvent);
      //      cout<<"here4"<<endl;
      //
      //
      //
      //========mt2wMu
      //cout<<"filling with "<<upVariables.MT2WMu<<" "<<upVariables.EventWeight<<endl;
      cout<<"the pileup weight is "<<centralVariables.EventWeight<<endl;
      MT2WMu_central->Fill(centralVariables.MT2WMu,centralVariables.EventWeight);
      
      /*      if(upVariables.MT2WMu!=0.0 && centralVariables.MT2WMu!=0){
	MT2WMu_upShift->Fill(upVariables.MT2WMu - centralVariables.MT2WMu);
	MT2WMu_Shift->Fill(upVariables.MT2WMu - centralVariables.MT2WMu);
	MT2WMu_RelShift->Fill(upVariables.MT2WMu/centralVariables.MT2WMu -1.0);
      }
      if(downVariables.MT2WMu!=0.0 && centralVariables.MT2WMu!=0.){
	MT2WMu_downShift->Fill(downVariables.MT2WMu - centralVariables.MT2WMu);
	MT2WMu_Shift->Fill(downVariables.MT2WMu - centralVariables.MT2WMu);
	MT2WMu_RelShift->Fill(downVariables.MT2WMu/centralVariables.MT2WMu-1.0);
      }
      */

      //=======JET PT AND HT
      double HTcentral=0;
      //cout<<"size of the sets"<<upVariables.Jets->size();
      for (int ijet=0;(int)ijet<centralVariables.Jets->size();++ijet){
	JetPt_central->Fill(centralVariables.Jets->at(ijet).Pt());
	HTcentral+=centralVariables.Jets->at(ijet).Pt();
      }
      HT_central->Fill(HTcentral);
      //=========MET
      double METcentral=0;
      if(centralVariables.MET<60.0){
	cout<<"the central MET is "<<centralVariables.MET<<endl;
      }
      MET_central->Fill(centralVariables.MET,centralVariables.EventWeight);
    }


    
    //MT2WMu_up->Draw("h");
    //gPad->Update();
    //cin.get();
    //exit(1);
      





    //outDir->Write();
    
    //    if(!myC){
    myC=new TCanvas("c","c",800,600);
      //    }else{
      //      myC->Clear();
      //    }


    //=============MT2WMU

    
    

    //    MT2WMu_central->SetFillColor(2);
    //    MT2WMu_central->Draw("h");
    //myC->Update();
    //    cin.get();
    //    MT2WMu_up->SetLineColor(1);
    //    MT2WMu_up->Draw("same h");
    //myC->Update();
    //cin.get();
    //    MT2WMu_down->SetLineColor(3);
    //    MT2WMu_down->Draw("same h");
    //    myC->Update();
    //    cin.get();
    //    myC->Print("MT2WMu_"+sysNames.at(iname)+".ps");

    //
    //    MT2WMu_upShift->SetFillColor(2);
    //    MT2WMu_upShift->Draw("h");
    //    myC->Update();
    //    cin.get();
    //
    //    MT2WMu_downShift->SetLineColor(3);
    //    MT2WMu_downShift->Draw("h");
    //    myC->Update();
    //    cin.get();



    //delete(centralTree);
    delete(upTree);
    delete(downTree);
    

    outDir->Write();

    //
    THStack* mystack;
    //
    mystack=plotUpDownCentral(MT2WMu_central,MT2WMu_up,MT2WMu_down,"MT2WMu_"+sysNames.at(iname));
    //cin.get();

    mystack=plotUpDownCentral(JetPt_central,JetPt_up,JetPt_down, "JetPt_"+sysNames.at(iname));
    //cin.get();

    mystack=plotUpDownCentral(HT_central,HT_up,HT_down, "HT_"+sysNames.at(iname));
    //cin.get();

    mystack=plotUpDownCentral(MET_central,MET_up,MET_down, "MET_"+sysNames.at(iname));
    //cin.get();


      
    

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
  centralTree= (TTree*)centralFile->Get("subTree");
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
  
  //  tree->SetBranchAddress("NJets",&name.NJets);
  //  tree->SetBranchAddress("treeEl",&name.treeElectron);
  //  tree->SetBranchAddress("treeMu",&name.treeMuon);
  //  tree->SetBranchAddress("jet1",&name.jet1);
  //  tree->SetBranchAddress("jet2",&name.jet2);
  //  tree->SetBranchAddress("jet3",&name.jet3);
  //  tree->SetBranchAddress("jet4",&name.jet4);
  //  tree->SetBranchAddress("jet5",&name.jet5);
  //  tree->SetBranchAddress("jet6",&name.jet6);
  //  tree->SetBranchAddress("jet7",&name.jet7);
}



vector<TString>* GetSystematicNames(TFile* upDownFile){

  vector<TString>* sysNames = new vector<TString>;
  
  TDirectory* sysDir = (TDirectory*)upDownFile->Get("Systematics");
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



