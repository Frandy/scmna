/*
 * circuit.h
 *
 *  Created on: Oct 15, 2012
 *      Author: chjd
 */

#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <list>
using std::list;
#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;

#include <cstddef>

#include "elem.h"
#include "task.h"

class Circuit
{
public:
	Circuit()=default;

	bool AddElem(Elem* elem);

	int AddNode(string name);

	bool AddTask(Task* task);

	Task* GetLastTaskByType(TaskType tp);

	void Print(ostream& out=cout);

// for switched-capacitor circuit
	bool GetScFlag() const;
	void SetScFlag(bool sc);
	void SetClk(double clk);
	double GetClk() const;
	void PostProcessForSC();
	void SetScInOut(int i);
	int NodeChangePhase(int p,int N);
	int Size() const;

public:
	list<Elem*> elemList;
	unordered_map<string, Elem*> elemMap;
	unordered_map<string, int> nodeMap ={{ "0", 0 } };
	vector<Task*> taskList;

	int size;
	double clkfreq;
	bool scflag;
	int scinout;
};

#endif /* CIRCUIT_H_ */
