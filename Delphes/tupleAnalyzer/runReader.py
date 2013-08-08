#!/usr/bin/python

import sys
from ROOT import gROOT

# scenarios and samples
scenarios = ['NoPU','50PU','140PU']
samples   = ['DiBoson','BosonJets','TopJets','TTbar','TDR4','TDR5','TDR6','TDR8']
#samples w.o weight
noWeight  = ['TDR4','TDR5','TDR6','TDR8']

# nTuple input path
base = 'nTuples/snowmass/'
Lumi=300
#base = 'nTuples/ECFA/'
#Lumi=3000



def help():
	print 'First argument analysis:'
	print '   SingleS'
	print ' then a list of inputs - read the code'
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
	else: 
		help()
else: 
	help()


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
if not flag: help()


# prepare empty dictionaries
dirsHT={}
inDir={}
weights={}
for scene in scenarios:
	inDir[scene]={}
#	weights[scene]={}

#--------------------------------------------- sample properties for processing
#
#------------------------------------------------ diboson
# HT dirs 
dirsHT['DiBoson']  = ['0-300/','300-700/','700-1300/','1300-2100/','2100-100000/']
#    xsec
weights['DiBoson'] = [249.97, 35.23, 4.137, 0.417, 0.0477]
#    multiply all by lumi
weights['DiBoson'] = scale(Lumi,weights['DiBoson'])
#
# NoPU
inDir['NoPU']['DiBoson']  = base+'/NoPU/DiBoson/'
# 50PU
inDir['50PU']['DiBoson']  = base+'/50PU/DiBoson/'
# 140PU
inDir['140PU']['DiBoson'] = base+'/140PU/DiBoson/'
#
#------------------------------------------------ boson+jets
# HT dirs 
dirsHT['BosonJets']  = ['0-300','300-600/','600-1100/','1100-1800/','1800-2700/','2700-3700/','3700-100000/']
#    xsec
weights['BosonJets'] = [34409.92339,2642.85309,294.12311,25.95000,2.42111,0.22690,0.02767]
#    multiply all by lumi
weights['BosonJets'] = scale(Lumi,weights['DiBoson'])
#
# NoPU
inDir['NoPU']['BosonJets']  = base+'/NoPU/bjets/'
# 50PU
inDir['50PU']['BosonJets']  = base+'/50PU/bjets/'
# 140PU
inDir['140PU']['BosonJets'] = base+'/140PU/bjets/'
#
#---------------------------------------------- ttbar
# HT dirs 
dirsHT['TTbar']  = ['0-600/','600-1100/','1100-1700/','1700-2500/','2500-100000/']
#    xsec
weights['TTbar'] = [530.89358,42.55351,4.48209,0.52795,0.05449]
#    multiply all by lumi
weights['TTbar'] = scale(Lumi,weights['TTbar'])
#
# NoPU
inDir['NoPU']['TTbar']  = base+'/NoPU/ttbar/'
# 50PU
inDir['50PU']['TTbar']  = base+'/50PU/ttbar/'
# 140PU
inDir['140PU']['TTbar'] = base+'/50PU/ttbar/'
#
#--------------------------------------------- tjets
# HT dirs 
dirsHT['TopJets']  = ['0-500/','500-1000/','1000-1600/','1600-2400/','2400-100000/']
#    xsec
weights['TopJets'] = [109.73602,5.99325,0.37680,0.03462,0.00312]
#    multiply all by lumi
weights['TopJets'] = scale(Lumi,weights['TopJets'])
#
# NoPU
inDir['NoPU']['TopJets']  = base+'/NoPU/tjets/'
# 50PU
inDir['50PU']['TopJets']  = base+'/50PU/tjets/'
# 140PU
inDir['140PU']['TopJets'] = base+'/140PU/tjets/'
#---------------------------------------------STCs
# HT dirs - none for STC samples
dirsHT['TDR4'] = ['/']
dirsHT['TDR5'] = ['/']
dirsHT['TDR6'] = ['/']
dirsHT['TDR8'] = ['/']
#    xsec - we multiply by lumi
weights['TDR4'] = [12.80*Lumi]
weights['TDR5'] = [ 3.82*Lumi]
weights['TDR6'] = [ 2.53*Lumi]
weights['TDR8'] = [ 2.19*Lumi]
#
# NoPU
inDir['NoPU']['TDR4'] = base+'/NoPU/tdr/4/'
inDir['NoPU']['TDR5'] = base+'/NoPU/tdr/5/'
inDir['NoPU']['TDR6'] = base+'/NoPU/tdr/6/'
inDir['NoPU']['TDR8'] = base+'/NoPU/tdr/8/'
# 50PU
inDir['50PU']['TDR4'] = base+'/50PU/tdr/4/'
inDir['50PU']['TDR5'] = base+'/50PU/tdr/5/'
inDir['50PU']['TDR6'] = base+'/50PU/tdr/6/'
inDir['50PU']['TDR8'] = base+'/50PU/tdr/8/'
# 140PU
inDir['140PU']['TDR4'] = base+'/140PU/tdr/4/'
inDir['140PU']['TDR5'] = base+'/140PU/tdr/5/'
inDir['140PU']['TDR6'] = base+'/140PU/tdr/6/'
inDir['140PU']['TDR8'] = base+'/140PU/tdr/8/'
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





