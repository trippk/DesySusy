from ROOT import gROOT, gDirectory,TCanvas, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter
from BreakDownInputPath import *
from RA4b_Samples import *
from RootObjects  import *
from ToyPlotClass import *
from DataBkgSigPlot import *

if __name__=='__main__':



    #THIRD EXAMPLE
    #
    #TAKE A LOOK AT RA4b_samples.py
    #
    #There you can see a lot of definitions of functions.
    #These functions correspond to instantiations of a class
    #called Sample, which intends to encapsulate all the properties
    #of the output of a certain NAF job.
    #
    #For example, lets retrieve the results obtained when running the RA4b-mu
    #selection over TTJets.

    ttjets=TTJets_RA4bmu_NoTail((True,True))
    #Three things:

    #1)'ttjets' is now an instantiation of Sample
    
    #2)the first True in (True,True) means that the sample should be reweighted
    #to the lumidata if it hasn't been already (it automatically detects this).

    #3) the second True means that if there are subsamples, it should merge them into a single
    #file after they have been reweighted.

    #now, ttjets.RootFile is the file that has the reweighted, merged (if necessary) results
    #for this calculation, we can open it.

    ttfile=TFile(ttjets.RootFile,"READ")

    #now lets get a data calculation

    data= MuHad_RA4bmu_NoTail((False,True))

    #It shoudl NOT reweight it to the datalumi (hence the False)
    #but it should merge the two subsamples, Run2012A and Run2012B (hence the True)

    #we can open the root file now
    
    datafile=TFile( data.RootFile,"READ")

    #Lets use the class DataBkgSigPlot for plotting a comparison between two histos
    #we can compare the cutflow of the two:

    dataflow=datafile.Get('CutFlow/global_flow_Out')
    mcflow=ttfile.Get('CutFlow/global_flow_Out_RW')


    #the class that we are using also takes a slightly-more complicated structure
    #than before, but not much.
    #lets being with providing the minimal stucture:

    #1) the properties of the plot: this can be empty
    plotProperties={}

    #2) a dictionary for the data histogram
    dataDict={}
    dataDict['MuHad']=[dataflow]

    #3) a dictionary for the mc histogram
    mcDict={}
    mcDict['TTJets']=[mcflow]

    #now we pack it all together into a list in the right order
    PACK=[plotProperties, dataDict,mcDict]

    #and now we create an isntance of the class DataBkgSigPlot:
    nicePlot=DataBkgSigPlot(PACK)
    # we can already draw it
    #
    nicePlot.Draw()
    raw_input('have a look at the current dir')
    #
    #
    #
    #
    #the plot has been stored as noName.eps,pdf and gif.
    #It is probably not very nice, since we have set no properties at all, we
    #only provided the bare minimum to make it work
    #lets make it nicer:

    dataOptDict={}
    dataOptDict['lineColor']=1
    dataOptDict['markerSie']=1
    dataOptDict['markerStyle']=20

    ttjetsOptDict={}
    ttjetsOptDict['lineColor']=GetColor('TTJets')#to use a standard color
    ttjetsOptDict['fillColor']=GetColor('TTJets')#to use a standard color
    
    #now place these to dictionaries next to the histogram pointers
    dataDict['MuHad'].append(dataOptDict)
    mcDict['TTJets'].append(ttjetsOptDict)
    
    #also, lets set the name of the output file
    plotProperties['outPlotName']='nicerPlot'
    
    #create another instance
    #note that repacking is NOT necessary, PACK already contains the changes
    nicerPlot=DataBkgSigPlot(PACK)

    nicerPlot.Draw()    
    raw_input('have another look at the current dir')
    
    
