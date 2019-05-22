/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 19/10/2018
 */

#include "CSC.h"
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

using network_topology = const std::vector<std::vector<std::pair<int, int>>>&; 
using machine_map = const std::vector<std::pair<int, int>>&;

class net_topo {
private:
	int npe; 								// number of PEs
	std::unordered_map<int, int> pe_to_node;	// maps pes to nodes
	std::unordered_map<int, int> pe_to_machine;	// maps pes to machines
	std::unique_ptr<csc> distances;			// distance storage structure


public:
	net_topo() {}

	~net_topo() {}
	// init, proxies to build them up
	// machine topology has which machine and node, respectively, each pe is in
	// network topology is a list of each node containing a list of their neighbors and the cost to them
	void init(machine_map machine_topo, network_topology network_topo);

	void init(machine_map machine_topo, network_topology  network_topo,
				int net_links);

	// returns id of caller's PE, for now let's centralize
	// int my_pe() {}

	// returns the id of the pe where the especified job is allocated
	// job needs to be found somewhere
	//int my_pe(int job_id) {}
	// cannot be done on current implementation due to unclouping

	// returns normalized minimal distance between 2 PEs
	// 0.1 for same node
	float distance(int pe1, int pe2);

	// returns hop count between 2 PEs
	// hop count on same core is 0
	int hop_count(int pe1, int pe2);

	// returns location based on id of PE and it's depth
	// int my_location(int id, depth_t depth) {}

	// ? returns path of minimal distance between 2 PEs
	// std::vector<int> path(int pe1, int pe2) {}

	// ? returns normalized distance of a path
	// float distance(std::vector<int> path) {}

	// returns a list of neighbors of a given PE
	// neighbors = hop count == 0 or lowest distances?
	std::vector<int> neighbors(int pe);

	// ? returns how close a pe is from another
	// similar to a hwloc_get_common_ancestor / acmp
	// 0 = not on same machine
	// 1 = on same machine
	// 2 = on same node
	// 3 = on same core
	int proximity(int pe1, int pe2);

	// returns node of pe
	int node_of(int pe) {
		return pe_to_node[pe];
	}

	// returns machine of pe
	int machine_of(int pe) {
		return pe_to_machine[pe];
	}

	// returns number of PEs of the topology
	int num_pes() {
		return npe;
	}

	// prints the system topology
	void print_topology() {}

	// reads a topology from a file, using it instead of finding it
	// Can be translated to a init with a file as arg?
	// to do: quality of life: default name stored
	void load_topology(string filename);

	// saves topology on a xml file
	void save_topology(string filename);

	// calculates time of a normalized distance, in microseconds
	// int unormalize(float dist) {}

};


// Ideas:
/* hwloc grouping by distance
 *
 */