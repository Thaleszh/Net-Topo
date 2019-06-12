/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

//#include "upper_matrix.h"
#include "net_topo_charm_proxy.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>

namespace {

// The fixture for testing class Foo.

class upper_matrix_test : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
 	upper_matrix_real mat;

  upper_matrix_test() {
    mat = upper_matrix_real(4);
  }

  ~upper_matrix_test() override {
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};



TEST_F(upper_matrix_test, get_set) {
	mat.set(1,2,1);
	ASSERT_EQ(mat.get(1,2), 1);
}

TEST_F(upper_matrix_test, cereal_archive_save) {

	mat.set(1,2,1);
	ASSERT_EQ(mat.get(1,2), 1);

	std::ofstream file("try.xml");
	cereal::XMLOutputArchive archive(file);
	archive(mat);
}

TEST_F(upper_matrix_test, cereal_archive_load) {
	upper_matrix_real new_mat;

	std::ifstream file("try.xml");

	cereal::XMLInputArchive archive(file);

	archive(new_mat);

    ASSERT_EQ(new_mat.get(1,2), 1);

}

TEST_F(upper_matrix_test, diagonal_zero) {
	ASSERT_EQ(mat.get(1,1), 0);
}

TEST_F(upper_matrix_test, diagonal_protection) {
	mat.set(1,1,1);
	ASSERT_EQ(mat.get(1,1), 0);
}

TEST_F(upper_matrix_test, reflection) {
	mat.set(1,2,1);
	ASSERT_EQ(mat.get(2,1), 1);
}

TEST_F(upper_matrix_test, full_matrix) {
	mat.set(0,1,1);
	mat.set(0,2,2);
	mat.set(0,3,3);
	mat.set(1,2,4);
	mat.set(1,3,5);
	mat.set(2,3,6);

	std::ofstream file("try.xml");
	cereal::XMLOutputArchive archive(file);
	archive(mat);

	EXPECT_EQ(mat.get(0,1), 1);
	EXPECT_EQ(mat.get(0,2), 2);
	EXPECT_EQ(mat.get(0,3), 3);
	EXPECT_EQ(mat.get(1,2), 4);
	EXPECT_EQ(mat.get(1,3), 5);
	EXPECT_EQ(mat.get(2,3), 6);

	// diagonals
	EXPECT_EQ(mat.get(0,0), 0);
	EXPECT_EQ(mat.get(1,1), 0);
	EXPECT_EQ(mat.get(2,2), 0);
	EXPECT_EQ(mat.get(3,3), 0);

	// reflections
	EXPECT_EQ(mat.get(1,0), 1);
	EXPECT_EQ(mat.get(2,0), 2);
	EXPECT_EQ(mat.get(3,0), 3);
	EXPECT_EQ(mat.get(2,1), 4);
	EXPECT_EQ(mat.get(3,1), 5);
	EXPECT_EQ(mat.get(3,2), 6);
}
TEST_F(upper_matrix_test, full_matrix_loaded) {
	upper_matrix_real new_mat;

	std::ifstream file("try.xml");

	cereal::XMLInputArchive archive(file);

	archive(new_mat);

    EXPECT_EQ(new_mat.get(0,1), 1);
	EXPECT_EQ(new_mat.get(0,2), 2);
	EXPECT_EQ(new_mat.get(0,3), 3);
	EXPECT_EQ(new_mat.get(1,2), 4);
	EXPECT_EQ(new_mat.get(1,3), 5);
	EXPECT_EQ(new_mat.get(2,3), 6);

	// diagonals
	EXPECT_EQ(new_mat.get(0,0), 0);
	EXPECT_EQ(new_mat.get(1,1), 0);
	EXPECT_EQ(new_mat.get(2,2), 0);
	EXPECT_EQ(new_mat.get(3,3), 0);

	// reflections
	EXPECT_EQ(new_mat.get(1,0), 1);
	EXPECT_EQ(new_mat.get(2,0), 2);
	EXPECT_EQ(new_mat.get(3,0), 3);
	EXPECT_EQ(new_mat.get(2,1), 4);
	EXPECT_EQ(new_mat.get(3,1), 5);
	EXPECT_EQ(new_mat.get(3,2), 6);
}

class CSC_small : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
 	std::unique_ptr<csc> small;

  CSC_small() {
  	small.reset(new csc(4,4));
  }

  ~CSC_small() override {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
	std::ifstream file("try.xml");

	cereal::XMLInputArchive archive(file);

	archive(small);
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST(CSC_xml, create_and_save) {
	printf("Going to create\n");
 	std::unique_ptr<csc> small(new csc(4,4));
 	printf("Created\n");

	small->create_index(0, 2);
	small->create_index(1, 2);
	small->create_index(2, 2);
	small->create_index(3, 2);
	printf("indexes created\n");	
	small->create(0, 1, 11);
	small->create(0, 3, 5);
	small->create(1, 0, 11);
	small->create(1, 2, 5);
	printf("half indexes created\n");
	small->create(2, 1, 5);
	small->create(2, 3, 5);
	small->create(3, 0, 5);
	small->create(3, 2, 5);
	printf("indexes created\n");
	printf("Insert\n");
	std::ofstream file("try.xml");
	printf("Going to archive\n");
	cereal::XMLOutputArchive archive(file);
	archive(small);
}

TEST_F(CSC_small, load_setup) {
}

TEST_F(CSC_small, basic_insert) {
	EXPECT_EQ(small->get(0, 1), 11);
	EXPECT_EQ(small->get(0, 3), 5);
}

TEST_F(CSC_small, get_self) {
	EXPECT_EQ(small->get(0, 0), 0);
}

TEST_F(CSC_small, get_inexistent) {
	EXPECT_EQ(small->get(0, 2), -1);
}

TEST_F(CSC_small, full_insert) {
	EXPECT_EQ(small->get(0, 1), 11);
	EXPECT_EQ(small->get(0, 3), 5);
	EXPECT_EQ(small->get(1, 0), 11);
	EXPECT_EQ(small->get(1, 2), 5);
	EXPECT_EQ(small->get(2, 1), 5);
	EXPECT_EQ(small->get(2, 3), 5);
	EXPECT_EQ(small->get(3, 0), 5);
	EXPECT_EQ(small->get(3, 2), 5);
}



TEST_F(CSC_small, set) {
	small->set(0, 1, 22);
	EXPECT_EQ(small->get(0, 1), 22);
}

TEST_F(CSC_small, set_inexistent) {
	small->set(0, 2, 22);
	EXPECT_EQ(small->get(0, 2), -1);

	small->set(0, 0, 25);
	EXPECT_EQ(small->get(0, 0), 0);
}

TEST_F(CSC_small, indexes_creation_first) {
	small.reset(new csc(4,4));
  	small->create_index(0, 2);
	small->create_index(1, 2);
	small->create_index(2, 2);
	small->create_index(3, 2);
	small->create(0, 1, 11);
	small->create(0, 3, 5);
	small->create(1, 0, 11);
	small->create(1, 2, 5);
	small->create(2, 1, 5);
	small->create(2, 3, 5);
	small->create(3, 0, 5);
	small->create(3, 2, 5);

	EXPECT_EQ(small->get(0, 1), 11);
	EXPECT_EQ(small->get(0, 3), 5);
	EXPECT_EQ(small->get(1, 0), 11);
	EXPECT_EQ(small->get(1, 2), 5);
	EXPECT_EQ(small->get(2, 1), 5);
	EXPECT_EQ(small->get(2, 3), 5);
	EXPECT_EQ(small->get(3, 0), 5);
	EXPECT_EQ(small->get(3, 2), 5);

}

TEST_F(CSC_small, creations_out_of_order) {
	small.reset(new csc(4,4));
  	small->create_index(0, 2);
	small->create_index(1, 2);
	small->create_index(2, 2);
	small->create_index(3, 2);
	small->create(1, 0, 11);
	small->create(2, 3, 5);
	small->create(2, 1, 5);
	small->create(3, 0, 5);
	small->create(1, 2, 5);
	small->create(0, 1, 11);
	small->create(0, 3, 5);
	small->create(3, 2, 5);

	EXPECT_EQ(small->get(0, 1), 11);
	EXPECT_EQ(small->get(0, 3), 5);
	EXPECT_EQ(small->get(1, 0), 11);
	EXPECT_EQ(small->get(1, 2), 5);
	EXPECT_EQ(small->get(2, 1), 5);
	EXPECT_EQ(small->get(2, 3), 5);
	EXPECT_EQ(small->get(3, 0), 5);
	EXPECT_EQ(small->get(3, 2), 5);
}


TEST_F(CSC_small, cereal_archive_save) {

	std::ofstream file("try.xml");
	cereal::XMLOutputArchive archive(file);
	archive(small);
}

TEST_F(CSC_small, cereal_archive_load) {
	std::unique_ptr<csc> new_small;

	std::ifstream file("try.xml");

	cereal::XMLInputArchive archive(file);

	archive(new_small);

	EXPECT_EQ(new_small->get(0, 1), 11);
	EXPECT_EQ(new_small->get(0, 3), 5);
	EXPECT_EQ(new_small->get(1, 0), 11);
	EXPECT_EQ(new_small->get(1, 2), 5);
	EXPECT_EQ(new_small->get(2, 1), 5);
	EXPECT_EQ(new_small->get(2, 3), 5);
	EXPECT_EQ(new_small->get(3, 0), 5);
	EXPECT_EQ(new_small->get(3, 2), 5);
}


TEST_F(CSC_small, distance_self) {
	EXPECT_EQ(small->distance(0, 0), 0);
}

TEST_F(CSC_small, distance_0_1) {
	EXPECT_EQ(small->distance(0, 1), 11);
}

TEST_F(CSC_small, distance_0_2) {
	EXPECT_EQ(small->distance(0, 2), 10);
}

TEST_F(CSC_small, distance_0_3) {
	EXPECT_EQ(small->distance(0, 3), 5);
}

TEST_F(CSC_small, distance_all_1s) {
	EXPECT_EQ(small->distance(1, 0), 11);
	EXPECT_EQ(small->distance(1, 2), 5);
	EXPECT_EQ(small->distance(1, 3), 10);
}

TEST_F(CSC_small, distance_all_2s) {
	EXPECT_EQ(small->distance(2, 0), 10);
	EXPECT_EQ(small->distance(2, 1), 5);
	EXPECT_EQ(small->distance(2, 3), 5);
}

TEST_F(CSC_small, distance_all_3s) {
	EXPECT_EQ(small->distance(3, 0), 5);
	EXPECT_EQ(small->distance(3, 1), 10);
	EXPECT_EQ(small->distance(3, 2), 5);
}

TEST_F(CSC_small, path_2_0) {
	std::list<node> path = small->path_find(2, 0);
	auto it = path.begin();
	ASSERT_EQ(path.size(), 2);
	EXPECT_EQ((*it).index, 3);
	it++;
	EXPECT_EQ((*it).index, 0);
}

TEST_F(CSC_small, path_2_0_loaded) {
	std::unique_ptr<csc> new_small;

	std::ifstream file("try.xml");

	cereal::XMLInputArchive archive(file);

	archive(new_small);
	std::list<node> path = new_small->path_find(2, 0);
	auto it = path.begin();
	ASSERT_EQ(path.size(), 2);
	EXPECT_EQ((*it).index, 3);
	it++;
	EXPECT_EQ((*it).index, 0);
}

TEST_F(CSC_small, path_self_0) {
	std::list<node> path = small->path_find(0, 0);
	ASSERT_EQ(path.size(), 0);
}

TEST_F(CSC_small, hops_self_0) {
	EXPECT_EQ(small->hops(0, 0), 0);
}

TEST_F(CSC_small, hops_0_1) {
	EXPECT_EQ(small->hops(0, 1), 1);
}

TEST_F(CSC_small, hops_0_2) {
	EXPECT_EQ(small->hops(0, 2), 2);
}

TEST_F(CSC_small, hops_2_0) {
	EXPECT_EQ(small->hops(2, 0), 2);
}


class CSC_big : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
 	std::unique_ptr<csc> big;

  CSC_big() {
  	big.reset(new csc(12, 12));
  }

  ~CSC_big() override {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).

  	big->create_index(0, 4);
	big->create_index(1, 4);
	big->create_index(2, 4);
  	big->create_index(3, 4);

  	for (int i = 0; i < 8; i++) {
  		big->create_index(i+4, 1);
  	}
	big->create(0, 1, 16);
	big->create(0, 3, 5);
	big->create(0, 4, 1);
	big->create(0, 5, 1);

	big->create(1, 0, 16);
	big->create(1, 2, 5);
	big->create(1, 6, 1);
	big->create(1, 7, 1);

	big->create(2, 1, 5);
	big->create(2, 3, 5);
	big->create(2, 8, 1);
	big->create(2, 9, 1);

	big->create(3, 0, 5);
	big->create(3, 2, 5);
	big->create(3, 10, 1);
	big->create(3, 11, 1);


	for (int i = 0; i < 4; i++) {
		big->create(2*i+4, i, 1);
		big->create(2*i+5, i, 1);
	}

  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(CSC_big, setup) {
}

TEST_F(CSC_big, full_insert) {
	EXPECT_EQ(big->get(0, 1), 16);
	EXPECT_EQ(big->get(0, 3), 5);
	EXPECT_EQ(big->get(1, 0), 16);
	EXPECT_EQ(big->get(1, 2), 5);
	EXPECT_EQ(big->get(2, 1), 5);
	EXPECT_EQ(big->get(2, 3), 5);
	EXPECT_EQ(big->get(3, 0), 5);
	EXPECT_EQ(big->get(3, 2), 5);

	EXPECT_EQ(big->get(0, 4), 1);
	EXPECT_EQ(big->get(0, 5), 1);

	EXPECT_EQ(big->get(1, 6), 1);
	EXPECT_EQ(big->get(1, 7), 1);

	EXPECT_EQ(big->get(2, 8), 1);
	EXPECT_EQ(big->get(2, 9), 1);

	EXPECT_EQ(big->get(3, 10), 1);
	EXPECT_EQ(big->get(3, 11), 1);

}

TEST_F(CSC_big, cereal_archive_save) {

	std::ofstream file("big_try.xml");
	cereal::XMLOutputArchive archive(file);
	archive(cereal::make_nvp("csc_big",big));
}

TEST_F(CSC_big, cereal_archive_load) {
	std::unique_ptr<csc> new_big;

	std::ifstream file("big_try.xml");

	cereal::XMLInputArchive archive(file);

	archive(new_big);

	EXPECT_EQ(big->get(0, 1), 16);
	EXPECT_EQ(big->get(0, 3), 5);
	EXPECT_EQ(big->get(1, 0), 16);
	EXPECT_EQ(big->get(1, 2), 5);
	EXPECT_EQ(big->get(2, 1), 5);
	EXPECT_EQ(big->get(2, 3), 5);
	EXPECT_EQ(big->get(3, 0), 5);
	EXPECT_EQ(big->get(3, 2), 5);

	EXPECT_EQ(big->get(0, 4), 1);
	EXPECT_EQ(big->get(0, 5), 1);

	EXPECT_EQ(big->get(1, 6), 1);
	EXPECT_EQ(big->get(1, 7), 1);

	EXPECT_EQ(big->get(2, 8), 1);
	EXPECT_EQ(big->get(2, 9), 1);

	EXPECT_EQ(big->get(3, 10), 1);
	EXPECT_EQ(big->get(3, 11), 1);
}

TEST_F(CSC_big, distance_0_5) {
	EXPECT_EQ(big->distance(0, 5), 1);
}

TEST_F(CSC_big, distance_9_2) {
	EXPECT_EQ(big->distance(9, 2), 1);
}

TEST_F(CSC_big, distance_2_3) {
	EXPECT_EQ(big->distance(2, 3), 5);
}

TEST_F(CSC_big, distance_all_0s) {
	EXPECT_EQ(big->distance(0, 2), 10);
	EXPECT_EQ(big->distance(0, 7), 16);
	EXPECT_EQ(big->distance(0, 8), 11);
	EXPECT_EQ(big->distance(0, 9), 11);
	EXPECT_EQ(big->distance(0, 10), 6);
}

TEST_F(CSC_big, distance_all_1s) {
	EXPECT_EQ(big->distance(1, 0), 15);
	EXPECT_EQ(big->distance(1, 2), 5);
	EXPECT_EQ(big->distance(1, 3), 10);
	EXPECT_EQ(big->distance(1, 4), 16);
	EXPECT_EQ(big->distance(1, 9), 6);
}

TEST_F(CSC_big, distance_all_2s) {
	EXPECT_EQ(big->distance(2, 0), 10);
	EXPECT_EQ(big->distance(2, 1), 5);
	EXPECT_EQ(big->distance(2, 3), 5);
	EXPECT_EQ(big->distance(2, 5), 11);
	EXPECT_EQ(big->distance(2, 11), 6);
}

TEST_F(CSC_big, distance_all_3s) {
	EXPECT_EQ(big->distance(3, 0), 5);
	EXPECT_EQ(big->distance(3, 1), 10);
	EXPECT_EQ(big->distance(3, 2), 5);
	EXPECT_EQ(big->distance(3, 8), 6);
	EXPECT_EQ(big->distance(3, 4), 6);
}

TEST_F(CSC_big, distance_all_others) {
	EXPECT_EQ(big->distance(4, 9), 12);
	EXPECT_EQ(big->distance(5, 10), 7);
	EXPECT_EQ(big->distance(6, 11), 12);
	EXPECT_EQ(big->distance(7, 4), 17);
	EXPECT_EQ(big->distance(8, 2), 1);
	EXPECT_EQ(big->distance(9, 6), 7);
	EXPECT_EQ(big->distance(10, 8), 7);
	EXPECT_EQ(big->distance(11, 5), 7);

	std::ofstream file("big_try.xml");
	cereal::XMLOutputArchive archive(file);
	archive(cereal::make_nvp("csc_big",big));

}

TEST_F(CSC_big, path_4_5) {
	std::list<node> path = big->path_find(4, 5);
	auto it = path.begin();
	ASSERT_EQ(path.size(), 2);
	EXPECT_EQ((*it).index, 0);
	it++;
	EXPECT_EQ((*it).index, 5);
}

TEST_F(CSC_big, path_4_7) {
	std::list<node> path = big->path_find(4, 7);
	ASSERT_EQ(path.size(), 5);
	auto it = path.begin();
	EXPECT_EQ((*it).index, 0);
	it++;
	EXPECT_EQ((*it).index, 3);
	it++;
	EXPECT_EQ((*it).index, 2);
	it++;
	EXPECT_EQ((*it).index, 1);
	it++;
	EXPECT_EQ((*it).index, 7);
	EXPECT_EQ((*it).ncost, 17);
}

TEST_F(CSC_big, hop_0_1) {
	EXPECT_EQ(big->hops(0, 1), 1);
}

TEST_F(CSC_big, hop_0_2) {
	EXPECT_EQ(big->hops(0, 2), 2);
}

TEST_F(CSC_big, hop_2_0) {
	EXPECT_EQ(big->hops(2, 0), 2);
}

TEST_F(CSC_big, hop_4_6) {
	EXPECT_EQ(big->hops(4, 6), 3);
}

TEST_F(CSC_big, hop_2_11) {
	EXPECT_EQ(big->hops(2, 11), 2);
}

TEST_F(CSC_big, hop_8_1) {
	EXPECT_EQ(big->hops(8, 1), 2);
}

TEST_F(CSC_big, hop_9_4) {
	EXPECT_EQ(big->hops(9, 4), 4);
}

TEST_F(CSC_big, hop_11_10) {
	EXPECT_EQ(big->hops(11, 10), 2);
}

TEST(net_topo_xml, creation) {
	net_topo net;

	std::vector<std::vector<pair<int, int>>> network_topo; 
	std::vector<std::pair<int, int>> machine_topo;

	std::vector<std::pair<int, int>> temporary;
	int npe = 4;
	//node 0
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 16));
	temporary.push_back(std::pair<int, int>(3, 5));
	network_topo.push_back(temporary);
	// node 1
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 16));
	temporary.push_back(std::pair<int, int>(2, 5));
	network_topo.push_back(temporary);
	// node 2
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 5));
	temporary.push_back(std::pair<int, int>(3, 5));
	network_topo.push_back(temporary);
	// node 3
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 5));
	temporary.push_back(std::pair<int, int>(2, 5));
	network_topo.push_back(temporary);
	for (int i = 0; i < 4; i++)
		machine_topo.push_back(std::pair<int, int>(i, 0)); // adds machine and node as self

	net.init(machine_topo, network_topo);

	// tests save to default filename
	net.save_topology();
	net.save_topology("net_topo_small");

}

TEST(net_topo_xml, load_default_name) {
	net_topo net;
	net.load_topology();
	EXPECT_EQ(net.num_pes(), 4);
	EXPECT_EQ(net.name(), "net_topo.xml");

}

class small_net_topo : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
 	net_topo net;

  small_net_topo() {

  }

  ~small_net_topo() override {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  	net.load_topology("net_topo_small");

  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  	net.save_topology();
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(small_net_topo, load_topology) {
}

TEST_F(small_net_topo, num_pes) {
	EXPECT_EQ(net.num_pes(), 4);
}

TEST_F(small_net_topo, distance_0_1) {
	EXPECT_EQ(net.distance(0,1), 15);
}

TEST_F(small_net_topo, distance_0_2) {
	EXPECT_EQ(net.distance(0,2), 10);
}

TEST_F(small_net_topo, distance_0_3) {
	EXPECT_EQ(net.distance(0,3), 5);
}

TEST_F(small_net_topo, memoi_check_0_1) {
	EXPECT_EQ(net.distance(0,1), 15);
}

TEST_F(small_net_topo, net_neighbors) {
	std::vector<int> neigh = net.net_neighbors(0);

	ASSERT_EQ(neigh.size(), 2);
	auto it = neigh.begin();
	EXPECT_EQ((*it), 1);
	it++;
	EXPECT_EQ((*it), 3);

}

TEST_F(small_net_topo, node_of_0) {
	EXPECT_EQ(net.node_of(0), 0);
}

TEST_F(small_net_topo, machine_of_0) {
	EXPECT_EQ(net.machine_of(0), 0);
}

TEST_F(small_net_topo, proximity_0_1) {
	EXPECT_EQ(net.proximity(0, 1), 0);
}

TEST_F(small_net_topo, hops_0_1) {
	EXPECT_EQ(net.hop_count(0,1), 1);
}

TEST_F(small_net_topo, hops_0_2) {
	EXPECT_EQ(net.hop_count(0,2), 2);
}

TEST_F(small_net_topo, hops_0_3) {
	EXPECT_EQ(net.hop_count(0,3), 1);
}

TEST_F(small_net_topo, name) {
	EXPECT_EQ(net.name(), "net_topo.xml");
}

TEST_F(small_net_topo, change_name) {
	EXPECT_EQ(net.name(), "net_topo.xml");
	net.change_name("new_name");
	EXPECT_EQ(net.name(), "new_name");
}

TEST(real_net_topo_, setup_xml_with_nodes) {
	net_topo net;

	std::vector<std::vector<pair<int, int>>> network_topo; 
	std::vector<std::pair<int, int>> machine_topo;

	for (int i = 0; i < 8; i++)
		machine_topo.push_back(std::pair<int, int>(0, i%4));

	machine_topo.push_back(std::pair<int, int>(1, 0));
	machine_topo.push_back(std::pair<int, int>(1, 1));
	machine_topo.push_back(std::pair<int, int>(1, 2));
	machine_topo.push_back(std::pair<int, int>(1, 3));


	std::vector<std::pair<int, int>> temporary;
	int npe = 12;
	//node 0
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(2, 2));
	temporary.push_back(std::pair<int, int>(4, 1));
	temporary.push_back(std::pair<int, int>(8, 8));
	network_topo.push_back(temporary);
	// node 1
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 2));
	temporary.push_back(std::pair<int, int>(3, 2));
	temporary.push_back(std::pair<int, int>(5, 1));
	temporary.push_back(std::pair<int, int>(9, 8));
	network_topo.push_back(temporary);
	// node 2
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 2));
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(6, 1));
	temporary.push_back(std::pair<int, int>(10, 8));
	network_topo.push_back(temporary);
	// node 3
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(2, 2));
	temporary.push_back(std::pair<int, int>(7, 1));
	temporary.push_back(std::pair<int, int>(11, 8));
	network_topo.push_back(temporary);
	// node 4
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 1));
	temporary.push_back(std::pair<int, int>(5, 2));
	temporary.push_back(std::pair<int, int>(6, 2));
	network_topo.push_back(temporary);
	// node 5
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 1));
	temporary.push_back(std::pair<int, int>(4, 2));
	temporary.push_back(std::pair<int, int>(7, 2));
	network_topo.push_back(temporary);
	// node 6
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(2, 1));
	temporary.push_back(std::pair<int, int>(4, 2));
	temporary.push_back(std::pair<int, int>(7, 2));
	network_topo.push_back(temporary);
	// node 7
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(3, 1));
	temporary.push_back(std::pair<int, int>(5, 2));
	temporary.push_back(std::pair<int, int>(6, 2));
	network_topo.push_back(temporary);
	// node 8
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 8));
	temporary.push_back(std::pair<int, int>(9, 2));
	temporary.push_back(std::pair<int, int>(10, 2));
	network_topo.push_back(temporary);
	// node 9
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 8));
	temporary.push_back(std::pair<int, int>(8, 2));
	temporary.push_back(std::pair<int, int>(11, 2));
	network_topo.push_back(temporary);
	// node 10
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(2, 8));
	temporary.push_back(std::pair<int, int>(8, 2));
	temporary.push_back(std::pair<int, int>(11, 2));
	network_topo.push_back(temporary);
	// node 11
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(3, 8));
	temporary.push_back(std::pair<int, int>(9, 2));
	temporary.push_back(std::pair<int, int>(10, 2));
	network_topo.push_back(temporary);

	net.init(machine_topo, network_topo);

	net.save_topology("real_net_topo");

}

TEST(real_net_topo_, setup_xml_no_nodes_no_memoi) {
	net_topo net;

	std::vector<std::vector<pair<int, int>>> network_topo; 
	std::vector<std::pair<int, int>> machine_topo;

	std::vector<std::pair<int, int>> temporary;
	int npe = 12;
	//node 0
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(2, 2));
	temporary.push_back(std::pair<int, int>(4, 1));
	temporary.push_back(std::pair<int, int>(8, 8));
	network_topo.push_back(temporary);
	// node 1
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 2));
	temporary.push_back(std::pair<int, int>(3, 2));
	temporary.push_back(std::pair<int, int>(5, 1));
	temporary.push_back(std::pair<int, int>(9, 8));
	network_topo.push_back(temporary);
	// node 2
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 2));
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(6, 1));
	temporary.push_back(std::pair<int, int>(10, 8));
	network_topo.push_back(temporary);
	// node 3
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(2, 2));
	temporary.push_back(std::pair<int, int>(7, 1));
	temporary.push_back(std::pair<int, int>(11, 8));
	network_topo.push_back(temporary);
	// node 4
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 1));
	temporary.push_back(std::pair<int, int>(5, 2));
	temporary.push_back(std::pair<int, int>(6, 2));
	network_topo.push_back(temporary);
	// node 5
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 1));
	temporary.push_back(std::pair<int, int>(4, 2));
	temporary.push_back(std::pair<int, int>(7, 2));
	network_topo.push_back(temporary);
	// node 6
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(2, 1));
	temporary.push_back(std::pair<int, int>(4, 2));
	temporary.push_back(std::pair<int, int>(7, 2));
	network_topo.push_back(temporary);
	// node 7
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(3, 1));
	temporary.push_back(std::pair<int, int>(5, 2));
	temporary.push_back(std::pair<int, int>(6, 2));
	network_topo.push_back(temporary);
	// node 8
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 8));
	temporary.push_back(std::pair<int, int>(9, 2));
	temporary.push_back(std::pair<int, int>(10, 2));
	network_topo.push_back(temporary);
	// node 9
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 8));
	temporary.push_back(std::pair<int, int>(8, 2));
	temporary.push_back(std::pair<int, int>(11, 2));
	network_topo.push_back(temporary);
	// node 10
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(2, 8));
	temporary.push_back(std::pair<int, int>(8, 2));
	temporary.push_back(std::pair<int, int>(11, 2));
	network_topo.push_back(temporary);
	// node 11
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(3, 8));
	temporary.push_back(std::pair<int, int>(9, 2));
	temporary.push_back(std::pair<int, int>(10, 2));
	network_topo.push_back(temporary);

	for (int i = 0; i < npe; i++)
		machine_topo.push_back(std::pair<int, int>(i, 0)); // adds node and machine as self

	net.init(machine_topo, network_topo, "real_net_topo_no_nodes_no_memoi", false);

	net.save_topology();
}

TEST(real_net_topo_, setup_xml_no_nodes) {
	net_topo net;

	std::vector<std::vector<pair<int, int>>> network_topo; 
	std::vector<std::pair<int, int>> machine_topo;

	std::vector<std::pair<int, int>> temporary;
	int npe = 12;
	//node 0
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(2, 2));
	temporary.push_back(std::pair<int, int>(4, 1));
	temporary.push_back(std::pair<int, int>(8, 8));
	network_topo.push_back(temporary);
	// node 1
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 2));
	temporary.push_back(std::pair<int, int>(3, 2));
	temporary.push_back(std::pair<int, int>(5, 1));
	temporary.push_back(std::pair<int, int>(9, 8));
	network_topo.push_back(temporary);
	// node 2
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 2));
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(6, 1));
	temporary.push_back(std::pair<int, int>(10, 8));
	network_topo.push_back(temporary);
	// node 3
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 2));
	temporary.push_back(std::pair<int, int>(2, 2));
	temporary.push_back(std::pair<int, int>(7, 1));
	temporary.push_back(std::pair<int, int>(11, 8));
	network_topo.push_back(temporary);
	// node 4
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 1));
	temporary.push_back(std::pair<int, int>(5, 2));
	temporary.push_back(std::pair<int, int>(6, 2));
	network_topo.push_back(temporary);
	// node 5
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 1));
	temporary.push_back(std::pair<int, int>(4, 2));
	temporary.push_back(std::pair<int, int>(7, 2));
	network_topo.push_back(temporary);
	// node 6
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(2, 1));
	temporary.push_back(std::pair<int, int>(4, 2));
	temporary.push_back(std::pair<int, int>(7, 2));
	network_topo.push_back(temporary);
	// node 7
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(3, 1));
	temporary.push_back(std::pair<int, int>(5, 2));
	temporary.push_back(std::pair<int, int>(6, 2));
	network_topo.push_back(temporary);
	// node 8
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(0, 8));
	temporary.push_back(std::pair<int, int>(9, 2));
	temporary.push_back(std::pair<int, int>(10, 2));
	network_topo.push_back(temporary);
	// node 9
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(1, 8));
	temporary.push_back(std::pair<int, int>(8, 2));
	temporary.push_back(std::pair<int, int>(11, 2));
	network_topo.push_back(temporary);
	// node 10
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(2, 8));
	temporary.push_back(std::pair<int, int>(8, 2));
	temporary.push_back(std::pair<int, int>(11, 2));
	network_topo.push_back(temporary);
	// node 11
	temporary = std::vector<std::pair<int, int>>();
	temporary.push_back(std::pair<int, int>(3, 8));
	temporary.push_back(std::pair<int, int>(9, 2));
	temporary.push_back(std::pair<int, int>(10, 2));
	network_topo.push_back(temporary);

	for (int i = 0; i < npe; i++)
		machine_topo.push_back(std::pair<int, int>(i, 0)); // adds node and machine as self

	net.init(machine_topo, network_topo);

	net.save_topology("real_net_topo_no_nodes");

}

class real_net_topo : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
 	net_topo net;

  real_net_topo() {

  }

  ~real_net_topo() override {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  	net.load_topology("real_net_topo");

  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  	net.save_topology();
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(real_net_topo, load_topology) {
}

TEST_F(real_net_topo, num_pes) {
	EXPECT_EQ(net.num_pes(), 12);
}

TEST_F(real_net_topo, num_nodes) {
	EXPECT_EQ(net.num_nodes(), 8);
}

TEST_F(real_net_topo, num_machines) {
	EXPECT_EQ(net.num_machines(), 2);
}


TEST_F(real_net_topo, distance_0_1) {
	EXPECT_FLOAT_EQ(net.distance(0,1), 0.2);
}

TEST_F(real_net_topo, distance_0_8) {
	EXPECT_EQ(net.distance(0,8),8);
}

TEST_F(real_net_topo, distance_0_11) {
	EXPECT_EQ(net.distance(0,11), 8);
}


TEST_F(real_net_topo, net_neighbors) {
	std::vector<int> neigh = net.net_neighbors(0);

	ASSERT_EQ(neigh.size(), 1);
	auto it = neigh.begin();
	EXPECT_EQ((*it), 1);

}

TEST_F(real_net_topo, node_of_0) {
	EXPECT_EQ(net.node_of(0), 0);
}

TEST_F(real_net_topo, machine_of_0) {
	EXPECT_EQ(net.machine_of(0), 0);
}

TEST_F(real_net_topo, node_of_3) {
	EXPECT_EQ(net.node_of(3), 3);
}

TEST_F(real_net_topo, machine_of_3) {
	EXPECT_EQ(net.machine_of(3), 0);
}

TEST_F(real_net_topo, machine_of_9) {
	EXPECT_EQ(net.machine_of(9), 1);
}

TEST_F(real_net_topo, node_of_9) {
	EXPECT_EQ(net.node_of(9), 5);
}

TEST_F(real_net_topo, proximity_0_0) {
	EXPECT_EQ(net.proximity(0, 0), 3);
}

TEST_F(real_net_topo, proximity_0_1) {
	EXPECT_EQ(net.proximity(0, 1), 1);
}

TEST_F(real_net_topo, proximity_0_4) {
	EXPECT_EQ(net.proximity(0, 4), 2);
}

TEST_F(real_net_topo, proximity_0_8) {
	EXPECT_EQ(net.proximity(0, 8), 0);
}

TEST_F(real_net_topo, proximity_8_9) {
	EXPECT_EQ(net.proximity(8, 9), 1);
}

TEST_F(real_net_topo, hops_0_1) {
	EXPECT_EQ(net.hop_count(0,1), 1);
}

TEST_F(real_net_topo, hops_0_2) {
	EXPECT_EQ(net.hop_count(0,2), 1);
}

TEST_F(real_net_topo, hops_0_4) {
	EXPECT_EQ(net.hop_count(0,4), 1);
}

TEST_F(real_net_topo, hops_0_8) {
	EXPECT_EQ(net.hop_count(0,8), 1);
}

TEST_F(real_net_topo, on_same_machine_0) {
	std::vector<int> pes = net.on_same_machine(0);
	EXPECT_EQ(pes.size(), 7);
	std::set<int> all_pes;
	for (int i = 0; i < pes.size(); i++) {
		all_pes.insert(pes[i]);
	}
	EXPECT_EQ(all_pes.size(), 7);
	for(int i = 1; i < all_pes.size(); i++)
		EXPECT_TRUE(all_pes.find(i) != all_pes.end());

}

TEST_F(real_net_topo, on_same_machine_8) {
	std::vector<int> pes = net.on_same_machine(8);
	EXPECT_EQ(pes.size(), 3);
	std::set<int> all_pes;
	for (int i = 0; i < pes.size(); i++) {
		all_pes.insert(pes[i]);
	}
	EXPECT_EQ(all_pes.size(), 3);
	for(int i = 9; i < all_pes.size(); i++)
		EXPECT_TRUE(all_pes.find(i) != all_pes.end());

}

TEST_F(real_net_topo, pes_of_machine_0) {
	std::vector<int> pes = net.pes_of_machine(0);
	EXPECT_EQ(pes.size(), 8);
	std::set<int> all_pes;
	for (int i = 0; i < pes.size(); i++) {
		all_pes.insert(pes[i]);
	}
	EXPECT_EQ(all_pes.size(), 8);
	for(int i = 0; i < all_pes.size(); i++)
		EXPECT_TRUE(all_pes.find(i) != all_pes.end());

}


TEST_F(real_net_topo, on_same_node_0) {
	std::vector<int> pes = net.on_same_node(0);
	EXPECT_EQ(pes.size(), 1);
	EXPECT_EQ(pes[0], 4);

}

TEST_F(real_net_topo, on_same_node_8) {
	std::vector<int> pes = net.on_same_node(8);
	EXPECT_EQ(pes.size(), 0);

}

TEST_F(real_net_topo, pes_of_node_0) {
	std::vector<int> pes = net.pes_of_node(0);
	EXPECT_EQ(pes.size(), 2);
	std::set<int> all_pes;
	for (int i = 0; i < pes.size(); i++) {
		all_pes.insert(pes[i]);
	}
	EXPECT_EQ(all_pes.size(), 2);
	EXPECT_TRUE(all_pes.find(0) != all_pes.end());
	EXPECT_TRUE(all_pes.find(4) != all_pes.end());
}

TEST_F(real_net_topo, num_net_links) {
	EXPECT_EQ(net.num_net_links(), 1);
}

class real_net_topo_no_nodes : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
 	net_topo net;

  real_net_topo_no_nodes() {

  }

  ~real_net_topo_no_nodes() override {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  	net.load_topology("real_net_topo_no_nodes");

  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  	net.save_topology();
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(real_net_topo_no_nodes, load_topology) {
}

TEST_F(real_net_topo_no_nodes, num_pes) {
	EXPECT_EQ(net.num_pes(), 12);
}

TEST_F(real_net_topo_no_nodes, distance_0_1) {
	EXPECT_FLOAT_EQ(net.distance(0,1), 2);
}

TEST_F(real_net_topo_no_nodes, distance_0_8) {
	EXPECT_EQ(net.distance(0,8),8);
}

TEST_F(real_net_topo_no_nodes, distance_0_11) {
	EXPECT_EQ(net.distance(0,11), 12);
}

TEST_F(real_net_topo_no_nodes, distance_5_10) {
	EXPECT_EQ(net.distance(5,10), 11);
}

TEST_F(real_net_topo_no_nodes, net_neighbor0) {
	std::vector<int> neigh = net.net_neighbors(0);
	ASSERT_EQ(neigh.size(), 4);
}

TEST_F(real_net_topo_no_nodes, net_neighbors_11) {
	std::vector<int> pes = net.net_neighbors(11);
	EXPECT_EQ(pes.size(), 3);
}

TEST_F(real_net_topo_no_nodes, node_of_0) {
	EXPECT_EQ(net.node_of(0), 0);
}

TEST_F(real_net_topo_no_nodes, machine_of_0) {
	EXPECT_EQ(net.machine_of(0), 0);
}

TEST_F(real_net_topo_no_nodes, node_of_3) {
	EXPECT_EQ(net.node_of(3), 3);
}

TEST_F(real_net_topo_no_nodes, machine_of_3) {
	EXPECT_EQ(net.machine_of(3), 3);
}

TEST_F(real_net_topo_no_nodes, machine_of_9) {
	EXPECT_EQ(net.machine_of(9), 9);
}

TEST_F(real_net_topo_no_nodes, node_of_9) {
	EXPECT_EQ(net.node_of(9), 9);
}

TEST_F(real_net_topo_no_nodes, proximity_0_0) {
	EXPECT_EQ(net.proximity(0, 0), 3);
}

TEST_F(real_net_topo_no_nodes, proximity_0_1) {
	EXPECT_EQ(net.proximity(0, 1), 0);
}

TEST_F(real_net_topo_no_nodes, proximity_0_4) {
	EXPECT_EQ(net.proximity(0, 4), 0);
}

TEST_F(real_net_topo_no_nodes, proximity_0_8) {
	EXPECT_EQ(net.proximity(0, 8), 0);
}

TEST_F(real_net_topo_no_nodes, proximity_8_9) {
	EXPECT_EQ(net.proximity(8, 9), 0);
}

TEST_F(real_net_topo_no_nodes, hops_0_1) {
	EXPECT_EQ(net.hop_count(0,1), 1);
}

TEST_F(real_net_topo_no_nodes, hops_0_2) {
	EXPECT_EQ(net.hop_count(0,2), 1);
}

TEST_F(real_net_topo_no_nodes, hops_0_4) {
	EXPECT_EQ(net.hop_count(0,4), 1);
}

TEST_F(real_net_topo_no_nodes, hops_0_10) {
	EXPECT_EQ(net.hop_count(0,10), 2);
}

TEST_F(real_net_topo_no_nodes, hops_4_11) {
	EXPECT_EQ(net.hop_count(4,11), 4);
}

TEST_F(real_net_topo_no_nodes, on_same_machine_0) {
	std::vector<int> pes = net.on_same_machine(0);
	EXPECT_EQ(pes.size(), 0);

}

TEST_F(real_net_topo_no_nodes, on_same_machine_8) {
	std::vector<int> pes = net.on_same_machine(8);
	EXPECT_EQ(pes.size(), 0);
}

TEST_F(real_net_topo_no_nodes, num_net_links) {
	EXPECT_EQ(net.num_net_links(), 20);
}

class real_net_topo_no_nodes_no_memoi : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
 	net_topo net;

  real_net_topo_no_nodes_no_memoi() {

  }

  ~real_net_topo_no_nodes_no_memoi() override {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  	net.load_topology("real_net_topo_no_nodes_no_memoi");

  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  	net.save_topology();
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(real_net_topo_no_nodes_no_memoi, load_topology) {
}

TEST_F(real_net_topo_no_nodes_no_memoi, num_pes) {
	EXPECT_EQ(net.num_pes(), 12);
}

TEST_F(real_net_topo_no_nodes_no_memoi, distance_0_1) {
	EXPECT_FLOAT_EQ(net.distance(0,1), 2);
}

TEST_F(real_net_topo_no_nodes_no_memoi, distance_0_8) {
	EXPECT_EQ(net.distance(0,8),8);
}

TEST_F(real_net_topo_no_nodes_no_memoi, distance_0_11) {
	EXPECT_EQ(net.distance(0,11), 12);
}

TEST_F(real_net_topo_no_nodes_no_memoi, distance_5_10) {
	EXPECT_EQ(net.distance(5,10), 11);
}

TEST_F(real_net_topo_no_nodes_no_memoi, net_neighbor0) {
	std::vector<int> neigh = net.net_neighbors(0);
	ASSERT_EQ(neigh.size(), 4);
}

TEST_F(real_net_topo_no_nodes_no_memoi, net_neighbors_11) {
	std::vector<int> pes = net.net_neighbors(11);
	EXPECT_EQ(pes.size(), 3);
}

TEST_F(real_net_topo_no_nodes_no_memoi, node_of_0) {
	EXPECT_EQ(net.node_of(0), 0);
}

TEST_F(real_net_topo_no_nodes_no_memoi, machine_of_0) {
	EXPECT_EQ(net.machine_of(0), 0);
}

TEST_F(real_net_topo_no_nodes_no_memoi, node_of_3) {
	EXPECT_EQ(net.node_of(3), 3);
}

TEST_F(real_net_topo_no_nodes_no_memoi, machine_of_3) {
	EXPECT_EQ(net.machine_of(3), 3);
}

TEST_F(real_net_topo_no_nodes_no_memoi, machine_of_9) {
	EXPECT_EQ(net.machine_of(9), 9);
}

TEST_F(real_net_topo_no_nodes_no_memoi, node_of_9) {
	EXPECT_EQ(net.node_of(9), 9);
}

TEST_F(real_net_topo_no_nodes_no_memoi, proximity_0_0) {
	EXPECT_EQ(net.proximity(0, 0), 3);
}

TEST_F(real_net_topo_no_nodes_no_memoi, proximity_0_1) {
	EXPECT_EQ(net.proximity(0, 1), 0);
}

TEST_F(real_net_topo_no_nodes_no_memoi, proximity_0_4) {
	EXPECT_EQ(net.proximity(0, 4), 0);
}

TEST_F(real_net_topo_no_nodes_no_memoi, proximity_0_8) {
	EXPECT_EQ(net.proximity(0, 8), 0);
}

TEST_F(real_net_topo_no_nodes_no_memoi, proximity_8_9) {
	EXPECT_EQ(net.proximity(8, 9), 0);
}

TEST_F(real_net_topo_no_nodes_no_memoi, hops_0_1) {
	EXPECT_EQ(net.hop_count(0,1), 1);
}

TEST_F(real_net_topo_no_nodes_no_memoi, hops_0_2) {
	EXPECT_EQ(net.hop_count(0,2), 1);
}

TEST_F(real_net_topo_no_nodes_no_memoi, hops_0_4) {
	EXPECT_EQ(net.hop_count(0,4), 1);
}

TEST_F(real_net_topo_no_nodes_no_memoi, hops_0_10) {
	EXPECT_EQ(net.hop_count(0,10), 2);
}

TEST_F(real_net_topo_no_nodes_no_memoi, hops_4_11) {
	EXPECT_EQ(net.hop_count(4,11), 4);
}

TEST_F(real_net_topo_no_nodes_no_memoi, on_same_machine_0) {
	std::vector<int> pes = net.on_same_machine(0);
	EXPECT_EQ(pes.size(), 0);

}

TEST_F(real_net_topo_no_nodes_no_memoi, on_same_machine_8) {
	std::vector<int> pes = net.on_same_machine(8);
	EXPECT_EQ(pes.size(), 0);
}

TEST_F(real_net_topo_no_nodes_no_memoi, num_net_links) {
	EXPECT_EQ(net.num_net_links(), 20);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}