#!/bin/zsh
DSTYPE=$1
DS=$2
GT=$3
CFGNAME=$4
if [ $5 ]
    then
    JSON=$5
fi
DESYSTORAGE="dcache-se-cms.desy.de"
XRDSERVER="root://cms-xrd-global.cern.ch/"

echo $CFGNAME

rm -f $CFGNAME
touch $CFGNAME

echo "import FWCore.ParameterSet.Config as cms" >> $CFGNAME
echo "from SusyCAF_Tree_"$DSTYPE"_cfg import *" >> $CFGNAME
if [ $5 ]
    then
    echo "import FWCore.PythonUtilities.LumiList as LumiList" >> $CFGNAME
    echo 'process.source.lumisToProcess = LumiList.LumiList(filename = "'$JSON'").getVLuminosityBlockRange()' >> $CFGNAME
fi

echo "from FWCore.ParameterSet.VarParsing import VarParsing as VP" >> $CFGNAME
echo "options = VP('standard')" >> $CFGNAME
echo "options.register('skipEvents', default = 0 )" >> $CFGNAME
echo "options.parseArguments()" >> $CFGNAME

echo 'process.GlobalTag.globaltag = "'$GT'"' >> $CFGNAME
echo "process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )" >> $CFGNAME
echo "readFiles = cms.untracked.vstring()" >> $CFGNAME
echo "secFiles = cms.untracked.vstring()" >> $CFGNAME
echo "process.source = cms.Source ('PoolSource',fileNames = readFiles, secondaryFileNames = secFiles,skipEvents = cms.untracked.uint32(0) )" >> $CFGNAME
value=0

rm -f filelist.txt
dbs search --query="find file where dataset=$DS" | grep .root > filelist.txt

flag=`dbs search --query="find site where dataset=$DS" | grep $DESYSTORAGE`
if [ $flag ]
    then
    XRDSERVER=""
fi

while read line
  do
  value=`expr $value + 1`;
  if [[ $value -eq 1 ]]
      then
      echo "readFiles.extend( [" >> $CFGNAME
  fi
  if [[ $value -eq 253 ]]
      then
      echo \"${XRDSERVER}${line}\""] );" >> $CFGNAME
      value=0
  else
      echo \"${XRDSERVER}${line}\""," >> $CFGNAME
  fi
done < filelist.txt

echo "] );" >> $CFGNAME
echo "secFiles.extend( [] )" >> $CFGNAME
echo "if options.skipEvents >= 0:" >> $CFGNAME
echo "    process.source.skipEvents=options.skipEvents" >> $CFGNAME
echo "else:" >> $CFGNAME
echo "    print 'Warning: negative skipEvents value. skipEvents will be set to zero.'" >> $CFGNAME
echo "process.options.wantSummary=True" >> $CFGNAME
echo "process.MessageLogger.cerr.FwkSummary.reportEvery=1" >> $CFGNAME
echo "">> $CFGNAME
echo "process.load('TopAnalysis.TopUtils.SignalCatcher_cfi')" >> $CFGNAME
