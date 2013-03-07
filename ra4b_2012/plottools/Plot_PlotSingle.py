import os,sys 
from ROOT import TFile, TCanvas, THStack,TLegend,gPad,gROOT

import plottools.Plot_RootObjects as rootobj
import plottools.Plot_SetRootObjectsOptions as setopt
import plottools.Plot_PlottingStyles as plotSt



def PlotSingle(h1,Properties):
    #========CANVAS
    myC=rootobj.BuildCanvas({})
    #=========TPAD1===========
    #pad1_logy=Properties.get('pad1_logy',1)            
    pad1=rootobj.BuildTPad(Properties,'pad1',0.1,0.3,0.9,0.9)

    pad1.Draw()

    #========SET THE OPTIONS
    h1.SetStats(Properties.get('h1SetStats',0))
    h1.SetLineColor(Properties.get('h1LineColor',1))
    h1.SetLineWidth(Properties.get('h1LineWidth',2))
    if Properties.get('drawNormalized',False)==True:
        print 'normalizing...'
        if h1.Integral() != 0:
            h1.Scale(1/h1.Integral())


    h1.SetTitle(Properties.get('plotTitle',""))
    h1.GetXaxis().SetTitle(Properties.get('xAxisTitle',""))
    h1.GetYaxis().SetTitle(Properties.get('YAxisTitle',""))    
    
    h1.Draw(Properties.get('draw_option','h'))
    #
    #
    #============TPAVE 
    mytpave=rootobj.BuildTPaveText(Properties)
    #
    #
    mytpave.Draw()
    #
    #
    myC.SaveAs(Properties.get('psFileName','defaultPsName.ps'))    
