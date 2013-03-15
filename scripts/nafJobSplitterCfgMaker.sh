#!/bin/zsh
DSTYPE=$1
DS=$2
GT=$3
CFGNAME=$4
if [ $5 ]
then
JSON=$5
fi

echo $CFGNAME

das="https://cmsweb.cern.ch/das/"
py="makepy?dataset="$DS"&instance=cms_dbs_prod_global"
file=`echo $py | sed 's/\//%2F/g'`
echo $down

wget --no-check-certificate "https://cmsweb.cern.ch/das/"$py

rm -f $CFGNAME
touch $CFGNAME

echo "import FWCore.ParameterSet.Config as cms" >> $CFGNAME
echo "from SusyCAF_Tree_"$DSTYPE"_cfg import *" >> $CFGNAME
if [ $4 ]
then
echo "import PhysicsTools.PythonAnalysis.LumiList as LumiList" >> $CFGNAME
echo 'process.source.lumisToProcess = LumiList.LumiList(filename = "'$JSON'").getVLuminosityBlockRange()' >> $CFGNAME
fi

echo "from FWCore.ParameterSet.VarParsing import VarParsing as VP" >> $CFGNAME
echo "options = VP('standard')" >> $CFGNAME
echo "options.register('skipEvents', default = 0 )" >> $CFGNAME
echo "options.parseArguments()" >> $CFGNAME

echo 'process.GlobalTag.globaltag = "'$GT'"' >> $CFGNAME
echo "process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )" >> $CFGNAME

value=0
while read line
do
line=`echo $line |  sed 's/ = secFiles/ = secFiles,skipEvents = cms.untracked.uint32(0) /g'`
#line=`echo $line |  sed 's/\/store/root:\/\/xrootd.t2.ucsd.edu\/\/store/g'`
value=`expr $value + 1`;
if [[ $value -lt 4 ]]
then
continue
fi
echo $line | sed 's/source/process.source/g' >> $CFGNAME
done < $file

echo "if options.skipEvents >= 0:" >> $CFGNAME
echo "    process.source.skipEvents=options.skipEvents" >> $CFGNAME
echo "else:" >> $CFGNAME
echo "    print 'Warning: negative skipEvents value. skipEvents will be set to zero.'" >> $CFGNAME
echo "process.options.wantSummary=True" >> $CFGNAME
echo "process.MessageLogger.cerr.FwkSummary.reportEvery=1" >> $CFGNAME
echo "">> $CFGNAME
echo "process.load('TopAnalysis.TopUtils.SignalCatcher_cfi')" >> $CFGNAME

rm $file
