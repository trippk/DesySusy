#/usr/bin/python
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

def TTJets():

    Scripts=[]
    Sample='TTJets'
    SubSample='SUMMER12'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S6_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)


def TTJetsBUG():
    Scripts=[]
    Sample='TTJets-BUG'
    SubSample='SUMMER12'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/schettle/nTuple12_v2/mc/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)    




def DYJetsToLL():

    Scripts=[]
    Sample='DYJets'
    SubSample='M-50'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/schettle/nTuple12_v2/mc/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/'
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
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='T-t-channel'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)                    
    #============================================
    SubSample='T-tW-channel'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='Tbar-s-channel'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='Tbar-t-channel'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================
    SubSample='Tbar-tW-channel'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12-PU_S7_START52_V9-v1' 
    Config='config_MC_RA4b.txt'
    nFiles=3
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)
    #============================================    

    newscript=open('runall_'+Sample+'_All','w')
    for sc in Scripts:
        newline='SendBatchJob '+sc+ ' $1 $2 '
        print newline
        newscript.write(newline+'\n')
    #

    
def WJetsToLNu():

    Scripts=[]
    Sample='WJetsToLNu'
    SubSample='NoSub'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/eron/nTuple12_v2/mc/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1'
    Config='config_MC_RA4b.txt'
    nFiles=3
    #
    script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)
    cleanUpDir('./'+Sample+'/'+SubSample)
    Scripts.append(script)




    
if __name__=='__main__':

    MuHad()
    ElectronHad()
    TTJets()
    TTJetsBUG()    
    DYJetsToLL()
    WJetsToLNu()
    SingleTop()
