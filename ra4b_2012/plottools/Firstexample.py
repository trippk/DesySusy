
from ROOT import gROOT, gDirectory,TCanvas, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter
from Plot_RootObjects  import *
from ToyPlotClass import *


if __name__=='__main__':

    """A little PyRoot macro to illustrate some of the ingredients in
    the more complex examples"""

    #==========================================================
    #     FIRST EXAMPLE
    #
    #lets create one histogram
    first=TH1D('myfirst','the first histogram',50,-10.0,10.0)
    first.FillRandom("gaus",10000)
    #lets define the properties of the plotted histogram
    #the properties are packed into dictionaries, so that
    #the key is the property name
    #and the dictioniary value is the value of the property
    #
    #
    properties={}#this means 'properties' is a python dictionary
    properties['lineColor']=1
    properties['fillColor']=2
    properties['axisColor']=4
    properties['histoTitle']='myFirstHistogram'
    #
    #
    #CREATE AN INSTANCE OF THE TOYCLASS
    #the code is defined in ToyPlotClass
    #
    #
    mytoy=ToyPlotterClass(first,properties)
    #
    #
    #BUILD A CANVAS
    #The code for this is defined in RootObjects.py
    print 'building the canvas'
    properties['canvas_Name']='myFirstCanvas'
    mycanvas=BuildCanvas(properties)
    #
    #
    #NOW PLOT mytoy
    mytoy.Draw()
    mycanvas.Update()
    raw_input('hit some key to continue')

