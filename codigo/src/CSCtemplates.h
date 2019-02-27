/*	Author: Thales A. Zirbel Hubner <thaleszh@gmail.com>
 *	Created: 04/12/2018
 */

#include <stdlib.h>

template <typename T>
class csc {
private:
	// note: current version is static, cannot be incremented
	int[] index; 	// indexing vector
	int[] line;		// line especification
	T[] value;		// values list
	int _nlinks; 	// number of conections
	int _nodes;		// number of nodes in list

public:
	
	template <typename T>
	csc<T>::csc(int nlinks, int nodes) : _nlinks(nlinks), _nodes(nodes) {

		line, value = (int*) malloc(sizeof(T) * _nlinks);
		index = (int*) malloc(sizeof(int) * (_nodes + 1) );
		for (int i = 0 ; i < _nlinks; i++) {
			value[i] = NULL;
			line[i] = -1;
		}
		index[0] = 0;
		for (int i = 1 ; i < _nodes; i++) {
			index[i] = -1;
		}
	}

	~csc() {
		free(index);
		free(line);
		free(value);
	}

	// number of nodes in structure
	int n_nodes() {
		return _nodes;
	}

	// number of links in structure
	int n_links() {
		return _nlinks;
	}

	// creates index for the node j, being nlinks the number of links it has
	void create_index(int j, int nlinks) {
		// does not verify if j < _nodes
		int k = 0;
		if (j > 0) k = index[j];
		index[j+1] = k + nlinks; // gotta check if it~s not one more or lower
	}

	// creates line position and sets, it assumes there is no limit
	void create(int i, int j, int val) {
		// does not verify if j,i < _nodes

		// just storing the upper matrix, so if i < j, we swap assuming symetry
		if(i > j) {
			temp = i;
			i = j;
			j = temp;
		}
		pos = index[j];
		int k = pos;
		while (line[k] != -1) k++; // not checking end of val list, assuming it was right from the start
		line[k] = i;
		value[k] = val;
	}

	// insert at position [i][j]
	template <typename T>
	void csc<T>::set(int i, int j, T val) {
		// just storing the upper matrix, so if i < j, we swap assuming symetry
		if(i > j) {
			temp = i;
			i = j;
			j = temp;
		}
		// store time
		pos = index[j];
		limit = index[j+1];
		int k = pos;
		while (line[k] != i && k < limit) k++;
		if (k = limit) return // cannot change what doesnt exist
		value[k] = val;

	}

	// get value at position [i][j]
	template <typename T>
	T csc<T>::get(int i, int j) {
		// does not verify if j,i < _nodes, but only returns -1
		// just storing the upper matrix, so if i < j, we swap assuming symetry
		if(i > j) {
			temp = i;
			i = j;
			j = temp;
		}
		// obtaining value
		pos = index[j]; // getting index of node's conections
		limit = index[j+1]; // checking boundary
		int k = pos; // starts at first element of list
		while (line[k] != i && k < limit) k++;
		if (k = limit) return -1 // place doesn´t exist!
		return value[k];
	}