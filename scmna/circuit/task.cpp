/*
 * task.cpp
 *
 *  Created on: Oct 19, 2012
 *      Author: chjd
 */

/*
 #include <unordered_map>
 using std::unordered_map;
 */

#include "task.h"

PrintType getPrintType(string tag)
{
	//static unordered_map<string,PrintType> typeMap = {{"m",PrintType::MAG}};
	if (tag == "m")
		return PrintType::MAG;
	else if (tag == "p")
		return PrintType::PHASE;
	else if (tag == "db")
		return PrintType::DB;
	else if (tag == "mp")
		return PrintType::MP;
	else if (tag == "dbp")
		return PrintType::DBP;
	else if (tag == "r")
		return PrintType::REAL;
	else if (tag == "i")
		return PrintType::IMAG;
	else if (tag == "ri")
		return PrintType::RI;
	else
		return PrintType::MP;
}

ostream& operator <<(ostream& out, TaskType& tp)
{
	switch (tp)
	{
	case TaskType::AC:
		out << "AC";
		break;
	case TaskType::DC:
		out << "DC";
		break;
	case TaskType::PRINT:
		out << "PRINT";
		break;
	default:
		out << "UNKNOWN";
		break;
	}
	return out;
}

ostream& operator <<(ostream& out, SpType& tp)
{
	switch (tp)
	{
	case SpType::LIN:
		out << "LIN";
		break;
	case SpType::DEC:
		out << "DEC";
		break;
	default:
		out << "UNKNOWN";
		break;
	}
	return out;
}

ostream& operator <<(ostream& out, PrintType& tp)
{
	switch (tp)
	{
	case PrintType::MP:
		out << "MAG_PHASE";
		break;
	case PrintType::DBP:
		out << "DBMAG_PHASE";
		break;
	case PrintType::RI:
		out << "REAL_IMAG";
		break;
	default:
		;
	}
	return out;
}

ostream& operator <<(ostream& out, VIType& tp)
{
	switch (tp)
	{
	case VIType::V:
		out << "VOLTAGE";
		break;
	case VIType::I:
		out << "CURRENT";
		break;
	default:
		;
	}
	return out;
}

/* ------------------ class Task ----------------- */
Task::Task(TaskType tp) :
	type(tp)
{
}
TaskType Task::Type() const
{
	return type;
}
void Task::Type(TaskType tp)
{
	type = tp;
}
void Task::Print(ostream& out)
{
	out << "task: " << type << endl;
}

/* ------------------ class TaskPrint ----------------- */

TaskPrint::TaskPrint() :
	Task(TaskType::PRINT)
{
}
void TaskPrint::AddPrint(PrintType tp, int np, int nn)
{
	volts.push_back(make_pair(tp, make_pair(np, nn)));
}
void TaskPrint::AddPrint(PrintType tp, string name)
{
	curts.push_back(make_pair(tp, name));
}
void TaskPrint::Print(ostream& out)
{
	if (!volts.empty() || !curts.empty())
		out << "task:" << type << endl;
	if (!volts.empty())
	{
		out << "voltage:" << endl;
		for (auto it = volts.begin(), et = volts.end(); it != et; it++)
		{
			out << "\t" << (it->first) << "\t" << (it->second.first) << " "
					<< (it->second.second) << endl;
		}
	}
	if (!curts.empty())
	{
		out << "current:" << endl;
		for (auto it = curts.begin(), et = curts.end(); it != et; it++)
		{
			out << "\t" << (it->first) << "\t" << (it->second) << endl;
		}
	}
}

/* ------------------ class TaskAC ----------------- */

TaskAC::TaskAC(SpType tp, int n, double s, double e) :
	Task(TaskType::AC), sptype(tp), num(n), sf(s), ef(e)
{
}
void TaskAC::GenFreqArray(vector<double>& freqArray)
{
	if (sptype == SpType::LIN)
		GenLinSamples(freqArray);
	else
		GenDecSmaples(freqArray);
}
void TaskAC::AddPrintTask(PrintType& tp, int np, int nn)
{
	printTask.AddPrint(tp, np, nn);
}
void TaskAC::AddPrintTask(PrintType& tp, string name)
{
	printTask.AddPrint(tp, name);
}

void TaskAC::Print(ostream& out)
{
	out << "task: " << type << "\t sptype: " << sptype << "\t points: " << num
			<< "\t start: " << sf << "\t end: " << ef << endl;
	printTask.Print(out);
}

void TaskAC::GenLinSamples(vector<double>& freqArray)
{
	double d = (ef - sf) / (num - 1);
	double f = sf;
	freqArray.push_back(f);
	for (int i = 0; i < num - 2; i++)
	{
		f += d;
		freqArray.push_back(f);
	}
	freqArray.push_back(ef);
}
void TaskAC::GenDecSmaples(vector<double>& freqArray)
{
	double d = pow(10, 1.0 / num);
	double f = sf;
	freqArray.push_back(f);
	//for (int i = 0; i < num - 2; i++)
	for(;f<ef;)
	{
		f *= d;
		freqArray.push_back(f);
	}
	//freqArray.push_back(ef);
}
