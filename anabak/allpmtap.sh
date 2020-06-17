#!/bin/bash
sns=$(ls /home/pmthome/aptest/|grep PA|grep 3.dat|cut -d '_' -f 3)
dates=$(ls /home/pmthome/aptest/|grep PA|grep 3.dat|cut -d '_' -f 2)
echo $sns
echo $dates
count=$(ls /home/pmthome/aptest/|grep PA|grep 3.dat|cut -d '_' -f 2|wc -l)
echo $count
for  ((i=1; i<=$count; i++)) 
do
date=$(echo $dates|cut -d ' ' -f $i)
sn=$(echo $sns|cut -d ' ' -f $i)
	if [ $date -lt 20191200 ]
	then
	echo $date
	else
	cd /home/pmthome/aptest/Analysis_cy/
	echo "#!/bin/bash"> myjobs/job$sn.sh
	echo "cd /home/pmthome/aptest/Analysis_cy/">>myjobs/job$sn.sh
	echo "/home/pmthome/aptest/Analysis_cy/AfterpulseAnalysis -s $sn -d $date">> myjobs/job$sn.sh
	qsub myjobs/job$sn.sh
#/home/pmthome/aptest/Analysis_cy/AfterpulseAnalysis -s $sn -d $date
	echo "now processing"$sn"date:"$date
	fi
#echo $date
done
