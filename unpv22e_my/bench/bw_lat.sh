#!/bin/sh

sum="0.0"
usec="0.0"

function doit
{
	Cmd=$1
	echo $Cmd

	Output=`$Cmd`
	echo $Output
	usec=`echo $Output | awk '{ print $2 }'`
	sum=`echo "scale=6; $sum + $usec" | bc -q`

	return
}

function average_bw
{
	n=$1
	echo average: sum=$sum n=$1
	avg=`echo "scale=6; $sum / $n" | bc -q`
	printf "%.1f\n\n" $avg
	sum="0.0"
}

function average_lat
{
	n=$1
	echo average: sum=$sum n=$1
	avg=`echo "scale=6; $sum / $n" | bc -q`
	printf "%.1f\n\n" $avg
	sum="0.0"
}

export PATH=$PATH:.

# Measure bandwidth and latency for message passing.
##########################################################################
if [ -x bw_pipe ]
then	for xfersize in 1024 2048 4096 8192 16384 32768 65536
	do
			# 5 measurements to average
		for i in 1 2 3 4 5
		do
				# 5 iterations, 10 megabytes
			doit "bw_pipe 5 10 $xfersize"
		done
		average_bw 5
	done
	echo "*****"
fi

if [ -x bw_pxmsg ]
then
	for xfersize in 1024 2048 4096 8192 #16384 32768 65536
	do
		for i in 1 2 3 4 5
		do
			doit "bw_pxmsg 5 10 $xfersize"
		done
		average_bw 5
	done
	echo "*****"
fi

if [ -x bw_svmsg ]
then
	for xfersize in 1024 2048 4096 8192 #16384
	do
		for i in 1 2 3 4 5
		do
			doit "bw_svmsg 5 10 $xfersize"
		done
		average_bw 5
	done
	echo "*****"
fi

##########################################################################
if [ -x lat_pipe ]
then
	for i in 1 2 3 4 5
	do
			# 5 measurements to average, 10,000 loops
		doit "lat_pipe 10000"
	done
	echo "*****"
	average_lat 5
fi

if [ -x lat_pxmsg ]
then
	for i in 1 2 3 4 5
	do
		doit "lat_pxmsg 10000"
	done
	echo "*****"
	average_lat 5
fi

if [ -x lat_svmsg ]
then
	for i in 1 2 3 4 5
	do
		doit "lat_svmsg 10000"
	done
	echo "*****"
	average_lat 5
fi

