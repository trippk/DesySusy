from Plot_StackWithRatio import *
from Plot_RootObjects import *
from Plot_SetRootObjectsOptions import *
from ROOT import gROOT,gPad,gDirectory,TCanvas,TLine, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter,TPad
#
#
#

class DataBkgSigPlot:
    """a class for plots of data,backgrounds and signals
    The input is a list that should contain:
    
    1) A dictionary of properties for the plot, with the key being the name
    of the property and the item is its value.

    2)dataDict is a dictionary with a string as the key, presumably the sample name,
    and a list as the item. The first element HAS to be the pointer to the histogram. If there is a second
    element it, is a dictionary with properties for the histogram.
    
    4 and 5) the same for background and signal samples.
    """

    def __init__(self,InputList):

        self.plotClass=InputList[0]
        self.plotProperties=InputList[1]
        self.dataDict=InputList[2]
        self.bkgDict=InputList[3]
        if len(InputList)>4:
            self.sigDict=InputList[4]
        else:
            self.sigDict={}

            #
        self.suffixList=[]
        #
        #CREATE A PLOT
        exec('self.Plot='+self.plotClass+'(self.plotProperties)')
        #self.SR=StackWithRatio(self.plotProperties)
        #self.Plot=StackPlot(self.plotProperties)
        #
        #
        if len(self.dataDict)==0:
            raise NameError("dataDict is empty")
        if len(self.bkgDict)==0:
            raise NameError("bkgDict is empty")
        if len(self.sigDict)==0:
            pass
            #print '**********************************'
            #print 'WARNING: NO SIGNALS ARE INTRODUCED'
            #print '**********************************'            
        #
        #
        #
        #
    def AddData(self):
        """adds all the histograms in the data dictionary"""
        for hname,dataList in self.dataDict.iteritems():

            #=================================================================
            #dataDict contains the name of the sample as key of the dictionary
            #and a List as the item.
            #the List contains the pointer to the histo as first element (always)
            #and a dictionary of properties as the second, (optional)
            #================================================================
            dataHist=dataList[0]
            if str(dataHist).find('nil')!= -1:
                raise NameError("the data histogram is a null pointer!")
            #
            #OPTIONS CAN BE SET HERE BUY UPDATING THE DICTIONARY dataList[1]
            if len(dataList)>1:
                #SetStandardOptions(dataList[1],"DataHistogram")
                SetHistoOptions(dataList[0],dataList[1])
         


            #
            #ADD IT
            self.Plot.AddData(dataList[0])
    #=============================================



    #==============================================
    # BACKGROUND  HISTOGRAMS
    #==============================================
    def AddBackgrounds(self):
        if len(self.bkgDict)==0:
            #print '*****************************************************************'
            #rint 'WARNING: there are no background samples being used, are you sure this is correct?'
            #print '*****************************************************************'            
            #
            pass
        else: 
            for hname,bkgList in sorted(self.bkgDict.iteritems()):

                #--get the histo pointer
                bkgHisto=bkgList[0]
                if str(bkgHisto).find('nil') != -1:
                    raise NameError('The mc histo is a null pointer for the sample ',hname)
            
                #--get the dictionary of options and properties
                optDict={}
                if len(bkgList)>1:
                   optDict=bkgList[1]

                #set the properties of the histogram
                SetHistoOptions(bkgHisto,optDict)
                
                #if specified, scale also the histogram
                weight=optDict.get('histo_weight',1.0)
                bkgHisto.Scale(weight)
                #
                #finally, add it to the stack.
                self.Plot.AddStack(bkgHisto)
            #
        #
    #
    #==============================================
    # SIGNAL  HISTOGRAMS
    #==============================================
    def AddSignals(self):
        if len(self.sigDict)==0:
            print '*************************************************************************'
            print 'WARNING: there are no signal samples being used, are you sure this is correct?'
            print '*************************************************************************'            
            #
        else: 
            for hname,sigList in self.sigDict.iteritems():
                #--get the histo pointer
                sigHisto=sigList[0]
                if str(sigHisto).find('nil') != -1:
                    print 'is called ',hname
                    raise NameError('The sigal histo is a null pointer for the sample ',hname)
                
                #--get the dictionary of options and properties
                optDict=sigList[1]

                #set the properties of the histogram
                SetHistoOptions(sigHisto,optDict)
                
                #if specified, scale also the histogram
                weight=optDict.get('histo_weight',1.0)
                #sigHisto.Scale(weight)
                #
                #finally, add it to the stack.
                self.Plot.AddSignal(sigHisto)
            #
        #
    #
    #===============================================



    def Draw(self):
        """Draws the plot and saves it"""


        #
        #
        #====ADD HISTOS
        self.AddData()
        self.AddBackgrounds()
        self.AddSignals()
        #
        #
        #
        #=====BUILD THE CANVAS
        canv=BuildCanvas(self.plotProperties)        
        #
        #
        #
        #=======PLOT THE stackwithratio ON IT
        self.Plot.Draw()
        canv.Update()
        #
        #
        #
        #=======BUILD A LEGEND
        leg=BuildLegend(self.plotProperties)
        #DATA
        for dataname,dataList in self.dataDict.iteritems():
            if len(dataList)>1:
                legend_entry=dataList[1].get('legend_entry',dataname)
                legend_opt=dataList[1].get('legend_opt','lep')
            else:
                legend_entry=dataname
                legend_opt='lep'
            leg.AddEntry(dataList[0],legend_entry,legend_opt)
        #BACKGROUNDS
        #I use reverse to have the same color structure in the legend
        #as the one I have in the stack
        for bkgname,bkgList in reversed(sorted(self.bkgDict.iteritems())):
            if len(bkgList)>1:
                legend_entry=bkgList[1].get('legend_entry',bkgname[2:])
                legend_opt=bkgList[1].get('legend_opt','f')
            else:
                legend_entry=dataname
                legend_opt='f'
            leg.AddEntry(bkgList[0],legend_entry,legend_opt)
        #SIGNAL
        for signame,sigList in self.sigDict.iteritems():
            if len(sigList)>1:
                legend_entry=sigList[1].get('legend_entry',signame[2:])
                legend_opt=sigList[1].get('legend_opt','lp')
            else:
                legend_entry=dataname
                legend_opt='lp'        
            leg.AddEntry(sigList[0],legend_entry,legend_opt)
        #
        #
        leg.Draw()
        #
        #
        #
        #
        #
        #
        #=============TPAVETEXT================
        #tlat=BuildTPaveText(self.plotProperties)
        #tlat.Draw()
        #
        #raw_input('noooow')
        canv.Update()
        #
        #
        #
        #=========SAVE AS PDF AND GIF==
        #
        #
        outplotname=self.plotProperties.get('outPlotName','noName')
        for suffix in self.suffixList :
            canv.SaveAs(outplotname+suffix)
        #===============================


    def SetOutputSuffixList(self, listOfOutputSuffices):
        self.suffixList = listOfOutputSuffices

    
if __name__=='__main__':
    
    print "hi!"

