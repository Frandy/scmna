/*
 * elem.h
 *
 *  Created on: Oct 13, 2012
 *      Author: chjd
 */

#ifndef ELEM_H_
#define ELEM_H_

#include <string>
#include <iostream>
using std::string;
using std::ostream;
using std::cout;
using std::endl;

#include "cplx.h"

#include "mnaElem.h"


enum class ElemType
{
	RES, // resistor
	CAP, // capacitor
	IND, // inductor
	VSRC, // voltage source
	ISRC, // current source
	VCVS, // e element
	CCVS, // f element
	VCCS, // g element
	CCCS, // h element
	DIODE, // diode
	MOS, // mos fet
	OPAMP, // opamp, ideal, E=inf=G
	SWITCH, // switch
	GSC, // delayed VCCS
	X,
// sub circuit
};

// fault type
enum class FaultType
{
	NONE, SHORT, OPEN, FAULT
};

const static double VINF = 1e8;
const static double VZERO = 1e-20;

// extreme value type
enum class ExType
{
	NONE, INF, ZERO
};

double ExValue(ExType tp);
double UnitValue(char unit);

ostream& operator <<(ostream& out, ElemType ty);

/* ------------------ class Elem ----------------- */
// common property, name, type, fault & inf/zero
// common virtual method, print, convert to ManElem, SymbElem.
class Elem
{
public:
	Elem()=default;
	Elem(string nm, ElemType tp);
	virtual ~Elem();
	string Name() const;
	ElemType Type() const;
	FaultType Fault() const;
	ExType InfZero() const;

	void Name(string nm);
	void Type(ElemType tp);
	void Fault(FaultType& tp);
	void InfZero(ExType& tp);

	virtual void Print(ostream& out = cout);

	virtual MnaElem* CvtoMnaElem();

	virtual void ChangePhase(int i,int N);
	virtual Elem* AddPhase(int N);

public:
//	constexpr  VINF = 1e8;
//	constexpr  VZERO = 1e-20;

protected:
	string name;
	ElemType type;
	FaultType fault;
	ExType inf;
};

/* ------------------ class ElemTwoNode ----------------- */
// base class of res, cap, ind, vsrc, iscr, switch, etc.
// elements with two nodes
class ElemTwoNode: public Elem
{
public:
	ElemTwoNode(string nm, ElemType tp, int p, int n);

	int NodeP() const;
	int NodeN() const;

	void NodeP(int p);
	void NodeN(int n);

	void ChangePhase(int i,int N);

protected:
	int nodep;
	int noden;
};

/* ------------------ class ElemRes ----------------- */
// resistor, name, type, nodep, noden, value = resistance, keep agree with netlist
class ElemRes: public ElemTwoNode
{
public:
	ElemRes()=default;
	ElemRes(string nm, int p, int n, double v);
	void Print(ostream& out = cout);
	double Value() const;
	void Value(double v);

	MnaElemRes* CvtoMnaElem();

private:
	double dValue;
};

/* ------------------ class ElemCap ----------------- */
// capacitor, name, type, nodep, noden, value = capacitance
class ElemCap: public ElemTwoNode
{
public:
	ElemCap()=default;
	ElemCap(string nm, int p, int n, double v);
	~ElemCap();
	void Print(ostream& out = cout);
	double Value() const;
	void Value(double v);

	MnaElemCap* CvtoMnaElem();

private:
	double dValue;
};

/* ------------------ class ElemInd ----------------- */
// inductor, name, type, nodep, noden, value = inductance
class ElemInd: public ElemTwoNode
{
public:
	ElemInd()=default;
	ElemInd(string nm, int p, int n, double v);
	void Print(ostream& out = cout);
	double Value() const;
	void Value(double v);

	MnaElemInd* CvtoMnaElem();

private:
	double dValue;
};

/* ------------------ class ElemVsrc ----------------- */
// voltage source, name, type, nodep, noden, value = dc voltage, cvalue = ac voltage
class ElemVsrc: public ElemTwoNode
{
public:
	ElemVsrc()=default;
	ElemVsrc(string nm, int p, int n, double d, Cplx c);
	void Print(ostream& out = cout);
	double Value() const;
	void Value(double v);
	Cplx CValue() const;
	void CValue(Cplx c);

	MnaElemVsrc* CvtoMnaElem();

	ElemVsrc* AddPhase(int N);

private:
	double dValue;
	Cplx cValue;
};

/* ------------------ class ElemIsrc ----------------- */
// current source, name, type, nodep, noden, value = dc voltage, cvalue = ac voltage
class ElemIsrc: public ElemTwoNode
{
public:
	ElemIsrc()=default;
	ElemIsrc(string nm, int p, int n, double d, Cplx c);
	void Print(ostream& out = cout);
	double Value() const;
	void Value(double v);
	Cplx CValue() const;
	void CValue(Cplx c);

	MnaElemIsrc* CvtoMnaElem();

private:
	double dValue;
	Cplx cValue;
};

/* ------------------ class ElemSwitch ----------------- */
// switch. name, type, nodep, noden, value = phase, take integer as phase
class ElemSwitch: public ElemTwoNode
{
public:
	ElemSwitch()=default;
	ElemSwitch(string nm, int p, int n, int v);
	void Print(ostream& out = cout);
	void Phase(int v);
	int Phase() const;

	MnaElemSwitch* CvtoMnaElem();

	void ChangePhase(int i,int N);
	ElemSwitch* AddPhase(int N);

private:
	int dValue;
};

/* ------------------ class ElemTwoPort ----------------- */
// base class of depedent source, opamp,
// elements with four nodes, or two ports
class ElemTwoPort: public Elem
{
public:
	ElemTwoPort(string nm, ElemType tp, int p, int n, int pp, int nn);

	int NodeP() const;
	int NodeN() const;
	int NodePP() const;
	int NodeNN() const;

	void NodeP(int p);
	void NodeN(int n);
	void NodePP(int pp);
	void NodeNN(int nn);
	void ChangePhase(int i,int N);
protected:
	int nodep;
	int noden;
	int nodepp;
	int nodenn;
};

/* ------------------ class ElemTwoPortDS ----------------- */
// base class of depedent source, VCVS, CCCS, VCCS, CCVS
class ElemTwoPortDS: public ElemTwoPort
{
public:
	ElemTwoPortDS(string nm, ElemType tp, int p, int n, int pp, int nn,
			double v);
	double Value() const;
	void Value(double v);
protected:
	double dValue;
};

/* ------------------ class ElemVCVS ----------------- */
// VCVS, name, type, output port, control port, value = E:voltage gain
class ElemVCVS: public ElemTwoPortDS
{
public:
	ElemVCVS()=default;
	ElemVCVS(string nm, int p, int n, int pp, int nn, double v);
	void Print(ostream& out = cout);

	MnaElemVCVS* CvtoMnaElem();
};

/* ------------------ class ElemCCCS ----------------- */
// CCCS, name, type, output port, control port, value = F:current gain
class ElemCCCS: public ElemTwoPortDS
{
public:
	ElemCCCS()=default;
	ElemCCCS(string nm, int p, int n, int pp, int nn, double v);
	void Print(ostream& out = cout);

	MnaElemCCCS* CvtoMnaElem();
};

/* ------------------ class ElemVCCS ----------------- */
// CCCS, name, type, output port, control port, value = G:transconductance
class ElemVCCS: public ElemTwoPortDS
{
public:
	ElemVCCS()=default;
	ElemVCCS(string nm, int p, int n, int pp, int nn, double v);
	void Print(ostream& out = cout);

	MnaElemVCCS* CvtoMnaElem();
};

/* ------------------ class ElemGsc ----------------- */
class ElemGsc : public ElemTwoPortDS
{
public:
	ElemGsc()=default;
	ElemGsc(string nm, int p, int n, int pp, int nn, double v);
	void Print(ostream& out = cout);

	MnaElemGsc* CvtoMnaElem();
};

/* ------------------ class ElemCCVS ----------------- */
// CCVS, name, type, output port, control port, value = H:transresistance
class ElemCCVS: public ElemTwoPortDS
{
public:
	ElemCCVS()=default;
	ElemCCVS(string nm, int p, int n, int pp, int nn, double v);
	void Print(ostream& out = cout);

	MnaElemCCVS* CvtoMnaElem();
};

/* ------------------ class ElemOpamp ----------------- */
// OPAMP, name, type, output port, control port
// if not infinity gain, it should be taken as a VCVS
class ElemOpamp: public ElemTwoPort
{
public:
	ElemOpamp()=default;
	ElemOpamp(string nm, int p, int n, int pp, int nn);
	void Print(ostream& out = cout);

	MnaElemOpamp* CvtoMnaElem();


	ElemOpamp* AddPhase(int N);

	//double Value() const;
	//void Value(double v);
};

#endif /* ELEM_H_ */
