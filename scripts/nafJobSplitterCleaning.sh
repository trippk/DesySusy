#!/bin/zsh

for dir in `ls | grep naf_`  
do
for completefile in `ls $dir/*.root`
do
rm -f $completefile.part*
done
done

