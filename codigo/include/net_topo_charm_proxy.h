

#include <vector>
#include <utility>
#include <unordered_set>
#include "net_topo.h"
#include "topology.h"

class net_topo_charm_proxy {

private:
	net_topo * net;

public:
	net_topo_charm_proxy(net_topo * nettopo) : net(nettopo) {}

	~net_topo_charm_proxy() {

	}

	void init(LBTopology* topo) {

		std::vector<std::vector<pair<int, int>>>* network_topo = new std::vector<std::vector<std::pair<int, int>>>(); 

		std::vector<std::pair<int, int>>* machine_topo = new std::vector<std::pair<int, int>>();

		std::vector<int> temporary;
		nlinks = 0;
		int i,j, node, num_neighbors;
		int npe = CkNumPes();
		int* neighbors = new int[topo->max_neighbors()];
		std::unordered_set<int>* visited = new std::unordered_set<int>();

		// nodes are strange, but will use nonetheless
		for (i = 0; i < npe; i++) {
			
		
		// get which nodes are where, which nodes associate to which
		}

		net->init(machine_topo, network_topo, nlinks);
	}

	void init_no_nodes(LBTopology* topo) {

		std::vector<std::vector<pair<int, int>>>* network_topo = new std::vector<std::vector<std::pair<int, int>>>(); 

		std::vector<std::pair<int, int>>* machine_topo = new std::vector<std::pair<int, int>>();

		std::vector<int> temporary;
		nlinks = 0;
		int i,j, node, num_neighbors;
		int npe = CkNumPes();
		int* neighbors = new int[topo->max_neighbors()];

		// 
		for (i = 0; i < npe; i++) {
			temporary = * new std::vector<int>();
			num_neighbors = 0;
			topo->neighbors(i, neighbors, num_neighbors);

			for (j = 0; j < num_neighbors; j++) {
				temporary.insert(new std::pair<int, int>(j, 1));
			}

			nlinks += num_neighbors; // updates number of links
			network_topo->insert(temporary); // adds pe to network
			machine_topo->insert(new std::pair<int, int>(i, i)); // adds node and machine
		}

		net->init(machine_topo, network_topo, nlinks);
	}

	float dist(vertex v1, vertex v2) {
		return net->dist(v1, v2);
	}


	int hop_count(int pe1, int pe2) {
		return net->hop_count(pe1,  pe2);
	}

	std::vector<int> path(int pe1, int pe2) {
		return net->path(pe1,  pe2);
	}

	float dist(std::vector<int> path) {
		return net->dist(path);
	}

	std::vector<int>* neighbors(int pe) {
		return net->neighbors(pe) 
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

	void read_topology(string filename) {
		net->read_topology(filename);
	}

	void save_topology(string filename) {
		net->save_topology(filename);

	}

};