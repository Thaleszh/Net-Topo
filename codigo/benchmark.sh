#!/bin/bash
repetitions=1
smallrep=5
giantrep=10

#create topologies
echo "mesh3D_8 swapped to no memoi"

./execBench init mesh3D 8 $smallrep false
echo "tree_10 swapped to no memoi"
./execBench init tree 10 $smallrep false

for i in "mesh3D 8" "tree 10" 
do
	./execBench load $i $smallrep
	./execBench prox $i $giantrep
	./execBench hops $i $giantrep 1
	./execBench dist $i $giantrep 1
	./execBench hops $i $smallrep 2
	./execBench dist $i $smallrep 2
done

echo "mesh3D_8 swapped init to memoi"
echo "tree_10 swapped init to memoi"

./execBench init mesh2D 16 $repetitions
./execBench init mesh3D 8 $repetitions
./execBench init tree 7 $repetitions
./execBench init tree 10 $repetitions

for i in "mesh2D 16" "tree 7"
do
	echo " "
	echo " "
	echo "running $i"
	./execBench load $i $smallrep
	./execBench prox $i $giantrep
	./execBench hops $i $giantrep 1
	./execBench dist $i $giantrep 1
	./execBench hops $i $smallrep 2
	./execBench dist $i $smallrep 2
done

for i in "mesh3D 8" "tree 10"
do
	echo " "
	echo " "
	echo "running $i"
	./execBench load $i $repetitions
	./execBench prox $i $giantrep
	./execBench hops $i $giantrep 1
	./execBench dist $i $giantrep 1
	./execBench hops $i $smallrep 2
	./execBench dist $i $smallrep 2
done

for i in "mesh2D 16" "tree 7" "mesh3D 8" "tree 10" 
do
	echo " "
	echo " "
	echo "running $i 1 Distance with memoi filled"
	./execBench fill $i $repetitions
	./execBench dist $i $giantrep 1
	echo " "
	echo " "
	echo "running $i 2 Distance with memoi filled"
	./execBench dist $i $giantrep 2
done
cd ..
grep "mesh3D_8" bench.txt >> bench_results/mesh3D_8.txt
grep "mesh2D_16" bench.txt >> bench_results/mesh2D_16.txt
grep "tree_7" bench.txt >> bench_results/tree_7.txt
grep "tree_10" bench.txt >> bench_results/tree_10.txt

cd bench_results

for i in "mesh2D_16" "tree_7" "mesh3D_8" "tree_10"
do
	grep "Init time" $i.txt >> "init.$i.txt"
	grep "Load time" $i.txt >> "load.$i.txt"
	grep "Proximity time" $i.txt >> "prox.$i.txt"
	grep "1 Hop time" $i.txt >> "hop.1.$i.txt"
	grep "2 Hop time" $i.txt >> "hop.2.$i.txt"
	grep "1 Distance time" $i.txt >> "dist.1.$i.txt"
	grep "2 Distance time" $i.txt >> "dist.2.$i.txt"
	grep "Fill time" $i.txt >> "fill.$i.txt"
done