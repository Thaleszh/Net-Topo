/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 19/10/2018
 */

#include "CSC.h"
#include <vector>
#include <utility>
#include <unordered_set>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>

namespace net_topo {


class topology {
private:
	int npe; 								// number of PEs
	std::unordered_map<int> pe_to_node;	// maps pes to nodes
	std::unordered_map<int> pe_to_machine;	// maps pes to machines
	std::unique_ptr<csc> distances;			// distance storage structure


public:
	// init, proxies to build them up
	// machine topology has which node each machine is in
	// network topology is a list of each node containing a list of their neighbors and the cost to them
	void init(std::vector<std::pair<int, int>>* machine_topo, std::vector<std::vector<std::pair<int, int>>>*  network_topo) {

		int links = 0;

		for(auto current : topo) {
			links += current.size();
		}

		init(machine_topo, network_topo, links);

	}

	void init(std::vector<std::pair<int, int>>* machine_topo, std::vector<std::vector<std::pair<int, int>>>*  network_topo,
				int net_links) {

		// handles machine topology
		int i = 0;
		for auto (current : machine_topo) {
			pe_to_machine[i] = current->first;
			pe_to_node[i] = current->second;
			i++;
		}

		// handles network topology
		distances->reset(new csc(network_topo.size(), net_links));

		int j;
		for(auto current : topo) {
			distances->create_index(i, current.size());
			j = 0;
			for (auto node : current) {
				distances->create(i, current.first, current.second);
				j++;
			}
			i++;
		}

	}


	// returns id of caller's PE, for now let's centralize
	// int my_pe() {}

	// returns the id of the pe where the especified job is allocated
	// job needs to be found somewhere
	//int my_pe(int job_id) {}
	// cannot be done on current implementation due to unclouping

	// returns normalized minimal distance between 2 PEs
	// 0.1 for same node
	float dist(int pe1, int pe2) {
		n1 = pe_to_machine[pe1];
		n2 = pe_to_machine[pe2];
		if (n1 == n2) return 0.1;
		// run distance on CSC
		return distances->distance(n1, n2);
	}

	// returns hop count between 2 PEs
	// hop count on same node is 0
	int hop_count(int pe1, int pe2) {
		// check if on same node
		n1 = pe_to_machine[pe1];
		n2 = pe_to_machine[pe2];
		if (n1 == n2) return 0;
		return distances->distance(n1, n2);
	}

	// returns location based on id of PE and it's depth
	// int my_location(int id, depth_t depth) {}

	// ? returns path of minimal distance between 2 PEs
	std::vector<int> path(int pe1, int pe2) {}

	// ? returns normalized distance of a path
	float dist(std::vector<int> path) {}

	// returns a list of neighbors of a given PE
	// neighbors = hop count == 0 or lowest distances?
	std::vector<int>* neighbors(int pe) {
		// add all things from nodes
		return distances->neighbors(pe);
	}

	// ? returns how close a pe is from another
	// similar to a hwloc_get_common_ancestor / acmp
	// 0 = not on same machine
	// 1 = on same machine
	// 2 = on same node
	// 3 = on same core
	int proximity(int pe1, int pe2) {
		if (pe1 == pe2) return 3;
		n1 = pe_to_machine[pe1];
		n2 = pe_to_machine[pe2];
		if (n1 != n2) return 0;
		return 1;
	}

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
	void read_topology(string filename) {
		std::ifstream file(filename);

		cereal::XMLInputArchive archive(file);

		archive(distances);
		archive(pe_to_machine);
		archive(pe_to_node);
		archive(npe);
	}

	// saves topology on a xml file
	void save_topology(string filename) {

		std::ofstream file(filename);
		cereal::XMLOutputArchive archive(file);
		archive(distances);
		archive(pe_to_machine);
		archive(pe_to_node);
		archive(npe);
	}

	// calculates time of a normalized distance, in microseconds
	// int unormalize(float dist) {}

}

} // end of namespace

// Ideas:
/* hwloc grouping by distance
 *
 */