import sys, os
import commands
from ROOT import gROOT, gDirectory,TCanvas, TF1,TH1F,TH1D,TH1I,TFile,TKey,TString,THStack,TList,TLegend,TPaveText
from FileMerger import MergeFiles
from ReweightFile import *
from BreakDownInputPath import *
from GetDate import *
#from myPrint import *
import copy
#
#
#
#
class Sample:
    """ a class for the handling of the samples, reweighting and merging and all that"""
    def __init__(self):
        
        self.isData = True
        self.RootFile = "Undefined"
        self.SampleName="Undefined"
        self.Estimation="Undefined"
        self.Tail="Undefined"
        self.MergedFile="Undefined"
	self.MergedFilePath="Undefined"
        self.isData=True
	self.MainDir="Undefined"
	self.ListOfRootFiles=[]
        self.SampleLumi=0.
        #self.dataLumi=5097.
        self.SampleLuminosity=0.0
        self.paraFile="para_config.txt"
        #Dictionaries
	self.dFileName={}
	self.dInDir={}
	self.dSampleName={}
	self.dSubSampleName={}
	self.dEstimation={}
	self.dTail={}
        self.dAbsPath={}
        self.dDates={}
        self.ReweightedFiles={}
        self.MergedFiles={}
        self.rwOptions={}
        #

    def AddRootFile(self,ExternRootFile):
	    """adds a root file to the list of files of the class"""
            print "------------------------------------------------"
	    print "appending the file ",ExternRootFile
            print "------------------------------------------------"
            print ""
	    self.ListOfRootFiles.append(ExternRootFile)

            #check that this file has not been added before
            if len(self.ListOfRootFiles)>1:
                for file in self.ListOfRootFiles[:-1]:
                    if file==ExternRootFile:
                        print "THE FILE HAD ALREADY BEEN ADDED"
                        raise NameError("THE FILE HAD ALREAY BEEN ADDED")
                    #
                #
            #
            #
            #lets use the name of the file as Key for the dictionaries
            #
	    fname,indir,samplename,subsamplename,estimation,tail,abspath=BreakDownInputPath(ExternRootFile)
	    self.dFileName[ExternRootFile]=fname
	    self.dInDir[ExternRootFile]=indir
	    self.dSampleName[ExternRootFile]=samplename	
            self.dSubSampleName[ExternRootFile]=subsamplename
	    self.dEstimation[ExternRootFile]=estimation
	    self.dTail[ExternRootFile]=tail
            self.dAbsPath[ExternRootFile]=abspath
            self.dDates[ExternRootFile]=GetDate(ExternRootFile)
            #
            #
            #
            #
            #
            #============PARAMETERS OF THE SAMPLE, ON FIRST ITERATION
	    if (self.SampleName == "Undefined"):
                self.SampleName=samplename
                self.Estimation=estimation
                self.Tail=tail
                #
                #The first dir should be called as the Sample+SubSample
                #self.MainDir='./'+self.SampleName
                self.MainDir=indir
                self.MergedFile="undefined"
                self.MergedFilePath=ExternRootFile
                self.AbsPath=abspath
                self.RootFile=abspath

            else:

                #========IF MORE THAN ONE FILE IS ADDED
                #----CHECK
                if samplename != self.SampleName:
                    print "ADDING FILES FROM DIFFERENT SAMPLES",self.SampleName,samplename
                    print 'ARE YOU SURE THIS IS CORRECT? '
                    answer=raw_input('answer \'yes\' if you are:   ')
                    if answer != 'yes':
                        sys.exit()
                if estimation != self.Estimation:
                    print "ADDING FILES FROM DIFFERENT ESTIMATIONs",self.Estimation,estimation
                    raise
                if tail != self.Tail:
                    print "ADDING FILES WITH DIFFERENT TAILS",self.Tail,tail
                    raise
                #
                #the rootfile should point to the merged file
                #self.RootFile=self.MergedFilePath

    #
    #
    #
    #
    #
    #
    #=======CHECKS IF ALREADY MERGED
    def CheckIfMerged(self,FilesToMerge,MergedFilePath):
        """checks if the files have already been merged
        It guesses the name of the merged file from
        the name of the list of files to merge
        if the file is not there, assumes no merging yet.
        if the file is there, compares the date with the files on
        the list, if one is newer than the merged file, it
        assumes one needs to re-merge
        """

        out=commands.getoutput('ls '+MergedFilePath)
        if out.find('No such file or directory') != -1:
            #the file is not there, they are not merged
            return False
        #
        #
        else:
            #the file is there, is it newer than all the rest?
            #
            #
            NewerFile=False
            for file in FilesToMerge:
                #CompareDate(file1,file2) returns whether f1 is NEWER than f2
                try:
                    NewerFile=CompareDate(file,MergedFilePath)
                    #a file has is newer than the merged, remerging is needed
                    #NewerFile=True
                except NameError, e:
                    print 'there was an exception ',e
            #
        #
        print ''
        print 'CHECKING FOR MERGING'
        print '' 
        if NewerFile:
            print '    Mergig IS needed'
        else:
            print '    Merging is NOT needed'
        print ''
        return  not NewerFile
    #
    #
    #
    #
    #
    #
    #
    def MergeThem(self,FilesToMerge,Merge,ForceMerge,suffix=''):

        """Merges a set of files into a single one"""
        #
        #

        verbose=False
        if len(FilesToMerge)<2:
            if verbose:
                print 'the list of files to merge has only one element'
            return



        if verbose:
            print 'the files to merge are',FilesToMerge
        FirstFile=FilesToMerge[0]
        if verbose:
            print 'the first file is',FirstFile
        if FirstFile.find("/") != -1:
            FirstFile=FirstFile[FirstFile.rfind('/')+1 : ]
        #
        #this should now be like A_B_C_D_E.root
        #if merged, there should be a file in maindir called
        #A_B_C_D_E_MERGED.root
        FirstFileList=FirstFile.split('_')
        MergedFileName=FirstFileList[0]
        for it,element in enumerate(FirstFileList):
            if it >1:
                MergedFileName+='_'+str(element)
            #
        #
        #
        #Now add the SUFFIX AND THE _MERGED#
        if verbose:
            print 'the suffix is ',suffix
        MergedFileName=MergedFileName.replace('.root',suffix+'_MERGED.root') 
        if verbose:
            print 'the mergedfilename is ',MergedFileName
        self.MergedFilePath=self.MainDir + '/' + self.SampleName + '/' + MergedFileName
        self.RootFile=self.MergedFilePath
        #
        #
        #
        #======CHECK IF ALREADY MERGED
        AreMerged=self.CheckIfMerged(FilesToMerge,self.MergedFilePath)
        #print 'it has been determined that AreMerged is '+str(AreMerged)
        #
        #
        #
        #
        #
        #============================================
        if AreMerged and not ForceMerge:
            #they are merged and not forced to merge
            return
        if AreMerged and ForceMerge:
            print "*****************"
            print "MERGING IS FORCED"
            print "*****************"            
            
        if not Merge:
            #no merging is wanted
            return
        
        #============================================
        #
        #
        #
        #
        #
        #we are here, so merge the files.
        #
        #
        #
        #
        print ""
        print "*******************************************************"
        print "The merged output file is called ", self.MergedFilePath
        print "*******************************************************"
        print ""        
        output=MergeFiles(FilesToMerge,self.MergedFilePath)
        if "Error" in output:
            if "has not a label and has non-zero content" in output:
                #this is possibly OK.
                pass
            else:
                print "THE MERGIG DID NOT GO WELL, ERROR"
                print "The Error Message was",output
                raise NameError('bad merging')
        #mvout=commands.getoutput('mv '+self.MergedFile+' '+self.MainDir)
        #self.MergedFilePath=self.MainDir+'/'+self.MergedFile
        self.RootFile=self.MergedFilePath
        print 'now RootFile has been assigned to ',self.RootFile

    #
    #
    #
    #
    #
    #
    #
    def OLDCheckIfFileIsReweighted(self,file):
        dummy=TFile(file,"READ")
        outfind=dummy.FindObjectAny("isReweighted")
        if str(outfind).find("nil") != -1:
            #the file has been not reweighted before
            isRW=False
        else:
            isRW=True
        #
        return isRW
    #
    #
    #
    #
    #
    #
    #
    #
    #
    #
    #
    def CheckIfFileIsReweighted(self,file,lumi):
        """Checks if the file has already been reweighted
        to a given luminosity"""
        #print "checking the file ",file
        verbose=True
        #the file to investigate is:
        FileGuess=file.replace('.root','_RWTo'+str(int(lumi))+'.root')
        if verbose: print 'INVESTIGATING ',FileGuess
        histoname='RWto'+str(int(lumi))

        if commands.getoutput('ls '+str(FileGuess)).find('No such file') != -1:
            #the file is not there yet, so now rw before
            if verbose: print 'file not found, so no RW is assumed'
            return False,FileGuess
        else:
            dummy=TFile(FileGuess,"READ")
        #
        #
        outfind=dummy.FindObjectAny(histoname)
        if str(outfind).find("nil") != -1:
            #the file has been not reweighted before
            'histo not found'
            if verbose: print 'the file is there but the histo isn\'t'
            isRW=False
        else:
            #check that it was reweighted AFTER this file
            #was produced
            filetime=GetDateSeconds(file)

            dateofrw=dummy.FindObjectAny("date_of_reweighting_to_"+str(int(lumi)))
            if str(dateofrw).find('nil')== -1:
                rwtime=int(dateofrw.GetTitle())
                #print 'the filetime is ',filetime
                #print 'the rwtime is ',rwtime
                #print 'rw file newer? ',int(rwtime)>int(filetime)
                #raw_input('continue')
                #
                #
                isRW=int(rwtime)>int(filetime)
                if verbose:
                    print '     rwtime is ',rwtime
                    print '     filetime is ',filetime
                    print '    ',(rwtime)>int(filetime)
            else:
                print 'the histo date_of_reweighting_to_'+str(int(lumi))+' was not found'
                
                isRW=False
        #
        #print 'returning ',isRW,FileGuess
        if isRW:
            yesorno='HAVE'
        else:
            yesorno='have NOT'
            
        print '     it has been determined that the files '+yesorno+' have been reweighted'
        return isRW,FileGuess
    #
    #
    #
    #
    #
    #
    #
    #
    #
    def ReweightIt(self,Lumi,File,options):
        """Reweights the files that make up the whole sample"""


        if len(options)<1 or len(options)>2:
            raise NameError("invalid lenght of options in Reweight It")

        #GET RW AND FORCE RW
        doRW=options[0]
        if len(options)==2:
            ForceRW=options[1]
        #
        #======CHECK IF IT HAD BEEN REWEIGHTED BEFORE
        print ''
        print ''
        print ''
        print '***********************************************'
        print 'Checking for REWEIGHTING:'
        print "FILE: ",File
        print "LUMI:" ,Lumi
        print ""
        isRW,FileGuess=self.CheckIfFileIsReweighted(File,Lumi)

        #print "RESULT: it has been determined that isRW is ",isRW
        #if not isRW:
        #    print ''
        #    print 'this means that the file IS going to be reweighted'
        #    print ''                    
         #print 'The file guess was',FileGuess
        #print "***********************************************"
        #print ""
        doReweight=False
            
        if (not isRW and doRW):
            doReweight=True
        elif(isRW and ForceRW):
            print "******************"
            print "FORCED REWEIGHTING"
            print "******************"
            doReweight=True
        #
        if doReweight:
            #
            #
            #
            
            RWFile=ReweightFile(File,Lumi,self.rwOptions)
            #

            if RWFile != FileGuess:
                if RWFile.find(FileGuess) == -1:
                    print 'RWFile = ',RWFile
                    print 'FileGUess = ',FileGuess
                    raw_input("the guess for the reweighted file is wrong. ERROR")
                

        if (len(self.ReweightedFiles)==0):
            self.ReweightedFiles[Lumi]=[copy.deepcopy(FileGuess)]
        else:
            self.ReweightedFiles[Lumi].append(copy.deepcopy(FileGuess))
            #
        #
        #
        self.RootFile=FileGuess
        
        return doReweight,FileGuess


    def ReweightAndMerge(self,options):
        """Reweights and Merges files in a sample
        The first input is the instance of sample on which to operate.
        The second input is a dictionary of options for the
        merging and reweighting"""

        #RETRIEVE THE OPTIONS
        RW=options.get('RW',False)
        Merge=options.get('Merge',False)
        ForceRW=options.get('ForceRW',False)
        ForceMerge=options.get('ForceMerge',False)
        dataLumiList=options.get('dataLumi',[0])
        mergedSuffix=options.get('mergedSuffix','')

        if not type(dataLumiList)==list:
            dataLumiList=[dataLumiList]
        #Lumi=dataLumi

        
        #this is done for every value of the lumi
        for dataLumi in dataLumiList:
            #
            #
            #
            if RW:
                #it must be MC, otherwise no RW!
                if self.isData :
                    if raw_input('This is a data sample. Do you really want to rescale it to '+str(dataLumi)+'? ') =='yes':
                        pass
                    else:
                        print 'exiting...'
                        return
                else:
                    #retrieve the samples
                    for file in self.ListOfRootFiles:
                        doReweight,RWFileGuess=self.ReweightIt(dataLumi,file,[RW,ForceRW])
                        #
                        #
                        #
                        if len(self.ListOfRootFiles)==1:
                            if doReweight:
                                #print 'Im guessing here ', RWFileGuess
                                self.RootFile=RWFileGuess
                            else:
                                #this shit is all wrong
                                pass
                            
                                #self.RootFile=file
                            #
                        #
                        #
                        #
                        #
                    if Merge:

                        #
                        #
                        #=====GO OVER THE THE DIFFERENT REWEIGHTED FILES
                        if len(self.ListOfRootFiles)>1:
                            for rwkey, rwlist in self.ReweightedFiles.iteritems():
                                #
                                self.MergeThem(rwlist,Merge,ForceMerge,mergedSuffix)
            else:
                if Merge:
                    if len(self.ListOfRootFiles)>1:
                        #print 'in here'
                        #MERGE THE UNREWEIGHTED FILES
                        self.MergeThem(self.ListOfRootFiles,Merge,ForceMerge,mergedSuffix)

                    else:
                        pass
                        #self.RootFile=self.ListOfRootFiles[0]
                        








if __name__ == "__main__":

    print "DEFINITION OF THE SAMPLE CLASSES"""

