/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <list>

struct node {
	int index;
	int ncost;
	int parent;

	node(int in, int cost, int creator) : index(in), ncost(cost), parent(creator) {}

/*	bool operator<(const node b) {
		if (index == b.index) return false;
		return ncost < b.ncost;
	}*/
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
	csc(int nlinks, int nodes);

	~csc();

	// number of nodes in structure
	int n_nodes() {
		return _nodes;
	}

	// number of links in structure
	int n_links() {
		return _nlinks;
	}

	// creates index for the node j, being nlinks the number of links it has
	void create_index(int j, int nlinks);

	// creates line position and sets, it assumes there is no limit
	void create(int i, int j, int val);
	// insert at position [i][j]

	void set(int i, int j, int val);

	// get value at position [i][j]
	int get(int i, int j);

	// returns the minimal path between a and b, doing djisktra
	// a is not included into the path
	std::list<node>* path_find(int a, int b);

	int distance(int a, int b);

	// returns number of hops between a and b
	int hops(int a, int b);
};
