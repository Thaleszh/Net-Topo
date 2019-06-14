#!/bin/bash
repetitions=25
smallrep=100
giantrep=200

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

echo "mesh3D_8 swapped Init time to memoi"
echo "tree_10 swapped Init time to memoi"

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

for i in "mesh3D_8" "mesh2D_16" "tree_10" "tree_7"
do
	echo "running $i 1 Distance time with memoi filled"
	echo "running $i 2 Distance time with memoi filled"
done

for i in "mesh2D 16" "tree 7" "mesh3D 8" "tree 10" 
do
	./execBench fill $i $repetitions
	./execBench dist $i $giantrep 1
	./execBench dist $i $giantrep 2
done
cd ..
grep "mesh3D_8" bench.txt > bench_results/mesh3D_8.txt
grep "mesh2D_16" bench.txt > bench_results/mesh2D_16.txt
grep "tree_7" bench.txt > bench_results/tree_7.txt
grep "tree_10" bench.txt > bench_results/tree_10.txt

cd bench_results

for i in "mesh2D_16" "tree_7" "mesh3D_8" "tree_10"
do
	grep "Init time" $i.txt | awk '{ print $4, $8, $12}' > "init.$i.txt"
	grep "Load time" $i.txt | awk '{ print $5}' > "load.$i.txt"
	grep "Proximity time" $i.txt | awk '{ print $5}' > "prox.$i.txt"
	grep "1 Hop time" $i.txt | awk '{ print $6}' > "hop.1.$i.txt"
	grep "2 Hop time" $i.txt | awk '{ print $6}' > "hop.2.$i.txt"
	grep "1 Distance time" $i.txt | awk '{ print $6}' > "dist.1.$i.txt"
	grep "2 Distance time" $i.txt | awk '{ print $6}' > "dist.2.$i.txt"
	grep "Fill time" $i.txt | awk '{ print $5}' > "fill.$i.txt"
done

awk '{print > out}; /to/ {out="dist.1.mesh2D_16.with_memoi.txt"}; /with/ {out="dist.1.mesh2D_16.with_fill.txt"}' out=dist.1.mesh2D_16.no_memoi dist.1.mesh2D_16.txt
awk '{print > out}; /to/ {out="dist.2.mesh2D_16.with_memoi.txt"}; /with/ {out="dist.2.mesh2D_16.with_fill.txt"}' out=dist.2.mesh2D_16.no_memoi dist.2.mesh2D_16.txt

awk '{print > out}; /to/ {out="dist.1.mesh3D_8.with_memoi.txt"}; /with/ {out="dist.1.mesh3D_8.with_fill.txt"}' out=dist.1.mesh3D_8.no_memoi dist.1.mesh3D_8.txt
awk '{print > out}; /to/ {out="dist.2.mesh3D_8.with_memoi.txt"}; /with/ {out="dist.2.mesh3D_8.with_fill.txt"}' out=dist.2.mesh3D_8.no_memoi dist.2.mesh3D_8.txt

awk '{print > out}; /to/ {out="dist.1.tree_7.with_memoi.txt"}; /with/ {out="dist.1.tree_7.with_fill.txt"}' out=dist.1.tree_7.no_memoi dist.1.tree_7.txt
awk '{print > out}; /to/ {out="dist.2.tree_7.with_memoi.txt"}; /with/ {out="dist.2.tree_7.with_fill.txt"}' out=dist.2.tree_7.no_memoi dist.2.tree_7.txt

awk '{print > out}; /to/ {out="dist.1.tree_10.with_memoi.txt"}; /with/ {out="dist.1.tree_10.with_fill.txt"}' out=dist.1.tree_10.no_memoi dist.1.tree_10.txt
awk '{print > out}; /to/ {out="dist.2.tree_10.with_memoi.txt"}; /with/ {out="dist.2.tree_10.with_fill.txt"}' out=dist.2.tree_10.no_memoi dist.2.tree_10.txt
