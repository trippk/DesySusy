
from ROOT import TH1,TH1F,TH1D,TH2D
from array import *


#
def GetLowEdges(histo):
    #
    axis=histo.GetXaxis()
    mylist=[]

    #get an array with GetNbins+1 values
    for bin in range(1,axis.GetNbins()+2 ):
        mylist.append(axis.GetBinLowEdge(bin))
        #print 'doing it ',bin
    #print 'length of the array is ',len(array('d',mylist))
    return array('d',mylist)

def MergeLastBins(histo,Properties):

    howManyBins=Properties.get('rebin_howManyBins')
    #whatBins=10 means to rebin the last 10 bins
    #
    #
    #print 'the histo has ',histo.GetNbinsX(),' bins'
    oldLowEdges=GetLowEdges(histo)
    newLowEdges=[]
    for num,i in enumerate(oldLowEdges):

        #print 'num and i ',num,i
        #copy the first entries
        if num+1 <= histo.GetNbinsX()+1 - howManyBins:
            #print 'appending ',i
            newLowEdges.append(i)
            #
        else:
            #copy the last onw
            newLowEdges.append(oldLowEdges[-1])
            break
        #
        #
    newLowEdges=array('d',newLowEdges)
    #=====check
    if( len(newLowEdges) != histo.GetNbinsX()-howManyBins+2):
        #print 'the size of the new low edges is not correct'
        #print 'len(newLowEdges) = ',len(newLowEdges)
        #print 'histo size = ',histo.GetNbinsX()
        #print 'howManyBins ' ,howManyBins
        return 0
    else:
        return newLowEdges


def RebinAll(histo,Properties):
    '''rebin all bins'''
    newName=Properties.get('rebin_newName',histo.GetName()+'_rebin')        
    rebinngroup=Properties.get('rebin_ngroup',1)
    histo.Rebin(rebinngroup,histo.GetName()+'_'+newname)
    
def Rebin(histo,Properties):


    #PARAMETERS OF REBIN
    #ngroup=Properties.get('rebin_ngroup')
    newName=Properties.get('rebin_newName',histo.GetName()+'_rebin')    

    rebinMode=Properties['rebin_rebinMode']
    if rebinMode=='mergeLastBins':
        lowEdges=MergeLastBins(histo,Properties)
        ngroup=histo.GetNbinsX()-Properties.get('rebin_howManyBins') +1
    else:
        print 'this does nothing'
    #
    #
    #DO IT
    #print 'ngroup is ',ngroup
    #print 'low edges size ',len(lowEdges)
    #print lowEdges
    #
    h=histo.Rebin(ngroup,histo.GetName()+'_'+newName,lowEdges)


    return h







