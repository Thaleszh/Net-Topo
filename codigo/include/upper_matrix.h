/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <stdlib.h>
#include <vector>

#include <cereal/types/vector.hpp>


using namespace std;

class upper_matrix {
 private:

	std::vector<int> vec; 	// matrix itself in a vector
	int _size; 	// size of structure
 public:

 	// serialization header
	template<class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(_size), CEREAL_NVP(vec));
	}

	upper_matrix() {}

	upper_matrix(int size);

	// needed for serialization
	upper_matrix(int size, std::vector<int> vector) : _size(size), vec(vector) {}	


	~upper_matrix() {}

	// insert at position [i][j]
	void set(int i, int j, int val);

	// get value at position [i][j]
	int get(int i, int j);
};