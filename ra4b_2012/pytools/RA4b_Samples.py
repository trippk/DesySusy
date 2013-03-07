#! /usr/bin/env pyth
import sys, os
import commands
import subprocess
from time import asctime
import random
import copy
#from SelectionSamples import SelectionSample
#from SampleClasses import SampleTask,Sample,FakeRatioSample,SelectionSample
from SampleClasses import Sample
from BreakDownInputPath import *
from GetDate import *


#========================================
#def MCRA4bmu_NoTail():
def MCSet(Estimation,Tail,options):
    #Estimation="RA4bmu"
    #Tail="NoTail"

    print "string is ",'TTJets_'   +Estimation+'_'+Tail+'(options)'
    #raw_input()
    exec("tt = TTJets_"   +Estimation+"_"+Tail+"(options)")
    #raw_input()    
    exec("wj = WJets_"    +Estimation+"_"+Tail+"(options)")
    #raw_input()        
    exec("sp = SingleTop_"+Estimation+"_"+Tail+"(options)")
    #raw_input()        
    exec("dy = DYJets_"   +Estimation+"_"+Tail+"(options)")

##    tt=   TTJets_RA4bmu_NoTail(True,True)
##    wj=    WJets_RA4bmu_NoTail(True,True)
##    sp=SingleTop_RA4bmu_NoTail(True,True)
##    dy=   DYJets_RA4bmu_NoTail(True,True)
    
    mydict={}
    mydict['MCSet_'+'Estimation_'+'Tail']=copy.deepcopy([tt,wj,sp,dy])

    return mydict

#==========================================
##  Data, mu channel
#==========================================
#==========================================
def MuHadReReco_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("MuHad/Run2012A-13Jul/MuHad_Run2012A-13Jul_RA4b-mu_NoTail_Tree.root")
    Samp.AddRootFile("MuHad/Run2012A-recover-06Aug/MuHad_Run2012A-recover-06Aug_RA4b-mu_NoTail_Tree.root")
    Samp.AddRootFile("MuHad/Run2012B-13Jul/MuHad_Run2012B-13Jul_RA4b-mu_NoTail_Tree.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================

def MuHad_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("SingleMu/Run2012A-PromptReco-v1/SingleMu_Run2012A-PromptReco-v1_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleMu/Run2012B-PromptReco-v1/SingleMu_Run2012B-PromptReco-v1_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
def SingleMuReReco_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("SingleMu/Run2012A-13Jul/SingleMu_Run2012A-13Jul_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleMu/Run2012A-recover-06Aug/SingleMu_Run2012A-recover-06Aug_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleMu/Run2012B-13Jul/SingleMu_Run2012B-13Jul_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================

def SingleMu_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("SingleMu/Run2012A-PromptReco-v1/SingleMu_Run2012A-PromptReco-v1_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleMu/Run2012B-PromptReco-v1/SingleMu_Run2012B-PromptReco-v1_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================

#==========================================
##  MC Bkg, mu channel
#==========================================
def TTJets_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    Samp.AddRootFile("TTJets/SUMMER12/TTJets_SUMMER12_RA4b-mu_NoTail.root")

    Samp.ReweightAndMerge(options)
    return Samp
#==========================================

def TTJetsMG_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    Samp.AddRootFile("TTJetsMG/Full-DR53/TTJetsMG_Full-DR53_RA4b-mu_NoTail.root")
    Samp.AddRootFile("TTJetsMG/Semi-DR53/TTJetsMG_Semi-DR53_RA4b-mu_NoTail.root")
    Samp.AddRootFile("TTJetsMG/Hadr-DR53/TTJetsMG_Hadr-DR53_RA4b-mu_NoTail.root")

    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def TTJetsPOWHEG_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("TTJetsPOWHEG/DR53-v1/TTJetsPOWHEG_DR53-v1_RA4b-mu_NoTail.root")
    Samp.AddRootFile("TTJetsPOWHEG/DR53-v2/TTJetsPOWHEG_DR53-v2_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def TTJetsMCatNLO_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("TTJetsMCatNLO/DR53/TTJetsMCatNLO_DR53_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def DYJets_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    Samp.AddRootFile("DYJets/M-50/DYJets_M-50_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def WJets_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("WJetsToLNu/NoSub/WJetsToLNu_NoSub_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def WJetsHTbinned_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("WJetsToLNu/HT250To300/WJetsToLNu_HT250To300_RA4b-mu_NoTail.root")
    Samp.AddRootFile("WJetsToLNu/HT300To400/WJetsToLNu_HT300To400_RA4b-mu_NoTail.root")
    Samp.AddRootFile("WJetsToLNu/HT400ToInf/WJetsToLNu_HT400ToInf_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def SingleTop_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("SingleTop/T-s-channel/SingleTop_T-s-channel_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleTop/T-t-channel/SingleTop_T-t-channel_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleTop/T-tW-channel/SingleTop_T-tW-channel_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleTop/Tbar-s-channel/SingleTop_Tbar-s-channel_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleTop/Tbar-t-channel/SingleTop_Tbar-t-channel_RA4b-mu_NoTail.root")
    Samp.AddRootFile("SingleTop/Tbar-tW-channel/SingleTop_Tbar-tW-channel_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================

#==========================================
##  MC Signal, el channel
#==========================================
def T2tt_t300n50_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t300n50/T2tt_t300n50_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t350n50_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t350n50/T2tt_t350n50_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t400n50_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t400n50/T2tt_t400n50_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t500n100_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t500n100/T2tt_t500n100_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t650n200_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t650n200/T2tt_t650n200_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t750n400_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t750n200/T2tt_t750n200_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t780n180_RA4bmu_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t800n180/T2tt_t780n180_RA4b-mu_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================


#==========================================
##  Data, el channel
#==========================================
#==========================================
def ElectronHadReReco_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("ElectronHad/Run2012A-13Jul/ElectronHad_Run2012A-13Jul_RA4b-el_NoTail.root")
    Samp.AddRootFile("ElectronHad/Run2012A_recover-06Aug/ElectronHad_Run2012A_recover-06Aug_RA4b-el_NoTail.root")
    Samp.AddRootFile("ElectronHad/Run2012B_13Jul/ElectronHad_Run2012B_13Jul_RA4b-el_NoTail.root")  
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def ElectronHad_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("ElectronHad/Run2012A-PromptReco-v1/ElectronHad_Run2012A-PromptReco-v1_RA4b-el_NoTail.root")
    Samp.AddRootFile("ElectronHad/Run2012B-PromptReco-v1/ElectronHad_Run2012B-PromptReco-v1_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
def SingleElectronReReco_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("SingleElectron/Run2012A-13Jul/SingleElectron_Run2012A-13Jul_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleElectron/Run2012A_recover-06Aug/SingleElectron_Run2012A_recover-06Aug_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleElectron/Run2012B_13Jul/SingleElectron_Run2012B_13Jul_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
def SingleElectron_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    #
    Samp.AddRootFile("SingleElectron/Run2012A-PromptReco-v1/SingleElectron_Run2012A-PromptReco-v1_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleElectron/Run2012B-PromptReco-v1/SingleElectron_Run2012B-PromptReco-v1_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================

#==========================================
##  MC Signal, el channel
#==========================================
#==========================================
def TTJets_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("TTJets/SUMMER12/TTJets_SUMMER12_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
def TTJetsMG_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    Samp.AddRootFile("TTJetsMG/Full-DR53/TTJetsMG_Full-DR53_RA4b-el_NoTail.root")
    Samp.AddRootFile("TTJetsMG/Semi-DR53/TTJetsMG_Semi-DR53_RA4b-el_NoTail.root")
    Samp.AddRootFile("TTJetsMG/Hadr-DR53/TTJetsMG_Hadr-DR53_RA4b-el_NoTail.root")

    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def TTJetsPOWHEG_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("TTJetsPOWHEG/DR53-v1/TTJetsPOWHEG_DR53-v1_RA4b-el_NoTail.root")
    Samp.AddRootFile("TTJetsPOWHEG/DR53-v2/TTJetsPOWHEG_DR53-v2_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def TTJetsMCatNLO_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("TTJetsMCatNLO/DR53/TTJetsMCatNLO_DR53_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def DYJets_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    
    Samp.AddRootFile("DYJets/M-50/DYJets_M-50_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def WJets_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("WJetsToLNu/NoSub/WJetsToLNu_NoSub_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def WJetsHTbinned_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    Samp.AddRootFile("WJetsToLNu/HT250To300/WJetsToLNu_HT250To300_RA4b-el_NoTail.root")
    Samp.AddRootFile("WJetsToLNu/HT300To400/WJetsToLNu_HT300To400_RA4b-el_NoTail.root")
    Samp.AddRootFile("WJetsToLNu/HT400ToInf/WJetsToLNu_HT400ToInf_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def SingleTop_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("SingleTop/T-s-channel/SingleTop_T-s-channel_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleTop/T-t-channel/SingleTop_T-t-channel_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleTop/T-tW-channel/SingleTop_T-tW-channel_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleTop/Tbar-s-channel/SingleTop_Tbar-s-channel_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleTop/Tbar-t-channel/SingleTop_Tbar-t-channel_RA4b-el_NoTail.root")
    Samp.AddRootFile("SingleTop/Tbar-tW-channel/SingleTop_Tbar-tW-channel_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================


#==========================================
##  MC Signal, el channel
#==========================================
def T2tt_t300n50_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t300n50/T2tt_t300n50_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t350n50_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t350n50/T2tt_t350n50_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t400n50_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t400n50/T2tt_t400n50_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t500n100_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t500n100/T2tt_t500n100_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t650n200_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t650n200/T2tt_t650n200_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t750n400_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t750n200/T2tt_t750n200_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================
#==========================================
def T2tt_t780n180_RA4bel_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #
    #
    Samp.AddRootFile("T2tt/t800n180/T2tt_t780n180_RA4b-el_NoTail.root")
    #
    Samp.ReweightAndMerge(options)
    return Samp
#==========================================

#==========================================


if __name__ == '__main__':

    
    #options={}
    #options['RW']=True
    #options['ForceRW']=False
    #options['Merge']=True
    #options['ForceMerge']=False
    #options['dataLumi']=[5097,10000]
    
   # Samp=SingleTop_RA4bel_NoTail(options)

    options={}
    options['RW']=False
    options['ForceRW']=False
    options['Merge']=True
    options['ForceMerge']=False
    options['dataLumi']=[20000]
    Samp=MuHad_RA4bmu_NoTail(options)
    print Samp.RootFile
    #options['SampleLumi']=5097
    

    
    #st=SingleTop_RA4bmu_NoTail((True,True))
    #lets get the Monte Carlo for RA4bmu and NoTail

    #MC_RA4bmu_NoTail=MCSet("RA4bmu","NoTail",(True,True))
    #MC_RA4bel_NoTail=MCSet("RA4bel","NoTail",(True,True))



