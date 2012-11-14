/*
 * elem.cpp
 *
 *  Created on: Oct 13, 2012
 *      Author: chjd
 */

#include "elem.h"
#include "mnaElem.h"

ostream& operator <<(ostream& out, ElemType tp)
{
	switch (tp)
	{
	case ElemType::RES:
		out << "RES";
		break;
	case ElemType::CAP:
		out << "CAP";
		break;
	case ElemType::IND:
		out << "IND";
		break;
	case ElemType::VSRC:
		out << "VSRC";
		break;
	case ElemType::ISRC:
		out << "ISRC";
		break;
	case ElemType::VCVS:
		out << "VCVS(E)";
		break;
	case ElemType::CCVS:
		out << "CCVS(F)";
		break;
	case ElemType::VCCS:
		out << "VCCS(G)";
		break;
	case ElemType::CCCS:
		out << "CCCS(H)";
		break;
	case ElemType::DIODE:
		out << "DIODE";
		break;
	case ElemType::MOS:
		out << "MOS";
		break;
	case ElemType::SWITCH:
		out << "SWITCH";
		break;
	case ElemType::GSC:
		out << "GSC";
		break;
	case ElemType::OPAMP:
		out << "OPAMP";
		break;
	case ElemType::X:
		out << "SUBCKT(X)";
		break;
	default:
		;
	}
	return out;
}

double ExValue(ExType tp)
{
	switch (tp)
	{
	case ExType::INF:
		return VINF;
	case ExType::ZERO:
		return VZERO;
	default:
		return VZERO;
	}
}

/* the char in this unit map will be checked by matchUnit */
double UnitValue(char unit)
{
	/*	static unordered_map<char, double> unitValueMap = { { 'f', 1e-15 },
	 { 'p', 1e-12 }, { 'n', 1e-9 }, { 'u', 1e-6 }, { 'm', 1e-3 },
	 { 'k',1e3 }, { 'x', 1e6 }, { 'g', 1e9 }};
	 return unitValueMap[unit];
	 */
	switch (unit)
	{
	case 'f':
		return 1.0e-15;
	case 'p':
		return 1.0e-12;
	case 'n':
		return 1.0e-9;
	case 'u':
		return 1.0e-6;
	case 'm':
		return 1.0e-3;
	case 'k':
		return 1.0e3;
	case 'x':
		return 1.0e6;
	case 'g':
		return 1.0e9;
	default:
		return 1.0;
	}
}

/* ------------------ class Elem ----------------- */

Elem::Elem(string nm, ElemType tp) :
	name(nm), type(tp)
{
	fault = FaultType::NONE;
	inf = ExType::NONE;
}

Elem::~Elem()
{

}

string Elem::Name() const
{
	return name;
}
ElemType Elem::Type() const
{
	return type;
}
ExType Elem::InfZero() const
{
	return inf;
}

FaultType Elem::Fault() const
{
	return fault;
}

void Elem::Name(string nm)
{
	name = nm;
}
void Elem::Type(ElemType tp)
{
	type = tp;
}
void Elem::Fault(FaultType& tp)
{
	fault = tp;
}
void Elem::InfZero(ExType& tp)
{
	inf = tp;
}
void Elem::Print(ostream& out)
{
	out << "<name: " << name << "\t type: " << type << ">" << endl;
}

MnaElem* Elem::CvtoMnaElem()
{
	return nullptr;
}

void Elem::ChangePhase(int i,int N)
{
	return;
}
Elem* Elem::AddPhase(int N)
{
	return nullptr;
}

/* ------------------ class ElemTwoNode ----------------- */

ElemTwoNode::ElemTwoNode(string nm, ElemType tp, int p, int n) :
	Elem(nm, tp), nodep(p), noden(n)
{
}
int ElemTwoNode::NodeP() const
{
	return nodep;
}
int ElemTwoNode::NodeN() const
{
	return noden;
}
void ElemTwoNode::NodeP(int p)
{
	nodep = p;
}
void ElemTwoNode::NodeN(int n)
{
	noden = n;
}

void ElemTwoNode::ChangePhase(int i,int N)
{
	if(i==1)
	{
		name = name + "_1";
	}
	else
	{
		name = name + "_2";
		nodep = nodep!=0?(nodep+N):0;
		noden = noden!=0?(noden+N):0;
	}
}


/* ------------------ class ElemRes ----------------- */

ElemRes::ElemRes(string nm, int p, int n, double v) :
	ElemTwoNode(nm, ElemType::RES, p, n), dValue(v)
{
}
void ElemRes::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << "\t R: " << dValue << endl;
}
double ElemRes::Value() const
{
	return dValue;
}
void ElemRes::Value(double v)
{
	dValue = v;
}

MnaElemRes* ElemRes::CvtoMnaElem()
{
	return new MnaElemRes(this);
}


/* ------------------ class ElemCap ----------------- */

ElemCap::ElemCap(string nm, int p, int n, double v) :
	ElemTwoNode(nm, ElemType::CAP, p, n), dValue(v)
{
}

ElemCap::~ElemCap()
{

}

void ElemCap::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << "\t C: " << dValue << endl;
}
double ElemCap::Value() const
{
	return dValue;
}
void ElemCap::Value(double v)
{
	dValue = v;
}

MnaElemCap* ElemCap::CvtoMnaElem()
{
	return new MnaElemCap(this);
}

/* ------------------ class ElemInd ----------------- */
ElemInd::ElemInd(string nm, int p, int n, double v) :
	ElemTwoNode(nm, ElemType::IND, p, n), dValue(v)
{
}
void ElemInd::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << "\t L: " << dValue << endl;
}
double ElemInd::Value() const
{
	return dValue;
}
void ElemInd::Value(double v)
{
	dValue = v;
}

MnaElemInd* ElemInd::CvtoMnaElem()
{
	return new MnaElemInd(this);
}
/* ------------------ class ElemVsrc ----------------- */

ElemVsrc::ElemVsrc(string nm, int p, int n, double d, Cplx c) :
	ElemTwoNode(nm, ElemType::VSRC, p, n), dValue(d), cValue(c)
{
}
void ElemVsrc::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << "\t dc:" << dValue << "\t ac: " << cValue
			<< endl;
}
double ElemVsrc::Value() const
{
	return dValue;
}
void ElemVsrc::Value(double v)
{
	dValue = v;
}
Cplx ElemVsrc::CValue() const
{
	return cValue;
}
void ElemVsrc::CValue(Cplx c)
{
	cValue = c;
}

MnaElemVsrc* ElemVsrc::CvtoMnaElem()
{
	return new MnaElemVsrc(this);
}

ElemVsrc* ElemVsrc::AddPhase(int N)
{
	ElemVsrc* elem = new ElemVsrc(*this);
	elem->ChangePhase(1,N);
	this->ChangePhase(2,N);
	return elem;
}

/* ------------------ class ElemIsrc ----------------- */

ElemIsrc::ElemIsrc(string nm, int p, int n, double d, Cplx c) :
	ElemTwoNode(nm, ElemType::ISRC, p, n), dValue(d), cValue(c)
{
}
void ElemIsrc::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << "\t dc:" << dValue << "\t ac: " << cValue
			<< endl;
}
double ElemIsrc::Value() const
{
	return dValue;
}
void ElemIsrc::Value(double v)
{
	dValue = v;
}
Cplx ElemIsrc::CValue() const
{
	return cValue;
}
void ElemIsrc::CValue(Cplx c)
{
	cValue = c;
}

MnaElemIsrc* ElemIsrc::CvtoMnaElem()
{
	return new MnaElemIsrc(this);
}
/* ------------------ class ElemSwitch ----------------- */

ElemSwitch::ElemSwitch(string nm, int p, int n, int v) :
	ElemTwoNode(nm, ElemType::SWITCH, p, n), dValue(v)
{
}
void ElemSwitch::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << "\t phase: " << dValue << endl;
}
int ElemSwitch::Phase() const
{
	return dValue;
}
void ElemSwitch::Phase(int p)
{
	dValue = p;
}

MnaElemSwitch* ElemSwitch::CvtoMnaElem()
{
	return new MnaElemSwitch(this);
}

void ElemSwitch::ChangePhase(int i,int N)
{
	if(i==1)
	{
		name = name + "_1";
		dValue = 1;
	}
	else
	{
		name = name + "_2";
		dValue = 2;
		nodep = nodep!=0?(nodep+N):0;
		noden = noden!=0?(noden+N):0;
	}
}
ElemSwitch* ElemSwitch::AddPhase(int N)
{
	ElemSwitch* elem = new ElemSwitch(*this);
	elem->ChangePhase(1,N);
	this->ChangePhase(2,N);
	this->Phase(2);
	elem->Phase(1);
	return elem;
}

/* ------------------ class ElemTwoPort ----------------- */

ElemTwoPort::ElemTwoPort(string nm, ElemType tp, int p, int n, int pp, int nn) :
	Elem(nm, tp), nodep(p), noden(n), nodepp(pp), nodenn(nn)
{
}
int ElemTwoPort::NodeP() const
{
	return nodep;
}
int ElemTwoPort::NodeN() const
{
	return noden;
}
int ElemTwoPort::NodePP() const
{
	return nodepp;
}
int ElemTwoPort::NodeNN() const
{
	return nodenn;
}
void ElemTwoPort::NodeP(int p)
{
	nodep = p;
}
void ElemTwoPort::NodeN(int n)
{
	noden = n;
}
void ElemTwoPort::NodePP(int ncp)
{
	nodepp = ncp;
}
void ElemTwoPort::NodeNN(int ncn)
{
	nodenn = ncn;
}

void ElemTwoPort::ChangePhase(int i,int N)
{
	if(i==1)
	{
		name = name + "_1";
	}
	else
	{
		name = name + "_2";
		nodep = nodep!=0?(nodep+N):0;
		noden = noden!=0?(noden+N):0;
		nodepp = nodepp!=0?(nodepp+N):0;
		nodenn = nodenn!=0?(nodenn+N):0;
	}
}


/* ------------------ class ElemTwoPortDS ----------------- */
ElemTwoPortDS::ElemTwoPortDS(string nm, ElemType tp, int p, int n, int pp,
		int nn, double v) :
	ElemTwoPort(nm, tp, p, n, pp, nn), dValue(v)
{
}
double ElemTwoPortDS::Value() const
{
	return dValue;
}
void ElemTwoPortDS::Value(double v)
{
	dValue = v;
}

/* ------------------ class ElemVCVS ----------------- */

ElemVCVS::ElemVCVS(string nm, int p, int n, int pp, int nn, double v) :
	ElemTwoPortDS(nm, ElemType::VCVS, p, n, pp, nn, v)
{
}
void ElemVCVS::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << " nc+: " << nodepp << " nc-: " << nodenn
			<< "\t E: " << dValue << endl;
}

MnaElemVCVS* ElemVCVS::CvtoMnaElem()
{
	return new MnaElemVCVS(this);
}

/* ------------------ class ElemCCCS ----------------- */
ElemCCCS::ElemCCCS(string nm, int p, int n, int pp, int nn, double v) :
	ElemTwoPortDS(nm, ElemType::CCCS, p, n, pp, nn, v)
{
}
void ElemCCCS::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << " nc+: " << nodepp << " nc-: " << nodenn
			<< "\t F: " << dValue << endl;
}

MnaElemCCCS* ElemCCCS::CvtoMnaElem()
{
	return new MnaElemCCCS(this);
}

/* ------------------ class ElemVCCS ----------------- */

ElemVCCS::ElemVCCS(string nm, int p, int n, int pp, int nn, double v) :
	ElemTwoPortDS(nm, ElemType::VCCS, p, n, pp, nn, v)
{
}
void ElemVCCS::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << " nc+: " << nodepp << " nc-: " << nodenn
			<< "\t G: " << dValue << endl;
}

MnaElemVCCS* ElemVCCS::CvtoMnaElem()
{
	return new MnaElemVCCS(this);
}


/* ------------------ class ElemGsc ----------------- */

ElemGsc::ElemGsc(string nm, int p, int n, int pp, int nn, double v) :
	ElemTwoPortDS(nm, ElemType::GSC, p, n, pp, nn, v)
{
}
void ElemGsc::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << " nc+: " << nodepp << " nc-: " << nodenn
			<< "\t G: " << dValue << endl;
}

MnaElemGsc* ElemGsc::CvtoMnaElem()
{
	return new MnaElemGsc(this);
}

/* ------------------ class ElemCCVS ----------------- */

ElemCCVS::ElemCCVS(string nm, int p, int n, int pp, int nn, double v) :
	ElemTwoPortDS(nm, ElemType::CCVS, p, n, pp, nn, v)
{

}
void ElemCCVS::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << " nc+: " << nodepp << " nc-: " << nodenn
			<< "\t H: " << dValue << endl;
}

MnaElemCCVS* ElemCCVS::CvtoMnaElem()
{
	return new MnaElemCCVS(this);
}


/* ------------------ class ElemOpamp ----------------- */

ElemOpamp::ElemOpamp(string nm, int p, int n, int pp, int nn) :
	ElemTwoPort(nm, ElemType::OPAMP, p, n, pp, nn)
{
}
void ElemOpamp::Print(ostream& out)
{
	out << "name: " << name << "\t type: " << type << "\t n+: " << nodep
			<< " n-: " << noden << " nc+: " << nodepp << " nc-: " << nodenn
			<< endl;
}

MnaElemOpamp* ElemOpamp::CvtoMnaElem()
{
	return new MnaElemOpamp(this);
}

ElemOpamp* ElemOpamp::AddPhase(int N)
{
	ElemOpamp* elem = new ElemOpamp(*this);
	elem->ChangePhase(1,N);
	ChangePhase(2,N);
	return elem;
}


