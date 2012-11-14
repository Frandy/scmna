/*
 * superNodeSet.h
 *
 *  Created on: Oct 23, 2012
 *      Author: chjd
 */

#ifndef SUPERNODESET_H_
#define SUPERNODESET_H_

#include <set>
#include <vector>
#include <cassert>
using std::set;
using std::vector;

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;

class SuperNodeSet {
public:
	SuperNodeSet()=default;
	SuperNodeSet(int N);
	void Sup(int p, int n);
	 void NodeMap();
	void NodeMapReindex();
	int Node(int m);
	int size();

	void Print(ostream& out=cout);

protected:
	set<set<int>>::iterator Find(int m);
protected:
	int N;
	set<set<int>> nodeSet;
	vector<int> nodeMap;
};


#endif /* SUPERNODESET_H_ */
