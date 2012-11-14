/*
 * mncelem.cpp
 *
 *  Created on: Oct 29, 2012
 *      Author: chjd
 */

#include "elem.h"
#include "mnaElem.h"

#include <iostream>
using std::clog;
using std::endl;

/* ------------------ class MnaElem ----------------- */

void MnaElem::Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet)
{
//	clog << "sup skiped" << endl;
	return;
}
void MnaElem::BkInc(int& rk, int& ck)
{
//	clog << "Bk inc skipped" << endl;
	return;
}
void MnaElem::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	return;
}

/* ------------------ class MnaElemRes ----------------- */
MnaElemRes::MnaElemRes(ElemRes* elem) :
	elem(elem)
{
	dValue = 1.0 / (elem->Value());
}
void MnaElemRes::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colp = colSet.Node(elem->NodeP());
	int coln = colSet.Node(elem->NodeN());
	int rowp = rowSet.Node(elem->NodeP());
	int rown = rowSet.Node(elem->NodeN());
	Cplx cValue = Cplx(dValue, 0);
	mna.CellAdd(rowp, colp, cValue);
	mna.CellAdd(rowp, coln, -cValue);
	mna.CellAdd(rown, colp, -cValue);
	mna.CellAdd(rown, coln, cValue);
}

/* ------------------ class MnaElemCap ----------------- */
MnaElemCap::MnaElemCap(ElemCap* elem) :
	elem(elem)
{
}

void MnaElemCap::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colp = colSet.Node(elem->NodeP());
	int coln = colSet.Node(elem->NodeN());
	int rowp = rowSet.Node(elem->NodeP());
	int rown = rowSet.Node(elem->NodeN());
	Cplx cValue = Cplx(elem->Value(), 0) * s;
	mna.CellAdd(rowp, colp, cValue);
	mna.CellAdd(rowp, coln, -cValue);
	mna.CellAdd(rown, colp, -cValue);
	mna.CellAdd(rown, coln, cValue);
}

/* ------------------ class MnaElemInd ----------------- */
MnaElemInd::MnaElemInd(ElemInd* elem) :
	elem(elem)
{
	dValue = 1.0 / (elem->Value());
}
void MnaElemInd::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colp = colSet.Node(elem->NodeP());
	int coln = colSet.Node(elem->NodeN());
	int rowp = rowSet.Node(elem->NodeP());
	int rown = rowSet.Node(elem->NodeN());
	Cplx cValue = Cplx(dValue, 0) / s;
	mna.CellAdd(rowp, colp, cValue);
	mna.CellAdd(rowp, coln, -cValue);
	mna.CellAdd(rown, colp, -cValue);
	mna.CellAdd(rown, coln, cValue);
}

/* ------------------ class MnaElemIsrc ----------------- */
MnaElemIsrc::MnaElemIsrc(ElemIsrc* elem) :
		elem(elem)
{
}
void MnaElemIsrc::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int rowp = rowSet.Node(elem->NodeP());
	int rown = rowSet.Node(elem->NodeN());
	Cplx cValue = elem->CValue();
	mna.RhsAdd(rowp, -cValue);
	mna.RhsAdd(rown, cValue);
}

/* ------------------ class MnaElemVCCS ----------------- */
MnaElemVCCS::MnaElemVCCS(ElemVCCS* elem) :
	elem(elem)
{
}
void MnaElemVCCS::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colp = colSet.Node(elem->NodePP());
	int coln = colSet.Node(elem->NodeNN());
	int rowp = rowSet.Node(elem->NodeP());
	int rown = rowSet.Node(elem->NodeN());
/*
	clog << "stamp template for :" << elem->Name() << endl;
	clog << ".\t" << colp << "\t" << coln << endl;
	clog << rowp << "\t" << "+\t" << "-\t" << endl;
	clog << rown << "\t" << "-\t" << "+\t" << endl;
*/
	Cplx cValue = Cplx(elem->Value(), 0);
	mna.CellAdd(rowp, colp, cValue);
	mna.CellAdd(rowp, coln, -cValue);
	mna.CellAdd(rown, colp, -cValue);
	mna.CellAdd(rown, coln, cValue);
}


/* ------------------ class MnaElemGsc ----------------- */
MnaElemGsc::MnaElemGsc(ElemGsc* elem) :
	elem(elem)
{
}
void MnaElemGsc::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colp = colSet.Node(elem->NodePP());
	int coln = colSet.Node(elem->NodeNN());
	int rowp = rowSet.Node(elem->NodeP());
	int rown = rowSet.Node(elem->NodeN());
/*
	clog << "stamp template for :" << elem->Name() << endl;
	clog << ".\t" << colp << "\t" << coln << endl;
	clog << rowp << "\t" << "+\t" << "-\t" << endl;
	clog << rown << "\t" << "-\t" << "+\t" << endl;
*/
	Cplx cValue = Cplx(elem->Value(), 0)/s;
	mna.CellAdd(rowp, colp, cValue);
	mna.CellAdd(rowp, coln, -cValue);
	mna.CellAdd(rown, colp, -cValue);
	mna.CellAdd(rown, coln, cValue);
}


/* ------------------ class MnaElemVsrc ----------------- */
MnaElemVsrc::MnaElemVsrc(ElemVsrc* elem) :
	elem(elem)
{
}
void MnaElemVsrc::Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet)
{
//	clog << "sup in vsrc" << endl;
	rowSet.Sup(elem->NodeP(), elem->NodeN());
}
void MnaElemVsrc::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colp = colSet.Node(elem->NodeP());
	int coln = colSet.Node(elem->NodeN());
	//int rown = bk;
	Cplx one(1, 0), mone(-1, 0);
	Cplx cValue = elem->CValue();
	mna.CellSet(rowk, colp, one);
	mna.CellSet(rowk, coln, mone);
	mna.RhsSet(rowk, cValue);
}

void MnaElemVsrc::BkInc(int& rk, int& ck)
{
	rowk = rk++;
}

/* ------------------ class MnaElemVCVS ----------------- */
MnaElemVCVS::MnaElemVCVS(ElemVCVS* elem) :
	elem(elem)
{
}
void MnaElemVCVS::Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet)
{
	rowSet.Sup(elem->NodeP(), elem->NodeN());
}
void MnaElemVCVS::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colpp = colSet.Node(elem->NodePP());
	int colnn = colSet.Node(elem->NodeNN());
	int colp = colSet.Node(elem->NodeP());
	int coln = colSet.Node(elem->NodeN());
	Cplx one(1, 0), mone(-1, 0);
	Cplx cValue = Cplx(elem->Value(), 0);
	mna.CellSet(rowk, colpp, cValue);
	mna.CellSet(rowk, colnn, -cValue);
	mna.CellSet(rowk, colp, mone);
	mna.CellSet(rowk, coln, one);
}

void MnaElemVCVS::BkInc(int& rk, int& ck)
{
	rowk = rk++;
}

/* ------------------ class MnaElemCCVS ----------------- */
MnaElemCCVS::MnaElemCCVS(ElemCCVS* elem) :
	elem(elem)
{
}
void MnaElemCCVS::Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet)
{
	colSet.Sup(elem->NodePP(), elem->NodeNN());
	rowSet.Sup(elem->NodeP(), elem->NodeN());
}
void MnaElemCCVS::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int colp = colSet.Node(elem->NodeP());
	int coln = colSet.Node(elem->NodeN());
	int rowpp = rowSet.Node(elem->NodePP());
	int rownn = rowSet.Node(elem->NodeNN());
	//int rowk = bk, colk = bk;
	Cplx one(1, 0), mone(-1, 0);
	Cplx cValue = Cplx(elem->Value(), 0);
	mna.CellSet(rowk, colp, mone);
	mna.CellSet(rowk, coln, one);
	mna.CellSet(rowk, colk, cValue);
	mna.CellSet(rowpp, colk, one);
	mna.CellSet(rownn, colk, mone);
}

void MnaElemCCVS::BkInc(int& rk, int& ck)
{
	rowk = rk++;
	colk = ck++;
}

/* ------------------ class MnaElemCCCS ----------------- */
MnaElemCCCS::MnaElemCCCS(ElemCCCS* elem) :
	elem(elem)
{
}
void MnaElemCCCS::Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet)
{
	colSet.Sup(elem->NodePP(), elem->NodeNN());
}
void MnaElemCCCS::AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
		SuperNodeSet& colSet)
{
	int rowpp = rowSet.Node(elem->NodePP());
	int rownn = rowSet.Node(elem->NodeNN());
	int rowp = rowSet.Node(elem->NodeP());
	int rown = rowSet.Node(elem->NodeN());
	//int colk = bk;
	Cplx one(1, 0), mone(-1, 0);
	Cplx cValue = Cplx(elem->Value(), 0);
	mna.CellSet(rowpp, colk, one);
	mna.CellSet(rownn, colk, mone);
	mna.CellSet(rowp, colk, cValue);
	mna.CellSet(rown, colk, -cValue);
}
void MnaElemCCCS::BkInc(int& rk, int& ck)
{
	colk = ck++;
}

/* ------------------ class MnaElemOpamp ----------------- */
MnaElemOpamp::MnaElemOpamp(ElemOpamp* elem) :
	elem(elem)
{
}
void MnaElemOpamp::Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet)
{
	colSet.Sup(elem->NodePP(), elem->NodeNN());
	rowSet.Sup(elem->NodeP(), elem->NodeN());
}


/* ------------------ class MnaElemSwitch ----------------- */
MnaElemSwitch::MnaElemSwitch(ElemSwitch* elem) :
	elem(elem)
{
}
void MnaElemSwitch::Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet)
{
	colSet.Sup(elem->NodeP(), elem->NodeN());
	rowSet.Sup(elem->NodeP(), elem->NodeN());
}
