/*  Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *  Created: 10/06/2019
 */

#include "net_topo.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>

namespace {

class sintetic_net_topo : public ::testing::Test {
 protected:
 	net_topo net;
  std::vector<std::vector<pair<int, int>>> network_topo; 
  std::vector<std::pair<int, int>> machine_topo;

  sintetic_net_topo() {
  	network_topo = std::vector<std::vector<pair<int, int>>>();
  	machine_topo = std::vector<std::pair<int, int>>();
  }

  ~sintetic_net_topo() override {}

  void SetUp() override {

  }

  void TearDown() override {
  	// net.save_topology();
  }

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
    build_mesh_2D(size, size);
  }

  void build_mesh_2D(int heigth, int length) {
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
    build_mesh_3D(size, size, size);
  }

  void build_mesh_3D(int heigth, int length, int depth) {
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
};

TEST_F(sintetic_net_topo, build_tree_7) {
  
  int max_depth = 7;
  int npe = pow(2, max_depth) - 1;
  build_tree(max_depth);
  EXPECT_EQ(machine_topo.size(), npe);
  EXPECT_EQ(network_topo.size(), npe);
}

TEST_F(sintetic_net_topo, xml_tree_7) {
  
  int max_depth = 7;
  build_tree(max_depth);

  net.init(machine_topo, network_topo);
  net.save_topology("tree_7");
}


TEST_F(sintetic_net_topo, build_tree_10) {
	
	int max_depth = 10;
	int npe = pow(2, max_depth) - 1;
  build_tree(max_depth);
  EXPECT_EQ(machine_topo.size(), npe);
  EXPECT_EQ(network_topo.size(), npe);
}

TEST_F(sintetic_net_topo, xml_tree_10) {
  
  int max_depth = 10;
  build_tree(max_depth);

  net.init(machine_topo, network_topo);
  net.save_topology("tree_10");
}

TEST_F(sintetic_net_topo, xml_tree_10_no_memoi) {
  
  int max_depth = 10;
  build_tree(max_depth);

  net.init(machine_topo, network_topo, "tree_10_no_memoi", false);
  net.save_topology();
}

TEST_F(sintetic_net_topo, build_mesh_2D_16) {

  int size = 16;
  int npe = size * size;
  build_mesh_2D(size);
  printf("size of topo %d\n", npe);
  EXPECT_EQ(machine_topo.size(), npe);
  EXPECT_EQ(network_topo.size(), npe);

}

TEST_F(sintetic_net_topo, xml_mesh_2D_16) {
  
  int size = 16;
  build_mesh_2D(size);

  printf("topo init\n");
  net.init(machine_topo, network_topo);
  printf("topo save\n");
  net.save_topology("mesh_2D_16");
}

TEST_F(sintetic_net_topo, build_mesh_3D_8) {
    // sets machine topology to null
  int size = 8;
  int npe = size * size * size;
  build_mesh_3D(size);
  printf("size of topo %d\n", npe);
  EXPECT_EQ(machine_topo.size(), npe);
  EXPECT_EQ(network_topo.size(), npe);

}

TEST_F(sintetic_net_topo, xml_mesh_3D_8) {
  
  int size = 8;
  build_mesh_3D(size);

  net.init(machine_topo, network_topo);
  net.save_topology("mesh_3D_8");
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
