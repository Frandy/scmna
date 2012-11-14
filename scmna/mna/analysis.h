/*
 * analysis.h
 *
 *  Created on: Oct 23, 2012
 *      Author: chjd
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <fstream>
using std::ofstream;
#include <cassert>

#include "circuit.h"
#include "mna.h"
#include "superNodeSet.h"

class MnaAnalysis
{
public:
	MnaAnalysis(Circuit* ckt);

	bool Analysis();
	/* for ac analysis */
	int AcInit();
	void AcLoad(MNA<Cplx>& mna, Cplx s);
	bool AcAnalysis(TaskAC* actask);
	bool ScAcAnalysis(TaskAC* actask);

	/* for mna analysis */
	void SuperNode();

	// save result
	void CollectResTitle(TaskPrint& printTask, vector<string>& res);
	template<class Type>
	void SaveVector(ofstream& fp, vector<Type>& data);
	void SaveRes(vector<string>& title, vector<vector<double>>& data,string filename = "ac.txt");

	// collect ac result
	void CollectResCplx(TaskPrint& printTask, MNA<Cplx>& mna,vector<double>& res);

protected:
	Circuit* tckt;
	vector<MnaElem*> mnaElemList;
	SuperNodeSet rowSet;
	SuperNodeSet colSet;
	int nodeCount;
};

#endif /* ANALYSIS_H_ */
