#!/bin/bash
echo "start processing..."
 JOBPATH=$PWD
  echo $JOBPATH
  echo -e "Please input the PMT serial number: \c"
  read pmtsn
 echo "the current PMT is $pmtsn "
  date=$(ls /home/pmthome/aptest |grep -m 1 $pmtsn |cut -d '_' -f 2)
#echo $date
echo "the $date sss"
rm /home/pmthome/aptest/Analysis_cy/AfterpulseAnalysis
make AfterpulseAnalysis
/home/pmthome/aptest/Analysis_cy/AfterpulseAnalysis -s $pmtsn -d $date
