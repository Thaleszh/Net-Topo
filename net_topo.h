/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 19/10/2018
 */

namespace net_topo {

// edge for topology
struct edge {
	vertex pe1;
	vertex pe2;
	int dist = -1; 	// inexistent by default

}

// vertex superstruct for machine elements
struct vertex {
	int depth;
	vertex(int _depth) : _depth(depth) {}
}

// machine root node structure
struct node : vertex {
	int node_id;
	int ncores;
	cores[] cores;
}

// machine core structure
struct core : vertex {
	int core_id;
	int npes;
	pe[] pes;
}

// Processing Element struct
struct pe : vertex {
	int pe_id;
}


class topology {
private:
	edge[] topology; 		// structure with topology
	int npe; 				// number of PEs

public:
	// returns id of caller's PE, for now let's centralize
	// int my_pe() {}

	// returns the id of the pe where the especified job is allocated
	int my_pe(job job) {}

	// returns normalized minimal distance between 2 PEs
	float dist(vertex pe1, vertex pe2) {}

	// returns hop count between 2 PEs
	// hop count on same node is 0
	int hop_count(peid pe1, peid pe2) {}

	// returns location based on id of PE and it's depth
	int my_location(peid id, depth_t depth) {}

	// ? returns path of minimal distance between 2 PEs
	vertex[] path(peid pe1, peid pe2) {}

	// ? returns normalized distance of a path
	float dist(vertex[] path) {}

	// returns a list of neighbors of a given PE
	// neighbors = hop count == 0 or lowest distances?
	node[] neighbors(pe) {}

	// returns true if 2 PEs are on same physical node
	bool same_node() {}

	// returns number of PEs of the topology
	int num_pes() {}

	// prints the system topology
	void print_topology() {}

	// calculates time of a normalized distance, in microseconds
	int unormalize(float dist) {}

}
}
// Ideas:
/* hwloc grouping by distance
 *
 */