#! /usr/bin/env pyth
import sys, os
import commands
import copy
from SampleClasses import Sample
from BreakDownInputPath import *
from GetDate import *






#==========================================================
# RA4belHighPt and NoTail
#==========================================================

def ElectronHad_RA4belHighPt_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("ElectronHad/Run2012A-PromptReco-v1/ElectronHad_Run2012A-PromptReco-v1_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    Samp.AddRootFile("ElectronHad/Run2012B-PromptReco-v1/ElectronHad_Run2012B-PromptReco-v1_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#

#==========================================
def SingleTop_RA4belHighPt_NoTail(options):

    Samp=Sample()
    Samp.isData=False

    #
    #
    Samp.AddRootFile("SingleTop/T-s-channel/SingleTop_T-s-channel_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/T-t-channel/SingleTop_T-t-channel_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/T-tW-channel/SingleTop_T-tW-channel_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/Tbar-s-channel/SingleTop_Tbar-s-channel_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/Tbar-t-channel/SingleTop_Tbar-t-channel_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/Tbar-tW-channel/SingleTop_Tbar-tW-channel_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def TTJets_RA4belHighPt_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    

    Samp.AddRootFile("TTJets/SUMMER12/TTJets_SUMMER12_RA4b-el-HighPt_NoTail_Tree_OUT.root")

    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def DYJets_RA4belHighPt_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    Samp.AddRootFile("DYJets/M-50/DYJets_M-50_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def WJets_RA4belHighPt_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    
    Samp.AddRootFile("WJetsToLNu/NoSub/WJetsToLNu_NoSub_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================



















#==========================================================
# RA4bmu and NoTail
#==========================================================

def MuHad_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("MuHad/Run2012A-PromptReco-v1/MuHad_Run2012A-PromptReco-v1_RA4b-mu_NoTail_Tree_OUT.root")
    Samp.AddRootFile("MuHad/Run2012B-PromptReco-v1/MuHad_Run2012B-PromptReco-v1_RA4b-mu_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#

#==========================================
def SingleTop_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False

    #
    #
    Samp.AddRootFile("SingleTop/T-s-channel/SingleTop_T-s-channel_RA4b-mu_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/T-t-channel/SingleTop_T-t-channel_RA4b-mu_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/T-tW-channel/SingleTop_T-tW-channel_RA4b-mu_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/Tbar-s-channel/SingleTop_Tbar-s-channel_RA4b-mu_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/Tbar-t-channel/SingleTop_Tbar-t-channel_RA4b-mu_NoTail_Tree_OUT.root")
    Samp.AddRootFile("SingleTop/Tbar-tW-channel/SingleTop_Tbar-tW-channel_RA4b-mu_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def TTJets_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    

    Samp.AddRootFile("TTJets/SUMMER12/TTJets_SUMMER12_RA4b-mu_NoTail_Tree_OUT.root")

    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def DYJets_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    Samp.AddRootFile("DYJets/M-50/DYJets_M-50_RA4b-mu_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def WJets_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    
    Samp.AddRootFile("WJetsToLNu/NoSub/WJetsToLNu_NoSub_RA4b-mu_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================



#==========================================
def LM6_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile("LM6/NoSub/LM6_NoSub_RA4b-mu_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#===========================================

#==========================================
def LM6_RA4belHighPt_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile("LM6/NoSub/LM6_NoSub_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#===========================================

#==========================================
def LM9_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile("LM9/NoSub/LM9_NoSub_RA4b-mu_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#===========================================

#==========================================
def LM9_RA4belHighPt_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile("LM9/NoSub/LM9_NoSub_RA4b-el-HighPt_NoTail_Tree_OUT.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#===========================================



if __name__ == '__main__':

    
    options={}
    options['RW']=True
    options['ForceRW']=False
    options['Merge']=True
    options['ForceMerge']=False
    options['dataLumi']=[5097]
    muhad=LM6_RA4bmu_NoTail(options)    
    #Samp=TTJets_RA4_NoTail(options)
    #ttjets=TTJets_RA4bmu_NoTail(options)
    #singletop=SingleTop_RA4bmu_NoTail(options)
    #dyjets=DYJets_RA4bmu_NoTail(options)

    options['RW']=False
    options['ForceRW']=False
    options['Merge']=True
    options['ForceMerge']=False
    options['dataLumi']=[5097]    

    print 'the file to be used for single top is ', muhad.RootFile
    
