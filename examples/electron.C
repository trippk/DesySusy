// Usage:  root -l electron.C+
// ----
//         dcache access: in that! order
//              ini cmssw
//		ini glite
//		voms-proxy-init --voms cms;
//		root -l electron.C+
// Available files: https://twiki.cern.ch/twiki/bin/view/CMS/LocalData

#include <set>
#include <map>
#include <vector>
#include <iostream>
#include "TFile.h"
#include "TStopwatch.h"
#include "NtupleTools2.h"

using namespace std;

void electron(){
	EasyChain* tree = new EasyChain("/susyTree/tree");
	TStopwatch t;
	t.Start();
//	------local files --------------------------------
//	tree->Add("nTuple_1.root");
//	tree->Add("nTuple_2.root");
//      ------ dcache single files -----------------------
//	tree->Add("dcap://dcache-ses-cms.desy.de:22125/pnfs/desy.de/cms/tier2/store/user/kruecker/ntuples/LM1_Spring10-START3X_V26_S09-v1_GEN-SIM-RECO/nTuple_1_1.root");
//	tree->Add("dcap://dcache-ses-cms.desy.de:22125/pnfs/desy.de/cms/tier2/store/user/kruecker/ntuples/LM1_Spring10-START3X_V26_S09-v1_GEN-SIM-RECO/nTuple_2_1.root");
//	tree->Add("dcap://dcache-ses-cms.desy.de:22125/pnfs/desy.de/cms/tier2/store/user/kruecker/ntuples/LM1_Spring10-START3X_V26_S09-v1_GEN-SIM-RECO/nTuple_3_1.root");
//      ------ dcache many files -------------------------
//	const string dir="/pnfs/desy.de/cms/tier2/store/user/kruecker/MinimumBias_Commissioning10-May6thPDSkim2_SD_JetMETTau-v1/";
	//AllRootFilesIn(dir,tree,"dcls")   // dcache -  needs proxy & dctools !
	//AllRootFilesIn(dir,tree)          // any 'ls'-able directory
	//AllRootFilesIn(dir,tree,10)       // any 'ls'-able directory, first 10 files
	//first 10 files in dir
//	AllRootFilesIn(dir,tree,"dcls",10); // needs proxy & dctools !
//      ------- castor single files ----------------------
//	tree->Add("rfio:/castor/cern.ch/user/e/elaird//SusyCAF/automated/2010_07_05_13_33_34/SusyCAF_Tree_2_1_eLv.root");
//	tree->Add("rfio:/castor/cern.ch/user/e/elaird//SusyCAF/automated/2010_07_05_13_33_34/SusyCAF_Tree_1_1_2gp.root");
//      ------- castor many files ------------------------
//	AllRootFilesIn("/castor/cern.ch/user/e/elaird//SusyCAF/automated/2010_07_05_13_33_34/",tree,"rfdir"); 
//      ------- lustre many files -------------------------
	const string dir="/scratch/hh/current/cms/user/kruecker/ntuples/QCD/QCD_Pt30/Summer10-START36_V9_S09-v1/GEN-SIM-RECODEBUG_START37_V5_Spring10/";
	AllRootFilesIn(dir,tree,"ls");
//---------------------------------------------------------------------------------------------------------------------------
//	tree->Add("/scratch/hh/current/cms/user/kruecker/ntuples/LM0_Spring10-START3X_V26_S09-v1_GEN-SIM-RECO_nTuple_1_1.root");
//	tree->Add("/scratch/hh/current/cms/user/kruecker/ntuples/LM0_Spring10-START3X_V26_S09-v1_GEN-SIM-RECO_nTuple_2_1.root");
//	tree->Add("/home/kruecker/Work/ntuples/LM0_Spring10-START3X_V26_S09-v1_GEN-SIM-RECO_nTuple_1_1.root");
//	tree->Add("/home/kruecker/Work/ntuples/LM0_Spring10-START3X_V26_S09-v1_GEN-SIM-RECO_nTuple_2_1.root");
	t.Stop();
	cout<<tree->GetNtrees()<<" files read. Real: "<<t.RealTime()<<" CPU: "<<t.CpuTime()<<endl;
	int N=tree->GetEntries();

	//cuts
	double elMinPt   = 3.; //6
	double elMaxEta  = 2.5;
	const string eid = "EIDRobustLoose";

	int cntEevts=0;
	int cntE=0;
	int cntELoose=0;
	double m_pt=0;
	double m_ptLoose=0;
	double m_relIso=0;
	double m_relIsoLoose=0;
	double m_metP4CaloEt=0;
	set<int> runs;
	TStopwatch timer;
	timer.Start();
	for(int i=0;i<N;++i){

		tree->GetEntry(i);

		//single variables
		unsigned event = tree->Get(event,"event");
		unsigned   run = tree->Get(run,  "run");
		runs.insert(run);

		//references
		vector<LorentzV>& Electrons = tree->Get(&Electrons,"electronP4Pat");
		if( Electrons.size()!=0) cntEevts++;
		else continue;

		//pointers
		LorentzV*         metP4Calo = tree->Get(&metP4Calo,"metP4Calo");
		m_metP4CaloEt+=metP4Calo->Et();

		//pointers
		XYZPointD*  beamSpotPosition = tree->Get(&beamSpotPosition, "beamSpotPosition");
		//references to vectors
		vector<float>&      ecalIso  = tree->Get(&ecalIso,          "electronEcalIsoPat");
		vector<float>&      hcalIso  = tree->Get(&hcalIso,          "electronHcalIsoPat");
		vector<float>&     trackIso  = tree->Get(&trackIso,         "electronTrackIsoPat");
		vector<float>&   eleidLoose  = tree->Get(&eleidLoose,       "electronEIDLoosePat");
		vector<float>&        eleid  = tree->Get(&eleid,            "electron"+eid+"Pat");
/*
		map<string,bool>& triggered        = tree->Get(&triggered,       "triggered");
		map<string,int>&  prescaled        = tree->Get(&prescaled,       "prescaled");
		map<string,bool>& L1triggered      = tree->Get(&L1triggered,     "L1triggered");
		map<string,int>&  L1prescaled      = tree->Get(&L1prescaled,     "L1prescaled");
		map<string,int>&  L1prescaled      = tree->Get(&L1prescaled,     "L1prescaled");
		map<string,int>&  parasiticTrigger = tree->Get(&parasiticTrigger,"parasiticTrigger");
		// may need options.register('SourceName', "DS:EG", ... in exampleTree_cfg.py
		if( triggered["HLT_Ele10_LW_L1R"] ) {
			cout<<"HLT_Ele10_LW_L1R triggered"<<endl;
		};
*/

		for(unsigned int k=0;k<Electrons.size();k++){
			if( eleid[k]==0 && eleidLoose[k]==0 ) continue;
			if(Electrons[k].pt()<elMinPt)               continue;
			if(fabs(Electrons[k].eta())<elMaxEta)       continue;
			double relIso = (trackIso[k]+ecalIso[k]+hcalIso[k])/Electrons[k].pt();
			if( eleid[k]==1) {
				cntE++;
				m_pt+=Electrons[k].pt();
				m_relIso += relIso;
			}
			if( eleidLoose[k]==1) {
				cntELoose++;
				m_ptLoose+=Electrons[k].pt();
				m_relIsoLoose += relIso;
			}

		}
		progress();
	}
	timer.Stop();
	cout<<"Real time: "<<timer.RealTime()<<" CPU:"<<timer.CpuTime()<<endl;

	cout<<"Events "<<N<<endl;
	cout<<"electron evts "<<cntEevts<<", electrons ("<<eid<<") "<<cntE<<", (Loose)  "<<cntELoose<<endl;
	cout<<"electron <pt>     ("<<eid<<") "<<m_pt/cntE<<endl;
	cout<<"electron <relIso> ("<<eid<<") "<<m_relIso/cntE<<endl;
	cout<<"electron <pt>     (Loose)  "<<m_ptLoose/cntELoose<<endl;
	cout<<"electron <relIso> (Loose)  "<<m_relIsoLoose/cntELoose<<endl;
	cout<<"calo MET in el.evts <Et>  "<<m_metP4CaloEt/cntEevts<<endl;
	cout<<endl;
	cout<<"Runs: ";
	for(set<int>::iterator it=runs.begin();it!=runs.end();++it) cout<<(*it)<<" ";cout<<endl;
}
