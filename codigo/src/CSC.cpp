/*	Author: Thales source. Zirbel Hubner <thaleszh@gmail.com>
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


// #include "CSC.h"
// #include <stdlib.h>
// #include <stdio.h> //temporary for debug
// #include <map>
// #include <set>
// #include <unordered_set>


// csc::csc(int nlinks, int nodes) : _nlinks(nlinks), _nodes(nodes) {
// 	line = new std::vector<int>(2 * nlinks, -1);
// 	value = new std::vector<int>(2 * nlinks, -1); // duplication needed
// 	index = new std::vector<int>(_nodes + 1, -1);
// }

// csc::~csc() {
// 	delete index;
// 	delete line;
// 	delete value;
// }

// creates index for the node column, being nlinks the number of links it has
void csc::create_index(int column, int nlinks) {
	if (column < _nodes) {
		int k = 0;
		if (column > 0) k = index[column];
		index[column+1] = k + nlinks; // gotta check if it~s not one more or lower
		// printf("Created index %d as %d, its limit is %d \n", column, index[column], index[column+1]);
	}
}

// creates line position and sets, it assumes there is no limit
void csc::create(int _line, int column, int val) {
	//if (column > _nodes || line > _nodes) return; 		// inexistant indexes
	if (_line == column) return; 						// self not stored, assumed 0
	//if (line[_nlinks * 2 - 1] != -1) return; 	// no more space

	int position = index[column];
	int limit = index[column+1];
	while (line[position] != -1 && position < limit) {
		position++;
	}
	line[position] = _line;
	int before = value[position];
	value[position] = val;
	// printf("Created value at %d as %d. line= %d, column= %d. its limit is %d \n", value[k], index[column], line, column, index[column+1]);
	if (before != -1) {
		// printf("Value destroyed at %d!! \n More info: Value before= %d, Value now= %d, line= %d, column= %d, limit= %d, position= %d, k= %d \n", index[column], before, value[k], line, column, index[column+1], position, k);
	}
}

// insert at position [line][column]
void csc::set(int _line, int column, int val) {
	if (_line == column) return;
	int position = index[column];				// getting index of node's connections
	int limit = index[column+1];			// checking boundary
	while (line[position] != _line && position < limit) position++;
	if (position == limit) return;		// cannot change what doesnt exist
	value[position] = val;

}

// get value at position [line][column]
int csc::get(int _line, int column) {		
	// does not verify if column,line < _nodes, but only returns -1
	if (_line == column) return 0;
	int position = index[column]; 			// getting index of node's conections
	int limit = index[column+1]; 		// checking boundary
	while (line[position] != _line && position < limit) position++;
	if (position == limit) return -1;	// place doesn´t exist!
	return value[position];
}


// returns the minimal path between source and sink, doing djisktra
// source is not included into the path
std::list<node>* csc::path_find(int source, int sink) {
	std::list<node>* path = new std::list<node>();
	if (source == sink) return path;
	// indicates lowest cost to visit, from source
	std::map<int, int> node_cost = * new std::map<int, int>();
	// indicates parent of the visited
	std::map<int, int> parent = * new std::map<int, int>(); 
	// printf("-----------------------------------\n");

	// proper function for node comparison, 
	auto compare = [](const node source, const node sink) -> bool {
		// if they are the same index, none is greater
		// this is important so that the set takes them as equal and doesn't add multiple of them
		if (source.index == sink.index) return false; 
		return source.ncost <= sink.ncost;
	};
	// indicates reachable but yet to visit nodes. It´s ordered by cost.
	std::set<node, decltype(compare)> unvisited(compare);	
	
	node current = * new node(source, 0, source);
	// inserts source into node_cost, parent
	node_cost[source] = 0;
	parent[source] = source;
	unvisited.insert(current);

	// initializations
	node new_node = current;
	int position, limit;
	int cost = 0;
	// to do: can add distance for any node from memoization
	// printf("visiting node %d. Parent is none, cost is %d \n", current.index, current.ncost);
	do  {
		// iterates current node, seeking new nodes and their cost
		unvisited.erase(unvisited.begin());

		position = index[current.index]; 			// getting index of node's conections
		limit = index[current.index+1]; 		// checking boundary
		// printf("position = %d, limit = %d \n", position, limit);
		while (position < limit) {
			// to do: remove node creation from unneeded

			new_node = * new node(line[position], cost + value[position], current.index);
			// printf("checking node %d to path. Parent is %d, ncost is %d \n", new_node.index, new_node.parent, new_node.ncost);
			// if it is first time finding node
			if (node_cost.count(new_node.index) == 0) {
				node_cost[new_node.index] = new_node.ncost;

				unvisited.insert(new_node);	// for some reason changes current

				// printf("firs time on node, it's cost now is %d \n", node_cost[new_node.index]);
				// printf("unvisited now has %u nodes to visit! \n", unvisited.size());

			} else {
				// if the cost is lower
				// printf("not first time on node, it's cost is %d, was stored as %d \n", new_node.ncost, node_cost[new_node.index]);
				if (node_cost[new_node.index] > new_node.ncost) {
					// since comparison returns they are equal, remove and add to overwrite
					// having updated cost on unvisited is important for ordering
					unvisited.erase(new_node);	
					unvisited.insert(new_node);
					// printf("inserted and erased");
					node_cost[new_node.index] = new_node.ncost;
					// printf("new cost stored as %d", node_cost[new_node.index]);
				}
			}
			position++;
		}

		// printf("\n changing current, there is still %u nodes to visit! \n", unvisited.size());
		
		// pops set
		current = *unvisited.begin();
		parent[current.index] = current.parent;
		// printf("visiting node %d. Parent is %d, cost is %d \n", current.index, current.parent, current.ncost);


		if (current.index == sink) {
			// printf("we reached our destination! \n");
			break; 	// if we reached it, its done
		}

		cost = node_cost[current.index]; // updating cost for next iteration
	} while (!unvisited.empty());

	// printf("\n Starting path finding from %d to %d \n", source, sink);
	
	position = 0;
	// creates path
	while (current.index != source) {
		// add itself to path
		path->push_front(current);
		// printf("added node %d to path. Parent is %d, cost is %d \n", current.index, current.parent, current.ncost);
		// next one is the parent
		current = * new node(current.parent, node_cost[current.parent], parent[current.parent]);
		position++;
		// if (position > 15) break; // endless loop breaker
	}
	// to do: memoization of all costs on visited
	return path;
}

int csc::distance(int source, int sink) {
	if (source == sink) return 0;
	std::list<node>* path = path_find(source, sink);
	if (path->size() == 0) return 0;
	const node* end = &(*std::prev(path->end()));
	// printf("node of index %d is on end of list, with cost of %d \n", end->index, end->ncost);
	int cost = end->ncost;
	delete path;
	// printf("-----------------------------------\n");
	return cost;
}

// returns number of hops between source and sink
int csc::hops(int source, int sink) {
	if (source == sink) return 0;
	// printf("-----------------------------------\n");
	// printf("Hop count from %d to %d\n", source, sink);
	std::set<int> visited = * new std::set<int>();
	std::set<int> unvisited = * new std::set<int>();	
	std::set<int> temporary = * new std::set<int>();

	unvisited.insert(source);
	temporary.insert(source);
	// initializations
	int new_node, position, limit;
	int cost = 0;

	while(!temporary.empty()) {
		temporary.clear();

		for (auto current : unvisited) {
			// iterates current node, seeking new nodes and their cost
			position = index[current]; 			// getting index of node's conections
			limit = index[current+1]; 		// checking boundary
			// printf(" Visiting node %d \n", current);

			while (position < limit) {
				new_node = line[position];
				// if it is first time finding node
				// printf("Checking node %d\n", new_node);
				if (visited.find(new_node) == visited.end()) {
					if (unvisited.find(new_node) == unvisited.end()) {
						// printf("added node %d to temporaries \n", new_node);
						temporary.insert(new_node);
					}
				}
				position++;
			}

			visited.insert(current);

		}
		cost++;
		// printf("Hop %d\n \n", cost);

		if (temporary.find(sink) != temporary.end()) {
			// printf("Found %d\n", sink);
			break; 	// if we reached it, its done
		}

		unvisited = temporary;

	}
	if (temporary.empty()) {
		// printf("Failed to find %d.\n", sink);
		return -1;
	}
	
	// printf("Hops: %d\n", cost);
	return cost;
}
