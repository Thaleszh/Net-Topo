/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <stdlib.h>
#include <list>
#include <map>
#include <set>


struct node {
	int index;
	int ncost;
	const node * parent;

	node(int in, int cost, const node * creator) : index(in), ncost(cost), parent(creator) {}
};

class csc {
protected:
	// note: current version is static, cannot be incremented
	// to do: current version storages every link twice, due to traversal problem
	// to do: make djisktra algorithm use memoization to speed up
	int* index; 	// indexing vector
	int* line;		// line especification
	int* value;		// values list
	int _nlinks; 	// number of conections, they are counted twice
	int _nodes;		// number of nodes in list

public:
	csc(int nlinks, int nodes) : _nlinks(nlinks), _nodes(nodes) {

		line, value = (int*) malloc(sizeof(int) * 2 * _nlinks); // count duplication or not?
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
		if (i == j) return;
		int pos = index[j];
		int k = pos;
		while (line[k] != -1) k++; // not checking end of val list, assuming it was right from the start
		line[k] = i;
		value[k] = val;
	}

	// insert at position [i][j]
	void set(int i, int j, int val) {
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
	int get(int i, int j) 
{		// does not verify if j,i < _nodes, but only returns -1
		// just storing the upper matrix, so if i < j, we swap assuming symetry
		// if(i > j) {
		// 	temp = i;
		// 	i = j;
		// 	j = temp;
		// } 
		// obtaining value
		if (i == j) return 0;
		int pos = index[j]; 						// getting index of node's conections
		int limit = index[j+1]; 					// checking boundary
		int k = pos; 							// starts at first element of list
		while (line[k] != i && k < limit) k++;
		if (k == limit) return -1;				// place doesn´t exist!
		return value[k];
	}

	// returns the minimal path between a and b, doing djisktra
	// a is not included into the path
	std::list<const node*> path_find(int a, int b) {
		std::list<const node*> path;
		if (a != b) {
			std::map<int, int> nodes;
			std::set<node> visited;

			// proper function for node comparison, 
			auto compare = [](const node a, const node b) -> bool {
				if (a.index == b.index) return false;
				return a.ncost < b.ncost;
			};
			std::set<node, decltype(compare)> unvisited;	
			
			const node* current = new node(a, 0, NULL);
			// inserts a into nodes
			nodes[a] = 0;
			unvisited.insert(* current);

			// initializations
			const node* new_node = current;
			int cost, pos, limit, k = 0;
			// can add distance for any node from memoization
			while (!unvisited.empty()) {
				
				// iterates current node, seeking new nodes and their cost
				pos = index[current->index]; 			// getting index of node's conections
				limit = index[current->index+1]; 		// checking boundary
				k = pos; 				// starts at first element of edge list

				while (k < limit) {
					new_node = new node(line[k], cost + value[k], current);
					// if it is first time finding node
					if (nodes[new_node->index] == 0) {
						unvisited.insert(* new_node);
						nodes[new_node->index] = new_node->ncost;
					} else {
						// if the cost is lower
						if (nodes[new_node->index] > new_node->ncost) {
							// since comparison returns they are equal, removing and adding works
							unvisited.erase(* new_node);	
							unvisited.insert(* new_node);
							nodes[new_node->index] = new_node->ncost;
						}
					}
					k++;
				}
				visited.insert(* current);
				// pops set
				unvisited.erase(* current);
				current = &(*unvisited.begin()); 


				if (current->index == b) break; 	// if we reached it, its done
			}
			// now to the path, distance is current.ncost
			while (current->index != a) {
				// add itself to path
				path.push_front(current);
				// next one is the parent
				current = current->parent;
			}
		}
		// memoization of all costs on visited
		return path;
	}

	int distance(int a, int b) {
		std::list<const node*> path = path_find(a, b);
		if (path.size() == 0) return 0;
		const node* end = *path.end();
		return end->ncost;
	}

	// returns number of hops between a and b
	int hops(int a, int b) {
		if (a == b) return 0;
		std::set<int> visited;
		std::set<int> unvisited;	
		std::set<int> temporary;
		int current = a;
		unvisited.insert(a);
		// initializations
		int new_node, pos, limit, k = 0;
		int cost = 1;
		// can add distance for any node from memoization
		do {
			while (!unvisited.empty()) {
				// iterates current node, seeking new nodes and their cost
				pos = index[current]; 			// getting index of node's conections
				limit = index[current+1]; 		// checking boundary
				k = pos; 					// starts at first element of edge list

				while (k < limit) {
					new_node = line[k];
					// if it is first time finding node
					if (visited.find(new_node) != visited.end()) {
						if (unvisited.find(new_node) != unvisited.end()) {
							temporary.insert(new_node);
						}
					}
					k++;
				}
				visited.insert(current);
				// pops set
				unvisited.erase(current);
				current = *unvisited.begin();
			}
			cost++;
			if (temporary.find(b) != temporary.end()) break; 	// if we reached it, its done
			unvisited = temporary;
			temporary.clear();

		} while (!temporary.empty());
		
		return cost;
	}
};
