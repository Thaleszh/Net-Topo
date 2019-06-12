 #!/bin/bash

croot=/home/eclvc/Documents/charm-6.9.0
tmp=/home/thaleszh/Documents/charm-6.9.0/src/ck-ldb
lb_test=$croot/tests/charm++/load_balancing/lb_test

# ./build charm++ netlrts-linux-x86_64 gcc  -j4

function clone_charm () {
	echo Setting up charm
	mkdir ~/Documents/charm
	git clone https://charm.cs.illinois.edu/gerrit/charm ~/Documents/charm
}

function build_charm() {
	echo Building up charm
	cd /home/eclvc/Documents/charm-6.9.0
	./build charm++ netlrts-linux-x86_64 gcc -j4
}

function make_charm() {
	echo making up charm
	cd /home/eclvc/Documents/charm-6.9.0/tmp
	# bash ./Makefile_lb.sh
	# make -f Make.lb
	# make depends
	make charm++
	cd ../tests/charm++/load_balancing/lb_test
	make clean
	make
}

function delete() {
	echo deleting up charm
	cd /home/eclvc/Documents/
	rm -rf charm-6.9.0
}


function rec_charm_my() {
	echo testing charm
	run_params="3600 150 10 40 60 4120"
	run_flags="ring +balancer myNeighborLB +LBDebug 1 +LBTestPESpeed ++nodelist ./nodelist ++nodegroup two"
	cd /home/eclvc/Documents/charm-6.9.0/tests/charm++/load_balancing/lb_test
	./charmrun +p12 ./lb_test $run_params $run_flags
}
	#3600 150 10 40 60 4120

function rec_charm() {
	echo testing charm
	run_params="3600 150 10 40 60 4120"
	run_flags="ring +balancer NeighborLB +LBDebug 1 +LBTestPESpeed ++nodelist ./nodelist ++nodegroup two"
	cd /home/eclvc/Documents/charm-6.9.0/tests/charm++/load_balancing/lb_test
	./charmrun +p12 ./lb_test $run_params $run_flags
}
	#100 1 10 40 60 120


function big_rec_charm_my() {
	echo testing charm
	run_params="5400 150 10 40 60 4120"
	run_flags="ring +balancer myNeighborLB +LBDebug 1 +LBTestPESpeed ++nodelist ./nodelist ++nodegroup two"
	cd /home/eclvc/Documents/charm-6.9.0/tests/charm++/load_balancing/lb_test
	./charmrun +p12 ./lb_test $run_params $run_flags
}

function big_rec_charm() {
	echo testing charm
	run_params="5400 150 10 40 60 4120"
	run_flags="ring +balancer NeighborLB +LBDebug 1 +LBTestPESpeed ++nodelist ./nodelist ++nodegroup two"
	cd /home/eclvc/Documents/charm-6.9.0/tests/charm++/load_balancing/lb_test
	./charmrun +p12 ./lb_test $run_params $run_flags
}

function test_charm() {
	echo testing charm-6.9.0
	run_params="100 100 10 40 30 9000"
	run_flags="ring +balancer myNeighborLB +LBDebug 1 +LBTestPESpeed ++nodelist ./nodelist ++nodegroup two"
	cd /home/eclvc/Documents/charm-6.9.0/tests/charm++/load_balancing/lb_test
	./charmrun +p12 ./lb_test $run_params $run_flags
}

echo "$1"
if [ "$1" == "start" ]
then
	ssh eclvc@centaur.local "$(typeset -f clone_charm); clone_charm"
	ssh eclvc@cyclops.local "$(typeset -f clone_charm); clone_charm"
elif [ "$1" == "build" ]
then
	ssh eclvc@centaur.local "$(typeset -f build_charm); build_charm"
	ssh eclvc@cyclops.local "$(typeset -f build_charm); build_charm"
elif [ "$1" == "transfer" ]
then
	cd ~/Documents
	tar czvf charmball.tar.gz charm-6.9.0
	scp -r charmball.tar.gz eclvc@centaur.local:/home/eclvc/Documents

	scp -r charmball.tar.gz eclvc@cyclops.local:/home/eclvc/Documents

	ssh eclvc@centaur.local "cd ~/Documents; tar xzvf charmball.tar.gz"
	ssh eclvc@cyclops.local "cd ~/Documents; tar xzvf charmball.tar.gz"

	# path_target=$croot/tmp
	# from=$tmp/../../tmp
	# for i in "net_topo.h" "net_topo.C" "upper_matrix.h" "upper_matrix.C" "CSC.C" "CSC.h" "Makefile_lb.sh"
	# do
	# 	scp -r $from/$i eclvc@centaur.local:$path_target/$i

	# 	scp -r $from/$i eclvc@cyclops.local:$path_target/$i
	# 	shift
	# done

	# path_target=$croot/src/ck-ldb
	# for i in "myNeighborLB.h" "myNeighborLB.C" "myNeighborLB.ci" "myNborBaseLB.ci" "myNborBaseLB.h"
	# do
	# 	scp -r $tmp/$i eclvc@centaur.local:$path_target/$i

	# 	scp -r $tmp/$i eclvc@cyclops.local:$path_target/$i
	# 	shift
	# done

	
elif [ "$1" == "make" ]
then

	scp -r $croot/tmp/net_topo_charm_proxy.h eclvc@centaur.local:$croot/tmp/net_topo_charm_proxy.h
	scp -r $tmp/myNborBaseLB.C eclvc@centaur.local:$croot/src/ck-ldb/myNborBaseLB.C

	# scp -r $tmp/NborBaseLB.C eclvc@centaur.local:$croot/src/ck-ldb/NborBaseLB.C

	ssh eclvc@centaur.local "$(typeset -f make_charm); make_charm"


	scp -r $croot/tmp/net_topo_charm_proxy.h eclvc@cyclops.local:$croot/tmp/net_topo_charm_proxy.h
	scp -r $tmp/myNborBaseLB.C eclvc@cyclops.local:$croot/src/ck-ldb/myNborBaseLB.C

	# scp -r $tmp/NborBaseLB.C eclvc@cyclops.local:$croot/src/ck-ldb/NborBaseLB.C

	ssh eclvc@cyclops.local "$(typeset -f make_charm); make_charm"

elif [ "$1" == "record" ]
then
	for i in {21..60}
	do
		echo "exec $i myNeighborLB"
		echo "exec $i" >> mydata.txt
		ssh eclvc@centaur.local "$(typeset -f rec_charm_my); rec_charm_my" >> mydata.txt

		echo "exec $i NeighborLB"
		echo "exec $i" >> data.txt
		ssh eclvc@centaur.local "$(typeset -f rec_charm); rec_charm" >> data.txt
	done

	for i in {16..45}
	do
		echo "exec $i myNeighborLB"
		echo "exec $i" >> mybigdata.txt
		ssh eclvc@centaur.local "$(typeset -f big_rec_charm_my); big_rec_charm_my" >> mybigdata.txt

		echo "exec $i NeighborLB"
		echo "exec $i" >> bigdata.txt
		ssh eclvc@centaur.local "$(typeset -f big_rec_charm); big_rec_charm" >> bigdata.txt
	done 

elif [ "$1" == "test" ]
then
	ssh eclvc@centaur.local "$(typeset -f test_charm); test_charm"

elif [ "$1" == "grep" ]
then
	grep "140" mydata.txt > myneighbortimes.txt
	grep "140" data.txt > neighbortimes.txt
	grep "140" bigdata.txt > bigneighbortimes.txt
	grep "140" mybigdata.txt > mybigneighbortimes.txt
	grep "Load topology elapsed" mybigdata.txt > loadtopotime.txt
	echo "normal" >> loadtopotime.txt 
	grep "Load topology elapsed" mydata.txt >> loadtopotime.txt
	grep "duration" mydata.txt > mylbtime.txt
	grep "duration" data.txt > lbtime.txt
	grep "duration" bigdata.txt > biglbtime.txt
	grep "duration" mybigdata.txt > mybiglbtime.txt

elif [ "$1" == "copy" ]
then
	path_target=$croot/src/ck-ldb
	echo "file path: $tmp/$2"
	echo "target path: $path_target/$2"
	while [ "$2" != "" ]
	do
		scp -r $tmp/$2 eclvc@centaur.local:$path_target/$2

		scp -r $tmp/$2 eclvc@cyclops.local:$path_target/$2
		shift
	done
fi