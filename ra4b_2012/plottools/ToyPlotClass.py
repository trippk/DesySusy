

from ROOT import gROOT, gDirectory,TCanvas, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter



def ToySetHistoOptions(hist,options):
    """sets the options of a histogram"""

    lineColor=options.get('lineColor')
    fillColor=options.get('fillColor')
    axisColor=options.get('axisColor')
    markerStyle=options.get('markerStyle')
    markerColor=options.get('markerStyle')
    histoTitle=options.get('histoTitle')

    if(lineColor != None):
        hist.SetLineColor(lineColor)
    if(fillColor != None):
        hist.SetFillColor(fillColor)
    if(axisColor != None):
        hist.SetAxisColor(axisColor)
    if(markerStyle != None):
        hist.SetMarkerStyle(markerStyle)
    if(markerColor != None):
        hist.SetMarkerColor(markerColor)        
    if(histoTitle != None):
        hist.SetTitle(histoTitle)
        
    

class ToyPlotterClass():
    """
    An example class to print TH1 histograms
    THe th1_reference is simply the TH1 that we
    would like to plot.
    The options_dictionary is a python dictionary in which
    the names of the properties are the keys and the values of the dictionary
    as the values of the properties
    """
    def __init__(self,th1_reference,options_dictionary):
        """this is the constructor
        it will set two things, the histogram and the options dictionary"""
        self.myHist=th1_reference.Clone()
        self.options=options_dictionary
        #
        
    #

    def Draw(self):
        #HERE THE OPTIONS ARE IMPLEMENTED
        ToySetHistoOptions(self.myHist,self.options)
        #
        #DRAW IT!
        self.myHist.Draw()
    #


    
