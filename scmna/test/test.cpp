/*
 * test.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: chjd
 */

#include "circuit.h"
#include "parser.h"
#include "analysis.h"

#include <fstream>
using std::ofstream;
#include <iostream>
using std::cout;

/*
ofstream errorlog("errorlog.txt");
//ofstream errorlog;
errorlog.rdbuf(cout.rdbuf());
*/

int main(int argc,char** argv){
	string usage = "usage: anadet test.sp";
	if(argc!=2)
	{
		cout << usage << endl;
		return 0;
	}
	string fname = argv[1];
//	string fname = "tsp.txt";
	Circuit* ckt = new Circuit();
	Parser cktparser(ckt);
	if(!cktparser.Run(fname))
		return -1;
	ckt->Print();
	MnaAnalysis ana(ckt);
	ana.Analysis();
	return 0;
}
