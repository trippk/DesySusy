#! /usr/bin/env python

import sys, os
import commands 
import subprocess
from time import asctime
import random

dcache_gate='dcap://dcache-cms-dcap.desy.de:22125/'
indir=''

batch_script = \
"""
#!/bin/zsh
## make sure the right shell will be used
#$ -S /bin/zsh
## the cpu time for this job
#$ -l h_cpu=00:40:00
## the maximum memory usage of this job
#$ -l h_vmem=1500M
## stderr and stdout are merged together to stdout
#$ -j y
##(send mail on job's end and abort)
#$ -m a
#$ -l site=hh
## define outputdir,executable,config file and LD_LIBRARY_PATH
#$ -v OUTDIR=
#$ -v EXECUTABLE=
#$ -v CONFIG=
#$ -v LD_LIBRARY_PATH=
## define dir for stdout
#$ -o
cd $OUTDIR
##initialize glite, dctools
ini glite
ini dctools
##pwd
#cp ../../src/Summer12_V2_DATA_AK5PF_UncertaintySources.txt /tmp/
##ls -lrt /tmp
echo job start at `date`

#echo total jobs at the NAF `qstat | wc -l`
#echo total running jobs at the NAF `qstat | grep -i " r " | wc -l`
#echo total own jobs at the NAF `qstat -u eron | wc -l`
#echo total own running jobs at the NAF `qstat -u eron | grep -i " r " | wc -l`
#echo this machine is `echo $HOST`

time $EXECUTABLE filename="$1" outname=$2 Estimation=$3 Tail=$4 isBatchJob=true
##cp $2 $OUTDIR
##echo cp $2 $OUTDIR
echo
echo done at time date `date`
"""


merge_script = \
"""
#!/bin/zsh
## make sure the right shell will be used
#$ -S /bin/zsh
## the cpu time for this job
#$ -l h_cpu=00:03:00
## the maximum memory usage of this job
#$ -l h_vmem=700M
## stderr and stdout are merged together to stdout
#$ -j y
## define input dir,output dir,executable,config file and LD_LIBRARY_PATH
#$ -v OUTDIR=
#$ -v OUTNAME=
#$ -v HADD=
#$ -v FILELIST=
#$ -v LD_LIBRARY_PATH=
#$ -o
save=$PWD
#
cd $OUTDIR
echo start merging at `date`
$HADD -f $OUTNAME $FILELIST 
echo
#
#
#
#
echo merging job done at `date`
"""



treeMerge_script = \
"""
#!/bin/zsh
## make sure the right shell will be used
#$ -S /bin/zsh
## the cpu time for this job
#$ -l h_cpu=00:03:00
## the maximum memory usage of this job
#$ -l h_vmem=4000M
## stderr and stdout are merged together to stdout
#$ -j y
## define input dir,output dir,executable,config file and LD_LIBRARY_PATH
#$ -v OUTDIR=
#$ -v OUTNAME=
#$ -v HADD=
#$ -v FILELIST=
#$ -v LD_LIBRARY_PATH=
#$ -o
save=$PWD
#
cd $OUTDIR
echo start merging at `date`
$HADD -f $OUTNAME $FILELIST 
echo "done merging, now remove"
rm -rf $FILELIST
#
#
#
#
echo merging job done at `date`
"""


# continues in createMergeScript

cronos_script = \
"""
#!/bin/zsh
## make sure the right shell will be used
#$ -S /bin/zsh
## the cpu time for this job
#$ -l h_cpu=00:05:00
## the maximum memory usage of this job
#$ -l h_vmem=1500M
## stderr and stdout are merged together to stdout
#$ -j y
## define input dir,output dir,executable,config file and LD_LIBRARY_PATH
#$ -v OUTDIR=
#$ -v RELDIR=
#$ -v OUTNAME=
#$ -v PYTHON=
#$ -v LD_LIBRARY_PATH=
#$ -o 
save=$PWD
#
cd $OUTDIR
##ini ROOT
##ini -d python2.5
ini cmssw
ini glite32
cd /scratch/hh/current/cms/user/eron/CMSSW_4_2_5/src
cmsenv
cd -

echo `pwd`
##echo calling with "$RELDIR/""$OUTNAME"
NAMEOFFILE=`cat "$RELDIR/"nameofrootfile`
echo "the name of the file is " $NAMEOFFILE

echo "calling cronos with" "$RELDIR/""$NAMEOFFILE"
which python
echo $PYTHON
python CronosExtractInfo.py "$RELDIR/""$NAMEOFFILE"
echo
#
#
#
#
date
"""

def SetCompletionStatus(status):

	mfile=open('completionstatus.txt','w')
	mfile.write(str(status))
	mfile.close()

	
def UsageAndExit():
	print \
	"""
	Usage:
	runOnAll indir [outdir] executable [config] [script] [cleanUp] [noJoin] [noDupRemoval] [nFiles=n] [outName=afile.root]
	
	indir   : input  dir - where the data is
	outdir  : output dir - where the individual output files and the final root file goes
	                       default is CWD
	executable 
	config    : configuration file for executable - only if  executable needs it
	script    : batch script - if not given a standard script will be created
	cleanUp   : keyword - if given erase all single root files
	noJoin    : keyword - if given do not run the file merging step
	noDupRemoval  : keyword - if given then do not ignore duplicate files
	nFiles=   : if given do create jobs with n files each - default is n=1
	outName=  : Name for the merged output file - default is out.root

	Example: ./runOnAll /scratch/hh/current/cms/user/kruecker/ntuples/data2011/Run2011A/DoubleMu/ myAnalysis myConfig.txt

	runOnAll reads $LD_LIBRARY_PATH - It is correctly set if you can run the executable.
	(ROOT and glib libs)
	"""
	sys.exit(0)
	
def InfoToConfig(config_filename):
	"""read the info about the sample and subsample and put it into config"""

	#retrieve the sample and subsample from the outfile name
	hmany=outname.count('_')

	Sample=''
	SubSample=''
	if hmany != 1:
		print "this is not a sample_subsample.root file"
		print "Not sure if this will work"
		print 'hmany =', hmany
	if hmany ==0:
		#this should be sample.root
		Sample=outname[:outname.find('.root')]
	elif hmany ==1:
		uscore=outname.find('_')
		Sample=outname[:uscore]
		SubSample=outname[uscore+1:outname.find('.root')]
		print "Sample obtained is ",Sample
		print "the SubSample obtained is ",SubSample
	else:
		print "using the file ",outname
		print "too many underscores in the outname"
		print "if you are seeing this, you are doing things in a way I did"
		print "not intend. Let's talk about it and see what we can do"
	#
	snamefound=False
	subsnamefound=False
	#
	config_mod=config_filename+'_mod'
	config=open(config_filename,'r')
	cmod=open(config_mod,'w')
	#
	for line in config:
		#print "the line is ",line
		sname=line.find('SampleName =')
		subsname=line.find('SubSampleName =')
		es=line.find('Estimation =')
		tl=line.find('Tail =')
	        if sname== -1 and subsname >=0:
			print "THIS SHOULD NOT HAPPEN in InfoToConfig"
			print sname
			print subsname
			print line
			raise
		elif sname>=0 and subsname == -1:
			newline=line[0:line.find('=')+1]+' '+Sample+'\n'			
		elif sname>=0 and subsname >=0:
			newline=line[0:line.find('=')+1]+' '+SubSample+'\n'			
		elif es>=0:
			newline=line[0:line.find('=')+1]+' '+Estimation+'\n'
		elif tl>=0:
			newline=line[0:line.find('=')+1]+' '+Tail+'\n'
		else:
			newline=line
		cmod.write(newline)
	#
	#
	config.close()
	cmod.close()
	#
	#print 'executing now '+'mv '+config_mod+'_'+config_filename
	out=commands.getoutput('mv '+config_mod+' '+config_filename)
	return Sample,SubSample

def readCommandLine(commandLine):
	""" read the command line input"""

	global	indir,outdir,executable,batch_filename,config_filename,cleanUp,noJoin,dupRemoval,nfiles,outname,reloutdir,Estimation,Tail
	global Sample,SubSample

	cleanUp        = False
	noJoin         = False
	dupRemoval     = True
	nfiles         = 1
	outname        = 'out.root'
	
	if len(commandLine) < 3 or len(commandLine) > 11: 
		print 'Wrong number of arguments'
		UsageAndExit()

	#get the tail and the estimation from the back of the file
	Tail=commandLine.pop(-1)
	print "the tail obtained is ",Tail
	Estimation=commandLine.pop(-1)
	print "the estimation obtained is", Estimation

	# inputdir
	indir = commandLine.pop(1)

	# check for ending /
	if indir[-1]!='/': indir+='/'
	# check if indir on dcache - if not: check if dir exits
	if indir.find('/store')!=0 and indir.find('/pnfs')!=0:
		if not os.path.isdir(indir) :
			print 'Cannot access '+indir
			sys.exit(0)

	# next arg - either outdir or executable
	something  = commandLine.pop(1)
	# check if 2nd argument is executable
	if commands.getoutput('file '+something).find('executable') >0:
		# outdir is not given, assume cwd
		outdir     = os.getcwd()
		executable = something
		print 'in here 1 ', outdir
 	else:
		outdir     = something
		executable = commandLine.pop(1)
		print 'in here 2 ', outdir		
		#randstr = str(abs(hash(asctime())))
		#outdir='/tmp/batchjobs/out_'+randstr
		#if not os.path.isdir('/tmp/batchjobs'):
		#	os.mkdir('/tmp/batchjobs')
		#print 'going to mkdir ' + outdir
		#os.mkdir(outdir)
		#print 'is the new directory there?', os.path.isdir(outdir)
		#print 'outdir is now ' + os.path.abspath(outdir)
		#outdir='./'
		#print 'outdir is now ' + os.path.abspath(outdir)
		#sys.exit(0)

	
	#full names
	print 'outdir before absolute path',outdir
	outdir = os.path.abspath(outdir)
	print 'currently in the directory ',commands.getoutput('pwd')
	print 'absolute path is then ', outdir
	executable = os.path.abspath(executable)

	#outdir relative to the main directory
	currentdir=commands.getoutput("pwd")	
	reloutdir1 = outdir.find(currentdir)
	print "current dir found at ",reloutdir1
	reloutdir = outdir[reloutdir1+len(currentdir)+1:]
	print "relative dir is ",reloutdir

	# check if output dir exists
	if not os.path.isdir(outdir) :
		print 'Cannot access '+outdir
		sys.exit(0)

	# read the rest of the input line
	files=[]
	filenames=[]
	while len(commandLine)>1:
		word=commandLine.pop(1)
		if word=='noDupRemoval': 
			dupRemoval=False
			continue
		if word=='cleanUp': 
			cleanUp=True
			continue
		if word=='noJoin':
			noJoin=True
			continue
		if word.find('outName')==0:
			outname=word[word.find('=')+1:]
			continue
		if word.find('nFiles')==0:
			nstr=word[word.find('=')+1:]
			if not nstr.isdigit():
				print 'what do you mean by '+word
				print 'Syntax is nFiles=n (no spaces!) where n=1,2,...'
				sys.exit(0)
			nfiles=int(nstr)
			if nfiles==0:
				print 'what do you mean by '+word
				print 'Syntax is nFiles=n (no spaces!) where n=1,2,...'
				sys.exit(0)
			continue
		try:
			file = open(word, 'r')
			files.append(file)
			filenames.append(word)
		except IOError:
			print 'Cannot open '+word
			sys.exit(0)

	if len(filenames)>2:
		print 'to many files ? ',filenames
		UsageAndExit()

	if  len(filenames)==2:
		# decide which is batch script and which config
		test = open(filenames[0],'r')
		cnt0=0
		for line in test:
			if line.find('#$ ')==0: cnt0+=1
		test.close()
		
		test = open(filenames[1],'r')
		cnt1=0
		for line in test:
			if line.find('#$ ')==0: cnt1+=1
		test.close()
		
		if cnt0==0 and cnt1==0:
			print 'Is this really a batch script? There are no #$'
			sys.exit(0)

		if cnt0>cnt1:
			batch_filename=filenames[0]
			config_filename=filenames[1]
		else:	
			batch_filename=filenames[1]
			config_filename=filenames[0]
	else:
		batch_filename=''
		config_filename=filenames[0]

	if(batch_filename!=''): batch_filename  = os.path.abspath(batch_filename)
	if(config_filename!=''): config_filename  = os.path.abspath(config_filename)


	#now read the sample and sub-sample from the name of the out file and put them into
	#the config file
	Sample,SubSample=InfoToConfig(config_filename)
	#now copy the config file to the output directory
	out=commands.getoutput('cp '+config_filename+' '+outdir+'/config.txt')
	out=commands.getoutput('cp para_config.txt '+outdir+'/para_config.txt')
	out=commands.getoutput('cp pu_config.txt '+outdir+'/pu_config.txt')
	outname=outname.replace('.root', '_'+Estimation+'_'+Tail+'.root')
	return

def removeDuplicates():
         # files are sorted!
         global rootfiles
         last=''
         cleaned=[]
         counter=0
         for file in rootfiles:
                 ir=file.count('_')
                 pr=file.rfind('_')
                 if ir<=3:
                         if counter<7:
                                 print 'Warning - file name not wellformed: '+file
                         elif counter==7:
                                 print '... further warnings skipped.'
                         counter+=1
                         cleaned.append(file)
                         continue
                 pr2=file[:pr-1].rfind('_')
                 pr3=file[:pr2-1].rfind('_')
                 #print "the number should be "+file[pr3+1:pr2]
                 if not file[pr3+1:pr2].isdigit():
                         if counter<7:
                                 print 'Warning - file name not wellformed: '+file
                                 print '          expect'+file[pr3+1:pr2]+' to be the job number.'
                         elif counter==7:
                                 print '... further warnings skipped.'
                         counter+=1
                         cleaned.append(file)
                         continue
                 comp=file[:pr-1]
                 #print comp
                 if comp.find('_') < 0:
                         if counter<7:
                                 print 'Warning - file name not wellformed: '+file
                         elif counter==7:
                                 print '... further warnings skipped.'
                         counter+=1
                         cleaned.append(file)
                         continue
                 if comp==last:
                         killed=cleaned.pop()
                         print 'Duplicate warning: '+killed+' has beenremoved.'
                 cleaned.append(file)
                 last=comp
         rootfiles=cleaned


## def removeDuplicates():
## 	# files are sorted!
## 	global rootfiles
## 	last=''
## 	cleaned=[]
## 	for file in rootfiles:
## 		ir=file.count('_')
## 		kin=file.rfind('_')
## 		if ir<=3:
## 			#print 'Warning - file name not well formed: '+file
## 			cleaned.append(file)
## 			continue
## 		comp=file[:kin-1]
## 		if comp.find('_') < 0: 
## 			#print 'Warning - file name not well formed: '+file
## 			cleaned.append(file)
## 			continue
## 		if comp==last:
## 			killed=cleaned.pop()
## 			print 'Duplicate warning: '+killed+' has been removed.'
## 		cleaned.append(file)
## 		last=comp
## 	rootfiles=cleaned			
		
def checkIndir():
	""" check number of files in indir
	    and return list of filenames
	"""
	global indir
	prefix=''
	lscommand='ls'
	# check if dcache
	if indir.find('/store')==0:
		indir='/pnfs/desy.de/cms/tier2/'+indir
		lscommand='dcls'
	if indir.find('/pnfs')==0:
		lscommand='dcls'

	#full name
	if lscommand=='ls': indir = os.path.abspath(indir)
	# check for ending /
	if indir[-1]!='/': indir+='/'
	
	# list directory
	allfiles=subprocess.Popen([lscommand,indir], stdout=subprocess.PIPE).communicate()[0]
	if allfiles.find('No valid Proxy found')==0:
		print 'No valid Proxy found'
		exit(0)
	if allfiles.find('The following regular expressions')==0:
		print 'Cannot: '+lscommand+' '+indir
		exit(0)

	# add dcache gate
	if lscommand=='dcls': indir=dcache_gate+indir
	
	# filter out *.root
	ret=[]
	for line in allfiles.split('\n'):
		if line.rfind('.root')>0 :ret.append(line)
	
	if len(ret)==0:
		print 'No files found in '+indir
		allfiles
		sys.exit(0)

	ret.sort()
	return ret

def printInputValues():
	global totfile
	print 'input dir :  '+indir
	print 'output dir:  '+outdir
	print 'executable:  '+executable
	if batch_filename=='':
		print 'batch_file:  internal default'
	else:
		print 'batch_file:  '+batch_filename
	print 'config_file: '+config_filename
	print 'cleanUp:     '+str(cleanUp)
	print 'noJoin:      '+str(noJoin)
	print 'dupRemoval:  '+str(dupRemoval)
	print 'files:       '+str(totfiles)
	print 'files/job:   '+str(nfiles)
	print 'output name  '+str(outname)
	if ldlp!='': print '$LD_LIBRARY_PATH found'
	
def createBatchScript():
	""" does what the name says"""
	
	if batch_filename!='':
		# if external batch_script is given
		batchLines = open(batch_filename,'r')
		add=''
	else:
		# use default batch script
		batchLines = batch_script.split('\n')  
		add='\n'

	batchfile = open('batch_script','w')
	for line in batchLines:
		if len(line)==0: continue
		if line.find('#$ -v OUTDIR=')>=0:         line+=outdir
		if line.find('#$ -o')>=0:                 line+=' '+outdir
		if line.find('#$ -v EXECUTABLE=')>=0:     line+=executable
		if line.find('#$ -v CONFIG=')>=0:         line+=config_filename
		if line.find('#$ -v LD_LIBRARY_PATH=')>=0:line+=ldlp
#		if line.find('echo batchjob created at ')>=0: line+
		batchfile.write(line+add)
	
	batchfile.close()
	if type(batchLines).__name__ == 'file': batchLines.close()



def AddCreationTimeToBatchScript(CreationTime):


	#print 'AddCreation is ', CreationTime
	batchfile = open('batch_script','r')
	newbatchfile=open('tempbatchfile','w')
	linefound=False
	#batchLines=.split('\n')
	for line in batchfile:

		#print "this line is ", line
		if(line.find('echo batchjob created')!=-1):
			#newline=line+' '+str(CreationTime)
			oldline=line
			newline='echo batchjob created at time'+' '+str(CreationTime)
			#print "it reads "+newline, 'although CreationTime is ',CreationTime
			line=line.replace(line,newline)
			#print "line now  reads "+line
			#batchfile.write(line+'\n')
			linefound=True
		#print "the line to be written is ", line
		newbatchfile.write(line)
	if(not linefound):
		newbatchfile.write('echo batchjob created at time'+' '+str(CreationTime)+'\n')
		#batchfile.write('test')
	newbatchfile.close()

	mvout=commands.getoutput('mv tempbatchfile batch_script')
	cout=commands.getoutput('grep -i "echo batchjob created" batch_script')
	#print "the grep says ", cout

	
def createMergeScript(jobids,filelist,outmergename,outname,nlistsize):
	""" does what the name says"""
	
	# first get path to hadd
	hadd=commands.getoutput('which hadd')
	if hadd.find('not found')>=0:
		print 'Cannot find hadd',hadd
		sys.exit(0)


	#================================================================
	#Now let's build chunks of 100 files for merging
	#================================================================
	mergeLines = merge_script.split('\n')  

	mergefile = open('merge_script','w')
	for line in mergeLines:
		if len(line)==0: continue
		if line.find('#$ -v OUTDIR=')>=0:         line+=outdir
		if line.find('#$ -o')>=0:                 line+=' '+outdir
		if line.find('#$ -v OUTNAME=')>=0:        line+=outmergename
		if line.find('#$ -v HADD=')>=0:           line+=hadd 		
		if line.find('#$ -v FILELIST=')>=0:       line+='"'+filelist+'"'
		if line.find('#$ -v LD_LIBRARY_PATH=')>=0:line+=ldlp
		mergefile.write(line+'\n')


	line='echo FILELIST=$FILELIST'
	mergefile.write(line+'\n')
	line='echo OUTNAME=$OUTNAME'
	mergefile.write(line+'\n')

	
	if outmergename==outname:


		line='echo FINAL MERGE'
		mergefile.write(line+'\n')		
		#INCORPORATE THE ESTIMATION NAME TO THE FINAL NAME
		#============================================================
		#OBTAINING THE WhatEstimation VARIABLE FROM THE CONFIG FILE
		#============================================================
		
		#whatestimation=commands.getoutput('grep -i estimation '+config_filename+' | awk \'{print $3}\'')
		#line='OUTNAMEESTIMATION=`echo '+outname+' | sed s/.root/_'+Estimation+'.root/`'
		#mergefile.write(line+'\n')
		#============================================================
		

		#============================================================
		#OBTAINING THE Tail VARIABLE FROM THE CONFIG FILE
		#============================================================
		#whattail=commands.getoutput('grep -i Tail '+config_filename+' | awk \'{print $3}\'')
		#it there is a tail, the size should be different than 0
		#if Tail.__len__()!=0:
		#	line='OUTNAMEESTIMATION2=$OUTNAMEESTIMATION'
		#	mergefile.write(line+'\n')		
		#	line='OUTNAMEESTIMATION=`echo $OUTNAMEESTIMATION2 | sed s/.root/_'+Tail+'.root/`'
		#	mergefile.write(line+'\n')
		#============================================================



                ##The outputs are not overwritten, a time identifier is added
                ## to the end of the name and the desired name is a symbolic link
                ##to the last file.
                ##===============================================================
		#line='TIMEID=`date | awk \'{ print $2 $3"at"$4 }\' | sed s/:/-/g`'
		#mergefile.write(line+'\n')		
		#line='OUTNAMEUNIQUE2=`echo $OUTNAME | sed s/.root/R/g`'
		#mergefile.write(line+'\n')		
		#line='OUTNAMEUNIQUE="$OUTNAMEUNIQUE2"_"$TIMEID".root'
		#mergefile.write(line+'\n')
		#line='mv $OUTNAME $OUTNAMEUNIQUE'
		#mergefile.write(line+'\n')		
		#line='echo "OUTNAME IS NOW " $OUTNAMEUNIQUE'
		#mergefile.write(line+'\n')		
		#================================================================
		
		#=============================================
		#MOVE THE FILE
		#=============================================
		#line='mkdir OutputContainer'
		#mergefile.write(line+'\n')
		#line='mv $OUTNAMEUNIQUE ./OutputContainer'
		#mergefile.write(line+'\n')

		#=============================================
		#LINK THE LAST FILE TO NAME-LAST
		#=============================================		
		#line='lastlink=`ls -lrt $OUTNAMEESTIMATION | awk \'{ print $11 }\'`'
		#mergefile.write(line+'\n')
		#line='echo "the last link was " $lastlink'
		#mergefile.write(line+'\n')		
		#line='OUTNAMELAST=`echo $OUTNAMEESTIMATION | sed s/.root/-LAST.root/`'
		#mergefile.write(line+'\n')		
		#line='echo "OUTNAMELAST IS" $OUTNAMELAST'
		#mergefile.write(line+'\n')		
		#line='rm -f $OUTNAMELAST '
		#mergefile.write(line+'\n')		
		#line='ln -s $lastlink $OUTNAMELAST'
		#mergefile.write(line+'\n')

		#=============================================
		#LINK THE LAST JOB TO THE FINAL NAME
		#=============================================		
		#line='rm -f $OUTNAMEESTIMATION'
		#mergefile.write(line+'\n')		
		#line='ln -s ./OutputContainer/$OUTNAMEUNIQUE $OUTNAMEESTIMATION'
		#mergefile.write(line+'\n')
		#line='rm -f nameofrootfile'
		#mergefile.write(line+'\n')		
		#line='echo "$OUTNAMEESTIMATION" > nameofrootfile'
		#mergefile.write(line+'\n')
		#=============================================				

	if cleanUp:
		# root files
		line='rm '+filelist+'\n'
		mergefile.write(line)		
		# old logfile
		# concatenate batch log files into common log file
		buf=''
		for id in jobids.split(','):
			buf+=' batch_script.o'+str(id)		
		#==============================================
		#FINAL MERGING
		#==============================================		
		if outmergename==outname:
			#check if is also the first merging
			if nlistsize==0:
				#it is also the first merging
				line='cat'+buf+' > runOnAllout-'+outmergename+'.log\n'
				mergefile.write(line+'\n')				
				line='rm'+buf+'\n'
				mergefile.write(line+'\n')
			else:
				#line='rm out_*[0-9]_merge.root\n'
				line='rm out_'+Estimation+'_'+Tail+'_*[0-9]_merge.root\n'
				mergefile.write(line+'\n')			
				line='rm  runOnAll_'+outname+'.log\n'
				mergefile.write(line+'\n')			
				line='cat runOnAllout-* > runOnAll_'+outname+'.log\n'
				mergefile.write(line+'\n')
				line='rm'+buf+'\n'
				mergefile.write(line+'\n')
				line='rm -f runOnAllout-*'
				mergefile.write(line+'\n') 
		else:
			line='cat'+buf+' > runOnAllout-'+outmergename+'.log\n'
			mergefile.write(line+'\n')				
			line='rm'+buf+'\n'
			mergefile.write(line+'\n')				
		#==============================================		
		line='cd $save\n'
		mergefile.write(line)
	mergefile.close()
	return filelist.split(' ')

ldlp=''





def createTreeMergeScript(StringTreeFiles,OutTreeMergeName):
	"""
	creates a script to merge the tree root files
	"""

	# first get path to hadd
	hadd=commands.getoutput('which hadd')
	if hadd.find('not found')>=0:
		print 'Cannot find hadd',hadd
		sys.exit(0)

	#treelist=''
	#for files in TreeFiles:
	#	modfiles=files.replace('.root','_tree.root')
	#	treelist+=str(modfiles)+' '
	#
	#
	#
	lines=treeMerge_script.split('\n')
	#
	#
	#
	treefile=open('treeMerge_script','w')
	for line in lines:
		if len(line)==0:
			continue
		if line.find('#$ -v OUTDIR=')>=0:         line+=outdir
		if line.find('#$ -o')>=0:                 line+=' '+outdir
		if line.find('#$ -v OUTNAME=')>=0:        line+=OutTreeMergeName
		if line.find('#$ -v HADD=')>=0:           line+=hadd 		
		if line.find('#$ -v FILELIST=')>=0:
			line+='"'+StringTreeFiles+'"'
			#print 'in the script writing ',StringTreeFiles
		if line.find('#$ -v LD_LIBRARY_PATH=')>=0:line+=ldlp
		treefile.write(line+'\n')
	#
	#

def createCronosScript(outname,cronosdir,reloutdir):
	""" does what the name says"""
	mergeLines = cronos_script.split('\n')  


	pyth=commands.getoutput('which python')
	
	mergefile = open('cronos_script','w')
	for line in mergeLines:
		if len(line)==0: continue
		if line.find('#$ -v OUTDIR=')>=0:         line+=cronosdir
		if line.find('#$ -v RELDIR=')>=0:         line+=reloutdir
		if line.find('#$ -o')>=0:                 line+=' '+cronosdir
		if line.find('#$ -v OUTNAME=')>=0:        line+=outname
		if line.find('#$ -v PYTHON=')>=0:         line+=pyth		
		if line.find('#$ -v LD_LIBRARY_PATH=')>=0:line+=ldlp
		mergefile.write(line+'\n')	
	line='echo OUTDIR=$OUTDIR'
	mergefile.write(line+'\n')
	line='echo OUTNAME=$OUTNAME'
	mergefile.write(line+'\n')		
##===================================================================


def readLDLP():
	""" read $LD_LIBRARY_PATH to pick the correct root glibc version  """	
	ldlp = os.environ.get("LD_LIBRARY_PATH")
	if ldlp==None:
		print '$LD_LIBRARY_PATH is not defined - needed to pick the right lib version.'
		sys.exit(0)
	return ldlp

def pack():
	global rootfiles
	a=rootfiles
	rootfiles=[]
	for i in range(0,len(a),nfiles):
		rootfiles.append( reduce(lambda a,b: a+' '+b,a[i:i+nfiles]) )
#----------------------------------------------------------------------------------
# dk 8.5.2011
#
if __name__ == "__main__":

	readCommandLine(sys.argv)


	# read LD_LIBRARY_PATH env variable
	ldlp=readLDLP()

	# check number of root files in indir
	rootfiles = checkIndir()


	# set the completion status to not done
	SetCompletionStatus(0)
	
	if dupRemoval: removeDuplicates()

	# total number of files
	totfiles = len(rootfiles)
		
	# pack rootfiles to nFile blocks
	if nfiles>1: pack() 
	
	printInputValues()

	if noJoin: print 'Going to create '+str(len(rootfiles))+' jobs.'
	else:      print 'Going to create '+str(len(rootfiles))+'+1 jobs.'
	createBatchScript()

	#cp executable and config file to outdir
	#copy only the executable
	#out=commands.getoutput('cp '+os.path.abspath(executable)+' '+outdir)

	#THIS FILE RECORDS THE qsub OUTPUT OF THE JOBS
	qsuboutputfile='qsub_output_'+outname[:-5]+'.log'
	qsubout = open(qsuboutputfile,'w')

	
	# now we submit the jobs
	#
	# a random str as unique name
	randstr = str(abs(hash(asctime())))
	# a string of jobids
	jobids=''
	# a string of output file names
	filelist=''

	#
	treejobids=''
	treefilelist=''
	chunksize=30
	listoffiles=[]
	listofjobids=[]
	ListOfOutFiles=[]
	modulus=0
	for idx, rootfile in enumerate(rootfiles):
		#outfile='out_'+str(idx)+'_'+randstr+'.root'
		outfile='out_' + Estimation + '_' + Tail + '_' + str(idx) + '_' + randstr + '.root'
		ListOfOutFiles.append(outfile)
		batchsendtime=commands.getoutput('date --rfc-3339=ns | awk \'{print $2}\'')

		batchfind=batchsendtime.find('+')
		bs=batchsendtime[:batchfind]
		#print "bs is ",bs
		#print "eval = ",eval(bs[9:17]+'.')/100000000.
		#batchsendtimes.append(eval(bs[0])*36000.+eval(bs[1])*3600.+eval(bs[3])*600.+eval(bs[4])*60.+eval(bs[6])*10.+eval(bs[7])+eval(bs[9:17]+'.')/10000000.)
		batchcreationtime=eval(bs[0])*36000.+eval(bs[1])*3600.+eval(bs[3])*600.+eval(bs[4])*60.+eval(bs[6])*10.+eval(bs[7])+eval(bs[9:17]+'.')/10000000.
		#print 'batchcreationtime ', batchcreationtime
		AddCreationTimeToBatchScript(batchcreationtime)
		#
		# add input directory to filenames
		files=''
		for f in rootfile.split(' '):
			files+=indir+f+' '
		print '.',
		sys.stdout.flush()

		#print 'going to send qsub -l  site=hh  batch_script "'+files+'" '+outfile
		#raw_input()
		out=commands.getoutput('qsub -l  site=hh  batch_script "'+files+'" '+outfile+' '+Estimation+' '+Tail)

		if out.find('Your job ') == 0:
			id=out.split(' ')[2]
			if id.isdigit():
                        #The output looks like 'Your job 203948209 has been...', so
		        #the next line would get 203948209 
				#jobids+=str(id)+','
				#filelist+=outfile+' '
				#NOW I CREATE THE CHUNKS OF chunksize FILES
				modulus=idx/chunksize
				if idx%chunksize==0:
					listoffiles.append(outfile+' ')
					listofjobids.append(str(id)+',')
				else:
					listoffiles[modulus]+=outfile+' '
					listofjobids[modulus]+=str(id)+','
				#==================================================
				#RECORD THE OUTPUT OF THE QSUB IN CASE ONE FAILS
				#==================================================
				qsubout.write(out+'\n')

			else: print 'Something wrong in submit: ',out
		else: print 'Something wrong in submit: ',out
	#This means jobids is equal to itself except for the last character
	#jobids   =   jobids[:-1]
	#filelist = filelist[:-1]
	#remove the last character of all the strings in the list

	nlist=range(modulus+1)
	for idx in nlist:
		listoffiles[idx]=listoffiles[idx][:-1]
		listofjobids[idx]=listofjobids[idx][:-1]
	#=============================================
	#Create and send each merging job
	#=============================================
	doCronos=False
	cronosdir=commands.getoutput("pwd")
	dofinalmerge=True
	nlist=range(modulus+1)
	for idx in nlist:
		
		#the outputname of this merging job
		#outrandfile='out_'+str(idx)+'_merge.root'
		outrandfile='out_'+Estimation+'_'+Tail+'_'+str(idx)+'_merge.root'
		
		outmergefile=outrandfile
		#If there are less than chunksize files,
		#then don't bother doing a final step
		#(the last element of nlist will be 0)
		if nlist[-1]==0:
			outmergefile=outname
			dofinalmerge=False
		
		print "outmergefile is "+outmergefile	
		createMergeScript(listofjobids[idx],listoffiles[idx],outmergefile,outname,nlist[-1])

		# send the merging job
		if not noJoin: out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+listofjobids[idx]+' merge_script')

		#record the jobs sent at this step
		if out.find('Your job ') == 0:
			id=out.split(' ')[2]
			if id.isdigit():
				jobids+=str(id)+','
				filelist+=outmergefile+' '
		        else: print 'Something wrong in submit: ',out
		else: print 'Something wrong in submit: ',out



		#
		#
		#
		#now for the tree files merger:
		#put the tree in the names
		
		thefiles=listoffiles[idx].split(',')
		listoftreefiles=''
		for file in thefiles:
			newfile=file.replace('.root','_tree.root')
			listoftreefiles+=newfile+','
		#remove the last comma
		listoftreefiles=listoftreefiles[:-1]


		outmergetreefile=outmergefile.replace('_merge.root','_tree_merge.root')
		#if only one merging is necessary, then its different:
		if nlist[-1]==0:
			outmergetreefile=outname.replace('.root','_Tree.root')
		#
		#
		#
		#print 'going to create the tree merger with ',listoftreefiles
		#print 'and merge it to ',outmergetreefile
		createTreeMergeScript(listoftreefiles,outmergetreefile)
		if not noJoin:
			out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+listofjobids[idx]+' treeMerge_script')
		#

		if out.find('Your job ') == 0:
			id=out.split(' ')[2]
			if id.isdigit():
				treejobids+=str(id)+','
				treefilelist+=outmergetreefile+' '
		        else: print 'Something wrong in submit: ',out
		else: print 'Something wrong in submit: ',out

		

		
		##if only one mergin was necessary, send the cronos script after it
		if nlist[-1]==0:
	                #cp the python script in the outdir
                	#out=commands.getoutput('cp ./CronosExtractInfo.py ' +outdir)
			if doCronos:
			    createCronosScript(outname,cronosdir,reloutdir)
			    out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+id+' cronos_script')
			    id=out.split(' ')[2]
			    print 'id cronos is ',id
			    if id.isdigit():
				    lastid=str(id)
			    else:
				    print "something is wrong with the completion script"
				    #out=commands.getoutput('qsub -l site=hh -m a hold_jid '+lastid+' completion_script')
			print "the submission is ",out

			#OutTreeMergeName=Sample+'_'+SubSample+'_'+Estimation+'_'+Tail+'_Tree.root'
			#createTreeMergeScript(ListOfOutFiles,OutTreeMergeName)
			#out=commands.getoutput('rm -f treeMerge_script.o\*')
			#out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+id+' treeMerge_script')


			#set the completion of the job now
		
			
	#REMOVE THE LAST CHARACTER
	jobids   =   jobids[:-1]
	filelist = filelist[:-1]
	treejobids = treejobids[:-1]
	treefilelist=treefilelist[:-1]
	print 'the tree file list is ',treefilelist
        #=============================================
	#Create and send the LAST merging job (if necessary)
	#=============================================
	if dofinalmerge:
		createMergeScript(jobids,filelist,outname,outname,nlist[-1])
		if not noJoin: out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+jobids+' merge_script')

		id=out.split(' ')[2]
		if id.isdigit():
			lastjobid=id


		#outmergetreefile=outname.replace('.root','_'+Estimation+'_'+Tail+'_Tree.root')
		outmergetreefile=outname.replace('.root','_Tree.root')
		print 'on the final merge, the files are ',treefilelist
		createTreeMergeScript(treefilelist,outmergetreefile)
		if not noJoin:
			#pass
			out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+treejobids+' treeMerge_script')
		#
		#
		#Send the cronos script
		#out=commands.getoutput('cp ./CronosExtractInfo.py ' +outdir)
		if doCronos:
		    createCronosScript(outname,cronosdir,reloutdir)
		    #
		    out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+lastjobid+' cronos_script')
		    id=out.split(' ')[2]		
		    print 'id cronos is ',id
		    if id.isdigit():
		        lastjobid=id
		    else:
			print "something is wrong with the completion script"
			
		#print 'executing the command ' + 'qsub -l site=hh -m a -hold_jid '+lastjobid+' completion_script'
		#out=commands.getoutput('rm -f completion_script.o\*')
		#out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+lastjobid+' completion_script')

		#OutTreeMergeName=Sample+'_'+SubSample+'_'+Estimation+'_'+Tail+'_Tree.root'
		#createTreeMergeScript(ListOfOutFiles,OutTreeMergeName)
		#out=commands.getoutput('rm -f treeMerge_script.o\*')
		#out=commands.getoutput('qsub -l site=hh -m a -hold_jid '+lastjobid+' treeMerge_script')
			
	
		
	else:
		print "Only one merging step necessary"
	
#  LocalWords:  PWD
