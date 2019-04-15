/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <stdlib.h>
// #include <boost/archive/xml_oarchive.hpp>
// #include <boost/archive/xml_iarchive.hpp>

// #include <boost/serialization/base_object.hpp>
// #include <boost/serialization/utility.hpp>


using namespace std;

class upper_matrix {
 private:
 	// boost serialization
	// friend class boost::serialization::access;
	// template<class Archive>
	// void serialize(Archive & ar, const unsigned int version) {
	// 	ar & _size;
	// 	ar & vec;
	// }

	int* vec; 	// matrix itself in a vector
	int _size; 	// size of structure
 public:
	upper_matrix(int size) : _size(size) {
		int cells = size * (size - 1) / 2;
		vec = new int[cells];
		for (int i = 0 ; i < cells; i++) {
			vec[i] = -1;
		}
	}	

	~upper_matrix() {
		delete[] vec;
	}

	// insert at position [i][j]
	void set(int i, int j, int val) {
		//if in middle there is no way to change
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