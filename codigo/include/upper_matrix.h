/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <stdlib.h>
#include <vector>
#include <stdio.h> //temporary for debug

#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace std;


class upper_matrix {
 public:
	upper_matrix() {}

 	~upper_matrix() {}
	// insert at position [i][j]
	virtual void set(int i, int j, int val) = 0;

	// get value at position [i][j]
	virtual int get(int i, int j) = 0;
};

class upper_matrix_real : public upper_matrix {
 protected:

	std::vector<int> vec; 	// matrix itself in a vector
	int _size; 	// size of structure
 public:

 	// serialization header
	template<class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(_size), CEREAL_NVP(vec));
	}

	upper_matrix_real() {}

	upper_matrix_real(int size) {
		_size = size;
		int cells = size * (size - 1) / 2;
		// printf("Creating upper_matrix with %d cells \n", cells);
		vec.resize(cells, -1);
	}

	// // needed for serialization
	// virtual upper_matrix(int size, std::vector<int> vector) = 0;	

	upper_matrix_real(int size, std::vector<int> vector) : _size(size), vec(vector){}

	~upper_matrix_real() {}

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
			// if (id > cells) return;
			// if (id < 0) return;
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
			// if (id > cells) return -1;
			// if (id < 0) return -1;
			return vec[id];
		}
		return 0;
	}
};

class upper_matrix_dummy : public upper_matrix {
public:

	template<class Archive>
	void serialize(Archive & archive) {
	}

	upper_matrix_dummy() {
	}

	upper_matrix_dummy(int size, std::vector<int> vector) {
	}

	~upper_matrix_dummy() {}

	void set(int i, int j, int val) {}

	int get(int i, int j) {
		return -1;
	}
};

CEREAL_REGISTER_TYPE(upper_matrix_dummy);
CEREAL_REGISTER_POLYMORPHIC_RELATION(upper_matrix, upper_matrix_dummy);

CEREAL_REGISTER_TYPE(upper_matrix_real);
CEREAL_REGISTER_POLYMORPHIC_RELATION(upper_matrix, upper_matrix_real);
