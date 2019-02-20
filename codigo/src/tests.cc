
#include "upper_matrix.h"
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

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

