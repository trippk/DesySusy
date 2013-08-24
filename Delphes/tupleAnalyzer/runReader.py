#!/usr/bin/python

import sys
from ROOT import gROOT

# scenarios and samples
scenarios = ['PhaseI_C0_NoPU','PhaseI_C0_140PU','PhaseII_C3_140PU','PhaseII_C4_140PU']
samples   = ['BosonJets','TopJets','TTbar','pMSSM1','pMSSM2']  # DiBoson not available
# by historic reason we have as dir names: diboson bjets  pmssm1  pmssm2  tjets  ttbar

#samples w.o weight
noWeight  = []

# nTuple input path
#base = 'nTuples/snowmass/'
# lumi in fb^-1
#Lumi=300
base = 'nTuples/ECFA/'
Lumi=3000

def help(where):
	print where+'?'
	print 'First argument analysis:'
	print '   SingleS'
	print ' then a list of inputs - read the code'
	print 'e.g. ./runReader.py SingleS PhaseI_C0_NoPU_pMSSM1' 
	sys.exit(0)

from operator import mul
def scale(fac,list):
	return map(mul,len(list)*[fac],list)

# choose the analysis and a sample
if len(sys.argv)>1:
	if sys.argv[1]=='SingleS':       # single lepton  stop - CMS
		gROOT.LoadMacro('readerSingleS.C+')
		from ROOT import readerSingleS as reader
	elif sys.argv[1]=='SingleSdev':  # single lepton testing version 
		gROOT.LoadMacro('readerSingleSdev.C+')
		from ROOT import readerSingleSdev as reader
	elif sys.argv[1]=='EWKino':  # EWKino
		gROOT.LoadMacro('readerEWKino.C+') # used to be EWKinoNew
		from ROOT import readerEWKino as reader
	elif sys.argv[1]=='AtlasH':  # Atlas hadronic
		gROOT.LoadMacro('readerAtlasH.C+') # used to be ?????
		from ROOT import readerAtlasH as reader
	else: 
		help(sys.argv[1])
else: 
	help("No argument given")


#prepare empty dictionaries
do={}
for scene in scenarios:
	do[scene]={}
	for samp in samples:
		do[scene][samp] = 0
# read input line and set flags what to process
flag=False
for e in sys.argv:
	for scene in scenarios:
		for samp in samples:
			if e==scene+'_'+samp: 
 				do[scene][samp] = 1
				flag=True
if not flag: help(sys.argv[2])


# prepare empty dictionaries
dirsHT={}
inDir={}
weights={}
for scene in scenarios:
	inDir[scene]={}
#	weights[scene]={}

#--------------------------------------------- sample properties for processing <<<<<<<<<<<<<<<<<<<<<<<<<<<
#
#------------------------------------------------ diboson
# HT dirs 
#dirsHT['DiBoson']  = ['0-300/','300-700/','700-1300/','1300-2100/','2100-100000/']
#    xsec
#weights['DiBoson'] = [249.97, 35.23, 4.137, 0.417, 0.0477]
#    multiply all by lumi
#weights['DiBoson'] = scale(Lumi,weights['DiBoson'])
#
# NoPU C0
#inDir['PhaseI_C0_NoPU']['DiBoson']   = base+'PhaseI_C0/NoPU/diboson/'
# 140PU C0
#inDir['PhaseI_C0_140PU']['DiBoson']  = base+'PhaseI_C0/140PU/diboson/'
# 140PU C3
#inDir['PhaseII_C3_140PU']['DiBoson'] = base+'/PhaseII_C3/140PU/diboson/'
# 140PU C4
#inDir['PhaseII_C4_140PU']['DiBoson'] = base+'/PhaseII_C4/140PU/diboson/'
#
#------------------------------------------------ boson+jets
# HT dirs 
dirsHT['BosonJets']  = ['0-300','300-600/','600-1100/','1100-1800/','1800-2700/','2700-3700/','3700-100000/']
#    xsec
weights['BosonJets'] = [34409.92339,2642.85309,294.12311,25.95000,2.42111,0.22690,0.02767]
#    multiply all by lumi
weights['BosonJets'] = scale(Lumi,weights['BosonJets'])
#
# NoPU C0
inDir['PhaseI_C0_NoPU']['BosonJets']    = base+'PhaseI_C0/NoPU/bjets/'
# 140PU C0
inDir['PhaseI_C0_140PU']['BosonJets']  = base+'PhaseI_C0/140PU/bjets/'
# 140PU C3
inDir['PhaseII_C3_140PU']['BosonJets'] = base+'/PhaseII_C3/140PU/bjets/'
# 140PU C4
inDir['PhaseII_C4_140PU']['BosonJets'] = base+'/PhaseII_C4/140PU/bjets/'
#
#---------------------------------------------- ttbar
# HT dirs 
dirsHT['TTbar']  = ['0-600/','600-1100/','1100-1700/','1700-2500/','2500-100000/']
#    xsec
weights['TTbar'] = [530.89358,42.55351,4.48209,0.52795,0.05449]
#    multiply all by lumi
weights['TTbar'] = scale(Lumi,weights['TTbar'])
#
# NoPU C0
inDir['PhaseI_C0_NoPU']['TTbar']    = base+'PhaseI_C0/NoPU/ttbar/'
# 140PU C0
inDir['PhaseI_C0_140PU']['TTbar']  = base+'PhaseI_C0/140PU/ttbar/'
# 140PU C3
inDir['PhaseII_C3_140PU']['TTbar'] = base+'/PhaseII_C3/140PU/ttbar/'
# 140PU C4
inDir['PhaseII_C4_140PU']['TTbar'] = base+'/PhaseII_C4/140PU/ttbar/'
#
#--------------------------------------------- tjets
# HT dirs 
dirsHT['TopJets']  = ['0-500/','500-1000/','1000-1600/','1600-2400/','2400-100000/']
#    xsec
weights['TopJets'] = [109.73602,5.99325,0.37680,0.03462,0.00312]
#    multiply all by lumi
weights['TopJets'] = scale(Lumi,weights['TopJets'])
#
# NoPU C0
inDir['PhaseI_C0_NoPU']['TopJets']    = base+'PhaseI_C0/NoPU/tjets/'
# 140PU C0
inDir['PhaseI_C0_140PU']['TopJets']  = base+'PhaseI_C0/140PU/tjets/'
# 140PU C3
inDir['PhaseII_C3_140PU']['TopJets'] = base+'/PhaseII_C3/140PU/tjets/'
# 140PU C4
inDir['PhaseII_C4_140PU']['TopJets'] = base+'/PhaseII_C4/140PU/tjets/'
#
#---------------------------------------------pMSSMs
# HT dirs - none for pMSSM samples
dirsHT['pMSSM1'] = ['/']
dirsHT['pMSSM2'] = ['/']
#    xsec - we multiply by lumi 3.846D-09 PYTHIA LO !!!!!!
#                                2.043D-10
weights['pMSSM1'] = [0.2043*Lumi]  # is this correct assigned ???
weights['pMSSM2'] = [3.846*Lumi]
#
# NoPU C0
inDir['PhaseI_C0_NoPU']['pMSSM1'] = base+'PhaseI_C0/NoPU/pmssm1'
inDir['PhaseI_C0_NoPU']['pMSSM2'] = base+'PhaseI_C0/NoPU/pmssm2'
# 140PU C0
inDir['PhaseI_C0_140PU']['pMSSM1'] = base+'PhaseI_C0/140PU/pmssm1'
inDir['PhaseI_C0_140PU']['pMSSM2'] = base+'PhaseI_C0/140PU/pmssm2'
# 140PU C3
inDir['PhaseII_C3_140PU']['pMSSM1'] = base+'PhaseII_C3/140PU/pmssm1'
inDir['PhaseII_C3_140PU']['pMSSM2'] = base+'PhaseII_C3/140PU/pmssm2'
# 140PU C4
inDir['PhaseII_C4_140PU']['pMSSM1'] = base+'PhaseII_C4/140PU/pmssm1'
inDir['PhaseII_C4_140PU']['pMSSM2'] = base+'PhaseII_C4/140PU/pmssm2'
#--------------------------------------------- end of sample properties

# 
from ROOT import TFile
from glob import glob
from sys import exit
def GetEntries(dirname):
	files = glob(dirname+'/*.root')
	if len(files)>1:
		print 'GetEntries: there is more than 1 root file in '+dirname
		exit(0)
	print dirname,files
	file=TFile(files[0])
	tree = file.Get("delphTree")
	return tree.GetEntries()
	
# do it
for scene in scenarios:
	for samp in samples:
		if do[scene][samp]: 
			f=''
			for i in range(len(dirsHT[samp])):
				entries = GetEntries(inDir[scene][samp]+dirsHT[samp][i])
				f=f+inDir[scene][samp]+dirsHT[samp][i]+' '+str(weights[samp][i]/entries)+' '
			print f,samp,scene
			if samp in noWeight:
				reader(f,scene+'_'+samp,False)
			else:   
				reader(f,scene+'_'+samp)





