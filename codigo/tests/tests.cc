
#include "upper_matrix.h"
#include "CSC.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class upper_matrix : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FooTest() {
    mat = new upper_matrix(4);
  }

  ~FooTest() override {
    delete mat;
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


TEST(upper_matrix, get_set) {
	mat.set(1,2,1);
	ASSERT_EQ(mat.get(1,2), 1);
}

TEST(upper_matrix, diagonal_zero) {
	ASSERT_EQ(mat.get(1,1), 0);
}

TEST(upper_matrix, diagonal_protection) {
	mat.set(1,1,1);
	ASSERT_EQ(mat.get(1,1), 0);
}

TEST(upper_matrix, reflection) {
	mat.set(1,2,1);
	ASSERT_EQ(mat.get(2,1), 1);
}

TEST(upper_matrix, full_matrix) {
	mat.set(0,1,1);
	mat.set(0,2,2);
	mat.set(0,3,3);
	mat.set(1,2,4);
	mat.set(1,3,5);
	mat.set(2,3,6);

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

class CSC_small : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FooTest() {
  	small = new CSC(4,4);
  }

  ~FooTest() override {
    delete small;
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  	small.create_index(0, 2);
	small.create(0, 1, 11);
	small.create(0, 3, 5);
	small.create_index(1, 2);
	small.create(1, 0, 11);
	small.create(1, 2, 5);
	small.create_index(2, 2);
	small.create(2, 1, 5);
	small.create(2, 3, 5);
	small.create_index(3, 2);
	small.create(3, 0, 5);
	small.create(3, 2, 5);
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST(CSC_small, setup) {
}

TEST(CSC_small, basic_insert) {
	EXPECT_EQ(small.get(0, 1), 11);
	EXPECT_EQ(small.get(0, 3), 5);
}

TEST(CSC_small, get_self) {
	EXPECT_EQ(small.get(0, 0), 0);
}

TEST(CSC_small, get_inexistent) {
	EXPECT_EQ(small.get(0, 2), -1);
}

TEST(CSC_small, full_insert) {
	EXPECT_EQ(small.get(0, 1), 11);
	EXPECT_EQ(small.get(0, 3), 5);
	EXPECT_EQ(small.get(1, 0), 11);
	EXPECT_EQ(small.get(1, 2), 5);
	EXPECT_EQ(small.get(2, 1), 5);
	EXPECT_EQ(small.get(2, 3), 5);
	EXPECT_EQ(small.get(3, 0), 5);
	EXPECT_EQ(small.get(3, 2), 5);
}

TEST(CSC_small, set) {
	small.set(0, 1, 22);
	EXPECT_EQ(small.get(0, 1), 22);
}

TEST(CSC_small, set_inexistent) {
	small.set(0, 2, 22);
	EXPECT_EQ(small.get(0, 2), -1);

	small.set(0, 0, 25);
	EXPECT_EQ(small.get(0, 0), 0)
}

TEST(CSC_small, indexes_creation_first) {
	small = new CSC(4,4);
  	small.create_index(0, 2);
	small.create_index(1, 2);
	small.create_index(2, 2);
	small.create_index(3, 2);
	small.create(0, 1, 11);
	small.create(0, 3, 5);
	small.create(1, 0, 11);
	small.create(1, 2, 5);
	small.create(2, 1, 5);
	small.create(2, 3, 5);
	small.create(3, 0, 5);
	small.create(3, 2, 5);

	EXPECT_EQ(small.get(0, 1), 11);
	EXPECT_EQ(small.get(0, 3), 5);
	EXPECT_EQ(small.get(1, 0), 11);
	EXPECT_EQ(small.get(1, 2), 5);
	EXPECT_EQ(small.get(2, 1), 5);
	EXPECT_EQ(small.get(2, 3), 5);
	EXPECT_EQ(small.get(3, 0), 5);
	EXPECT_EQ(small.get(3, 2), 5);

}

TEST(CSC_small, creations_out_of_order) {
	small = new CSC(4,4);
  	small.create_index(0, 2);
	small.create_index(1, 2);
	small.create_index(2, 2);
	small.create_index(3, 2);
	small.create(1, 0, 11);
	small.create(2, 3, 5);
	small.create(2, 1, 5);
	small.create(3, 0, 5);
	small.create(1, 2, 5);
	small.create(0, 1, 11);
	small.create(0, 3, 5);
	small.create(3, 2, 5);

	EXPECT_EQ(small.get(0, 1), 11);
	EXPECT_EQ(small.get(0, 3), 5);
	EXPECT_EQ(small.get(1, 0), 11);
	EXPECT_EQ(small.get(1, 2), 5);
	EXPECT_EQ(small.get(2, 1), 5);
	EXPECT_EQ(small.get(2, 3), 5);
	EXPECT_EQ(small.get(3, 0), 5);
	EXPECT_EQ(small.get(3, 2), 5);
}

TEST(CSC_small, distance_self) {
	EXPECT_EQ(small.distance(0, 0), 0);
}

TEST(CSC_small, distance_basic) {
	EXPECT_EQ(small.distance(0, 1), 11);
	EXPECT_EQ(small.distance(0, 2), 10);
	EXPECT_EQ(small.distance(0, 3), 5);
}

TEST(CSC_small, distance_complete) {
	EXPECT_EQ(small.distance(0, 2), 10);
	EXPECT_EQ(small.distance(1, 0), 11);
	EXPECT_EQ(small.distance(1, 2), 5);
	EXPECT_EQ(small.distance(1, 3), 10);
	EXPECT_EQ(small.distance(2, 0), 10);
	EXPECT_EQ(small.distance(2, 1), 5);
	EXPECT_EQ(small.distance(2, 3), 5);
	EXPECT_EQ(small.distance(3, 0), 5);
	EXPECT_EQ(small.distance(3, 1), 10);
	EXPECT_EQ(small.distance(3, 2), 5);
}

TEST(CSC_small, path_simple) {
	std::list<const node*> path = small.path_find(3, 0);
	auto it = path.begin();
	ASSERT_EQ(path.size(), 2);
	EXPECT_EQ(it->index, 3);
	it++;
	EXPECT_EQ(it->index, 2);
}

TEST(CSC_small, path_self) {
	std::list<const node*> path = small.path_find(0, 0);
	ASSERT_EQ(path.size(), 0);
}

TEST(CSC_small, hops_self) {
	EXPECT_EQ(small.hops(0, 0), 0);
}

TEST(CSC_small, hops_simple) {
	EXPECT_EQ(small.hops(0, 1), 1);
	EXPECT_EQ(small.hops(0, 2), 2);
	EXPECT_EQ(small.hops(2, 0), 2);

}


class CSC_big : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FooTest() {
  	big = new CSC(12,12);
  }

  ~FooTest() override {
    delete big;
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  	big.create_index(0, 4);
	big.create(0, 1, 16);
	big.create(0, 3, 5);
	big.create(0, 4, 1);
	big.create(0, 5, 1);

	big.create_index(1, 4);
	big.create(1, 0, 16);
	big.create(1, 2, 5);
	big.create(1, 6, 1);
	big.create(1, 7, 1);

	big.create_index(2, 4);
	big.create(2, 1, 5);
	big.create(2, 3, 5);
	big.create(2, 8, 1);
	big.create(2, 9, 1);

	big.create_index(3, 4);
	big.create(3, 0, 5);
	big.create(3, 2, 5);
	big.create(3, 10, 1);
	big.create(3, 11, 1);

	for (int i = 0; i < 8; i+2) {
		big.create_index(i+4, 1);
		big.create(i+4, i, 1);

		big.create_index(i+5, 1);
		big.create(i+5, i, 1);
	}

  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST(CSC_big, setup) {
}

TEST(CSC_big, full_insert) {
	EXPECT_EQ(big.get(0, 1), 16);
	EXPECT_EQ(big.get(0, 3), 5);
	EXPECT_EQ(big.get(1, 0), 16);
	EXPECT_EQ(big.get(1, 2), 5);
	EXPECT_EQ(big.get(2, 1), 5);
	EXPECT_EQ(big.get(2, 3), 5);
	EXPECT_EQ(big.get(3, 0), 5);
	EXPECT_EQ(big.get(3, 2), 5);

	EXPECT_EQ(big.get(0, 4), 1);
	EXPECT_EQ(big.get(0, 5), 1);

	EXPECT_EQ(big.get(1, 6), 1);
	EXPECT_EQ(big.get(1, 7), 1);

	EXPECT_EQ(big.get(2, 8), 1);
	EXPECT_EQ(big.get(2, 9), 1);

	EXPECT_EQ(big.get(3, 10), 1);
	EXPECT_EQ(big.get(3, 11), 1);

}

TEST(CSC_big, distance_basic) {
	EXPECT_EQ(big.distance(0, 5), 1);
	EXPECT_EQ(big.distance(9, 2), 1);
	EXPECT_EQ(big.distance(2, 3), 1);
}

TEST(CSC_big, distance_complete) {
	EXPECT_EQ(big.distance(0, 2), 10);
	EXPECT_EQ(big.distance(0, 7), 16);
	EXPECT_EQ(big.distance(0, 8), 11);
	EXPECT_EQ(big.distance(0, 9), 11);
	EXPECT_EQ(big.distance(0, 10), 6);

	EXPECT_EQ(big.distance(1, 0), 15);
	EXPECT_EQ(big.distance(1, 2), 5);
	EXPECT_EQ(big.distance(1, 3), 10);
	EXPECT_EQ(big.distance(1, 4), 16);
	EXPECT_EQ(big.distance(1, 9), 6);

	EXPECT_EQ(big.distance(2, 0), 10);
	EXPECT_EQ(big.distance(2, 1), 5);
	EXPECT_EQ(big.distance(2, 3), 5);
	EXPECT_EQ(big.distance(2, 5), 11);
	EXPECT_EQ(big.distance(2, 11), 6);

	EXPECT_EQ(big.distance(3, 0), 5);
	EXPECT_EQ(big.distance(3, 1), 10);
	EXPECT_EQ(big.distance(3, 2), 5);
	EXPECT_EQ(big.distance(3, 8), 6);
	EXPECT_EQ(big.distance(3, 4), 6);

	EXPECT_EQ(big.distance(4, 9), 12);
	EXPECT_EQ(big.distance(5, 10), 7);
	EXPECT_EQ(big.distance(6, 11), 12);
	EXPECT_EQ(big.distance(7, 4), 17);
	EXPECT_EQ(big.distance(8, 2), 1);
	EXPECT_EQ(big.distance(9, 6), 7);
	EXPECT_EQ(big.distance(10, 8), 7);
	EXPECT_EQ(big.distance(11, 5), 7);
}

TEST(CSC_big, path_simple) {
	std::list<const node*> path = big.path_find(4, 5);
	auto it = path.begin();
	ASSERT_EQ(path.size(), 2);
	EXPECT_EQ(it->index, 0);
	it++;
	EXPECT_EQ(it->index, 5);
}

TEST(CSC_big, path_long) {
	std::list<const node*> path = big.path_find(4, 7);
	ASSERT_EQ(path.size(), 5);
	auto it = path.begin();
	EXPECT_EQ(it->index, 0);
	it++;
	EXPECT_EQ(it->index, 3);
	it++;
	EXPECT_EQ(it->index, 2);
	it++;
	EXPECT_EQ(it->index, 1);
	it++;
	EXPECT_EQ(it->index, 8);
	EXPECT_EQ(it->ncost, 17);
}

TEST(CSC_big, hops) {
	EXPECT_EQ(big.hops(0, 1), 1);
	EXPECT_EQ(big.hops(0, 2), 2);
	EXPECT_EQ(big.hops(2, 0), 2);
	EXPECT_EQ(big.hops(4, 6), 3);
	EXPECT_EQ(big.hops(2, 11), 3);
	EXPECT_EQ(big.hops(8, 1), 3);
	EXPECT_EQ(big.hops(9, 4), 4);
	EXPECT_EQ(big.hops(11, 10), 2);

}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

