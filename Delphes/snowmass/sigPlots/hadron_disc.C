TF1* fun(const char* nam,double b,double s,int col,int style=1){
	TF1* stc = new TF1(nam,"[1]/sqrt([0]+(x*[0])^2)",-0.001,0.5);
	stc->SetParameter(0,b);
	stc->SetParameter(1,s);
	stc->SetLineColor(col);
	stc->SetLineStyle(style);
	return stc;
}
void hadron_disc(){
	// unchanged
	double b = 2161;
	double s4= 2033;
	double s5= 1121;
	double s6=  820;
	double s8=  621;
	TString what="0-lepton analysis";

	TCanvas* c1 = new TCanvas("c1","",600,600);
	c1->SetLogy();
	c1->SetGridx();
	c1->SetGridy();

	TH1F* frame = new TH1F("frame","",1,0,0.5);
	frame->SetMinimum(1);
	frame->SetMaximum(100);
	frame->SetXTitle("#deltaB_{sys}/B");
	frame->SetYTitle("Discovery sensitivity [#sigma]");
	frame->GetYaxis()->SetTitleOffset(1.4);
	frame->GetXaxis()->SetTitleOffset(1.2);
	frame->SetStats(0);
//	frame->SetTitle("LHC #sqrt{s} = 14 TeV, #scale[0.6]{#int}Ldt = 300 fb^{-1}");
	frame->SetTitleFont(22,"x");
	frame->SetTitleFont(22,"y");
	frame->Draw();
	TLegend* leg = new TLegend(0.58,0.692,0.9,0.9,"50 pileup events");

	TF1* stc4 = fun("stc4",b,s4,1);
	stc4->Draw("same");
	leg->AddEntry(stc4,"STC4","l");

	TF1* stc5 = fun("stc5",b,s5,kGreen+1,2);
	stc5->Draw("same");
	leg->AddEntry(stc5,"STC5","l");

	TF1* stc6 = fun("stc6",b,s6,4,2);
	stc6->Draw("same");
	leg->AddEntry(stc6,"STC6","l");

	TF1* stc8 = fun("stc8",b,s8,6);
	stc8->Draw("same");
	leg->AddEntry(stc8,"STC8","l");


	leg->Draw();
	c1->Draw();
//	TPaveText *pt = (TPaveText*)(c1->GetPrimitive("title")); 
//	pt->SetTextFont(22);
//	pt->SetTextSize(0.04);
//	c1->Modified();
	TLine* line = new TLine(0,5,0.5,5);
	line->SetLineColor(kGreen);
	line->SetLineWidth(3);
	line->Draw();
	TLatex* t1 = new TLatex(0.01,80,"#sqrt{s} = 14 TeV, #scale[0.6]{#int}Ldt = 300 fb^{-1}");
	t1->SetTextSize(0.03);
	t1->Draw();
	TLatex* t2 = new TLatex(0.015,60,what);
	t2->SetTextSize(0.03);
	t2->Draw();
	c1->SaveAs("hadron_disc_lumi300.pdf");



}
