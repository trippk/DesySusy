from ROOT import gROOT, gDirectory,TCanvas, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter
from Plot_RootObjects  import *
from ToyPlotClass import *


if __name__=="__main__":

    #==========================================================
    #     SECOND EXAMPLE
    #
    #lets create one histogram
    second=TH1D('mysecond','the second histogram',50,-10.0,10.0)
    second.FillRandom("gaus",10000)    
    #and now another
    third=TH1D('mythird','the third histogram',50,-10.0,10.0)
    third.FillRandom("gaus",10000)
    #
    #lets define the properties of the plotted histograms
    properties1={}
    properties1['lineColor']=1
    properties1['fillColor']=2
    properties1['axisColor']=4
    properties1['histoTitle']='mySecondHistogram'
    #CREATE AN INSTANCE OF THE TOYCLASS FOR THE FIRST
    mytoy1=ToyPlotterClass(second,properties1)
    #
    #
    properties2={}
    properties2['lineColor']=4
    properties2['fillColor']=8
    properties2['axisColor']=9
    properties2['histoTitle']=''
    #CREATE AN INSTANCE OF THE TOYCLASS FOR THE SECOND
    mytoy2=ToyPlotterClass(third,properties2)    
    #
    #
    #
    #BUILD A NEW CANVAS
    print 'building the canvas'
    canv_properties={}
    canv_properties['canvas_Name']='mySecondCanvas'
    mycanvas2=BuildCanvas(canv_properties)
    #BUILD A TPAD (also defined in RootObjects)
    tpadproperties={}
    pad1=BuildTPad(tpadproperties,'pad1',0.0,0.33,0.99,0.99)
    #AND ANOTHER
    pad2=BuildTPad(tpadproperties,'pad2',0.0,0.07,0.99,0.3)
    #
    #DRAW THEM
    pad1.Draw()
    pad2.Draw()
    #
    #
    pad1.cd()
    mytoy1.Draw()
    mycanvas2.Update()    
    #
    #
    pad2.cd()
    mytoy2.Draw()
    #    
    #
    #
    mycanvas2.Update()
    raw_input('hit some key to continue')

