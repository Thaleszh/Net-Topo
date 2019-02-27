/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <stdlib.h>


class csc {
protected:
	// note: current version is static, cannot be incremented
	// to do: current version storages every link twice, due to traversal problem
	// to do: make djisktra algorithm use memoization to speed up
	int[] index; 	// indexing vector
	int[] line;		// line especification
	int[] value;	// values list
	int _nlinks; 	// number of conections, they are counted twice
	int _nodes;		// number of nodes in list

public:
	csc(int nlinks, int nodes) : _nlinks(nlinks), _nodes(nodes) {

		line, value = (int*) malloc(sizeof(int) * _nlinks);
		index = (int*) malloc(sizeof(int) * (_nodes + 1) );
		for (int i = 0 ; i < _nlinks; i++) {
			value[i] = -1;
			line[i] = -1;
		}
		index[0] = 0;
		for (int i = 1 ; i < _nodes; i++) {
			index[i] = -1;
		}
	}

	~csc() {
		free(index);
		free(line);
		free(value);
	}

	// number of nodes in structure
	int n_nodes() {
		return _nodes;
	}

	// number of links in structure
	int n_links() {
		return _nlinks;
	}

	// creates index for the node j, being nlinks the number of links it has
	void create_index(int j, int nlinks) {
		// does not verify if j < _nodes
		int k = 0;
		if (j > 0) k = index[j];
		index[j+1] = k + nlinks; // gotta check if it~s not one more or lower
	}

	// creates line position and sets, it assumes there is no limit
	void create(int i, int j, int val) {
		// does not verify if j,i < _nodes

		// just storing the upper matrix, so if i < j, we swap assuming symetry
		/*if(i > j) {
			temp = i;
			i = j;
			j = temp;
		} */
		pos = index[j];
		int k = pos;
		while (line[k] != -1) k++; // not checking end of val list, assuming it was right from the start
		line[k] = i;
		value[k] = val;
	}

	// insert at position [i][j]
	void set(int i, int j, int val) {
		// just storing the upper matrix, so if i < j, we swap assuming symetry
		/*if(i > j) {
			temp = i;
			i = j;
			j = temp;
		} */
		// store time
		pos = index[j];
		limit = index[j+1];
		int k = pos;
		while (line[k] != i && k < limit) k++;
		if (k = limit) return // cannot change what doesnt exist
		value[k] = val;

	}

	// get value at position [i][j]
	int get(int i, int j) {
		// does not verify if j,i < _nodes, but only returns -1
		// just storing the upper matrix, so if i < j, we swap assuming symetry
		/*if(i > j) {
			temp = i;
			i = j;
			j = temp;
		} */
		// obtaining value
		pos = index[j]; 						// getting index of node's conections
		limit = index[j+1]; 					// checking boundary
		int k = pos; 							// starts at first element of list
		while (line[k] != i && k < limit) k++;
		if (k = limit) return -1 				// place doesn´t exist!
		return value[k];
	}

	// returns the minimal path between a and b, doing graph traversal
	int[] distance(int a, int b) {
		if (a == b) return 0;
		unordered_map<int, int> nodes;
		set<int> visited;
		set<int> unvisited; // needs to be turned into some ordered list
		int current = a;
		int cost, new_node, k, ncost = 0;
		// can add distance for any node from memoization
		while !(unvisited.empty()) {
			
			// iterates current node, seeking new nodes and their cost
			pos = index[a]; 			// getting index of node's conections
			limit = index[a+1]; 		// checking boundary
			k = pos; 					// starts at first element of edge list

			while (k < limit) {
				new_node = value[k];
				ncost = cost + value[k];
				// if it is first time finding node
				if nodes[new_node] == NULL {
					unvisited.insert(new_node);
					nodes[new_node] = ncost;
				k++;
			}
			visited.insert(current);
			unvisited.erase(current);
			// find next current based on ncost
			cost = nodes[current];
			if (current == b) break; 	// if we reached it, its done
		}

	}

