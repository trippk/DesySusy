#!/afs/desy.de/products/python/.amd64_rhel50/2.6/bin/python
from math import sqrt
from sys import exit

# sample names according to: base/base_tag_sample.txt
samples = ['TTbar','BosonJets','TopJets','pMSSM1','pMSSM2']

# we print a table without the last n lines
# with systematic error sys1 and sys2
# base is used for path/ and fileName as base/base_
# tag is PhaseI_C0_NoPU,PhaseI_C0_140PU,PhaseII_C3_140PU,PhaseII_C4_140PU
def aTable(base,tag,sys1,sys2,n):
	first=base+'/'+base+'_'+tag+'_'
	linesList=[]
	for s in samples:
		file = open(first+s+'.txt')
		linesList.append(file.readlines())
		file.close
	N=len(linesList[0])-n
	print base
	print ('%16s '+6*'%11s ') % (tag,'ttbar','B+jets','single t','sum bgrds','pMSSM1','pMSSM2')
	for i in range(2,N):
		d=[] # data
		for k in range(len(linesList)):
			d.append(float(linesList[k][i].split('\t')[3]))
		what=linesList[0][i].split('\t')[0][10:]
		nbgrd=d[0]+d[1]+d[2]
		print ('%16s '+6*'%11i ') % ( what,d[0],d[1],d[2],nbgrd,d[3],d[4] )
		a=sqrt(nbgrd)
		b=sqrt(nbgrd*(1+sys1*sys1*nbgrd))
		c=sqrt(nbgrd*(1+sys2*sys2*nbgrd))
		as3=sqrt(d[3]+nbgrd)
		bs3=sqrt(d[3]+nbgrd*(1+sys1*sys1*nbgrd))
		cs3=sqrt(d[3]+nbgrd*(1+sys2*sys2*nbgrd))
		as4=sqrt(d[4]+nbgrd)
		bs4=sqrt(d[4]+nbgrd*(1+sys1*sys1*nbgrd))
		cs4=sqrt(d[4]+nbgrd*(1+sys2*sys2*nbgrd))
	print ('%22s '+42*' '+2*'%11f ') % ( 's/sqrt(b)',d[3]/a,d[4]/a )
	print ('%22s '+42*' '+2*'%11f ') % ( 's/sqrt(b+('+str(sys1)+'*b)^2)',d[3]/b,d[4]/b )
	print ('%22s '+42*' '+2*'%11f ') % ( 's/sqrt(b+('+str(sys2)+'*b)^2)',d[3]/c,d[4]/c )

	print ('%22s '+42*' '+2*'%11f ') % ( 's/sqrt(s+b)',d[3]/as3,d[4]/as4 )
	print ('%22s '+42*' '+2*'%11f ') % ( 's/sqrt(s+b+('+str(sys1)+'*b)^2)',d[3]/bs3,d[4]/bs4 )
	print ('%22s '+42*' '+2*'%11f ') % ( 's/sqrt(s+b+('+str(sys2)+'*b)^2)',d[3]/cs3,d[4]/cs4 )
	print



ana='SingleS'
# here we print <<<<<<<<<<<<<<<<<<<<<<<<
aTable(ana,'PhaseI_C0_NoPU', 0.25,0.15,3)
print '----------------------------------------------------------------------------------------'
aTable(ana,'PhaseI_C0_140PU', 0.25,0.15,3)
print '----------------------------------------------------------------------------------------'
aTable(ana,'PhaseII_C3_140PU',0.25,0.15,3)
print '----------------------------------------------------------------------------------------'
aTable(ana,'PhaseII_C4_140PU',0.25,0.15,3)
