#!/bin/zsh
DS=$1
GT=$2
cfgname=$3

echo $cfgname

das="https://cmsweb.cern.ch/das/"
py="makepy?dataset="$DS"&instance=cms_dbs_prod_global"
file=`echo $py | sed 's/\//%2F/g'`
echo $down

wget --no-check-certificate "https://cmsweb.cern.ch/das/"$py

rm -f $cfgname
touch $cfgname

echo "import FWCore.ParameterSet.Config as cms" >> $cfgname
echo "from SusyCAF_Tree_mc_cfg import *" >> $cfgname

echo "from FWCore.ParameterSet.VarParsing import VarParsing as VP" >> $cfgname
echo "options = VP('standard')" >> $cfgname
echo "options.register('skipEvents', default = 0 )" >> $cfgname
echo "options.parseArguments()" >> $cfgname

echo 'process.GlobalTag.globaltag = "'$GT'"' >> $cfgname
echo "process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )" >> $cfgname

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
echo $line | sed 's/source/process.source/g' >> $cfgname
done < $file

echo "if options.skipEvents >= 0:" >> $cfgname
echo "    process.source.skipEvents=options.skipEvents" >> $cfgname
echo "else:" >> $cfgname
echo "    print 'Warning: negative skipEvents value. skipEvents will be set to zero.'" >> $cfgname
echo "process.options.wantSummary=True" >> $cfgname
echo "process.MessageLogger.cerr.FwkSummary.reportEvery=1" >> $cfgname
echo "">> $cfgname
echo "process.load('TopAnalysis.TopUtils.SignalCatcher_cfi')" >> $cfgname

rm $file
