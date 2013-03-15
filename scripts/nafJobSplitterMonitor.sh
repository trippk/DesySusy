#!/bin/zsh
REPEAT=24
if [ $1 ]
then
REPEAT=$1
fi

for ((i = 0; i < $REPEAT; i++))
do
for dir in `ls | grep naf_ | grep _cfg`
do
for errFile in `nafJobSplitter.pl check $dir | grep cmsRun | awk '{print $6}'`
do
set -- "${errFile%.txt*}";
errFile="${1##*: }".txt
rm $errFile
echo $errFile
done
nafJobSplitter.pl check $dir
echo `ls $dir | grep .root | wc -l`
done
value=`expr $REPEAT - 1`;
if [[ $i -lt $value ]]
then
sleep 3600
fi
done
