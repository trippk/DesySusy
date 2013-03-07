#!/usr/bin/python
import sys, os
import commands
from ROOT import gROOT, gDirectory,TCanvas, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter,TColor
from FileMerger import MergeFiles
from BreakDownInputPath import *
from copy import *
#from Plotter import *
from Plot_DataBkgSigPlot import *
from Plot_SetRootObjectsOptions import GetColor
from Plot_PlottingStyles import *

from RA4b_subTrees import *
from CheckDirectory import *
from CreateHTML import *
from myPrint import *






if __name__=='__main__':

    #
    #
    #
    datasample=[]
    datasample.append("mu")
    datasample.append("el")
    #datasample.append("singleMu")
    #datasample.append("singleEl")
    #
    #
    for lep in datasample:
        
         DataOptions={}
         DataOptions['RW']=False
         DataOptions['Merge']=True
         #import the data sample============================
         if lep=="mu":
             Data=MuHad_RA4bmu_NoTail(DataOptions)
             Data.SampleLumi=5097
         elif lep=="el":
             Data=ElectronHad_RA4belHighPt_NoTail(DataOptions)
             Data.SampleLumi=5097
         elif lep=="singleMu":
             dataLumi=5101
             #
         elif lep=="singleEl":
             dataLumi=5101             
             #
         #
         #
         #===================================================

         
         #==========DIRECTORY FOR THE PLOTS=================
         #if this dir does not exist, it will be created
         topPlotsDir=Data.SampleName+'/ControlPlots/'+Data.Estimation+'/'+Data.Tail
         #
         #
         #
         #
         #
         #
         #======IMPORT THE MC SAMPLES================
         htmlOptions={}
         #
         #
         #
         MCsamples={}

         MCOptions={}
         MCOptions['RW']=True
         MCOptions['Merge']=True
         MCOptions['dataLumi']=[Data.SampleLumi]
         if lep=="mu":
             #TTJETS   
             TTJets=TTJets_RA4bmu_NoTail(MCOptions)
             MCsamples['TTJets']=[TTJets]
             #DYJETS
             DYJets=DYJets_RA4bmu_NoTail(MCOptions)
             MCsamples['DYJets']=[DYJets]
             #WJETS
             WJets=WJets_RA4bmu_NoTail(MCOptions)
             MCsamples['WJets']=[WJets]
             #SingleTop
             SingleTop=SingleTop_RA4bmu_NoTail(MCOptions)
             MCsamples['SingleTop']=[SingleTop]
             #
             #
             #
             htmlOptions['sample']=Data.SampleName
             htmlOptions['PtCut']=20.0
             htmlOptions['METCut']=60.0             
         elif lep=="el":
             #TTJETS
             TTJets=TTJets_RA4belHighPt_NoTail(MCOptions)
             MCsamples['TTJets']=[TTJets]
             #DYJETS
             DYJets=DYJets_RA4belHighPt_NoTail(MCOptions)
             MCsamples['DYJets']=[DYJets]
             #WJETS
             WJets=WJets_RA4belHighPt_NoTail(MCOptions)
             MCsamples['WJets']=[WJets]
             #SingleTop
             SingleTop=SingleTop_RA4belHighPt_NoTail(MCOptions)
             MCsamples['SingleTop']=[SingleTop]
             #
             #
             #
             htmlOptions['sample']=Data.SampleName
             htmlOptions['PtCut']=30.0
             htmlOptions['METCut']=60.0


         #SIGNAL SAMPLES
         #
         #
         Sigsamples={}
         SigOptions={}
         SigOptions['RW']=True
         SigOptions['True']=True
         SigOptions['dataLumi']=[Data.SampleLumi]
         if lep=='mu':
             #lm6
             lm6=LM6_RA4bmu_NoTail(SigOptions)
             Sigsamples['LM6']=[lm6]
             #lm9
             lm9=LM9_RA4bmu_NoTail(SigOptions)             
             Sigsamples['LM9']=[lm9]
         elif lep=='el':
             #lm6
             lm6=LM6_RA4belHighPt_NoTail(SigOptions)
             Sigsamples['LM6']=[lm6]
             #lm9
             lm9=LM9_RA4belHighPt_NoTail(SigOptions)             
             Sigsamples['LM9']=[lm9]
         #
         #
         #===================================================
         #
         #
         #
         #OPEN THE ROOT FILES===============================
         print 'data root file',Data.RootFile
         datafile=TFile(Data.RootFile,"READ")
         #
         #
         #MC
         MCsamples['TTJets'].append(TFile(str(TTJets.RootFile),"READ"))
         MCsamples['SingleTop'].append(TFile(SingleTop.RootFile,"READ"))
         MCsamples['DYJets'].append(TFile(DYJets.RootFile,"READ"))
         MCsamples["WJets"].append(TFile(WJets.RootFile,"READ"))
         #
         Sigsamples['LM6'].append(TFile(str(lm6.RootFile),"READ"))
         Sigsamples['LM9'].append(TFile(str(lm9.RootFile),"READ"))

         #
         #
         print 'USING THE FILES '
         print 'data ',Data.RootFile
         print 'ttjets',TTJets.RootFile
         print 'singletop',SingleTop.RootFile
         print 'dyjets',DYJets.RootFile
         print 'wjets',WJets.RootFile
         print 'LM6',lm6.RootFile
         print 'LM9',lm9.RootFile
         print '----------------'
         #
         raw_input()
         #
         #
         #===================================================
         #
         #
         #
         #
         #
         #====Form the empty dictionaries for the histograms
         dataDict={}
         #the key is the name of the sample, 
         datakey=Data.SampleName
         #the itemx is a list
         dataDict[datakey]=[]
         #the first entry of the list will have to be the histogram pointer
         #I put 'nil' but I will overwrite it with the pointer later on.
         dataDict[datakey].append('nil')
         #the second one is a dictionary of properties, than CAN be empty
         myoptdict={}
         dataDict[datakey].append(myoptdict)
         #
         #
         #
         #====Form the empty dictionaries for the MC histograms
         mcDict={}
         for sampleskey,sampleslist in MCsamples.iteritems():

             #for the MC, it has to be the same key
             #as for the MCsample dictionary.
             mckey=sampleskey
             #same thing, I put 'nil' in here but will put the right
             #pointer afterwards
             mcDict[mckey]=['nil']
             myoptdict={}
             #
             mcDict[mckey].append(myoptdict)

         #CONSTRUCT THE empty SIGNAL DICTIONARY, OPTIONAL
         sigDict={}
         for sampleskey, sampleslist in Sigsamples.iteritems():
             #
             #
             sigDict[sampleskey]=['nil']
             myoptdict={}
             #
             sigDict[sampleskey].append(myoptdict)
         #   
         #
         #
         #
         #
         #
         #
         #
         #====PLOTS====#
         if lep=='mu':
             Plots=['MT2WMu']
             Plots.append('MTMu')             
         elif lep=='el':
             Plots=['MT2WEle']
             Plots.append('MTEle')                          


         Plots.append('HT')
         #Plots.append('RegionA')
         #Plots.append('RegionB')
         #Plots.append('RegionC')
         #Plots.append('RegionD')         
         #
         #
         plotsdir=Data.SampleName+'/Plots'
         myDataDir=datafile
         print myDataDir.Print()
         for num,plot in enumerate(Plots):
             #print 'currently at plot ',plot
             print 'mydatadir is ',myDataDir
             #
             #
             plotPath=plotsdir+'/'+plot
             #
             #===============GET THE DATA HISTOGRAM=========================
             #
             #
             datahist = myDataDir.Get(plot)
             print 'datahist = ',datahist
             print 'plotpath is ',plotPath
             datahist.SetDirectory(0)
             #
             #
             if str(datahist).find('nil')!=-1:
                 raise('data histogram not found')
             #
             for datakey,dataList in dataDict.iteritems():
                 #PUTTING IN A HISTO CLONE
                 dataclone=datahist.Clone()
                 dataclone.SetDirectory(0)
                 dataList[0]=dataclone
                 #
                 #Get the style of the histo from the
                 #standard style and put it in the dictionary
                 dataList[1]=Style_DataHistogram()
             #
             #
             dataPath=str(myDataDir.GetPath())
             dataPath_index=dataPath.rfind(':')
             dataPath=dataPath[dataPath_index+2 : ]
             if dataPath=='':
                 PlotPath=plot
             else:
                 PlotPath=dataPath+"/"+plot
             #
             #==============GET THE MONTE CARLO HISTOGRAMS=================
             #
             #
             for mckey,mclist in MCsamples.iteritems():
                 #key is a string with the name of the sample
                 #mclist[0] is the datasample
                 #mclist[1] is the opened root file
                 mcfile=mclist[1]
                 #
                 #
                 dummyhist = mcfile.Get(PlotPath)
                 dummyhist.SetDirectory(0)
                 #
                 #
                 if str(dummyhist).find('nil') != -1:
                     print plot, 'was not found '
                     raise NameError('ERROR, the histo was not found')
                 #====UPDATE THE MC DICTIONARY WITH THE HISTO POINTER CLONE
                 histoclone=dummyhist.Clone()
                 histoclone.SetDirectory(0)
                 mcDict[mckey][0]=histoclone
                 #SET THE DICTIONARY OPTIONS FROM THE STANDARDS
                 #GetColor is defined in SetRootObjectsOptions
                 mcDict[mckey][1]['lineColor']=GetColor(mckey)
                 mcDict[mckey][1]['fillColor']=GetColor(mckey)
                 mcDict[mckey][1]['NoErrors']=True
                 #


             #===========GET THE SIGNAL SAMPLES
             #
             #
             
             for sigkey,siglist in Sigsamples.iteritems():

                 sigfile=siglist[1]
                 dummyhist=sigfile.Get(PlotPath)

                 #raw_input()
                 #
                 if str(dummyhist).find('nil') != -1:
                     print plot, 'was not found '
                     print 'in the file',sigkey
                     raise NameError('ERROR, the histo was not found')
                 #===UPDATE THE SIGNAL DICTIONARY
                 histoclone=dummyhist.Clone()
                 histoclone.SetDirectory(0)
                 sigDict[sigkey][0]=histoclone
                 sigDict[sigkey][1]=Style_SignalHistogram(sigkey)

             #
             #
             #
             #
             #NOW CONSTRUCT THE PLOT WITH THESE HISTOGRAMS
             #
             #
             #===FIRST CONSTUCT THE PROPERTIES OF THE PLOTS
             #get some standard properties
             PlotProperties=Style_ControlPlots()
             PlotProperties['xtitle_ratioplot']=GetUnits(datahist.GetName())
             PlotProperties['tpave_text']='Work in progress #int Ldt = '+str(Data.SampleLumi)+' pb^{-1}, #sqrt{s} = 8 TeV'
             PlotProperties['StackMaximum']=10.0 #times the default maximum
             #
             #
             #PlotProperties['fillstyle_ratioplot']=3144
             #PlotProperties['fillcolor_ratioplot']=TColor.kYellow-7
             #PlotProperties['drawingOptions_ratioplot']='ep'
             #
             #if you want the MC not stacked, uncomment the next line
             #PlotProperties['drawingOptions_stack']='NoStack'
             #
             #
             #
             #
             
             #
             #PATH OF THE SAVED PLOT
             currentDir=Data.SampleName+'/Plots'
             outPlotName=datahist.GetName()
             PlotProperties['outPlotName']=outPlotName
             #======================================
             #
             #
             #
             #
             #
             #      CREATE THE PLOT
             #
             #
             #
             #
             #
             #
             #========================================
             #Lets create the input list for the class that we are using
             
             #PROTECTION AGAINST EMPTY PLOTS:
             if dataDict.values()[0][0].GetEntries()!=0:
                 #
                 #
                 InputList=['StackWithRatio',PlotProperties,dataDict,mcDict,sigDict]
                 newPlot=DataBkgSigPlot(InputList)
                 #
                 #      DRAW IT
                 newPlot.Draw()
                 #
                 #
                 #     PUT THE PLOTS IN THE CORRESPONDING DIR:
                 CheckDirectory(currentDir)
                 #
                 commands.getoutput('mv '+outPlotName+'*'+' '+currentDir)
                 #
                 #
                 #
                 #CREATE AN HTML FILE
                 plotdescription=datahist.GetTitle()                 
                 #CreateHTML(outPlotName,plotdescription,currentDir)
                 #myHTML.AddPlot(outPlotName,plotdescription,currentDir)
         #        
         #


         #======WRITE THE HTML FILES
         #myHTML.DumpToFile()


         
    #loop over samples


#END
