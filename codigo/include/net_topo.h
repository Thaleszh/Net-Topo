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
using pe_map = std::vector<std::unordered_set<int>>;

class net_topo {
private:
	string filename = "net_topo.xml";
	std::unordered_map<int, int> pe_to_node;	// maps pes to nodes
	pe_map machine_to_pe; // structure to hold pes of each machine
	pe_map node_to_pe; //structure to hold pes of each node
	std::unordered_map<int, int> pe_to_machine;	// maps pes to machines, maybe change to map nodes to machines?
	std::unique_ptr<csc> distances;			// distance storage structure


public:
	net_topo() {}

	~net_topo() {}

	// machine topology has which machine and node, respectively, each pe is in
	// network topology is a list (of nodes) containing a list of pairs (neighborPE, distance to it) 
	// list of nodes must be clustered by machine (all nodes of same machine in sequence)
	void init(machine_map machine_topo, network_topology network_topo);

	// option to init with filename
	void init(machine_map machine_topo, network_topology network_topo, string name) {
		filename = name;
		init(machine_topo, network_topo);
	}

	// returns normalized minimal distance between 2 PEs
	// 0.2 for same machine
	// 0.1 for same node
	// runs djisktra with memoization
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
	std::vector<int> neighbors(int source_pe);

	std::vector<int> net_neighbors(int machine);
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

	// returns other pes on same machine
	std::vector<int> on_same_machine(int source_pe);

	// returns pes of machine
	std::vector<int> pes_of_machine(int machine);

	// returns other pes on same node
	std::vector<int> on_same_node(int source_pe);

	// returns pes of node
	std::vector<int> pes_of_node(int node);

	// returns number of PEs on the topology
	int num_pes() {
		return pe_to_node.size();
	}

	// returns number of machines on the topology
	int num_machines() {
		return machine_to_pe.size();
	}

	// returns number of nodes on the topology
	int num_nodes() {
		return node_to_pe.size();
	}

	// returns number of nodes on the topology
	int num_net_links() {
		return distances->n_links();
	}

	string name() {
		return filename;
	}

	void change_name(string new_name) {
		filename = new_name;
	}

	// reads a topology from a file, using it instead of finding it
	// Can be translated to a init with a file as arg?
	// to do: quality of life: default name stored
	void load_topology(string filename);

	void load_topology() {
		load_topology(filename);
	}

	// saves topology on a xml file
	void save_topology(string filename);

	void save_topology() {
		save_topology(filename);
	}

};


// Ideas:
/* hwloc grouping by distance
 *
 */