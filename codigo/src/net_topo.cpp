/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 19/10/2018
 */



#include "net_topo.h"

#include <cereal/archives/xml.hpp>
#include <cereal/types/unordered_map.hpp>


// init, proxies to build them up
// machine topology has which node each machine is in
// network topology is a list of each node containing a list of their neighbors and the cost to them
void net_topo::init(machine_map machine_topo, network_topology  network_topo) {

	int links = 0;

	for(auto current : network_topo) {
		links += current.size();
	}

	init(machine_topo, network_topo, links);

}

void net_topo::init(machine_map machine_topo, network_topology  network_topo,
			int net_links) {

	// handles machine topology
	int i = 0;
	for (auto current : machine_topo) {
		pe_to_machine[i] = current.first;
		pe_to_node[i] = current.second;
		i++;
	}
	npe = i;

	// sections topology
	std::unordered_set<int> visited;
	int num_neighbors;

	i = 0;
	for (auto current : network_topo) {
		// resets visited
		visited = std::unordered_set<int>();
		visited.insert(pe_to_machine[]); // add self to ignore self for additions

		// for each pe in machine
		for (auto node : current) {
		}

	}

	// handles network topology
	printf("Creating csc with %d nodes and %d links \n", network_topo.size(), net_links);
	distances.reset(new csc(net_links, network_topo.size()));

	i = 0;
	for (auto current : network_topo ) {
		distances->create_index(i, current.size());
		i++;
	}

	i = 0;
	for (auto current : network_topo ) {
		printf("Creating index at %d with size %d \n", i, current.size());
		for (auto node : current) {
			printf("Creating index at %d, %d with cost %d \n", i, node.first, node.second);
			distances->create(i, node.first, node.second);
			printf("Created index\n");
		}
		printf("Created all indexes\n");
		i++;
	}


}

// returns normalized minimal distance between 2 PEs
// 0.1 for same node
float net_topo::distance(int pe1, int pe2) {
	int n1 = pe_to_machine[pe1];
	int n2 = pe_to_machine[pe2];
	if (n1 == n2) return 0.1;
	// run distance on CSC
	return distances->distance(n1, n2);
}

// returns hop count between 2 PEs
// hop count on same core is 0
int net_topo::hop_count(int pe1, int pe2) {
	// check if on same node
	if (pe1 == pe2) return 0;
	int n1 = pe_to_machine[pe1];
	int n2 = pe_to_machine[pe2];
	if (n1 == n2) return 1;
	return distances->hops(n1, n2);
}

// returns a list of neighbors of a given PE
// neighbors = hop count == 0 or lowest distances?
std::vector<int> net_topo::neighbors(int pe) {
	// add all things from nodes
	return distances->neighbors(pe);
}

// ? returns how close a pe is from another
// similar to a hwloc_get_common_ancestor / acmp
// 0 = not on same machine
// 1 = on same machine
// 2 = on same node
// 3 = on same core
int net_topo::proximity(int pe1, int pe2) {
	if (pe1 == pe2) return 3;
	int n1 = pe_to_machine[pe1];
	int n2 = pe_to_machine[pe2];
	if (n1 != n2) return 0;
	return 1;
}

// reads a topology from a file, using it instead of finding it
// Can be translated to a init with a file as arg?
void net_topo::load_topology(string filename) {
	std::ifstream file(filename);

	cereal::XMLInputArchive archive(file);
	archive(npe, distances, pe_to_machine, pe_to_node);
}

// saves topology on a xml file
void net_topo::save_topology(string filename) {

	std::ofstream file(filename);
	cereal::XMLOutputArchive archive(file);
	archive(CEREAL_NVP(npe), CEREAL_NVP(distances), CEREAL_NVP(pe_to_machine), CEREAL_NVP(pe_to_node));

}

// end of namespace

// Ideas:
/* hwloc grouping by distance
 *
 */