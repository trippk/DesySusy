
import sys, os
import commands
import subprocess
from time import asctime
import random


def BreakDownInputPath(filepath):
    """Extracts the directory, sample name, estimation and tail of a given input file"""  


    #First Establish whether it is a merged file
    
    if not "MERGED" in filepath:        

        #the input file should have sample_subsample_estimation_tail
        #and it should be stored in sample/subsample

        #is it a root file?
        if filepath[-5:] != ".root":
            print "Not a root file!!"
            raise
        #
        #

        ###########
        #BEGIN NEW VERSION
        ###########
        
        #Get the filename
        if (filepath.count("/") < 2) :
            print "Not a valid path!!"
            raise
        
        filepathSplit=filepath.split("/")
        FileName=filepathSplit[-1]
        SubSampleFolder=filepathSplit[-2]
        SampleFolder=filepathSplit[-3]
        TopFolder=filepath[:filepath.find(SampleFolder + "/")]
        #Remove trailing slash of TopFolder, if present
        if TopFolder == "":
            TopFolder = "."
        elif (TopFolder[-1] == "/") :
            TopFolder = TopFolder[:-1]

        #Check the filename
        if (FileName.count("_") < 3) :
            print "Not a valid filename: " + FileName
            raise

        FileNameSplit=FileName.split("_")
        SampleName=FileNameSplit[0]
        SubSample=FileNameSplit[1]
        Estimation=FileNameSplit[2]
        Tail=FileNameSplit[3]

        #Remove trailing .root of Tail, if present
        Tail=Tail.replace(".root", "")
        
        #Check sample and subsample names
        if (SubSampleFolder != SubSample) :
            print "Not a valid path or subsample name"
            raise
        if (SampleFolder != SampleName) :
            print "Not a valid path or sample name"
            raise
        
        InDir=TopFolder
        pwd=commands.getoutput('pwd')
        AbsPath=pwd+'/'+filepath

        #########
        #END NEW VERSION
        #########


#       #three underscores?
#       nunderscores=filepath.count("_")
#       if nunderscores < 3:
#           print "Wrong Input File format, it should be Sample/SubSample/Sample_SubSample_estimation_tail.root"
#           raise
#
#       
#       #
#       #the file has the format:
#       # Sample/SubSample/Sample_SubSample_Estimation_Tail.root
#       #
#       
#       lastslash=0
#
#       if filepath.count("/") == 3:
#           if filepath[0] != "." or filepath[1] !="/":
#               print "Wrong Input File format, it should be Sample/SubSample/Sample_SubSample_estimation_tail.root"
#           else:
#               pass
        
#               #the format is ./sample/subsample/sample_subsample_estimation_tail.root
#       elif filepath.count("/") >3:
#           print "Wrong Input File format, it should be Sample/SubSample/Sample_SubSample_estimation_tail.root"
#       elif filepath.count("/") <2:
#           print "Wrong Input File format, it should be Sample/SubSample/Sample_SubSample_estimation_tail.root"                
#           
#       
#       lastslash=filepath.rfind("/")
#       FileName=filepath[lastslash+1:]
#
#       if filepath[0] == "." and filepath[1] =="/":
#           FDir=filepath[2:filepath.find("/",2)]
#           SDir=filepath[ filepath.find("/",2)+1: filepath.find("/", filepath.find("/",2)+1 )]
#       else:
#           FDir= filepath[:filepath.find("/")]
#           SDir=filepath[filepath.find("/")+1: filepath.find("/", filepath.find("/")+1 )]
#
#       InDir=FDir+'/'+SDir
#
#       pwd=commands.getoutput('pwd')
#       AbsPath=pwd+'/'+filepath
#       #
#       uno=FileName.find("_")
#       SampleName=FileName[:uno]
#       dos=FileName.find("_",uno+1)
#       SubSample=FileName[uno+1:dos]
#       tres=FileName.find("_",dos+1)
#       Estimation=FileName[dos+1:tres]
#       punto=FileName.find(".")
#       Tail=FileName[tres+1:punto]
#       #
#       #FileName=
    else:
        ##still dont know what to do if they are merged
        pass
    #================================================================================
    #print FileName,InDir,SampleName,SubSample,Estimation,Tail,AbsPath
    return FileName,InDir,SampleName,SubSample,Estimation,Tail,AbsPath




if __name__ == "__main__":



    print "hey"
    FileName,InDir,SampleName,SubSample,Estimation,Tail,AbsPath=BreakDownInputPath("../SomeRandomDir/ElectronHad/May10ReReco/ElectronHad_May10ReReco_synch-el_FullSynchro.root")
    print FileName
    print InDir
    print SampleName
    print SubSample
    print Estimation
    print Tail
    print AbsPath
