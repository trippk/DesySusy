
#this is an example of how to use the Plot_PlotPair.py

import Plot_PlotMultiple as pm
import Plot_PlottingStyles as style
from ROOT import TH1D


def ExamplePlotMultiple():
    """example"""

    #CREATE THE HISTOS
    h1=TH1D("h1","first histo",50,-5.0,5.0)
    h2=TH1D("h2","second histo",50,-5.0,5.0)
    h3=TH1D("h3","third histo",50,-5.0,5.0)
    h4=TH1D("h4","fourth histo",50,-5.0,5.0)            

    #FILL THEM
    h1.FillRandom('gaus',1000)
    h2.FillRandom('gaus',2000)
    h3.FillRandom('gaus',3000)
    h4.FillRandom('gaus',100000)        

    #PACK THEM INTO A LIST WITH 2-TUPLES OF NAME,HISTO
    myList=[]
    name1='h1'
    name2='h2'
    name3='h3'
    name4='h4'    
    myList.append((name1,h1))
    myList.append((name2,h2))
    myList.append((name3,h3))
    myList.append((name4,h4))


    
    #DICTIONARY OF PLOT PROPERTIES
    PlotProperties={}
    Properties={}
    Properties=style.Style_ControlPlots(0)
    Properties['drawNormalized']=True 
    Properties['pad1_logy']=0 #1 for log plot

    #names appearing in the legend
    Properties[name1+'_LegendTag']='gaus1'
    Properties[name2+'_LegendTag']='gaus1'
    Properties[name3+'_LegendTag']='gaus1'
    Properties[name4+'_LegendTag']='gaus1'    

    #histo colors
    Properties[name1+'_LineColor']=1
    Properties[name2+'_LineColor']=2
    Properties[name3+'_LineColor']=4
    Properties[name4+'_LineColor']=6


    #
    Properties['variableName']='normal distribution'
    Properties['psFileName']='example_plotmultiple.pdf'
    Properties['draw_option']='hist nostack'
    pm.PlotMultiple(myList,Properties)
    #
    #
    #
if __name__=='__main__':
    
    ExamplePlotMultiple()
    
