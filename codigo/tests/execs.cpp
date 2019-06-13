/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 12/06/2019
 */

#include "net_topo.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>

#include <chrono>
#include <ctime>
#include <cstdlib>

net_topo net;
std::vector<std::vector<pair<int, int>>> network_topo; 
std::vector<std::pair<int, int>> machine_topo;
string name = "none";
string type;
int _size;

using namespace chrono;

void build_tree(int max_depth) {
    int depth = 0;
    int nodes, nodes_depth;
    int i, id;
    std::vector<std::pair<int, int>> temporary;

    // ignores root
    for (i = 0; i < pow(2, max_depth) - 1; i++)
      machine_topo.push_back(std::pair<int, int>(i, 0));

    // initiates 0
    temporary = std::vector<std::pair<int, int>>();
    // if not on last depth add future neighbors
    if (depth + 1 < max_depth) {
      temporary.push_back(std::pair<int, int>(1, 1));
      temporary.push_back(std::pair<int, int>(2, 1));
    }
    network_topo.push_back(temporary);


    // number of nodes on last level
    nodes_depth = 1;
    // total nodes
    nodes = 1;
    // for each level
    for (depth = 1; depth < max_depth; depth++){
      // updates nodes on last level
      nodes_depth *= 2;
      // printf("total nodes of: %d \n nodes_depth of %d\n", nodes, nodes_depth);
      
      // for each node on level
      for (i = nodes; i < nodes_depth + nodes; i++) {
        // initiates node
        // printf("Creating id of %d\n", i);
        temporary = std::vector<std::pair<int, int>>();
        // links to parent
        // printf("adding link from %d to %d\n", i, ((i - 1) / 2));
        // i-1 / 2 = parent
        temporary.push_back(std::pair<int, int>(((i - 1) / 2), 1));
        // if not on last depth add future neighbors
        if (depth + 1 < max_depth) {
          id = 1+ (i * 2);
          // printf("adding link from %d to %d\n", i, id);
          temporary.push_back(std::pair<int, int>(id, 1));
          // printf("adding link from %d to %d\n", i, id + 1);
          temporary.push_back(std::pair<int, int>(id+1, 1));;
        }
        // next node
        network_topo.push_back(temporary);
      }

      // total nodes updated
      nodes += nodes_depth;
    }
}

  void build_mesh_2D(int size) {
  	int length = size;
  	int heigth = size;
    std::vector<std::pair<int, int>> temporary;
    // sets machine topology to null
    int npe = heigth * length;
    int i, j, base_id, id;
    for (i = 0; i < npe; i++)
      machine_topo.push_back(std::pair<int, int>(i, 0));

    // do first line
    base_id = 0;
    // do first one
    temporary = std::vector<std::pair<int, int>>();
    // add the one next    
    temporary.push_back(std::pair<int, int>(base_id + 1, 1));
    // add the one in next line    
    temporary.push_back(std::pair<int, int>(base_id + length, 1));
    
    network_topo.push_back(temporary);
    // do all others
    for (j = 1; j < length - 1; j++) {
      id = base_id+j;
      temporary = std::vector<std::pair<int, int>>();
      // add the one before      
      temporary.push_back(std::pair<int, int>(id - 1, 1));
      // add the one next      
      temporary.push_back(std::pair<int, int>(id + 1, 1));
      // add the one in next line      
      temporary.push_back(std::pair<int, int>(id + length, 1));
      
      network_topo.push_back(temporary);
    }
    // do last one
    base_id += length;
    id = base_id - 1;
    temporary = std::vector<std::pair<int, int>>();
    // add the one before    
    temporary.push_back(std::pair<int, int>(id - 1, 1));
    // add the one in next line    
    temporary.push_back(std::pair<int, int>(id + length, 1));
    
    network_topo.push_back(temporary);

    // do all middle lines
    for (i = 1; i < heigth - 1; i++){
      // do first one
      temporary = std::vector<std::pair<int, int>>();
      // add the one in last line      
      temporary.push_back(std::pair<int, int>(base_id - length, 1));
      // add the one next      
      temporary.push_back(std::pair<int, int>(base_id + 1, 1));
      // add the one in next line      
      temporary.push_back(std::pair<int, int>(base_id + length, 1));
      
      network_topo.push_back(temporary);
      // do all others
      for (j = 1; j < length - 1; j++) {
        id = base_id+j;
        temporary = std::vector<std::pair<int, int>>();
        // add the one in last line        
        temporary.push_back(std::pair<int, int>(id - length, 1));
        // add the one before        
        temporary.push_back(std::pair<int, int>(id - 1, 1));
        // add the one next        
        temporary.push_back(std::pair<int, int>(id + 1, 1));
        // add the one in next line        
        temporary.push_back(std::pair<int, int>(id + length, 1));
        
        network_topo.push_back(temporary);
      }
      base_id += length;
      
      // do last one
      id = base_id - 1;
      temporary = std::vector<std::pair<int, int>>();
      // add the one in last line      
      temporary.push_back(std::pair<int, int>(id - length, 1));
      // add the one before      
      temporary.push_back(std::pair<int, int>(id - 1, 1));
      // add the one in next line      
      temporary.push_back(std::pair<int, int>(id + length, 1));
      
      network_topo.push_back(temporary);

    }

    // do last line
    // do first one
    temporary = std::vector<std::pair<int, int>>();
    // add the one in last line    
    temporary.push_back(std::pair<int, int>(base_id - length, 1));
    // add the one next    
    temporary.push_back(std::pair<int, int>(base_id + 1, 1));
    
    network_topo.push_back(temporary);
    // do all others
    for (j = 1; j < length - 1; j++) {
      id = base_id+j;
      temporary = std::vector<std::pair<int, int>>();
      // add the one in last line      
      temporary.push_back(std::pair<int, int>(id - length, 1));
      // add the one before      
      temporary.push_back(std::pair<int, int>(id - 1, 1));
      // add the one next      
      temporary.push_back(std::pair<int, int>(id + 1, 1));
      
      network_topo.push_back(temporary);
    }
    base_id += length;
    // do last one
    id = base_id - 1;
    temporary = std::vector<std::pair<int, int>>();
    // add the one in last line    
    temporary.push_back(std::pair<int, int>(id - length, 1));
    // add the one before    
    temporary.push_back(std::pair<int, int>(id - 1, 1));
    
    network_topo.push_back(temporary);
  }

  void build_mesh_3D(int size) {
	int heigth = size;
	int length = size;
	int depth = size;
	  int npe = heigth * length * depth ;
	  int i, j, z, base_id, id;
	  for (i = 0; i < npe; i++)
	    machine_topo.push_back(std::pair<int, int>(i, 0));

	  std::vector<std::pair<int, int>> temporary;
	  // do first line

	  // do all middle lines
	  base_id = 0;
	  for (i = 0; i < heigth; i++) {
	    for (j = 0; j < length; j++) {
	      for (z = 0; z < depth; z++) {
	        id = base_id+z;
	        temporary = std::vector<std::pair<int, int>>();
	        // add the one in last line
	        if (i != 0) {
	          // printf("Linking %d with %d \n", id, id - (length * depth)); 
	          temporary.push_back(std::pair<int, int>(id - (length * depth), 1));
	        }
	        // add the one before
	        if (j != 0) {
	          // printf("Linking %d with %d \n", id, id - length);
	          temporary.push_back(std::pair<int, int>(id - depth, 1));
	        }
	        // for the one in last depth
	        if (z != 0) {
	          // printf("Linking %d with %d \n", id, id - 1);
	          temporary.push_back(std::pair<int, int>(id - 1, 1));
	        }
	        // for the one in next depth
	        if (z != depth - 1) {
	          // printf("Linking %d with %d \n", id, id + 1);
	          temporary.push_back(std::pair<int, int>(id + 1, 1));
	        }
	        // add the one next
	        if (j != length - 1) {
	          // printf("Linking %d with %d \n", id, id + length);
	          temporary.push_back(std::pair<int, int>(id + depth, 1));
	        }    
	        // add the one in next line
	        if (i != heigth - 1) {
	          // printf("Linking %d with %d \n", id, id + (length * depth));
	          temporary.push_back(std::pair<int, int>(id + (length * depth), 1));
	        }
	        
	        network_topo.push_back(temporary);
	      }
	      base_id += depth;
	    }
	  }
  }

std::vector<int> neighbors_tree(int source) {
	std::vector<int> temporary = std::vector<int>();
	if (source != 0) {
		// printf("Linking %d with %d \n", source, ((source - 1) / 2)); 
		temporary.push_back(((source - 1) / 2));
	}
	// add the one before
	if (source * 2 < net.num_machines()) {
		// printf("Linking %d with %d \n", source, 1+ (source * 2));
		temporary.push_back(1+ (source * 2));
		// printf("Linking %d with %d \n", source, 2+ (source * 2));
		temporary.push_back(2+ (source * 2));
	}
	return temporary;


}

std::vector<int> neighbors_mesh2D(int source) {
  std::vector<int> temporary = std::vector<int>();
  int i = source / _size;
  int j = source % _size;
  // printf("finding neighbors of %d. i = %d, j = %d \n", source, i, j);
  if (i != 0) {
    // printf("Linking %d with %d \n", source, source - _size); 
    temporary.push_back((source - _size ));
  }
  // add the one before
  if (j != 0) {
    // printf("Linking %d with %d \n", source, source - 1);
    temporary.push_back((source - 1));
  }
  // add the one next
  if (j != _size - 1) {
    // printf("Linking %d with %d \n", source, source + 1);
    temporary.push_back((source + 1));
  }    
  // add the one in next line
  if (i != _size - 1) {
    // printf("Linking %d with %d \n", source, source + _size);
    temporary.push_back((source + _size));
  }
  return temporary;
}

std::vector<int> neighbors_mesh3D(int source) {
  std::vector<int> temporary = std::vector<int>();
  int i = source / (_size * _size);
  int j = (source / _size) % _size;
  int z = source % _size;
  if (i != 0) {
    // printf("Linking %d with %d \n", source, source - (_size * _size)); 
    temporary.push_back((source - (_size * _size)));
  }
  // add the one before
  if (j != 0) {
    // printf("Linking %d with %d \n", source, source - _size);
    temporary.push_back((source - _size));
  }
  // for the one in last size
  if (z != 0) {
    // printf("Linking %d with %d \n", source, source - 1);
    temporary.push_back((source - 1));
  }
  // for the one in next size
  if (z != _size - 1) {
    // printf("Linking %d with %d \n", source, source + 1);
    temporary.push_back((source + 1));
  }
  // add the one next
  if (j != _size - 1) {
    // printf("Linking %d with %d \n", source, source + _size);
    temporary.push_back((source + _size));
  }    
  // add the one in next line
  if (i != _size - 1) {
    // printf("Linking %d with %d \n", source, source + (_size * _size));
    temporary.push_back((source + (_size * _size)));
  }
  return temporary;

}

std::vector<int> neighbors(int source) {
	if (type == "tree") {
		return neighbors_tree(source);
	} else if (type == "mesh2D") {
		return neighbors_mesh2D(source);
	} else if (type == "mesh3D") {
		return neighbors_mesh3D(source);
	} else {
		return std::vector<int>();
	}
}

std::set<int> neighbors(int source, int how_far) {
	std::set<int> neighborhood = std::set<int>();
	std::set<int> unvisited = std::set<int>();
	std::set<int> new_neighbors = std::set<int>();
	std::vector<int> temporary;
	unvisited.insert(source);
	// printf("neighbors up to %d hops \n", how_far);
	for (int i = 0; i < how_far; i++) {
		new_neighbors = std::set<int>();
		for (auto node : unvisited) {
			// printf("finding neighbors of %d \n", node);
			neighborhood.insert(node);	
			temporary = neighbors(node);
			new_neighbors.insert(temporary.begin(), temporary.end());
		}
		// printf("found %d neighbors \n", new_neighbors.size());
		unvisited = new_neighbors;
	}
	// printf("neighbors now %d, about to merge with %d \n", neighborhood.size(), unvisited.size());
	neighborhood.insert(unvisited.begin(), unvisited.end());
	// printf("Source: %d, neighbors after insert %d \n", source, neighborhood.size());
	neighborhood.erase(source);
	return neighborhood;
}

void init(int size, int iterations, bool with_memoi) {
	printf("\n Starting init of %s of size %d, running %d iterations \n", 
		type.c_str(), size, iterations );
  	network_topo = std::vector<std::vector<pair<int, int>>>();
  	machine_topo = std::vector<std::pair<int, int>>();
  	if (type == "mesh2D") {
  			build_mesh_2D(size);
  			name = "mesh2D_";
  	} else if (type == "mesh3D") {
  			build_mesh_3D(size);
  			name = "mesh3D_";
  	} else if (type == "tree") {
  			build_tree(size);
  			name = "tree_";
  	} else {
  			printf("Only mesh2D, mesh3D and tree avaliable\n");
  			return;
  	}
  	name += std::to_string(size);

  	for (int i = 0; i < iterations; i++) {
	  	auto start = steady_clock::now();
	  	net.init(machine_topo, network_topo, name, with_memoi);
	  	auto init_end = steady_clock::now();
	  	net.save_topology();
	  	auto end = steady_clock::now();
	  	printf("[%s] Init time: %d microseconds, Save time: %d microseconds, Total time: %d microseconds\n", 
	  			name.c_str(),
	  			duration_cast<microseconds>(init_end - start), 
	  			duration_cast<microseconds>(end - init_end), 
	  			duration_cast<microseconds>(end - start));
	  	printf("[%s] total nodes: %d\n", type.c_str(), net.num_machines());
    }
}

void load() {
	net.load_topology(name);
}

void multi_load(int iterations) {
	printf("\n Starting multiload of %s, running %d iterations \n", 
		name.c_str(), iterations );
	for (int i = 0; i < iterations; i++) {
		auto start = steady_clock::now();
		load();
	  	auto end = steady_clock::now();
	  	printf("[%s] - Load time: %d microseconds\n",
	  			name.c_str(),
	  			duration_cast<microseconds>(end - start));
	}

}

void proximity(int range, int iterations) {
	printf("\n Starting distance on %s, up to %d nodes, running %d iterations \n", 
		name.c_str(), range, iterations );
	std::vector<int> n1;
	std::vector<int> n2;
	srand(time(0));
	for (int i = 0; i < iterations; i++) {
		n1.push_back(1+(rand() % range));
		n2.push_back(1+(rand() % range));
	}
	for (int i = 0; i < iterations; i++) {
		auto start = steady_clock::now();
		net.proximity(n1[i], n2[i]);
	  	auto end = steady_clock::now();
	  	printf("[%s] - Proximity time: %d ns\n", 
	  			name.c_str(),
	  			duration_cast<nanoseconds>(end - start));

	}
}

void hops(int how_far, int range, int iterations) {
	printf("\n Starting hops on %s, reaching up to %d, up to %d nodes, running %d iterations \n", 
		name.c_str(), how_far, range, iterations );
	std::vector<int> n1;
	std::set<int> n2;
	for (int i = 0; i < iterations; i++) {
		n1.push_back(1+(rand() % range));
	}
	for (int i = 0; i < iterations; i++) {
		n2 = neighbors(n1[i], how_far);
		for (auto neighbor : n2) {
			auto start = steady_clock::now();
			net.hop_count(n1[i], neighbor);
		  	auto end = steady_clock::now();
		  	printf("[%s] - %d Hop time: %d ns\n", 
		  			name.c_str(),
		  			how_far,
		  			duration_cast<nanoseconds>(end - start));
		}
		printf("[%s] - %d Hops finished.\n", 
		  			name.c_str(),
		  			how_far);
	}
}

void distance(int how_far, int range, int iterations) {
	printf("\n Starting distance on %s, reaching up to %d, up to %d nodes, running %d iterations \n", 
		name.c_str(), how_far, range, iterations );
	std::vector<int> n1;
	std::set<int> n2;
	for (int i = 0; i < iterations; i++) {
		n1.push_back(1+(rand() % range));
	}
	for (int i = 0; i < iterations; i++) {
		n2 = neighbors(n1[i], how_far);
		for (auto neighbor : n2) {
			// printf("finding dist of %d to %d \n", n1[i], neighbor);
			auto start = steady_clock::now();
			net.distance(n1[i], neighbor);
		  	auto end = steady_clock::now();
		  	printf("[%s] - %d Distance time: %d ns\n", 
		  			name.c_str(),
		  			how_far,
		  			duration_cast<nanoseconds>(end - start));

		}
		printf("[%s] - %d dist finished.\n", 
  			name.c_str(),
  			how_far);
	}
}

void fill(int iterations) {
	printf("\n Starting Fill on %s, running %d iterations \n", 
		name.c_str(), iterations );
	for (int i = 0; i < iterations; i++) {
		auto start = steady_clock::now();
		net.fill_memoi();
	  	auto end = steady_clock::now();
	  	printf("[%s] - Fill time: %d microseconds\n", 
	  			name.c_str(),
	  			duration_cast<microseconds>(end - start));
	  	if (i + 1 == iterations) {
	  		net.save_topology();
	  	} else
			load();
	}
}

int main(int argc, char **argv) {
  if (argc < 2) {
  	printf("Missing operation type (init, load, fill, hops, prox or dist) \n");
  	printf("Usage: %s <operation> \n", argv[0]);
  	return 0;
  }
  string arg1 = argv[1];
  if (arg1 == "init") {
  	if (argc < 5) {
  		printf("Missing type (mesh2D, mesh3D or tree) and/or size \n");
  		printf("Usage: %s init <type> <size> <iterations> <with_memoi=true> \n", argv[0]);
  		return 0;
  	}
  	type = argv[2];
  	if (argc < 6) {
  		init( atoi(argv[3]), atoi(argv[4]), true);
  		return 0;
  	}
  	init( atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
  	return 0;
  }
  if (argc < 3) {
	printf("Missing name \n");
	return 0;
  }
  if (argc < 5) {
	printf("Missing iterations: \n");
	printf("Usage: %s <operation> <name> <size> <iterations> \n", argv[0]);
	return 0;
  }
  type = argv[2];
  string arg3 = argv[3];
  _size = atoi(argv[3]);
  int arg4 = atoi(argv[4]);
  name = type;
  name += "_";
  name += arg3;
  if (arg1 == "fill") {
  		load();
  		fill(arg4);
  } else if (arg1 == "hops") {

  		if (argc < 6) {
			printf("Missing neighborhood distance to it \n");
			return 0;
  		}
  		load();
  		hops(atoi(argv[5]), net.num_machines(), arg4);
  } else if (arg1 =="prox") {
  		load();
  		proximity(net.num_machines(), arg4);
  } else if (arg1 == "dist") {
		if (argc < 6) {
			printf("Missing neighborhood distance to it \n");
			return 0;
		}
  		load();
  		distance(atoi(argv[5]), net.num_machines(), arg4);
  } else if (arg1 == "load") {
  		multi_load(arg4);
  } else {
  		printf("Invalid operation type, pick from (init, load, fill, hops, prox or dist) \n");
  }
  return 0;
}