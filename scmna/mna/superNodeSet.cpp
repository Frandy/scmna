/*
 * superNodeSet.cpp
 *
 *  Created on: Oct 29, 2012
 *      Author: chjd
 */

#include <algorithm>
using std::set_union;

#include "superNodeSet.h"

#include <iostream>
using std::clog;
using std::endl;

SuperNodeSet::SuperNodeSet(int N):N(N)
{
	for (auto i = 0; i < N; i++)
	{
		set<int> t;
		t.insert(i);
		nodeSet.insert(t);
	}
	nodeMap = vector<int> (N);
/*	clog << "node set, after init:" << endl;
	for(auto it=nodeSet.begin(),et=nodeSet.end();it!=et;it++)
	{
		for(auto tit=it->begin(),tet=it->end();tit!=tet;tit++)
			clog << (*tit) << "\t";
		clog << endl;
	}
*/
}
void SuperNodeSet::Sup(int p, int n)
{
	// if p==0 || n==0, may simple
	if (p > n)
		return Sup(n, p);
	set<set<int>>::iterator pit = nodeSet.begin();
	for(auto et=nodeSet.end();pit!=et;pit++)
	{
		if(pit->find(p)!=pit->end())
			break;
	}
	set<set<int>>::iterator nit = nodeSet.begin();
	for(auto et=nodeSet.end();nit!=et;nit++)
	{
		if(nit->find(n)!=nit->end())
			break;
	}
	// p & n in the same set
	if (pit == nit)
		return;
	// merge n to p
	set<int> temp;
	set_union(pit->begin(),pit->end(),nit->begin(),nit->end(),inserter(temp,temp.begin()));
	nodeSet.erase(pit);
	nodeSet.erase(nit);
	nodeSet.insert(temp);
}
void SuperNodeSet::NodeMap()
{
	int index = 0;
	for (auto nit = nodeSet.begin(), net = nodeSet.end(); nit != net; index++, nit++)
	{
		auto bit = nit->begin();
		for (auto it = nit->begin(), et = nit->end(); it != et; it++)
			nodeMap[(*it)] = (*bit);
	}
	assert(nodeMap[0] == 0);//,"after node sup, first node must still be the 0"
}
void SuperNodeSet::NodeMapReindex()
{
//	clog << "begin node reindex" << endl;
//	clog << "node set size: " << nodeSet.size() << endl;
	 int index = 0;
	for (auto nit = nodeSet.begin(), net = nodeSet.end(); nit != net; index++, nit++)
	{
		for (auto it = nit->begin(), et = nit->end(); it != et; it++)
		{
//			clog << "node: " << (*it) << "\t index: " << index << endl;
			nodeMap[(*it)] = index;
		}
	}
	assert(nodeMap[0] == 0);//"after node sup, first node must still be the 0"
//	clog << "node reindex done" << endl;
}

int SuperNodeSet::Node(int m)
{
	return nodeMap[m];
}

int SuperNodeSet::size()
{
	return nodeSet.size();
}

set<set<int>>::iterator SuperNodeSet::Find(int m)
{
	auto it = nodeSet.begin();
	for (auto et = nodeSet.end(); it != et; it++)
	{
		if (it->end() != it->find(m))
		{
			return it;
		}
	}
	return it;
}

void SuperNodeSet::Print(ostream& out)
{
// print the set
	int i = 0;
	out << "set map:" << endl;
	for(auto pit=nodeSet.begin(),pet=nodeSet.end();pit!=pet;pit++,i++)
	{
		out << "i:" << i << endl;
		for(auto nit=pit->begin(),net=pit->end();nit!=net;nit++)
			out << "\t" << (*nit);
		out << endl;
	}
/*
	out << "node map:" << endl;
	for(i=0;i<nodeMap.size();i++)
	{
		out << i << " -> " << nodeMap[i] << endl;
	}
*/
}
