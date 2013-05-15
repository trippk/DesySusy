import os,sys 
from ROOT import TFile, TCanvas, THStack,TLegend,gPad,gROOT

import Plot_RootObjects as rootobj
import Plot_SetRootObjectsOptions as setopt
import Plot_PlottingStyles as plotSt






def PlotPair(h1,h2,Properties):
    #========CANVAS
    myC=rootobj.BuildCanvas({})
    #=========TPAD1===========
    #pad1_logy=Properties.get('pad1_logy',1)            
    pad1=rootobj.BuildTPad(Properties,'pad1',0.1,0.3,0.9,0.9)
    #=========TPAD2===========    
    pad2=rootobj.BuildTPad(Properties,'pad2',0.1,0.1,0.9,0.3)
    #=======DRAW THE PADS=====

    pad1.Draw()
    pad2.Draw()        
    pad1.cd()
    pad1.DrawFrame(0.001,0.001,1.,1.)
    pad2.cd()
    pad2.DrawFrame(0.001,0.001,1.,1.)
    #
    #
    pad1.cd()
    #
    #
    #
    #========SET THE OPTIONS
    h1.SetStats(Properties.get('h1SetStats',0))
    h1.SetLineColor(Properties.get('h1LineColor',1))
    h1.SetLineWidth(Properties.get('h1LineWidth',2))
    h2.SetLineColor(Properties.get('h2LineColor',2))
    h2.SetLineWidth(Properties.get('h2LineWidth',2))
    if Properties.get('drawNormalized',False)==True:
        print 'normalizing...'
        if h1.Integral() != 0:
            h1.Scale(1/h1.Integral())
        if h2.Integral() != 0:            
            h2.Scale(1/h2.Integral())

    myS=rootobj.BuildTHStack({})
    myS.Add(h1)
    myS.Add(h2)
    myS.Draw(Properties.get('draw_option','hist e nostack'))
    myS.SetMinimum(Properties.get('stack_minimum',myS.GetMinimum()))
    #h1.Draw("h")
    #h2.Draw("h same")
    #
    #
    #=======LEGEND
    myLegend=rootobj.BuildLegend({})
    h1Tag=Properties.get('h1Tag','h1')
    h2Tag=Properties.get('h2Tag','h2')
    #
    #
    #
    myLegend.AddEntry(h1,h1Tag)
    myLegend.AddEntry(h2,h2Tag)
    myLegend.Draw()
    #

    
    #=============RATIO PLOT====================
    #////////////////////////////////////////////
    pad2.cd()
    #set the title of the ratio plot as the name of the variable
    variableName=Properties.get('variableName','')
    print 'puta ',variableName
    ratioplot_up=rootobj.BuildRatioPlot(h2,h1,{'xtitle_ratioplot':variableName})
    ratioplot_up.SetLineColor(h2.GetLineColor())
    ratioplot_up.Draw()
    gPad.Update()

    
    #============TLINE
    line=rootobj.BuildTLine(ratioplot_up.GetXaxis().GetXmin(),1,ratioplot_up.GetXaxis().GetXmax(),1,Properties)
    line.Draw()

    #============TPAVE 
    mytpave=rootobj.BuildTPaveText(Properties)
    
    pad1.cd()
    mytpave.Draw()

    #myC.SaveAs('test.pdf')
    myC.SaveAs(Properties.get('psFileName','defaultPdfName.pdf'))    
