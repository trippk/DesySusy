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

using namespace std;


class HistoPack{
private:
  
  int NBins_PV;
  int NBins_HT;
  int NBins_MET;
public:
  
  TH1D* MT2WMu;
  TH1D* MTMu;
  TH1D* MT2WEl;
  TH1D* MT2;
  TH1D* JetPt;
  TH1D* HT;
  TH1D* MET;

  vector<TH1D*>MTMu_PrimaryVtx;
  vector<TH1D*>MTMu_HT;
  vector<TH1D*>MTMu_MET;
  //
  vector<TH1D*>MET_PrimaryVtx;
  vector<TH1D*>MET_HT;
  vector<TH1D*>MET_MET;
  //
  vector<TH1D*>JetPt_PrimaryVtx;
  vector<TH1D*>JetPt_HT;
  vector<TH1D*>JetPt_MET;

  HistoPack(TString suffix){
    
    NBins_PV=3;
    NBins_HT=3;
    NBins_MET=3;

    MT2WMu= new TH1D("MT2WMu"+suffix,"MT2WMu"+suffix,100,0.,500);    
    MTMu= new TH1D("MTMu"+suffix,"MTMu"+suffix,100,0.,500);    
    MT2WEl= new TH1D("MT2WEl"+suffix,"MT2WEl"+suffix,100,0.,500);    
    MT2= new TH1D("MT2"+suffix,"MT2"+suffix,100,0.,500);    
    JetPt=new TH1D("JetPt"+suffix,"JetPt"+suffix,30,20.0,500.0);
    HT=new TH1D("HT"+suffix,"HT"+suffix,20,100.0,1000.0);
    MET=new TH1D("MET"+suffix,"MET"+suffix,100.,0,1000.0);

    for( int ibins=0; ibins<NBins_PV;++ibins){
      MTMu_PrimaryVtx.push_back(new TH1D("MTMu"+suffix+"_PV_bin"+Form("%d",ibins),"MTMu"+suffix+"_PV_bin"+Form("%d",ibins),100,0.0,500.0));
      MET_PrimaryVtx.push_back(new TH1D("MET"+suffix+"_PV_bin"+Form("%d",ibins),"MET"+suffix+"_PV_bin"+Form("%d",ibins),100,0.0,500.0));
      JetPt_PrimaryVtx.push_back(new TH1D("JetPt"+suffix+"_PV_bin"+Form("%d",ibins),"JetPt"+suffix+"_PV_bin"+Form("%d",ibins),100,0.0,500.0));
    }
    for( int ibins=0; ibins<NBins_HT;++ibins){
      MTMu_HT.push_back(new TH1D("MTMu"+suffix+"_HT_bin"+Form("%d",ibins),"MTMu"+suffix+"_HT_bin"+Form("%d",ibins),100,0.0,500.0));
      MET_HT.push_back(new TH1D("MET"+suffix+"_HT_bin"+Form("%d",ibins),"MET"+suffix+"_HT_bin"+Form("%d",ibins),100,0.0,500.0));
      JetPt_HT.push_back(new TH1D("JetPt"+suffix+"_HT_bin"+Form("%d",ibins),"JetPt"+suffix+"_HT_bin"+Form("%d",ibins),100,0.0,500.0));
    }
    for( int ibins=0; ibins<NBins_MET;++ibins){
      MTMu_MET.push_back(new TH1D("MTMu"+suffix+"_MET_bin"+Form("%d",ibins),"MTMu"+suffix+"_MET_bin"+Form("%d",ibins),100,0.0,500.0));
      MET_MET.push_back(new TH1D("MET"+suffix+"_MET_bin"+Form("%d",ibins),"MET"+suffix+"_MET_bin"+Form("%d",ibins),100,0.0,500.0));
      JetPt_MET.push_back(new TH1D("JetPt"+suffix+"_MET_bin"+Form("%d",ibins),"JetPt"+suffix+"_MET_bin"+Form("%d",ibins),100,0.0,500.0));
    }
  }
};




//void systematicUncertainties(TString _centralFile, TString _upDownFile){
void METStudies(int intData, TString _centralFile, TString _upDownFile, TString _outFile){




  void LoopAndPlot(TreeVariables&, HistoPack&, TTree*);

  int lib=gSystem->Load("libRA4b.so");
  if (lib != 0){
    cout<<"the library did not load properly"<<endl;
  }
  
  bool isData;
  if(intData !=0){
    isData=true;
  }else{
    isData=false;
  }


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



  //CENTRAL TREE
  TTree* centralTree= (TTree*)centralFile->Get("subTree");
  cout<<"centralTree pointer is "<<centralTree<<endl;
  TreeVariables centralVariables;
  DefineBranches(centralTree,centralVariables);


  //HISTOS

  TCanvas* myC;

  if(!isData){
    for (int iname=0; iname<(int)sysNames.size();++iname){
      //==========CREATE A DIR IN THE OUTPUTFILE
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
      //
      //===============GET THE TREES
      //UPTREE
      TString upPath="Systematics/"+sysNames.at(iname)+"up"+"/subTree";
      //cout<<"upPath = "<<upPath<<endl;
      TTree* upTree= (TTree*)upDownFile->Get((TString)upPath);
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
      //cin.get();
      //
      //
      //cout<<"tree being used is "<<centralTree->GetName()<<endl;
      //cout<<"looping over "<<NMAX<<endl;
      //NMAX=1;



      LoopAndPlot(centralVariables,centralHistos,centralTree);
      LoopAndPlot(upVariables,upHistos,upTree);
      LoopAndPlot(downVariables,downHistos,downTree);





      //=============================================
      //LOOP OVER THE UP TREE
      //=============================================
      /*      for (int iEvent=0; iEvent<NMAX_up; ++iEvent){


	if (iEvent%100000 == 0)cout<<"events up"<<iEvent<<endl;

	upTree-> GetEntry(iEvent);
	MT2WMu_up->Fill(upVariables.MT2WMu,upVariables.EventWeight);
	MTMu_up->Fill(upVariables.MTMu,upVariables.EventWeight);

	double HTup=0;
	for (int ijet=0;(int)ijet<upVariables.Jets->size();++ijet){
	  //cout<<"filling the jet with "<<upVariables.Jets->at(ijet).Pt()<<endl;
	  //cout<<"here5.4"<<" "<<ijet<<endl;
	  JetPt_up->Fill(upVariables.Jets->at(ijet).Pt(),upVariables.EventWeight);
	  HTup+=upVariables.Jets->at(ijet).Pt();
	}

	HT_up->Fill(HTup, upVariables.EventWeight);
	double METup=0;
	MET_up->Fill(upVariables.MET,upVariables.EventWeight);
      }
    

      //as a function of the number of primary vertices
      //if(








      //=============================================
      //LOOP OVER THE DOWN TREE
      //=============================================
      for (int iEvent=0; iEvent<NMAX_down; ++iEvent){
	if (iEvent%10000 == 0)cout<<"events down"<<iEvent<<endl;
	downTree->GetEntry(iEvent);
	MT2WMu_down->Fill(downVariables.MT2WMu,downVariables.EventWeight);
	MTMu_down->Fill(downVariables.MTMu,downVariables.EventWeight);


	double HTdown=0;
	for (int ijet=0;(int)ijet<downVariables.Jets->size();++ijet){
	  double HT=0;
	  JetPt_down->Fill(downVariables.Jets->at(ijet).Pt(), downVariables.EventWeight);
	  HTdown+=downVariables.Jets->at(ijet).Pt();
	}

	HT_down->Fill(HTdown,downVariables.EventWeight);
	double METdown=0;
	MET_down->Fill(downVariables.MET,downVariables.EventWeight);
      }






      //=============================================
      //LOOP OVER THE CENTRAL TREE
      //=============================================
      for (int iEvent=0; iEvent<NMAX_central ; ++iEvent){

	if (iEvent%10000 == 0)cout<<"events central"<<iEvent<<endl;
	centralTree->GetEntry(iEvent);
	//      cout<<"here4"<<endl;
	//
	//
	//
	//========mt2wMu
	//cout<<"filling with "<<upVariables.MT2WMu<<" "<<upVariables.EventWeight<<endl;
	//cout<<"the pileup weight is "<<centralVariables.EventWeight<<endl;
	MT2WMu_central->Fill(centralVariables.MT2WMu,centralVariables.EventWeight);
	MTMu_central->Fill(centralVariables.MTMu,centralVariables.EventWeight);
      
	//=======JET PT AND HT
	double HTcentral=0;
	for (int ijet=0;(int)ijet<centralVariables.Jets->size();++ijet){
	  JetPt_central->Fill(centralVariables.Jets->at(ijet).Pt(), centralVariables.EventWeight);
	  HTcentral+=centralVariables.Jets->at(ijet).Pt();
	}
	HT_central->Fill(HTcentral,centralVariables.EventWeight);
	//=========MET
	double METcentral=0;
	if(centralVariables.MET<60.0){
	  cout<<"the central MET is "<<centralVariables.MET<<endl;
	}
	MET_central->Fill(centralVariables.MET,centralVariables.EventWeight);
      }
      */

    
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
      //THStack* mystack;
      //
      //mystack=plotUpDownCentral(MT2WMu_central,MT2WMu_up,MT2WMu_down,"MT2WMu_"+sysNames.at(iname));
      //cin.get();

      //mystack=plotUpDownCentral(JetPt_central,JetPt_up,JetPt_down, "JetPt_"+sysNames.at(iname));
      //cin.get();

      //mystack=plotUpDownCentral(HT_central,HT_up,HT_down, "HT_"+sysNames.at(iname));
      //cin.get();

      //      mystack=plotUpDownCentral(MET_central,MET_up,MET_down, "MET_"+sysNames.at(iname));
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
  }


  else{


    //IF IT IS A DATA FILE
    outfile->cd();
    TH1D* MT2WMu= new TH1D("MT2WMu"," ",100,0.,500);    
    TH1D* MTMu= new TH1D("MTMu","central ",100,0.,500.);    
    TH1D* MT2WEl= new TH1D("MT2WEl","central ",100,0.,500.);    
    TH1D* MT2= new TH1D("MT2","central ",100,0.,500.);    
    TH1D* JetPt=new TH1D("JetPt","JetPt",30,20.0,500.0);
    TH1D* MET=new TH1D("MET","MET",100.,0,1000.0);
    TH1D* HT=new TH1D("HT","HT",20,100.0,1000.0);

    TString dataPath="subTree";
    TTree* dataTree=(TTree*)centralFile->Get((TString)dataPath);
    if(!dataTree){cout<<"did NOT get the data tree, ERROR"<<endl;}
    //===============DEFINE THE BRANCHES
    TreeVariables dataVariables;
    DefineBranches(dataTree,dataVariables);
    
    //==============EVENT LOOP===============//
    int NMAX=dataTree->GetEntries();
    for (int iEvent=0; iEvent<NMAX; ++iEvent){
      
      if (iEvent%100000 == 0)cout<<"events data"<<iEvent<<endl;
      
      dataTree-> GetEntry(iEvent);
      MT2WMu->Fill(dataVariables.MT2WMu,1.0);
      MTMu->Fill(dataVariables.MTMu,1.0);
      
      double HTdata=0;
      for (int ijet=0;(int)ijet<dataVariables.Jets->size();++ijet){
	JetPt->Fill(dataVariables.Jets->at(ijet).Pt(),1.0);
	HTdata+=dataVariables.Jets->at(ijet).Pt();
      }
      
      HT->Fill(HTdata, 1.0);
      MET->Fill(dataVariables.MET,1.0);
    }

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

  int NEvents=tree->GetEntries();
  for(int iEvent=0;iEvent<NEvents;++iEvent){
    if (iEvent%10000 == 0)cout<<"events up"<<iEvent<<endl;	
    
    tree->GetEntry(iEvent);
    double EW=variables.EventWeight;
    histos.MT2WMu->Fill(variables.MT2WMu, EW);
    histos.MTMu->Fill(variables.MTMu,EW);
    double HT=0;
    for(int ijet=0; ijet<(int)variables.Jets->size();++ijet){
      
      histos.JetPt->Fill(variables.Jets->at(ijet).Pt(), EW);
      HT+=variables.Jets->at(ijet).Pt();
    }
    histos.HT->Fill(HT,EW);
    histos.MET->Fill(variables.MET,EW);
    

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

}


double FillJetsPt(TH1D* histo, double Weight, std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > >*  jets){
  double HT=0;
  for(int ijet=0; ijet<(int)jets->size();++ijet){
    histo->Fill(jets->at(ijet).Pt(), Weight);
    HT+=jets->at(ijet).Pt();
  }
  return HT;
}

