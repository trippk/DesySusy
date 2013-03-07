import os,sys 
from ROOT import TFile, TCanvas, THStack,TLegend,gPad,gROOT

import Plot_RootObjects as rootobj
import Plot_SetRootObjectsOptions as setopt
import Plot_PlottingStyles as plotSt






def PlotMultiple(hList,Properties):
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
    #get the histos
    for i,h in enumerate(hList):
        htag=h[0]
        histo=h[1]

        if i==0:
            histo.SetStats(Properties.get(htag+'_SetStats',0))
        #    
        histo.SetLineColor(Properties.get(htag+'_LineColor',1))
        histo.SetLineWidth(Properties.get(htag+'_LineWidth',2))

        
        if Properties.get('drawNormalized',False)==True:
            print 'normalizing...'
            if histo.Integral() != 0:
                histo.Scale(1/histo.Integral())


    myS=rootobj.BuildTHStack({})
    for i,h in enumerate(hList):
        htag=h[0]
        histo=h[1]
        myS.Add(histo)
    #
    #
    myS.Draw(Properties.get('draw_option','hist nostack'))
    #
    #
    #=======LEGEND
    
    myLegend=rootobj.BuildLegend({})
    for i,h in enumerate(hList):
        htag=h[0]
        histo=h[1]
        legendTag=Properties.get(htag+'_LegendTag','')
        myLegend.AddEntry(histo,legendTag)
    #
    myLegend.Draw()
    #

    
    #=============RATIO PLOT====================
    #////////////////////////////////////////////
    pad2.cd()
    #set the title of the ratio plot as the name of the variable
    h1=hList[0][1]
    h2=hList[1][1]
    ratioplot=rootobj.BuildRatioPlot(h2,h1,{'xtitle_ratioplot':Properties['variableName']})
    ratioplot.SetLineColor(h2.GetLineColor())
    ratioplot.Draw()

    for i,h in enumerate(hList):
        if i>0:
            h1=hList[0][1]
            h2=h[1]
            ratioplot=rootobj.BuildRatioPlot(h2,h1,{'xtitle_ratioplot':Properties['variableName']})
            ratioplot.SetLineColor(h2.GetLineColor())
            if i==1:
                ratioplot.Draw()
            else:
                ratioplot.Draw("same")
                gPad.Update()
            #
        #
    #    
    
    #============TLINE
    line=rootobj.BuildTLine(ratioplot.GetXaxis().GetXmin(),1,ratioplot.GetXaxis().GetXmax(),1,Properties)
    line.Draw()

    #============TPAVE 
    mytpave=rootobj.BuildTPaveText(Properties)
    
    pad1.cd()
    mytpave.Draw()

    #myC.SaveAs('test.pdf')
    myC.SaveAs(Properties.get('psFileName','defaultPsName.ps'))

    
