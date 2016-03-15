#!/bin/sh

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

function average
{
	n=$1
	echo average: sum=$sum n=$1
	avg=`echo "scale=6; $sum / ($n * 1000000)" | bc -q`
	printf "%.3f\n\n" $avg
	sum="0.0"
}

##########################################################################
export PATH=$PATH:.

sum="0.0"
nloop=1000000

if [ -x incr_pxmutex1 ]
then
	echo "______________________________ mutex lock; threads"
	for nthreads in 1 2 3 4 5
	do
			# 3 measurements to average
		for i in 1 2 3
		do
			doit "incr_pxmutex1 $nloop $nthreads"
		done
		average 3
	done
fi

# incr_rwlock1 uses Posix read-write locks.
if [ -x incr_rwlock1 ]
then
	echo "______________________________ read-write lock; threads"
	for nthreads in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_rwlock1 $nloop $nthreads"
		done
		average 3
	done
fi

if [ -x incr_pxsem1 ]
then
	echo "______________________________ Posix memory-based semaphore; threads"
	for nthreads in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_pxsem1 $nloop $nthreads"
		done
		average 3
	done
fi

if [ -x incr_pxsem2 ]
then
	echo "______________________________ Posix named semaphore; threads"
	for nthreads in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_pxsem2 $nloop $nthreads"
		done
		average 3
	done
fi

if [ -x incr_svsem1 ]
then
	echo "______________________________ System V semaphore; threads"
	for nthreads in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_svsem1 $nloop $nthreads"
		done
		average 3
	done
fi

if [ -x incr_svsem2 ]
then
	echo "______________________________ System V semaphores w/UNDO; threads"
	for nthreads in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_svsem2 $nloop $nthreads"
		done
		average 3
	done
fi

if [ -x incr_fcntl1 ]
then
	echo "______________________________ fcntl lock; threads (1 only)"
	# Cannot do fcntl locking between multiple threads.
	for nthreads in 1
	do
		for i in 1 2 3
		do
			doit "incr_fcntl1 /tmp/incr_fcntl1 $nloop $nthreads"
		done
		average 3
	done
fi

##########################################################################

if [ -x incr_pxmutex5 ]
then
	echo "______________________________ mutex lock; processes"
	for nproc in 1 2 3 4 5
	do
			# 3 measurements to average
		for i in 1 2 3
		do
			doit "incr_pxmutex5 $nloop $nproc"
		done
		average 3
	done
fi

if [ -x incr_pxsem5 ]
then
	echo "______________________________ Posix memory-based semaphore; processes"
	for nproc in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_pxsem5 $nloop $nproc"
		done
		average 3
	done
fi

if [ -x incr_pxsem6 ]
then
	echo "______________________________ Posix named semaphore; processes"
	for nproc in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_pxsem6 $nloop $nproc"
		done
		average 3
	done
fi

if [ -x incr_svsem5 ]
then
	echo "______________________________ System V semaphore; processes"
	for nproc in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_svsem5 $nloop $nproc"
		done
		average 3
	done
fi

if [ -x incr_svsem6 ]
then
	echo "______________________________ System V semaphore w/UNDO; processes"
	for nproc in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_svsem6 $nloop $nproc"
		done
		average 3
	done
fi

if [ -x incr_fcntl5 ]
then
	echo "______________________________ fcntl lock; processes"
	for nproc in 1 2 3 4 5
	do
		for i in 1 2 3
		do
			doit "incr_fcntl5 /tmp/incr_fcntl5 $nloop $nproc"
		done
		average 3
	done
fi
