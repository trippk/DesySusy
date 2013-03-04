#!/bin/zsh
for i in 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4
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
sleep 3600
done
