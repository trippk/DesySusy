import sys, os
import commands as com



def SetUpSampleAndScript(Sample,SubSample,FilesDir,ConfigFile,nFiles):
    """Sets up the directory and the runall_ scripts to run a sample"""
    command='runOnAll'
    Executable='makeobjects'
    optclean='cleanUp'

    user=com.getoutput('whoami')
    
    #Change these lines if you want to write the output somewhere else.
    # The 'user' will always be different and so we can't write in
    # somebody else's space.
    #
    #     
    if user=='eron':
        topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/'
    else:
        topdir='./'
    #
    #
    #
    TargetDir=Sample+'/'+SubSample
    TargetName=Sample+'_'+SubSample+'.root'
    #is the directory there already?
    #print "TargetDir is",TargetDir
    #print "going to do the ls "
#    print "with the sample ",TargetDir
#out=com.getoutput('ls -lrt '+TargetDir)
#print out
 #   print
  #  print
   # print
    out=com.getoutput('ls -lrt '+topdir+TargetDir)
    if out.find('No such')>=0:
        print 'lets make the directory'
        out=com.getoutput('ls '+topdir+Sample)
        if out.find('No such')>=0:
            mkd=com.getoutput('mkdir '+topdir+Sample)
        #
        mkd=com.getoutput('mkdir '+topdir+TargetDir)
    #
    out=com.getoutput('ln -fs ' + topdir + Sample + ' ./' )
    print 'out is ',out
    #name of the script
    scriptname='runall_'+Sample+'_'+SubSample

    #create the script
    SP=' '
    line='./'+command  +SP  
    line+=FilesDir     +SP  
    line+=TargetDir    +SP
    line+=Executable   +SP
    line+=ConfigFile   +SP
    line+=optclean     +SP
    line+='outName='+TargetName +SP
    line+='nFiles='+str(nFiles) +SP
    line+='$1'         +SP
    line+='$2'        +'\n'
    
    #write it into the script file
    scfile=open(scriptname,'w')
    scfile.write(line)
    scfile.close()
    #set the executable permission
    out=com.getoutput('chmod u+x '+scriptname)

    return scriptname




if __name__=='__main__':

    indir='/pnfs/desy.de/cms/tier2/store/user/schettle/nTuple11_v4/mc/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/'
    config='config_MC_RA4b.txt'
    SetUpSampleAndScript('TTJets','SUMMER11',indir,config,3)
