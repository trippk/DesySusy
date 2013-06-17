#!/usr/bin/python


import sys, os
import commands
import subprocess
from time import asctime
import random
from ROOT import gROOT, SetOwnership,gDirectory,TCanvas,TObject,TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText,TIter,TListIter
from BreakDownInputPath import *
import copy


def LoopAndScale(dir,Weight):
    """goes through all histos and reweights them"""

    nextkey=TIter(dir.GetListOfKeys())
    key=nextkey()


    #=================================================
    #Iterate over the histos in the file
    #ListOfNewH=[]

    while(str(key).find('nil') == -1):

        #GET THE KEY
        obj=dir.Get(key.GetName() )

        #print 'the current directory is ',gDirectory.pwd()
        #rint 'currently at object',obj.GetName()
        #raw_input()
        #
        #IF a histo
        #
        #
        if obj.IsA().InheritsFrom("TH1"):

            
            newhist=obj.Clone(key.GetName())
            SetOwnership(newhist,False)
            newhist.Scale(Weight)

        elif obj.IsA().InheritsFrom("TDirectory"):

            newdir=gDirectory.mkdir(obj.GetName())
            newdir.cd()
            LoopAndScale(obj,Weight)
            #
            #
            #set it to return back to where it was
            gDirectory.GetMotherDir().cd()
        else:
            print "Unknown object"
            print "It is called ",key.GetName()
        #
        #dir.cd()
        key=nextkey()
    


        #for obj in ListOfNewH:
        #if obj.IsA().InheritsFrom("TH1"):
            #print 'going to write ',obj.GetName()
        #    obj.Write("",TObject.kOverwrite)        
    





def ReweightFile(filepath,LumiData,paraFile="para_config.txt"):

    #raw_input('going to make the call with '+str(filepath))
    FileName,InDir,Sample,SubSample,Estimation,Tail,AbsPath = BreakDownInputPath(filepath)

    #=====the output should be the same as
    #=====the filename + '_RW2X', where X is the lumi
    OutFileName=filepath
    print ""
    print "---------------------------------------------"
    print "INSIDE ReweightFile"
    print '   filepath= ',str(filepath)
    print '   filename= ',OutFileName
    #
    #
    OutFileName=OutFileName.replace('.root','_RWTo'+str(int(LumiData))+'.root')
    #OutFileName=AbsPath.replace(FileName,OutFileName)
    #
    #
    print ''
    print "   Going to reweight the file",filepath, "to the lumi",LumiData
    print ""
    print "          the outFileName will be ",OutFileName
    
    #INPUT FILE#
    infile=TFile(filepath,"READ")
    #except:
    #print "Unexpected error:", sys.exc_info()[0]
    #print 'infile is ',str(infile)
    #return
    infile.cd()

    if not infile.IsOpen():
        print "       the file could not be opened. Bye"
        return
    #============
    #
    #
    #
    #
    outfile=TFile(OutFileName,"RECREATE")
    outfile.cd()
    #
    #
    #
    #=======PARAMETERS FOR THE REWEIGHTING
    parameters_dict={}
    config=open(paraFile,'r')
    for line in config:
        if  line.find('#') == -1:        
            thisline=line.split()
            #
            if len(thisline)==0:
                continue
            try:
                parameters_dict[thisline[0]]=float(thisline[2])
            except IndexError:
                print "this is not going to work"
                print "thisline is", thisline
                print 'and the index is ',thisline[0]
                raise
        else:
            pass
    #===========
    filtereff_key='FE_'+Sample+'_'+SubSample
    try:
        FE=parameters_dict[filtereff_key]
    except KeyError:
        print filtereff_key, "does not exist in the dictionary"
        
        return
    #
    xsec_key='xs_'+Sample+'_'+SubSample
    try:
        XS=parameters_dict[xsec_key]
    except KeyError:
        print xsec_key, "does not exist in the dictionary"
        return
    #
    tnoe_key='TNoE_'+Sample+'_'+SubSample
    try:
        TNOE=parameters_dict[tnoe_key]
    except KeyError:
        print tnoe_key, "does not exist in the dictionary"
        return
    #


    #COMPUTE THE WEIGHTS
    #if LumiData==-1:
        #LumiData=5097.
    print "----Reweighting information----"
    print '        XS =',XS
    print '        FE =',FE
    print '        TNOE =',TNOE
    Weight=float(LumiData)*XS*FE/TNOE;
    print "        LumiData =",LumiData
    print "        cross section is",XS
    print "        FE is ", FE
    print "        number of entries ",TNOE
    print "        the weight is ",Weight
    #raw_input("ready to continue?")

    #
    #
    #
    #REWEIGHT ALL THE HISTOS IN THE FILE:
    LoopAndScale(infile,Weight)

    #outfile.Write()
    #
    #
    #
    #
    #MAKE THE ROOT FILE REMEMBER
    outfile.cd()
    histoname='RWto'+str(int(LumiData))
    isrw=infile.Get(histoname)
    #
    #
    if str(isrw).find('nil') != -1:
        #create it
        newh=TH1D(histoname,"",1,0.5,1.5)
        newh.Fill(1.0)
        newh.Write()
        #
    datehist=outfile.Get("date_of_reweighting_to_"+str(int(LumiData)))
    date=commands.getoutput('date +%s')    
    if str(datehist).find('nil') != -1:

        dateh=TH1D("date_of_reweighting_to_"+str(int(LumiData)),date,1,0.5,1.5)
        dateh.Fill(1.0)
        dateh.Write()
    else:
        datehist.SetTitle(date)
        datehist.Write("",TObject.kOverwrite)
    #

    outfile.Write()
    print "EXITING ReweightFile. Bye"
    print "----------------------------------"
    print ""
    return OutFileName
    #
    #
    #
    #
def ReweightFileWithMacro(filepath):

    """it uses a root macro for the reweighting"""
    
    FileName,InDir,SampleName,SubSample,Estimation,Tail,AbsPath = BreakDownInputPath(filepath)
        
    OutFileName=FileName
    ESCPARopen='\('
    ESCPARclose='\)'
    ESCQUOTE='\\"'
    comma=','


    oscommand= 'root -l -b -q ReweightFile.C++' + ESCPARopen + \
    ESCQUOTE + filepath    + ESCQUOTE + comma + \
    ESCQUOTE + InDir       + ESCQUOTE + comma + \
    ESCQUOTE + SampleName  + ESCQUOTE + comma + \
    ESCQUOTE + SubSample   + ESCQUOTE + comma + \
    ESCQUOTE + Estimation  + ESCQUOTE + comma + \
    ESCQUOTE + Tail        + ESCQUOTE + ESCPARclose

    
    #ESCQUOTE + Tail        + ESCQUOTE + comma + \
    #ESCQUOTE + OutFileName + ESCQUOTE + ESCPARclose

    print "about to execute the command ", oscommand
    out=commands.getoutput(oscommand)
    #print out
    if(out.find("error") != -1 or out.find("not") != -1 ):
        print out
    #


if __name__ == "__main__":

    """Reweights a file to a given luminosity"""
    #

    filepath=sys.argv[1]
    if len(sys.argv)>2:
        lumi=sys.argv[2]
    else:
        lumi=5097
    #
    
    print "GOINT TO REWEIGHT THE FILE ",filepath
    print "TO THE LUMINOSITY ",lumi
    #ReweightFile("SingleTop/T-s-channel/SingleTop_T-s-channel_RA4b-mu_NoTail.root",) 
    ReweightFile(filepath,lumi)

