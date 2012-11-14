/*
 * circuit.cpp
 *
 *  Created on: Oct 29, 2012
 *      Author: chjd
 */

#include "circuit.h"

bool Circuit::AddElem(Elem* elem)
{
	auto ok = elemMap.insert(make_pair(elem->Name(), elem));
	if (ok.second)
		elemList.push_back(elem);
	return ok.second;
}

int Circuit::AddNode(string name)
{
	int N = nodeMap.size();
	auto ok = nodeMap.insert(make_pair(name, N));
	if (ok.second)
		return N;
	else
		return ok.first->second;
}

bool Circuit::AddTask(Task* task)
{
	taskList.push_back(task);
	return true;
}

Task* Circuit::GetLastTaskByType(TaskType tp)
{
	for (auto it = taskList.rbegin(), et = taskList.rend(); it != et; it++)
	{
		if ((*it)->Type() == tp)
			return (*it);
	}
	return nullptr;
}

void Circuit::Print(ostream& out)
{
	out << "elements:" << elemList.size() << endl;
	for (auto it = elemList.begin(), et = elemList.end(); it != et; it++)
	{
		(*it)->Print(out);
	}
	out << "tasks:" << taskList.size() << endl;
	for (auto it = taskList.begin(), et = taskList.end(); it != et; it++)
	{
		(*it)->Print(out);
	}
	out << "print circuit done." << endl;
}

bool Circuit::GetScFlag() const
{
	return scflag;
}

void Circuit::SetScFlag(bool sc)
{
	scflag = sc;
}

// for switched-capacitor circuit
void Circuit::SetClk(double clk)
{
	clkfreq = clk;
}

double Circuit::GetClk() const
{
	return clkfreq;
}

int Circuit::NodeChangePhase(int p,int N)
{
	int t = p!=0?(p+N):0;
	return t;
}

int Circuit::Size() const
{
	if(scflag)
		return 2*nodeMap.size()-1;
	else
		return nodeMap.size();
}

// add phase, change it to phase 2, return phase 1. ?


void Circuit::PostProcessForSC()
{
	auto it = elemList.begin();
	auto L = elemList.size();
	auto N = nodeMap.size()-1;
	for(auto i=0;i<L;i++,it++)
	{
		if((*it)->Type()==ElemType::VSRC)
		{
			ElemVsrc* elem = ((ElemVsrc*)(*it))->AddPhase(N);
			if(scinout==3 || scinout==4) // veo vee, phase 2 input valid
				elem->CValue(Cplx(0,0));
			else
				((ElemVsrc*)(*it))->CValue(Cplx(0,0));
			elemList.insert(it,elem);
			// one of the voltage source should be shorted, set its cvalue zero is enough ?
		}
		else if((*it)->Type()==ElemType::SWITCH)
		{
			ElemSwitch* elem = (ElemSwitch*)(*it);
			if(elem->Fault()==FaultType::SHORT)
			{
				clog << "switch short:\n\t";
				(*it)->Print(clog);
				ElemSwitch* elem = ((ElemSwitch*)(*it))->AddPhase(N);
				elemList.insert(it,elem);
			}
			else if(elem->Fault()==FaultType::OPEN)
			{
				elemList.erase(it);
				it--;
			}
			else{
				// no fault
				elem->ChangePhase(elem->Phase(),N);
			}
		}
		else if((*it)->Type()==ElemType::CAP)
		{
			// C -> four symbols
			ElemCap* elem = (ElemCap*)(*it);
			string name = elem->Name();
			int p = elem->NodeP();
			int n = elem->NodeN();
			int pp = p!=0?(p+N):0;
			int nn = n!=0?(n+N):0;
			double v = elem->Value();
			ElemVCCS* g1 = new ElemVCCS("rc_"+name+"_1",p,n,p,n,v);
			ElemVCCS* g2 = new ElemVCCS("rc_"+name+"_2",pp,nn,pp,nn,v);
			ElemGsc* g3 = new ElemGsc("gc_"+name+"_1",p,n,nn,pp,v);
			ElemGsc* g4 = new ElemGsc("gc_"+name+"_2",pp,nn,n,p,v);
			elemList.insert(it,g1);
			elemList.insert(it,g2);
			elemList.insert(it,g3);
			elemList.insert(it,g4);
			delete (ElemCap*) elem;
			elemList.erase(it);
			it--;
		}
		else
		{
			// AddPhase, add elem in the second phase, also change the current phase
			Elem* elem = (*it)->AddPhase(N);
			elemList.insert(it,elem);
		}
	}
	// set out
	if(scinout==2 || scinout==4)
	{
		TaskAC* task =(TaskAC*) GetLastTaskByType(TaskType::AC);
		for(auto it=task->printTask.volts.begin(),et=task->printTask.volts.end();it!=et;it++)
		{
/*
			if(it->second.first!=0)
				it->second.first += N;
			if(it->second.second!=0)
				it->second.second += N;
*/
			pair<int,int>* node = &(it->second);
			if (node->first != 0)
				node->first += N;
			if (node->second != 0)
				node->second += N;
		}
	}
}

void Circuit::SetScInOut(int i)
{
	scinout = i;
}
