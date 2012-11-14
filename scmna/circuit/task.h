/*
 * task.h
 *
 *  Created on: Oct 15, 2012
 *      Author: chjd
 */

#ifndef TASK_H_
#define TASK_H_

#include <utility>
using std::pair;
using std::make_pair;
#include <iostream>
using std::ostream;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <cmath>
using std::pow;

enum class TaskType
{
	DC, AC, PRINT
};

ostream& operator <<(ostream& out, TaskType& tp);

enum class SpType
{
	LIN, DEC
};

ostream& operator <<(ostream& out, SpType& tp);

enum class PrintType
{
	NONE,MAG, DB, PHASE, REAL, IMAG, MP, RI, DBP
};

PrintType getPrintType(string tag);

ostream& operator <<(ostream& out, PrintType& tp);

enum class VIType
{
	V, I
};

ostream& operator <<(ostream& out, VIType& tp);

/* ------------------ class Task ----------------- */

class Task
{
public:
	Task()=default;
	Task(TaskType tp);
	TaskType Type() const;
	void Type(TaskType tp);
	virtual void Print(ostream& out=cout);
protected:
	TaskType type;
};

/* ------------------ class TaskPrint ----------------- */

class TaskPrint: public Task
{
public:
	TaskPrint();
	void AddPrint(PrintType tp, int np, int nn = 0);
	void AddPrint(PrintType tp, string name);
	void Print(ostream& out=cout);
public:
	vector<pair<PrintType, pair<int, int> > > volts;
	vector<pair<PrintType, string> > curts;
};

/* ------------------ class TaskAC ----------------- */

class TaskAC: public Task
{
public:
	TaskAC()=default;
	TaskAC(SpType tp, int n, double s, double e);
	void GenFreqArray(vector<double>& freqArray);
	void AddPrintTask(PrintType& tp, int np, int nn = 0);
	void AddPrintTask(PrintType& tp, string name);

	void Print(ostream& out = cout);

	// decide the output port or edge for ac analysis, only needed by symbolic
	//void TheOutput();
private:
	void GenLinSamples(vector<double>& freqArray);
	void GenDecSmaples(vector<double>& freqArray);

private:
	SpType sptype;
	int num;
	double sf;
	double ef;
public:
	TaskPrint printTask;
};

#endif /* TASK_H_ */
