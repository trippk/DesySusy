from ROOT import gROOT, gDirectory,TCanvas, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter
from Plot_DataBkgSigPlot import *
from Plot_PlottingStyles import *
from Plot_SetRootObjectsOptions import GetColor
from Plot_RootObjects import GetUnits

#AN EXAMPLE OF A DATA PLOT VERSUS 3 STACKED MC PLOTS
#
#
#Create the histograms
dataH=TH1D('muon_pt','the pt of a gluino',50,0.0,100.0)
ttjets=TH1D('ttjets_muon_pt','the pt of a gluino',50,0.0,100.0)
wjets=TH1D('wjets_muon_pt','the pt of a gluino',50,0.0,100.0)
singletop=TH1D('singletop_pt','the pt of a gluino',50,0.0,100.0)
lm6=TH1D('lm6_pt','the pt of a gluino',50,0.0,100.0)
#
#Fill them
gaus =  gROOT.GetFunction("gaus");
gaus.SetParameter("Mean",50)
gaus.SetParameter("Sigma",50)
dataH.FillRandom("gaus",15000)
ttjets.FillRandom("gaus",10000)
wjets.FillRandom("gaus",6000)
singletop.FillRandom("gaus",4000)

landau= gROOT.GetFunction("landau");
landau.SetParameter("Sigma",30)
landau.SetParameter("Mpv",30)
lm6.FillRandom("landau",500)

#
#SET THE HISTOGRAM PROPERTIES DICTIONARY
#
#remember: it's a dictionary with the form {name  :   [ histo, properties_dictionary ] }
#                                            |            |             |
#              where this things are        key        list[0]       list[1]
#
#
#Data
#Style_DataHistogram returns a dictionary with
#some properties common for data histograms.
dataProperties=Style_DataHistogram()
#
dataDict={}
#
##the first element of the list is the histogram.
#'data' is now the key
dataDict['data']=[dataH]
#the second element of the list is the dictionary of properties
dataDict['data'].append(dataProperties)
#extend the dictionary obtained with the function Style_DataHistogram
#with a new property, which is the x label.
dataDict['data'][1]['xtitle_ratioplot']="P_{T}"
#dataDict['data'][1]['xtitle_ratioplot']=GetUnits(dataH.GetName())
#
#
#
#
#
sigProperties=Style_DataHistogram()
sigDict={}
sigDict['signal']=[lm6]
sigDict['signal'].append(sigProperties)
sigDict['signal'][1]['markerColor']=5
#======MC SAMPLES
mcDict={}
mcDict['ttjets']=[ttjets]
mcDict['wjets']=[wjets]
mcDict['singletop']=[singletop]
#set the properties dictionary
for key,values in mcDict.iteritems():
    values.append({})
    values[1]['lineColor']=GetColor(key)
    values[1]['fillColor']=GetColor(key)
#
#
#
#
#=====PLOT PROPERTIES
#
#get a standard dictionary from Style_ControlPlots
PlotProperties=Style_ControlPlots()
#extend it for the name of the plot
PlotProperties['outPlotName']='testPlot'
#=========================
#
#
#
#CREATE THE PLOT
#
#
#
#=========================
InputList=['StackWithRatio',PlotProperties,dataDict,mcDict,sigDict]
newPlot=DataBkgSigPlot(InputList)
#
# DRAW IT
#
#
newPlot.Draw()



