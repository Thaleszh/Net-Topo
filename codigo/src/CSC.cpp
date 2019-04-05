/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include "CSC.h"
#include <stdlib.h>
#include <stdio.h> //temporary for debug
#include <map>
#include <set>
#include <unordered_set>


csc::csc(int nlinks, int nodes) : _nlinks(nlinks), _nodes(nodes) {
	line = new int[2 * nlinks];
	value = new int[2 * nlinks]; // duplication needed
	index = new int[_nodes + 1];
	for (int i = 0 ; i < 2*nlinks; i++) {
		value[i] = -1;
		line[i] = -1;
	}
	index[0] = 0;
	for (int i = 1 ; i < nodes + 1; i++) {
		index[i] = -1;
	}
}

csc::~csc() {
	delete[] index;
	delete[] line;
	delete[] value;
}

// creates index for the node j, being nlinks the number of links it has
void csc::create_index(int j, int nlinks) {
	if (j < _nodes) {
		int k = 0;
		if (j > 0) k = index[j];
		index[j+1] = k + nlinks; // gotta check if it~s not one more or lower
		// printf("Created index %d as %d, its limit is %d \n", j, index[j], index[j+1]);
	}
}

// creates line position and sets, it assumes there is no limit
void csc::create(int i, int j, int val) {
	//if (j > _nodes || i > _nodes) return; 		// inexistant indexes
	if (i == j) return; 						// self not stored, assumed 0
	//if (line[_nlinks * 2 - 1] != -1) return; 	// no more space

	int pos = index[j];
	int limit = index[j+1];
	int k = pos;
	while (line[k] != -1 && k < limit) {
		k++;
	}
	line[k] = i;
	int before = value[k];
	value[k] = val;
	// printf("Created value at %d as %d. i= %d, j= %d. its limit is %d \n", value[k], index[j], i, j, index[j+1]);
	if (before != -1) {
		// printf("Value destroyed at %d!! \n More info: Value before= %d, Value now= %d, i= %d, j= %d, limit= %d, pos= %d, k= %d \n", index[j], before, value[k], i, j, index[j+1], pos, k);
	}
}

// insert at position [i][j]
void csc::set(int i, int j, int val) {
	if (i == j) return;
	// store time
	int pos = index[j];
	int limit = index[j+1];
	int k = pos;
	while (line[k] != i && k < limit) k++;
	if (k == limit) return; // cannot change what doesnt exist
	value[k] = val;

}

// get value at position [i][j]
int csc::get(int i, int j) {		
	// does not verify if j,i < _nodes, but only returns -1
	// obtaining value
	if (i == j) return 0;
	int pos = index[j]; 					// getting index of node's conections
	int limit = index[j+1]; 				// checking boundary
	int k = pos; 							// starts at first element of list
	while (line[k] != i && k < limit) k++;
	if (k == limit) return -1;				// place doesn´t exist!
	return value[k];
}


// returns the minimal path between a and b, doing djisktra
// a is not included into the path
std::list<node>* csc::path_find(int a, int b) {
	std::list<node>* path = new std::list<node>();
	if (a != b) {
		std::map<int, int> nodes;
		std::map<int, int> visited; // indicated parent of the visited
		printf("-----------------------------------\n");

		// proper function for node comparison, 
		auto compare = [](const node a, const node b) -> bool {
			if (a.index == b.index) return false;
			return a.ncost <= b.ncost;
		};
		std::set<node, decltype(compare)> unvisited(compare);	
		
		const node* current = new node(a, 0, a);
		// inserts a into nodes
		nodes[a] = 0;
		visited[a] = a;
		unvisited.insert(* current);

		// initializations
		const node* new_node = current;
		int pos, limit, k;
		int cost = 0;
		// can add distance for any node from memoization
		printf("visiting node %d. Parent is none, cost is %d \n", current->index, current->ncost);
		do  {
			// iterates current node, seeking new nodes and their cost
			unvisited.erase(unvisited.begin());

			pos = index[current->index]; 			// getting index of node's conections
			limit = index[current->index+1]; 		// checking boundary
			printf("pos = %d, limit = %d \n", pos, limit);
			k = pos; 				// starts at first element of edge list
			while (k < limit) {
				// to do: remove node creation from unneeded
				new_node = new node(line[k], cost + value[k], current->index);
				printf("checking node %d to path. Parent is %d, ncost is %d \n", new_node->index, new_node->parent, new_node->ncost);
				// if it is first time finding node
				if (nodes.count(new_node->index) == 0) {
					nodes[new_node->index] = new_node->ncost;
					unvisited.insert(* new_node);		
					printf("firs time on node, it's cost now is %d \n", nodes[new_node->index]);
					printf("unvisited now has %u nodes to visit! \n", unvisited.size());
				} else {
					// if the cost is lower
					printf("not first time on node, it's cost is %d, was stored as %d \n", new_node->ncost, nodes[new_node->index]);
					if (nodes[new_node->index] > new_node->ncost) {
						// since comparison returns they are equal, removing and adding works
						unvisited.erase(* new_node);	
						unvisited.insert(* new_node);
						printf("inserted and erased");
						nodes[new_node->index] = new_node->ncost;
						printf("new cost stored as %d", nodes[new_node->index]);
					}
				}
				k++;
			}

			// pops set
			printf("\n changing current, there is still %u nodes to visit! \n", unvisited.size());
			current = &(*unvisited.begin()); 
			visited[current->index] = current->parent;
			printf("visiting node %d. Parent is %d, cost is %d \n", current->index, current->parent, current->ncost);


			if (current->index == b) {
				printf("we reached our destination! \n");
				break; 	// if we reached it, its done
			}

			cost = nodes[current->index]; // updating cost for next ones
		} while (!unvisited.empty());

		printf("\n Starting path finding from %d to %d \n", a, b);
		// now to the path, distance is current.ncost
		k = 0;
		while (current->index != a) {
			// add itself to path
			path->push_front(* current);
			printf("added node %d to path. Parent is %d, cost is %d \n", current->index, current->parent, current->ncost);
			// next one is the parent
			current = new node(current->parent, nodes[current->parent], visited[current->parent]);
			k++;
			if (k > 15) break;
		}
	}
	// memoization of all costs on visited
	return path;
}

int csc::distance(int a, int b) {
	if (a == b) return 0;
	std::list<node>* path = path_find(a, b);
	if (path->size() == 0) return 0;
	const node* end = &(*std::prev(path->end()));
	printf("node of index %d is on end of list, with cost of %d \n", end->index, end->ncost);
	int cost = end->ncost;
	delete path;
	printf("-----------------------------------\n");
	return cost;
}

// returns number of hops between a and b
int csc::hops(int a, int b) {
	if (a == b) return 0;
	std::set<int> visited;
	std::set<int> unvisited;	
	std::set<int> temporary;

	int current = a;
	visited.insert(a);
	// initializations
	int new_node, pos, limit, k;
	int cost = 0;
	// can add distance for any node from memoization
	do {
		temporary.clear();
		do {
			// iterates current node, seeking new nodes and their cost
			pos = index[current]; 			// getting index of node's conections
			limit = index[current+1]; 		// checking boundary
			k = pos; 					// starts at first element of edge list
			// printf("Visiting node %d \n", current);
			while (k < limit) {
				new_node = line[k];
				// if it is first time finding node
				if (visited.count(new_node) == 0) {
					if (unvisited.count(new_node) == 0) {
						// printf("added node %d to temporaries \n", new_node);
						temporary.insert(new_node);
					}
				}
				k++;
			}
			// pops set
			current = *unvisited.begin();
			unvisited.erase(current);
			visited.insert(current);

		} while (!unvisited.empty());
		cost++;
		if (temporary.find(b) != temporary.end()) break; 	// if we reached it, its done
		unvisited = temporary;

	} while (!temporary.empty());
	
	return cost;
}
