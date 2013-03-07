
#this is an example of how to use the Plot_PlotPair.py

import Plot_PlotPair as pp
import Plot_PlottingStyles as style
from ROOT import TH1D


def ExamplePlotPair():
    """example"""

    #CREATE THE HISTOS
    h1=TH1D("h1","first histo",50,-5.0,5.0)
    h2=TH1D("h2","second histo",50,-5.0,5.0)    

    #FILL THEM
    h1.FillRandom('gaus',1000)
    h2.FillRandom('gaus',2000)

    #DICTIONARY OF PLOT PROPERTIES
    PlotProperties={}
    Properties={}
    Properties=style.Style_ControlPlots(0)
    Properties['drawNormalized']=True 

    Properties['pad1_logy']=0 #1 for log plot

    #names appearing in the legen
    Properties['h1Tag']='gaus1'
    Properties['h2Tag']='gaus2'

    #
    Properties['variableName']='normal distribution'
    Properties['psFileName']='example_plotpair.pdf'
    Properties['draw_option']='hist nostack'
    pp.PlotPair(h1,h2,Properties)
    #
    #
    #
if __name__=='__main__':
    
    ExamplePlotPair()
    
