/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <stdlib.h>

class upper_matrix {
private:
	int[] vec; 	// matrix itself in a vector
	int _size; 	// size of structure

public:
	upper_matrix(int size) : _size(size) {
		cells = size * (size - 1) / 2;
		vec = (int*) malloc(sizeof(int) * cells);
		for (int i = 0 ; i < cells; i++) {
			vec[i] = -1;
		}
	}

	~upper_matrix() {
		free(vec);
	}

	// insert at position [i][j]
	void set(int i, int j, int val) {
		//if in middle there is no way to change
		if (i != j) {
			int id = (_size - 1) * i + j
			if ( i > j ){
				id -= (i*(i+1))/2;
			} else {
				// simetric case
				id -= (j*(j+1))/2
			}
			vec[id] = val;
		}
	}

	// get value at position [i][j]
	int get(int i, int j) {
		if (i != j) {
			int id = (_size - 1) * i + j
			if ( i > j ){
				id -= (i*(i+1))/2;
			} else {
				// simetric case
				id -= (j*(j+1))/2
			}
			return vec[id];
		}
		return 0;
	}