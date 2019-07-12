/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 19/10/2018
 */



#include "net_topo.h"

#include <cereal/archives/xml.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>


// init, proxies to build them up
// machine topology has which node each machine is in
// network topology is a list of each node containing a list of their neighbors and the cost to them
void net_topo::init(machine_map machine_topo, network_topology network_topo) {

	// handles machine topology
	int i = 0;
	int machine = 0;
	int node_count = 0;
	std::unordered_set<int> nodes;
	std::unordered_set<int> pes;
	// assuming all nodes from a machine come in sequence
	for (auto current : machine_topo) {
		pe_to_machine[i] = current.first;
		// machine changed
		if (current.first != machine) {
			// insert nodes found
			machine_to_pe.push_back(pes);
			machine = current.first;
			node_count = nodes.size();
			// printf("Machine changed! Node count: %d \n", node_count);
			pes = std::unordered_set<int>();
		}
		// printf("Node of %d is %d \n", i, current.second + node_count);
		pe_to_node[i] = current.second + node_count; // absolute nodes stored
		pes.insert(i); // add pe to machine
		nodes.insert(current.second + node_count); // 
		i++;
	}
	machine_to_pe.push_back(pes);
	node_count == nodes.size(); // total number of nodes

	// printf("Number of nodes: %d \n", nodes.size());
	// init each node set
	for (i = 0; i< nodes.size(); i++) {
		pes = std::unordered_set<int>();
		node_to_pe.push_back(pes);
	}

	// fills init node set
	for (i = 0; i< pe_to_node.size(); i++) {
		// printf("Adding PE %d to node %d \n", i, pe_to_node[i]);
		node_to_pe[pe_to_node[i]].insert(i);
	}


	// sections topology
	
	int num_neighbors, temp_machine;
	int net_links = 0;
	std::unordered_map<int, int> temp_map;
	std::vector<std::unordered_map<int, int>> dist_to_machine;
	// for each machine
	i = 0;
	for (auto machine : machine_to_pe) {
		// resets visited
		temp_map = std::unordered_map<int, int>();
		// adds itself to map
		temp_map[i] = 1;
		// for each pe of machine
		for (auto pe : machine) {
			// for each neighbor
			for(auto neighbor : network_topo[pe]) {
				// gets machine of neighbor
				temp_machine = pe_to_machine[neighbor.first];
				// if its the first time
				if (temp_map[temp_machine] == 0) {
					// sets distance
					temp_map[temp_machine] = neighbor.second;

				} else {
					// if its second time but distance is lower
					if (temp_map[temp_machine] > neighbor.second)
						temp_map[temp_machine] = neighbor.second;
				}
				
			}
		}
		// remove self from map
		temp_map.erase(i);
		// add number of links found
		net_links+= temp_map.size();
		// gets distances on that machine
		dist_to_machine.push_back(temp_map);
		i++;
	}
	net_links /= 2;
	// handles network topology
	// printf("Creating csc with %d nodes and %d links \n", dist_to_machine.size(), net_links);
	distances.reset(new csc(net_links, dist_to_machine.size()));

	i = 0;
	for (auto links : dist_to_machine ) {
		// printf("Creating index of %d with size %d\n", i, links.size());
		distances->create_index(i, links.size());
		i++;
	}

	i = 0;
	for (auto links : dist_to_machine ) {
		// printf("Creating index at %d with size %d \n", i, links.size());
		for (auto link : links) {
			// printf("Creating index at %d, %d with cost %d \n", i, link.first, link.second);
			distances->create(i, link.first, link.second);
			// printf("Created index\n");
		}
		// printf("Created all indexes\n");
		i++;
	}

}

// returns normalized minimal distance between 2 PEs
// 0.1 for same node
float net_topo::distance(int pe1, int pe2) {
	int n1 = pe_to_machine[pe1];
	int n2 = pe_to_machine[pe2];
	// if on same machine
	if (n1 == n2) {
		n1 = pe_to_node[pe1];
		n2 = pe_to_node[pe2];
		// if on same node
		if (n1 == n2) return 0.1;
		return 0.2;
	}
	// run distance on CSC
	return distances->distance(n1, n2);
}

// returns hop count between 2 PEs
// hop count on same core is 0
int net_topo::hop_count(int pe1, int pe2) {
	if (pe1 == pe2) return 0;
	// check if on same node
	// int n1 = pe_to_node[pe1];
	// int n2 = pe_to_node[pe2];
	// if (n1 == n2) return 1;
	// check if same machine
	int n1 = pe_to_machine[pe1];
	int n2 = pe_to_machine[pe2];
	if (n1 == n2) return 1;
	return distances->hops(n1, n2);
}

std::vector<int> net_topo::on_same_machine(int source_pe) {
	// gets machine of source
	std::vector<int> pes;
	int source_machine = pe_to_machine[source_pe];
	// gets all PEs of machine
	for (auto pe : machine_to_pe[source_machine]){
		if (pe != source_pe)
			pes.push_back(pe);
	}
	return pes;
}

std::vector<int> net_topo::pes_of_machine(int machine) {
	std::vector<int> pes;
	// gets all PEs of machine
	for (auto pe : machine_to_pe[machine]){
			pes.push_back(pe);
	}
	return pes;
}

std::vector<int> net_topo::on_same_node(int source_pe) {
	// gets node of source
	std::vector<int> pes;
	int source_node = pe_to_node[source_pe];
	// gets all PEs of node
	for (auto pe : node_to_pe[source_node]){
		if (pe != source_pe)
			pes.push_back(pe);
	}
	return pes;
}

std::vector<int> net_topo::pes_of_node(int node) {
	std::vector<int> pes;
	// gets all PEs of node
	for (auto pe : node_to_pe[node]){
			pes.push_back(pe);
	}
	return pes;
}

// returns a list of neighbors of a given PE
// neighbors = hop count == 0 or lowest distances?
std::vector<int> net_topo::net_neighbors(int machine) {
	return distances->neighbors(machine);
}

// ? returns how close a pe is from another
// similar to a hwloc_get_common_ancestor / acmp
// 0 = not on same machine
// 1 = on same machine
// 2 = on same node
// 3 = on same core
int net_topo::proximity(int pe1, int pe2) {
	// if same pe
	if (pe1 == pe2) return 3;
	int n1 = pe_to_machine[pe1];
	int n2 = pe_to_machine[pe2];
	// if not on same machine
	if (n1 != n2) return 0;
	n1 = pe_to_node[pe1];
	n2 = pe_to_node[pe2];
	// if on same node
	if (n1 == n2) return 2;
	// if on same machine 
	return 1;
}

// reads a topology from a file, using it instead of finding it
// Can be translated to a init with a file as arg?
void net_topo::load_topology(string filename) {
	std::ifstream file(filename);

	cereal::XMLInputArchive archive(file);
	archive(filename, distances, machine_to_pe, node_to_pe, pe_to_machine, pe_to_node);
}

// saves topology on a xml file
void net_topo::save_topology(string filename) {

	std::ofstream file(filename);
	cereal::XMLOutputArchive archive(file);
	archive(CEREAL_NVP(filename), CEREAL_NVP(distances), CEREAL_NVP(machine_to_pe), CEREAL_NVP(node_to_pe), CEREAL_NVP(pe_to_machine), CEREAL_NVP(pe_to_node));

}

// end of namespace

// Ideas:
/* hwloc grouping by distance
 *
 */