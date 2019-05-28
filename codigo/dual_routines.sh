 #!/bin/bash

croot=/home/eclvc/Documents/charm
tmp=/home/thaleszh/Documents/charm/src/ck-ldb
lb_test=$croot/tests/charm++/load_balancing/lb_test
run_flags="ring +balancer myNeighborLB ++nodelist $lb_test/nodelist ++nodegroup two"

function clone_charm () {
	echo Setting up charm
	mkdir ~/Documents/charm
	git clone https://charm.cs.illinois.edu/gerrit/charm ~/Documents/charm
}

function build_charm() {
	echo Building up charm
	cd /home/eclvc/Documents/charm/tmp
	make charm++
	cd ../tests/charm++/load_balancing/lb_test
	make clean
	make
}

echo "$1"
if [ "$1" == "start" ]
then
	su -c eclvc "$(typeset -f clone_charm); clone_charm"
	ssh eclvc@cyclops.local "$(typeset -f clone_charm); clone_charm"
elif [ "$1" == "build" ]
then

	scp -r $croot/tmp/net_topo_charm_proxy.h eclvc@centaur.local:$croot/tmp/net_topo_charm_proxy.h
	scp -r $tmp/myNborBaseLB.C eclvc@centaur.local:$croot/src/ck-ldb/myNborBaseLB.C

	ssh eclvc@centaur.local "$(typeset -f build_charm); build_charm"


	scp -r $croot/tmp/net_topo_charm_proxy.h eclvc@cyclops.local:$croot/tmp/net_topo_charm_proxy.h
	scp -r $tmp/myNborBaseLB.C eclvc@cyclops.local:$croot/src/ck-ldb/myNborBaseLB.C
	ssh eclvc@cyclops.local "$(typeset -f build_charm); build_charm"

elif [ "$1" == "test" ]
then

	ssh eclvc@centaur.local "$lb_test/charmrun +p12 $lb_test/lb_test 100 100 10 40 10 100 $run_flags"

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