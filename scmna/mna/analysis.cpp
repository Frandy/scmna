/*
 * analysis.cpp
 *
 *  Created on: Oct 29, 2012
 *      Author: chjd
 */

#include "analysis.h"
#include "stringenc.h"

#include <iostream>
using std::clog;
using std::endl;

MnaAnalysis::MnaAnalysis(Circuit* ckt)
{
	if(ckt->GetScFlag())
	{
		ckt->PostProcessForSC();
		clog << "for sc circuit, after post process" << endl;
		ckt->Print(clog);
		clog << "node count of the two-phase equivalent circuit: " << endl;
		clog << "\t" << (ckt->Size()) << endl;
	}
	for (auto it = ckt->elemList.begin(), et = ckt->elemList.end(); it != et; it++)
	{
		mnaElemList.push_back((*it)->CvtoMnaElem());
	}
/*	int i = 0;
	for (auto it = mnaElemList.begin(), et = mnaElemList.end(); it != et; it++)
	{
		clog << i << "\t(*it): " << (*it) << endl;
		i++;
	}
*/
	tckt = ckt;
	//	clog << "mna analysis build" << endl;
	colSet = SuperNodeSet(ckt->Size());
	rowSet = SuperNodeSet(ckt->Size());
}

bool MnaAnalysis::Analysis()
{
//		clog << "begin analysis ... " << endl;
	SuperNode();
	clog << "row set:" << endl;
	rowSet.Print(clog);
	clog << "col set:" << endl;
	colSet.Print(clog);

//		clog << "super node done" << endl;
	for (auto it = tckt->taskList.begin(), et = tckt->taskList.end(); it != et; it++)
	{
		if ((*it)->Type() == TaskType::AC)
		{
			if (tckt->GetScFlag())
			{
				ScAcAnalysis((TaskAC*) (*it));
			}
			else
				AcAnalysis((TaskAC*) (*it));
		}
	}
	return true;
}

bool MnaAnalysis::ScAcAnalysis(TaskAC* actask)
{
	clog << "begin analysis for sc circuit... " << endl;
	vector<double> freqArray;
	actask->GenFreqArray(freqArray);
	bool ok = false;
	int N = AcInit();
	double clock = tckt->GetClk();
	MNA<Cplx> mna(N - 1);
	vector<vector<double> > resdata;
	for (auto it = freqArray.begin(), et = freqArray.end(); it != et; it++)
	{
		//Cplx s = Cplx(0, (*it) * 2 * M_PI);
		double alpha = (*it)/clock*M_PI;
		Cplx s = Cplx(cos(alpha),sin(alpha));
		//Cplx s = Cplx(0, (*it));

//		clog << "freq:" << (*it) << "\t" << "s: " << s << endl;

		mna.Clear();
		AcLoad(mna, s);
		ok = mna.Solve();
		if (!ok)
			break;
		vector<double> linedata;
		linedata.push_back((*it));
		CollectResCplx(actask->printTask, mna, linedata);
		resdata.push_back(linedata);
	}
	vector<string> title;
	title.push_back("freq");
	CollectResTitle(actask->printTask, title);
	SaveRes(title, resdata);
	clog << "analysis done." << endl;
	return ok;
}

bool MnaAnalysis::AcAnalysis(TaskAC* actask)
{
	clog << "begin analysis ... " << endl;
	vector<double> freqArray;
	actask->GenFreqArray(freqArray);
	bool ok = false;
	int N = AcInit();
	MNA<Cplx> mna(N - 1);
	vector<vector<double> > resdata;
	for (auto it = freqArray.begin(), et = freqArray.end(); it != et; it++)
	{
		Cplx s = Cplx(0, (*it) * 2 * M_PI);
		//Cplx s = Cplx(0, (*it));
		mna.Clear();
		AcLoad(mna, s);
		ok = mna.Solve();
		if (!ok)
			break;
		vector<double> linedata;
		linedata.push_back((*it));
		CollectResCplx(actask->printTask, mna, linedata);
		resdata.push_back(linedata);
	}
	vector<string> title;
	title.push_back("freq");
	CollectResTitle(actask->printTask, title);
	SaveRes(title, resdata);
	clog << "analysis done." << endl;
	return ok;
}

void MnaAnalysis::SuperNode()
{
//	int i	= 0;
	for (auto it = mnaElemList.begin(), et = mnaElemList.end(); it != et; it++)
	{
//		clog << "sup mna elem: " << i << endl;
//		clog << "(*it):" << (*it) << endl;
		(*it)->Sup(rowSet, colSet);
//		rowSet.Print(clog);
//		colSet.Print(clog);
//		clog << "sup mna elem: " << i << " done." << endl;
//		i++;
	}
//		clog << "sup ok, next reindex node" << endl;
	rowSet.NodeMapReindex();
	colSet.NodeMapReindex();
//		clog << "node reindex ok" << endl;
}

void MnaAnalysis::CollectResCplx(TaskPrint& printTask, MNA<Cplx>& mna,
		vector<double>& res)
{
	//task->printTask->CollectResData(ckt,mna,rowSet,colSet);
	vector<pair<PrintType, pair<int, int> > >* volts = &(printTask.volts);
	if (!volts->empty())
	{
		for (auto it = volts->begin(), et = volts->end(); it != et; it++)
		{
			int p = colSet.Node(it->second.first);
			int n = colSet.Node(it->second.second);
			Cplx vp = mna.GetX(p);
			Cplx vn = mna.GetX(n);
			Cplx value = vp - vn;
			//			clog << value << endl;
			switch (it->first)
			{
			case PrintType::DBP:
			{
				res.push_back(20 * log10(abs(value)));
				res.push_back(180 * (arg(value)) / M_PI);
				break;
			}
			case PrintType::MP:
			{
				res.push_back(abs(value));
				res.push_back(180 * (arg(value)) / M_PI);
				break;
			}
			case PrintType::RI:
			{
				res.push_back(value.real());
				res.push_back(value.imag());
				break;
			}
			default:
				;
			}
		}
	}
}

void MnaAnalysis::CollectResTitle(TaskPrint& printTask, vector<string>& res)
{
	// problem is , this node int is after hash, not the exact node in netlist
	vector<pair<PrintType, pair<int, int> > >* volts = &(printTask.volts);
	if (!volts->empty())
	{
		for (auto it = volts->begin(), et = volts->end(); it != et; it++)
		{
			string port = "(" + toString(it->second.first) + "," + toString(
					it->second.second) + ")";
			switch (it->first)
			{
			case PrintType::DBP:
			{
				res.push_back("VDB" + port);
				res.push_back("VP" + port);
				break;
			}
			case PrintType::MP:
			{
				res.push_back("VM" + port);
				res.push_back("VP" + port);
				break;
			}
			case PrintType::RI:
			{
				res.push_back("VR" + port);
				res.push_back("VI" + port);
				break;
			}
			default:
				;
			}
		}
	}
}

template<class Type>
void MnaAnalysis::SaveVector(ofstream& fp, vector<Type>& data)
{
	for (auto it = data.begin(), et = data.end(); it != et; it++)
	{
		fp << (*it) << "\t";
	}
	fp << endl;
}

void MnaAnalysis::SaveRes(vector<string>& title, vector<vector<double>>& data,
		string filename)
{
	ofstream fp(filename.c_str());
	fp << data.size() << "\t" << title.size() << endl;
	SaveVector(fp, title);
	for (auto it = data.begin(), et = data.end(); it != et; it++)
	{
		SaveVector(fp, (*it));
	}
	fp.close();
}

/* for ac analysis */
int MnaAnalysis::AcInit()
{
	SuperNode();
	int rowk = rowSet.size();
	int colk = colSet.size();
	for (auto it = mnaElemList.begin(), et = mnaElemList.end(); it != et; it++)
		(*it)->BkInc(rowk, colk);
	assert(rowk==colk);
	return rowk;
}

void MnaAnalysis::AcLoad(MNA<Cplx>& mna, Cplx s)
{
	for (auto it = mnaElemList.begin(), et = mnaElemList.end(); it != et; it++)
		(*it)->AcLoad(mna, s, rowSet, colSet);
}
