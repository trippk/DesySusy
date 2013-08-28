TF1* fun(const char* nam,double b,double s,double sys,int col,int style=1){
	TF1* stc = new TF1(nam,"[1]*x/300./sqrt([0]*x/300.+([2]*[0]*x/300.)^2)",1,3000);
	stc->SetParameter(0,b+s);
	stc->SetParameter(1,s);
	stc->SetParameter(2,sys);
	stc->SetLineColor(col);
	stc->SetLineStyle(style);
	return stc;
}
void hadron_excl_lumi(){

	double  b   = 2161;
	double  s4  = 2033;
	double  s5  = 1121;
	double  s6  =  820;
	double  s8  =  621;
//	double  sys =  0.25;
//	TString Sys =  "0.25";
	double  sys =  0.15;
	TString Sys =  "0.15";

	TString what="0-lepton analysis";

	TCanvas* c1 = new TCanvas("c1","",600,600);
	c1->SetLogx();
	c1->SetGridx();
	c1->SetGridy();

	TH1F* frame = new TH1F("frame","",1,1,3000);
	frame->SetMinimum(0);
	frame->SetMaximum(15);
	frame->SetXTitle("int. luminosity [fb^{-1}]");
	frame->SetYTitle("Exclusion sensitivity [#sigma]");
	frame->GetYaxis()->SetTitleOffset(1.4);
	frame->GetXaxis()->SetTitleOffset(1.2);
	frame->SetStats(0);
	frame->SetTitleFont(22,"x");
	frame->SetTitleFont(22,"y");
	frame->Draw();
	TLegend* leg = new TLegend(0.58,0.692,0.9,0.9,"50 pileup events");

	TF1* stc4 = fun("stc4",b,s4,sys,1);
	stc4->Draw("same");
	leg->AddEntry(stc4,"STC4","l");

	TF1* stc5 = fun("stc5",b,s5,sys,kGreen+1,2);
	stc5->Draw("same");
	leg->AddEntry(stc5,"STC5","l");

	TF1* stc6 = fun("stc6",b,s6,sys,4,2);
	stc6->Draw("same");
	leg->AddEntry(stc6,"STC6","l");

	TF1* stc8 = fun("stc8",b,s8,sys,6);
	stc8->Draw("same");
	leg->AddEntry(stc8,"STC8","l");


	leg->Draw();
	c1->Draw();

	TLine* line = new TLine(1,2,3000,2);
	line->SetLineColor(kRed);
	line->SetLineWidth(3);
	line->Draw();
	TLatex* t1 = new TLatex(2,14,"#sqrt{s} = 14 TeV, #deltaB_{sys}/B = "+Sys);
	t1->SetTextSize(0.03);
	t1->Draw();
	TLatex* t2 = new TLatex(2,13,what);
	t2->SetTextSize(0.03);
	t2->Draw();
	c1->SaveAs("hadron_excl_sys"+Sys+".pdf");


}
