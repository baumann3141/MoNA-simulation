#!/bin/bash

for file in `ls *.cross`
do
	pot=`echo $file | awk -F_ '{print $3}' | awk -F. '{print $1"."$2}'`
	awk -v pot=$pot '{if ($2>0){print pot" "$0}}' $file
	echo ""
done
