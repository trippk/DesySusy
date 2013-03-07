

from Looper import *


myfiles={}
myfiles['a']='../MuHad/Run2012A-13Jul/MuHad_Run2012A-13Jul_RA4b-mu_NoTail_Tree_Out.root'
#
#
loop=Looper()
#
loop.InitializeFiles(myfiles)

loop.StartLoop(loop.TFiles['a'])


pattern={}
#pattern['name_contains']='PtJet'
pack=loop.NextHisto(pattern)
print pack
while pack != 'End':
    pack=loop.NextHisto(pattern)
    print 'pack = ',pack

