#!/bin/bash
for (( count=$3; count>0; count-- ))
do
	echo "ejecución $count:" >> $2 
	echo >> $2
	{ time $1 ;} 2>> $2
	echo >> $2
done