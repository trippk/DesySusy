#!/afs/desy.de/products/python/.amd64_rhel50/2.6/bin/python
from math import sqrt
from sys import exit

# sample names according to: base/base_tag_sample.txt
samples = ['DiBoson','TTbar','BosonJets','TopJets','TDR4','TDR5','TDR6','TDR8']

# we print a table without the last n lines
# with systematic error sys1 and sys2
# base is used for path/ and fileName as base/base_
# tag is NoPU,50PU,140PU
def aTable(base,tag,sys1,sys2,n):
	first=base+'/'+base+'_'+tag+'_'
	linesList=[]
	for s in samples:
		file = open(first+s+'.txt')
		linesList.append(file.readlines())
		file.close
	N=len(linesList[0])-n
	print base
	print ('%12s '+9*'%10s ') % (tag,'dibos','ttbar','B+jets','single t','sum bgrds','STC4','STC5','STC6','STC8')
	for i in range(2,N):
		d=[] # data
		for k in range(len(linesList)):
			d.append(float(linesList[k][i].split('\t')[3]))
		what=linesList[0][i].split('\t')[0][10:]
		nbgrd=d[0]+d[1]+d[2]+d[3]
		print ('%12s '+9*'%10i ') % ( what,d[0],d[1],d[2],d[3],nbgrd,d[4],d[5],d[6],d[7] )
	a=sqrt(nbgrd)
	b=sqrt(nbgrd*(1+sys1*sys1*nbgrd))
	c=sqrt(nbgrd*(1+sys2*sys2*nbgrd))
	print ('%12s '+55*' '+4*'%10f ') % ( 's/sqrt(b)',d[4]/a,d[5]/a,d[6]/a,d[7]/a )
	print ('%19s '+48*' '+4*'%10f ') % ( 's/sqrt(b+('+str(sys1)+'*b)^2)',d[4]/b,d[5]/b,d[6]/b,d[7]/b )
	print ('%19s '+48*' '+4*'%10f ') % ( 's/sqrt(b+('+str(sys2)+'*b)^2)',d[4]/c,d[5]/c,d[6]/c,d[7]/c )
	print



# here we plot <<<<<<<<<<<<<<<<<<<<<<<<
aTable('SingleS','NoPU', 0.25,0.15,2)
aTable('SingleS','50PU', 0.25,0.15,2)
aTable('SingleS','140PU',0.25,0.15,2)
print '-----------------------------------------------------------------------------'
aTable('EWKino', 'NoPU',  0.3,0.2,6)
aTable('EWKino', '50PU',  0.3,0.2,6)
aTable('EWKino', '140PU', 0.3,0.2,6)
print '-----------------------------------------------------------------------------'
aTable('AtlasH', 'NoPU',  0.3,0.2,0)
aTable('AtlasH', '50PU',  0.3,0.2,0)
aTable('AtlasH', '140PU', 0.3,0.2,0)
