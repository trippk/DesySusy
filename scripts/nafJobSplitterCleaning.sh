#!/bin/zsh

for dir in `ls | grep naf_`  
do
task=`echo $dir | sed 's/naf_//g'`
for file in `ls $dir/out*.txt`
do
remove=`echo $file | sed 's/out/'$task'-/g' | sed 's/.txt/.root.part/g'`
rm -f $remove\*
done
done
