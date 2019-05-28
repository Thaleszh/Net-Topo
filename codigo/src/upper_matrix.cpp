/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <upper_matrix.h>
#include <stdio.h> //temporary for debug

upper_matrix::upper_matrix(int size) : _size(size) {
	int cells = size * (size - 1) / 2;
	vec.resize(cells, -1);
}	

// insert at position [i][j]
void upper_matrix::set(int i, int j, int val) {
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
int upper_matrix::get(int i, int j) {
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