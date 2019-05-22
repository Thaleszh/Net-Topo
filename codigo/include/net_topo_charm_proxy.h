

#include <unordered_set>
#include "net_topo.h"
#include <middle-conv.h>
#include <topology.h>

class net_topo_charm_proxy {

private:
	net_topo * net;

public:
	net_topo_charm_proxy(net_topo * nettopo) : net(nettopo) {}

	~net_topo_charm_proxy() {

	}

	void init(LBTopology* topo) {
		// vector of vectors for network topology
		std::vector<std::vector<pair<int, int>>> network_topo; 

		// vector with machine/node of each pe
		std::vector<std::pair<int, int>> machine_topo;

		// vector that will be inserted into network topology vector
		std::vector<std::pair<int, int>> temporary;
		int i, node;
		int npe = Converse::CkNumPes();
		int* neighbors = new int[topo->max_neighbors()];

		for (i = 0; i < npe; i++) {
			node = CmiPhysicalNodeID(i);	// gets node 
			machine_topo.push_back(std::pair<int, int>(node, node)); // adds node and machine
		}


		// nodes are strange, but will use nonetheless
		// count of links in network topology
		int nlinks = 0;

		// variables used in middle of function:
		// pes: used for nodes on physical unit
		// j, k: used for iteration
		// num_neighbors: number of neighboring pes
		// num_pes: number of pes in node
    	int* pes;
    	int j, k, num_neighbors, temp_node, num_pes;
    	// number of nodes in topology
		int num_nodes = CmiNumPhysicalNodes();
		// set to check which machine were already added to neighbors of current machine
		std::unordered_set<int> visited;

		// for each machine in topology
		for (i = 0; i < num_nodes; i++) {
			// resets
			temporary = std::vector<std::pair<int, int>>();
			visited = std::unordered_set<int>();
			visited.insert(node); // add self to ignore self for additions
			// gets pes on node
			CmiGetPesOnPhysicalNode(node, &pes, &num_pes);
			// for each pe in node
			for (j = 0; j < num_pes; j++) {
				num_neighbors = 0;
				// gets neighbors of it
				topo->neighbors(pes[i], neighbors, num_neighbors);

				// for each neighbor
				for (k = 0; k < num_neighbors; k++) {
					// gets its node
					temp_node = CmiPhysicalNodeID(neighbors[k]);
					// check if node already visited
					if (visited.find(temp_node) == visited.end()) {
						// adds nodes to visited
						visited.insert(temp_node);
					} 
				}				
			}
			// removes self to not be counted and added
			visited.erase(node);
			nlinks += visited.size(); // updated number of links on network topology
			// for each neighbor node
			for(auto current : visited) {
				// adds neighbor in vector, with a distance of one hop
				temporary.push_back(std::pair<int, int>(current, 1));
			}
			// adds vector of neighbors to network vector
			network_topo.push_back(temporary);

		}

		// calls net_topo init
		net->init(machine_topo, network_topo, nlinks);
	}

	void init_no_nodes(LBTopology* topo) {

		std::vector<std::vector<pair<int, int>>> network_topo;

		std::vector<std::pair<int, int>> machine_topo;

		std::vector<std::pair<int, int>> temporary;
		int nlinks = 0;
		int i,j, node, num_neighbors;
		int npe = Converse::CkNumPes();
		int* neighbors = new int[topo->max_neighbors()];

		// for each pe
		for (i = 0; i < npe; i++) {
			temporary = std::vector<std::pair<int, int>>();
			num_neighbors = 0;
			topo->neighbors(i, neighbors, num_neighbors);
			// for each neighbor
			for (j = 0; j < num_neighbors; j++) {
				temporary.push_back(std::pair<int, int>(j, 1));
			}

			nlinks += num_neighbors; // updates number of links
			network_topo.push_back(temporary); // adds pe to network
			machine_topo.push_back(std::pair<int, int>(i, i)); // adds node and machine as self
		}

		net->init(machine_topo, network_topo, nlinks);
	}

	float distance(int v1, int v2) {
		return net->distance(v1, v2);
	}


	int hop_count(int pe1, int pe2) {
		return net->hop_count(pe1, pe2);
	}

	std::vector<int> neighbors(int pe) {
		return net->neighbors(pe);
	}

	int proximity(int pe1, int pe2) {
		return net->proximity(pe1, pe2);
	}

	int machine_of(int pe) {
		return net->machine_of(pe);
	}

	int node_of(int pe) {
		return net->node_of(pe);
	}

	int num_pes() {
		return net->num_pes();
	}

	void print_topology() {
		net->print_topology();
	}

	void load_topology(string filename) {
		net->load_topology(filename);
	}

	void save_topology(string filename) {
		net->save_topology(filename);

	}

};