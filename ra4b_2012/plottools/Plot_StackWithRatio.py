from Plot_RootObjects import *
from Plot_SetRootObjectsOptions import *
from ROOT import SetOwnership,gROOT,gPad,gDirectory,TCanvas,TLine, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter,TPad
from ROOT import TLine
import copy
#======================================
class StackWithRatio:
    """a class to plot stacks and a ratio plot"""

    #def __init__(self,dataLumi,xT,yST,yRT,optDict):
    def __init__(self,Properties):
        #
        #
        self.rebOff=0  # rebinning offset
        self.rebN=0  # # bins to rebin
        #self.lumi=Properties.get('dataLumi')  # guess what
        self.xmin=0.0
        self.xmax=0.0
        self.pad1=0.0
        self.pad2=0.0
        self.theStack=BuildTHStack(Properties)        
        self.sumMC=0
        self.dataHistograms=[]
        self.signalHistograms=[]
        self.Properties=Properties
    #======================================





        
    #======================================
    #ADD BACKGROUND TO THE STACK
    #======================================
    def AddStack(self,histo):
        """it adds a certain histogram to the background stack """
        #dont forget about the rebinning
        if str(histo).find('nil') != -1:
            raise NameError('You are trying to add a null pointer to the stack')
        #
        self.theStack.Add(histo)

        if(self.sumMC==0):
            self.sumMC=histo.Clone()
        else:
            self.sumMC.Add(histo)
    #==========================================



    #======================================
    #ADD THE DATA
    #======================================
    def AddData(self,histo):
        """it simply adds a data histogram to the list"""
        self.dataHistograms.append(histo)
    #======================================


    #======================================
    #ADD THE SIGNAL
    #======================================
    def AddSignal(self,histo):
        """add a signal histogram to the list"""
        #print 'line width ',histo.GetLineWidth()
        self.signalHistograms.append(histo)
    #======================================        


    #======================================
    #DRAW THE OBJECTS
    #======================================
    def Draw(self):
        """draws the whole thing, stack + data + signals"""
        
        if self.sumMC==0:
            print "WARNING: no stack plot"
        if len(self.dataHistograms)==0:
            print "WARNING: no data plots have been added"
        if len(self.signalHistograms)==0:
            pass
            #print "WARNING: no signal plots have been added"

        if str(gPad).find('nil')==0:
            raise NameError('No Canvas has been defined at this point')
        #
        #=====GET DATA HISTOGRAM
        datah=self.dataHistograms[0]
        #
        #
        #=========TPAD1===========
        #pad1_logy=self.Properties.get('pad1_logy',1)

        pad1=BuildTPad(self.Properties,'pad1',0.1,0.3,0.9,0.9)
        #
        #
        #=========TPAD2===========
        pad2_logy=self.Properties.get('pad2_logy',0)        
        pad2=BuildTPad(self.Properties,'pad2',0.1,0.1,0.9,0.3)
        #
        #=======DRAW THE PADS=====
        self.pad1=pad1
        self.pad2=pad2
        pad1.Draw()
        pad2.Draw()        
        pad1.cd()
        pad1.DrawFrame(0.001,0.001,1.,1.)
        pad2.cd()
        pad2.DrawFrame(0.001,0.001,1.,1.)
        #
        #
        #
        #
        #
        #
        #
        #
        #===============================
        #=======ON THE FIRST PAD
        #===============================
        pad1.cd()
        #pad1.DrawFrame(0.,0.,1.,1.)
        #
        #
        #
        #=========SET AND DRAW THE STACK
        self.theStack.Draw(self.Properties.get('drawingOptions_stack','h'))
        #self.theStack.Draw('h')
        SetTHStackOptions(self.theStack,self.Properties)
        self.theStack.SetMinimum(self.Properties.get('StackMinimum',0.1))
        stackmaximum=self.Properties.get('StackMaximum')
        if stackmaximum != None:
            currentmax=self.theStack.GetMaximum()
            self.theStack.SetMaximum(currentmax*self.Properties.get('StackMaximum'))
        #
        #=========DRAW THE DATA
        datah.Draw(self.Properties.get('drawingOptions_data','same p e1'))
        gPad.Update()        
        #raw_input('before signal')        
        

        #=========DRAW THE SIGNALS
        for sig in self.signalHistograms:
            sig.Draw(self.Properties.get('drawingOptions_sig','same '))
            gPad.Update()
            #raw_input('signal added')
        #==========================
        #
        #
        #
        #
        #
        #
        #
        #=============RATIO PLOT====================
        #////////////////////////////////////////////
        pad2.cd()
        #pad2.DrawFrame(0.,0.,1.,1.)        
        ratioplot=BuildRatioPlot(datah,self.sumMC,self.Properties)
        #
        #
        #============DRAW THE RATIO PLOT
        ratioplot.Draw(self.Properties.get('drawingOptions_ratioplot','ep'))
        gPad.Update()
        #
        #
        #============DRAW A LINE IN RATIO 
        self.line=BuildTLine(ratioplot.GetXaxis().GetXmin(),1,ratioplot.GetXaxis().GetXmax(),1,self.Properties)
        self.line.Draw()
        gPad.Update()

        #set the active pad back to the first one
        #
        #
        pad1.cd()

        

        

        
                
    #=====================================
    #REBIN
    #=====================================    
    def Rebin(self,histo):
        """rebinning of a histogram"""
        nb=histo.GetNbinsX()
        widht=histo.GetBinWidth(1)
        ax=histo.GetXaxis()
        xmin=ax.GetXmin()
        xmax=ax.GetXmax()

        N=nb-self.rebOff + 1
        bins=[]
        bins.append(xmin)
        bins.append(ax.GetBinLowEdge(rebOff+1))

        k=0
        for i in xrange(2,int(N)):
            bins.append(bins[i-1]+rebN*width)
            if bins[i]>= xmax:
                bins[i]=xmax
                k=1
                break
            #
        #
        hname=histo.GetName()
        newH =histo.Rebin(k-1,hname+'_reb',bins)
        #
        histo.Delete()
        histo=newH #does this work???
        #
        return histo

