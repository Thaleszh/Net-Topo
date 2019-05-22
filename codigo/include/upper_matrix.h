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

	upper_matrix(int size) : _size(size) {
		int cells = size * (size - 1) / 2;
		vec.resize(cells, -1);
	}	

	// needed for serialization
	upper_matrix(int size, std::vector<int> vector) : _size(size), vec(vector) {}	


	~upper_matrix() {}

	// insert at position [i][j]
	void set(int i, int j, int val) {
		//if in middle there is no way to change. Its 0.
		if (i != j) {
			if (j > i) {
				int temp = j;
				j = i;
				i = temp;
			}
			int id = (j*(2*_size - j - 1))/2 + (i - j -1);
			vec[id] = val;
		}
	}

	// get value at position [i][j]
	int get(int i, int j) {
		if (i != j) {
			if (j > i) {
				int temp = j;
				j = i;
				i = temp;
			}
			int id = (j*(2*_size - j - 1))/2 + (i - j -1);
			return vec[id];
		}
		return 0;
	}
};