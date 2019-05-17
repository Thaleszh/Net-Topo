/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <list>
#include <vector>
#include <cereal/types/vector.hpp>
#include "upper_matrix.h"
#include <cereal/types/memory.hpp>

struct node {
	int index;
	int ncost;
	int parent;

	node(int in, int cost, int creator) : index(in), ncost(cost), parent(creator) {}
};

class csc {
protected:
	// note: current version is static, cannot be incremented
	// to do: current version storages every link twice, due to traversal problem

	std::vector<int> index; 	// indexing vector
	std::vector<int> line;		// line especification
	std::vector<int> value;		// values list
	int _nlinks; 	// number of conections, they are counted twice
	int _nodes;		// number of nodes in list
	std::unique_ptr<upper_matrix> memoi; // memoization structure used in distances

public:
	
	csc() {}

	// needed for serialization
	csc(std::vector<int> id, std::vector<int> ln, std::vector<int> val, int nlinks, int nodes, std::unique_ptr<upper_matrix> upp_matrix) 
	: index(id), line(ln), value(val), _nlinks(nlinks), _nodes(nodes), memoi(std::move(upp_matrix)) {}

	csc(int nlinks, int nodes);

	~csc();

	// serialization header
	template <class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(index), CEREAL_NVP(line), CEREAL_NVP(value), CEREAL_NVP(_nlinks), CEREAL_NVP(_nodes), cereal::make_nvp("upper_matrix", memoi));
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

	// returns minimal distance between a and b, doing djisktra
	int distance(int a, int b);

	// returns number of hops between a and b
	int hops(int a, int b);

	// get neighbors of i
	std::vector<int>* neighbors(int i);

};
