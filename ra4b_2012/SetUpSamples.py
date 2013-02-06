#!/usr/bin/python
import sys, os
import commands as com
from SetUpSample import SetUpSampleAndScript


#
#
def SampleFromSubSamples(SubSampleList):
    """dont know yet """
    
def cleanUpDir(outdir):
    #out=com.getoutput("rm -f merge_script.o*")
    file=open("cleanUpDir","w")
    out=com.getoutput('chmod u+x cleanUpDir')
    print outdir
    line='rm -f merge_script.o*'
    file.write(line+'\n')
    file.close()
    out=com.getoutput("mv cleanUpDir "+outdir+"/")

def ElectronHad():

    Scripts=[]
    Sample='ElectronHad'
    SubSample='Run2012A-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/data/ElectronHad/Run2012A-PromptReco-v1/190456-196531'
    Config='config_DATA_RA4b.txt'
    nFiles=3

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

    Scripts=[]
    Sample='ElectronHad'
    SubSample='Run2012B-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/data/ElectronHad/Run2012B-PromptReco-v1/SGE/190456-196531'
    Config='config_DATA_RA4b.txt'
    nFiles=7
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)        
    
    Sample='ElectronHad'
    SubSample='Run2012A-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/ElectronHad/Run2012A-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=7
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)
    
    Scripts=[]
    Sample='ElectronHad'
    SubSample='Run2012A-recover-06Aug'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/ElectronHad/Run2012A-recover_06Aug/'
    Config='config_DATA_RA4b.txt'
    nFiles=9
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

    Scripts=[]
    Sample='ElectronHad'
    SubSample='Run2012B-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/ElectronHad/Run2012B-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=8
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

def MuHad():

    Scripts=[]
    Sample='MuHad'
    SubSample='Run2012A-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/data/MuHad/Run2012A-PromptReco-v1/190456-196531'
    Config='config_DATA_RA4b.txt'
    nFiles=3

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)


    Sample='MuHad'
    SubSample='Run2012B-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/data/MuHad/Run2012B-PromptReco-v1/SGE/190456-196531'
    Config='config_DATA_RA4b.txt'
    nFiles=7

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)
    
    Sample='MuHad'
    SubSample='Run2012A-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/MuHad/Run2012A-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=7
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)
    
    Scripts=[]
    Sample='MuHad'
    SubSample='Run2012A-recover-06Aug'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/MuHad/Run2012A-recover-06Aug/'
    Config='config_DATA_RA4b.txt'
    nFiles=9
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

    Scripts=[]
    Sample='MuHad'
    SubSample='Run2012B-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/MuHad/Run2012B-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=8
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

def SingleElectron():

    Scripts=[]
    Sample='SingleElectron'
    SubSample='Run2012A-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/data/SingleElectron/Run2012A-PromptReco-v1/'
    Config='config_DATA_RA4b.txt'
    nFiles=3
    
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)


    Sample='SingleElectron'
    SubSample='Run2012B-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/data/SingleElectron/Run2012B-PromptReco-v1/'
    Config='config_DATA_RA4b.txt'
    nFiles=7
    
    Sample='SingleElectron'
    SubSample='Run2012A-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleElectron/Run2012A-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=6
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)
    
    Scripts=[]
    Sample='SingleElectron'
    SubSample='Run2012A-recover-06Aug'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleElectron/Run2012A-recover-06Aug/'
    Config='config_DATA_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

    Scripts=[]
    Sample='SingleElectron'
    SubSample='Run2012B-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleElectron/Run2012B-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

def SingleMu():

    Scripts=[]
    Sample='SingleMu'
    SubSample='Run2012A-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/data/SingleMu/Run2012A-PromptReco-v1/190456-196531'
    Config='config_DATA_RA4b.txt'
    nFiles=3
    
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)


    Sample='SingleMu'
    SubSample='Run2012B-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/data/SingleMu/Run2012B-PromptReco-v1/'
    Config='config_DATA_RA4b.txt'
    nFiles=7
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

    Sample='SingleMu'
    SubSample='Run2012A-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleMu/Run2012A-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=7
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)
    
    Scripts=[]
    Sample='SingleMu'
    SubSample='Run2012A-recover-06Aug'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleMu/Run2012A-recover-06Aug/'
    Config='config_DATA_RA4b.txt'
    nFiles=6
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

    Scripts=[]
    Sample='SingleMu'
    SubSample='Run2012B-13Jul'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleMu/Run2012B-13Jul/'
    Config='config_DATA_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

def DoubleMu():

    Scripts=[]
    Sample='DoubleMu'
    SubSample='Run2012A-13Jul2012-v1'
    FilesDir='/scratch/hh/dust/naf/cms/user/schettle/nTuple12_v4c/data/DoubleMu/Run2012A-13Jul2012-v1'
    Config='config_DATA_RA4b.txt'
    nFiles=3

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)


    Sample='DoubleMu'
    SubSample='Run2012B-13Jul2012-v4'
    FilesDir='/scratch/hh/dust/naf/cms/user/schettle/nTuple12_v4c/data/DoubleMu/Run2012B-13Jul2012-v4'
    Config='config_DATA_RA4b.txt'
    nFiles=7

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

def DoubleEl():

    Scripts=[]
    Sample='DoubleElectron'
    SubSample='Run2012A-13Jul2012-v1'
    FilesDir='/scratch/hh/dust/naf/cms/user/schettle/nTuple12_v4c/data/DoubleElectron/Run2012A-13Jul2012-v1'
    Config='config_DATA_RA4b.txt'
    nFiles=3

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)


    Sample='DoubleElectron'
    SubSample='Run2012B-13Jul2012-v1'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/DoubleElectron/Run2012B-13Jul2012-v1'
    Config='config_DATA_RA4b.txt'
    nFiles=7

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)

def MuEG():

    Scripts=[]
    Sample='MuEG'
    SubSample='Run2012A-13Jul2012-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/schettle/nTuple12_v4b/data/MuEG/Run2012A-13Jul2012-v1/'
    Config='config_DATA_RA4b.txt'
    nFiles=3

    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)    
    Scripts.append(script)



def TTJets():

    Scripts=[]
    Sample='TTJets'
    SubSample='SUMMER12'
    FilesDir='/scratch/hh/lustre/cms/user/sahin/nTuple12_v4b/mc/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='SUMMER12POWHEG'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/schettle/nTuple12_v4b/mc/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def TTJetsMG():

    Scripts=[]
    Sample='TTJetsMG'
    SubSample='Hadr-DR53'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/TTJetsMG/HadronicMGDecays_ext-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=5
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='Semi-DR53'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/TTJetsMG/SemiLeptMGDecays_ext-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    
    SubSample='Full-DR53'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/TTJetsMG/FullLeptMGDecays-v2/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def TTJetsPOWHEG():

    Scripts=[]
    Sample='TTJetsPOWHEG'
    SubSample='DR53-v1'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=4
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='DR53-v2'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/schettle/nTuple12_v4b/mc/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/'
    Config='config_MC_RA4b.txt'
    nFiles=8
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def TTJetsMCatNLO():
    
    Scripts=[]
    Sample='TTJetsMCatNLO'
    SubSample='DR53'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/TTJetsMCatNLO/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=6
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def LM9():

    Scripts=[]
    Sample='LM9'
    SubSample='NoSub'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/mc/SUSY_LM9_sftsht_8TeV-pythia6/Summer12-PU_S7_START52_V9-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def LM6():

    Scripts=[]
    Sample='LM6'
    SubSample='NoSub'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/mc/SUSY_LM6_sftsht_8TeV-pythia6/Summer12-PU_S7_START52_V9-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def T2tt():

    Scripts=[]
    Sample='T2tt'
    SubSample='t500n100'
    FilesDir='/scratch/hh/current/cms/user/costanza/store/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/'
    Config='config_Scan_RA4b.txt'
    nFiles=300
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    ########################################
    SubSample='t650n200'
    FilesDir='/scratch/hh/current/cms/user/costanza/store/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/'
    Config='config_Scan_RA4b.txt'
    nFiles=300
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    ########################################
    SubSample='t750n400'
    FilesDir='/scratch/hh/current/cms/user/costanza/store/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/'
    Config='config_Scan_RA4b.txt'
    nFiles=300
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    ########################################
    SubSample='t350n50'
    FilesDir='/scratch/hh/current/cms/user/costanza/store/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/'
    Config='config_Scan_RA4b.txt'
    nFiles=300
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    ########################################
    SubSample='t400n50'
    FilesDir='/scratch/hh/current/cms/user/costanza/store/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/'
    Config='config_Scan_RA4b.txt'
    nFiles=300
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    ########################################
    SubSample='NoSub'
    FilesDir='/scratch/hh/current/cms/user/costanza/store/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/'
    Config='config_Scan_RA4b.txt'
    nFiles=20
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def DYJetsToLL():

    Scripts=[]
    Sample='DYJets'
    SubSample='M-50'
    FilesDir='/scratch/hh/lustre/cms/user/sahin/nTuple12_v4b/mc/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    cleanUpDir('./'+Sample+'/'+SubSample)
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    Scripts.append(script)

    Sample='DYJets'
    SubSample='M-10to50'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/schettle/nTuple12_v4c/mc/DYJetsToLL_M-10To50filter_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    cleanUpDir('./'+Sample+'/'+SubSample)
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    Scripts.append(script)


def SingleTop():

    Scripts=[]
    Sample='SingleTop'
    SubSample='T-s-channel'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleTop/T_schannel'
    Config='config_MC_RA4b.txt'
    nFiles=8
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='T-t-channel'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleTop/T_tchannel'
    Config='config_MC_RA4b.txt'
    nFiles=5
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)                    
    #============================================
    SubSample='T-tW-channel'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleTop/T_tWchannel'
    Config='config_MC_RA4b.txt'
    nFiles=6
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='Tbar-s-channel'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleTop/Tbar_schannel'
    Config='config_MC_RA4b.txt'
    nFiles=4
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='Tbar-t-channel'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleTop/Tbar_tchannel'
    Config='config_MC_RA4b.txt'
    nFiles=4
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='Tbar-tW-channel'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/SingleTop/Tbar_tWchannel' 
    Config='config_MC_RA4b.txt'
    nFiles=5
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================    

    newscript=open('runall_'+Sample+'_All','w')
    for sc in Scripts:
        newline=sc+ ' $1 $2 '
        print newline
        newscript.write(newline+'\n')
    #

    
def WJetsToLNu():

    Scripts=[]
    Sample='WJetsToLNu'
    SubSample='NoSub'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/mc/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='HT250To300'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/WJetsToLNu/HT250To300/'
    Config='config_MC_RA4b.txt'
    nFiles=15
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='HT300To400'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/WJetsToLNu/HT300To400/'
    Config='config_MC_RA4b.txt'
    nFiles=15
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='HT400ToInf'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/NTuple12_V4c/WJetsToLNu/HT400ToInf/'
    Config='config_MC_RA4b.txt'
    nFiles=15
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='3Jets'
    FilesDir='/scratch/hh/dust/naf/cms/user/dhorton/NTuple12_V4c/WJetsToLNu/3Jets/'
    Config='config_MC_RA4b.txt'
    nFiles=4
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

    SubSample='4Jets'
    FilesDir='/scratch/hh/dust/naf/cms/user/dhorton/NTuple12_V4c/WJetsToLNu/4Jets/'
    Config='config_MC_RA4b.txt'
    nFiles=4
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)


def WWJetsTo2L2Nu():

    Scripts=[]
    Sample='WWJetsTo2L2Nu'
    SubSample='NoSub'
    FilesDir='/scratch/hh/lustre/cms/user/schettle/nTuple12_v4b/mc/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def WZJetsTo3LNu():

    Scripts=[]
    Sample='WZJetsTo3LNu'
    SubSample='NoSub'
    FilesDir='/scratch/hh/lustre/cms/user/schettle/nTuple12_v4b/mc/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def WZJetsTo2L2Q():

    Scripts=[]
    Sample='WZJetsTo2L2Q'
    SubSample='NoSub'
    FilesDir='/scratch/hh/lustre/cms/user/schettle/nTuple12_v4b/mc/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def ZZJetsTo2L2Q():

    Scripts=[]
    Sample='ZZJetsTo2L2Q'
    SubSample='NoSub'
    FilesDir='/scratch/hh/lustre/cms/user/schettle/nTuple12_v4b/mc/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def ZZJetsTo2L2Nu():

    Scripts=[]
    Sample='ZZJetsTo2L2Nu'
    SubSample='NoSub'
    FilesDir='/scratch/hh/lustre/cms/user/schettle/nTuple12_v4b/mc/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)

def ZZJetsTo4L():

    Scripts=[]
    Sample='ZZJetsTo4L'
    SubSample='NoSub'
    FilesDir='/scratch/hh/lustre/cms/user/schettle/nTuple12_v4b/mc/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)


def LM6():
    Scripts=[]
    Sample='LM6'
    SubSample='NoSub'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/mc/SUSY_LM6_sftsht_8TeV-pythia6/Summer12-PU_S7_START52_V9-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)



def LM9():
    Scripts=[]
    Sample='LM9'
    SubSample='NoSub'
    FilesDir=' /pnfs/desy.de/cms/tier2/store/user/msahin/nTuple12_v2/mc/SUSY_LM9_sftsht_8TeV-pythia6/Summer12-PU_S7_START52_V9-v1/'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)


    
if __name__=='__main__':

    MuHad()
#    ElectronHad()
#    SingleMu()
#    SingleElectron()
    DoubleMu()
    DoubleEl()
#    MuEG()
    TTJets()
    TTJetsMG()
    TTJetsPOWHEG()
#    TTJetsMCatNLO()
    DYJetsToLL()
    WJetsToLNu()
    ################
    #DiBoson samples
    ################
    WWJetsTo2L2Nu()
    WZJetsTo3LNu()
    WZJetsTo2L2Q()
    ZZJetsTo2L2Nu()
    ZZJetsTo4L()
    ZZJetsTo2L2Q()
    ################
    SingleTop()
#    LM9()
#    LM6()
    T2tt()
