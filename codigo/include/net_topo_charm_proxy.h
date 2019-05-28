

#include <unordered_set>
#include "net_topo.h"
#include <middle-conv.h>
#include <topology.h>

class net_topo_charm_proxy {

private:
	net_topo * net;

public:
	net_topo_charm_proxy(net_topo * nettopo) : net(nettopo) {}

	net_topo_charm_proxy(){}

	~net_topo_charm_proxy() {

	}

	void init(LBTopology* topo) {
		// vector of vectors for network topology


		std::vector<std::vector<pair<int, int>>> network_topo;

		std::vector<std::pair<int, int>> machine_topo;

		std::vector<std::pair<int, int>> temporary;
		int i,j, node, num_neighbors;
		int npe = Converse::CkNumPes();
		int* neighbors;

		// for each pe
		for (i = 0; i < npe; i++) {
			temporary = std::vector<std::pair<int, int>>();
			num_neighbors = 0;
			neighbors = new int[topo->max_neighbors()];
			topo->neighbors(i, neighbors, num_neighbors);
			// for each neighbor
			for (j = 0; j < num_neighbors; j++) {
				// CmiPrintf("Creating link from %d to %d\n", i, neighbors[j]);
				temporary.push_back(std::pair<int, int>(neighbors[j], 1));
			}

			// CmiPrintf("Creating machine/node with from %d %d %d\n", i, i, 0);
			network_topo.push_back(temporary); // adds pe to network
			machine_topo.push_back(std::pair<int, int>(i, 0)); // adds machine and node as self
		}
		// CkPrintf("Calling net_topo init\n");
		net->init(machine_topo, network_topo);
	}

	float distance(int v1, int v2) {
		return net->distance(v1, v2);
	}


	int hop_count(int pe1, int pe2) {
		return net->hop_count(pe1, pe2);
	}

	std::vector<int> on_same_machine(int pe) {
		return net->on_same_machine(pe);
	}

	std::vector<int> net_neighbors(int machine) {
		return net->net_neighbors(machine);
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

	void load_topology(string filename) {
		net->load_topology(filename);
	}

	void save_topology(string filename) {
		net->save_topology(filename);

	}

};