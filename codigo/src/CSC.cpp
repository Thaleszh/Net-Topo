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
	line.resize(2 * nlinks, -1);
	value.resize(2 * nlinks, -1);

	index.resize(_nodes + 1, -1);
	index[0] = 0;

	memoi.reset(new upper_matrix(nodes));
}

csc::~csc() {}

// creates index for the node column, being nlinks the number of links it has
void csc::create_index(int column, int nlinks) {
	if (column < _nodes) {
		int k = 0;
		if (column > 0) k = index[column];
		index[column+1] = k + nlinks;
		// printf("Created index %d as %d, its limit is %d \n", column, index[column], index[column+1]);
	}
}

// creates line position and sets, it assumes there is no limit
void csc::create(int _line, int column, int val) {
	//if (column > _nodes || line > _nodes) return; 		// inexistant indexes
	if (_line == column) return; 						// self not stored, assumed 0
	//if (line[_nlinks * 2 - 1] != -1) return; 	// no more space if dynamic

	int position = index[column];
	int limit = index[column+1];
	// printf("Before While, position is %d, limit is %d\n ", position, limit);
	while (line[position] != -1 && position < limit) {
		// printf("In While, position is: %d \n", position);

		position++;
	}
	// printf("After While position is %d, limit is %d\n\n ", position, limit);
	line[position] = _line;
	int before = value[position];
	value[position] = val;
	// printf("After Acesses \n\n");
	// cannot update memoi: direct link is not necessarely shortest
	// memoi->set(_line, column, val);
	// printf("Value added on memoi. Line: %d Column = %d, Value = %d. On memoi: %d \n", _line, column, val, memoi->get(_line, column));
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
	// cannot update memoi: direct link is not necessarely shortest
	// memoi->set(_line, column, val);
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
std::list<node> csc::path_find(int source, int sink) {
	std::list<node> path = std::list<node>();
	if (source == sink) return path;
	// indicates lowest cost to visit, from source
	std::map<int, int> node_cost = std::map<int, int>();
	// indicates parent of the visited
	std::map<int, int> parent = std::map<int, int>(); 
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
	
	node current = node(source, 0, source);
	// inserts source into node_cost, parent
	node_cost[source] = 0; // would be done in memoi pickup
	parent[source] = source;
	unvisited.insert(current);

	// initializations
	node new_node = current;
	int position, limit;
	int cost = 0;
	// picking up distances from memoi. Not being done because of parent chain
	// note: memoi->get[source, source] will be 0
	// for (int i = 0; i < _nodes; i++) {
	// 	cost = memoi->get(source, i);
	// 	if (cost != -1) {

	// 	}
	// }
	// printf("visiting node %d. Parent is none, cost is %d \n", current.index, current.ncost);
	do  {
		// iterates current node, seeking new nodes and their cost
		unvisited.erase(unvisited.begin());

		position = index[current.index]; 			// getting index of node's conections
		limit = index[current.index+1]; 		// checking boundary
		// printf("position = %d, limit = %d \n", position, limit);
		while (position < limit) {
			// to do: remove node creation from unneeded

			new_node = node(line[position], cost + value[position], current.index);
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
		
		// updates memoi for visited
		if (memoi->get(source, current.index) == -1) {
			memoi->set(source, current.index, node_cost[current.index]);
		}
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
		path.push_front(current);
		// printf("added node %d to path. Parent is %d, cost is %d \n", current.index, current.parent, current.ncost);
		// next one is the parent
		current = node(current.parent, node_cost[current.parent], parent[current.parent]);
		position++;
		// if (position > 15) break; // endless loop breaker
	}

	return path;
}

int csc::distance(int source, int sink) {
	int mem_val = memoi->get(source, sink);
	if (mem_val != -1) {
		// printf("Memoi used from %d to %d!! \n", source, sink);
		return mem_val;
	}
	// indicates lowest cost to visit, from source
	std::map<int, int> node_cost = std::map<int, int>();
	// indicates parent of the visited
	// printf("-----------------------------------\n");

	// proper function for node comparison, 
	auto compare = [](const node source, const node sink) -> bool {
		// if they are the same index, none is greater
		// this is important so that the set takes them as equal and doesn't add multiple of them
		if (source.index == sink.index) return false; 
		return source.ncost <= sink.ncost;
	};
	// indicates reachable but yet to visit nodes. It´s ordered by cost.
	// memoid set is needed to accelerate main loop
	// std::set<int> memoid = * new std::set<int>(); // represents who was added from memoi and can be skipped
	std::set<node, decltype(compare)> unvisited(compare);	
	
	node current = node(source, 0, source);
	// inserts source into node_cost, parent
	// node_cost[source] = 0; // would be done in memoi pickup
	// unvisited.insert(current);
	// initializations
	node new_node = current;
	int position, limit, cost;
	// picking up distances from memoi. Not being done because of parent chain
	// note: memoi->get[source, source] will be 0
	for (int i = 0; i < _nodes; i++) {
		cost = memoi->get(source, i);
		if (cost != -1) {
			new_node = node(i, cost, -1);
			// printf("checking memoi of node %d to %d. Parent is %d, ncost is %d \n", new_node.index, source, new_node.parent, new_node.ncost);
			unvisited.insert(new_node);
			node_cost[i] = cost;
			// memoid.insert(i);
		}
	}
	cost = 0;
	bool found = false;
	// printf("visiting node %d. Parent is none, cost is %d \n", current.index, current.ncost);
	do  {
		// iterates current node, seeking new nodes and their cost
		unvisited.erase(unvisited.begin());

		position = index[current.index]; 			// getting index of node's conections
		limit = index[current.index+1]; 		// checking boundary
		// printf("position = %d, limit = %d \n", position, limit);
		while (position < limit) {
			// to do: remove node creation from unneeded, via memoization set
			new_node = node(line[position], cost + value[position], current.index);
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
		
		// updates memoi for visited
		if (memoi->get(source, current.index) == -1) {
			memoi->set(source, current.index, node_cost[current.index]);
		}
		// pops set
		current = *unvisited.begin();
		// printf("visiting node %d. Parent is %d, cost is %d \n", current.index, current.parent, current.ncost);
		cost = node_cost[current.index]; // updating cost for next iteration

		// adds distance to memoi
		if (memoi->get(source, current.index) == -1) {
			memoi->set(source, current.index, cost);
		}
		if (current.index == sink) {
			// printf("we reached our destination: %d! \n", current.index);
			found = true;
			break; 	// if we reached it, its done
		}

	} while (!unvisited.empty());

	// printf("Cost of %d, it´s registered cost is: \n", cost, node_cost[current.index]);

	// printf("-----------------------------------\n");
	if (found) return cost;
	// if not found returns -1;
	return -1;
}

// returns number of hops between source and sink
int csc::hops(int source, int sink) {
	if (source == sink) return 0;
	// printf("-----------------------------------\n");
	// printf("Hop count from %d to %d\n", source, sink);
	std::set<int> visited = std::set<int>();
	std::set<int> unvisited = std::set<int>();	
	std::set<int> temporary = std::set<int>();

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

	// get neighbors of i
std::vector<int> csc::neighbors(int i) {
	//if (line[_nlinks * 2 - 1] != -1) return; 	// no more space if dynamic
	int_vec neighbors = int_vec();
	int position = index[i];
	int limit = index[i+1];
	while (line[position] != -1 && position < limit) {
		neighbors.push_back(line[position]);
		position++;
	}
	return neighbors;
}
