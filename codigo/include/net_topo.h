/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 19/10/2018
 */

#include "CSC.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>

namespace net_topo {

// edge for topology
struct edge {
	vertex v1;
	vertex v2;
	int dist = -1; 	// inexistent by default

}

// vertex superstruct
struct vertex {
	int depth;
}

// Processing Element struct
struct pe : vertex {
	int pe_id;
}


class topology {
private:
	edge[] topology; 		// structure with high level topology
	root[] trees;			// structure with machine topology
	int npe; 				// number of PEs
	std::unique_ptr<csc> distances;			// distance storage structure

public:
	// init, to be extended with diferent forms of initialization
	virtual init() {}

	// returns id of caller's PE, for now let's centralize
	// int my_pe() {}

	// returns the id of the pe where the especified job is allocated
	// job needs to be found somewhere
	int my_pe(int job_id) {}

	// returns normalized minimal distance between 2 PEs
	// 0.1 for same node
	float dist(vertex v1, vertex v2) {
	// check if on same node
	// check if on memoi structure
	// find machine nodes
	// run distance on CSC
	}

	// returns hop count between 2 PEs
	// hop count on same node is 0
	int hop_count(pe_id pe1, pei_d pe2) {
	// check if on same node
	// check if on memoi structure
	// find machine nodes
	// run distance on CSC
	}

	// returns location based on id of PE and it's depth
	int my_location(pe_id id, depth_t depth) {
	
	}

	// ? returns path of minimal distance between 2 PEs
	vertex[] path(pe_id pe1, pe_id pe2) {}

	// ? returns normalized distance of a path
	float dist(vertex[] path) {}

	// returns a list of neighbors of a given PE
	// neighbors = hop count == 0 or lowest distances?
	vertex[] neighbors(pe_id pe) {}

	// ? returns how close a pe is from another
	// similar to a hwloc_get_common_ancestor / acmp
	// 0 = not on same machine
	// 1 = on same machine
	// 2 = on same node
	// 3 = on same core
	int proximity(pe_id pe1, pe_id pe2) {}

	// returns number of PEs of the topology
	int num_pes() {}

	// prints the system topology
	void print_topology() {}

	// reads a topology from a file, using it instead of finding it
	// Can be translated to a init with a file as arg?
	void read_topology(string filename) {
		std::ifstream file(filename);

		cereal::XMLInputArchive archive(file);

		archive(distances);
	}

	// saves topology on a file
	// format? tilfe type? Hwloc uses xml
	void save_topology(string filename) {

		std::ofstream file(filename);
		cereal::XMLOutputArchive archive(file);
		archive(distances);

	}

	// calculates time of a normalized distance, in microseconds
	int unormalize(float dist) {}

}

} // end of namespace

// Ideas:
/* hwloc grouping by distance
 *
 */