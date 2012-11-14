/*
 * anaElem.h
 *
 *  Created on: Oct 13, 2012
 *      Author: chjd
 */

#ifndef MNAELEM_H_
#define MNAELEM_H_

//#include "elem.h"
#include "mna.h"
#include "superNodeSet.h"

class Elem;
class ElemRes;
class ElemCap;
class ElemInd;
class ElemVsrc;
class ElemIsrc;
class ElemVCVS;
class ElemCCCS;
class ElemVCCS;
class ElemCCVS;
class ElemOpamp;
class ElemSwitch;
class ElemGsc;


/* ------------------ class MnaElem ----------------- */

class MnaElem
{
public:
	MnaElem()=default;
	virtual void Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet);
	virtual void BkInc(int& rk,int& ck);
	virtual void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
};


/* ------------------ class MnaElemRes ----------------- */

class MnaElemRes: public MnaElem
{
public:
	MnaElemRes()=default;
	MnaElemRes(ElemRes* elem);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	double dValue;
	ElemRes* elem;
};


/* ------------------ class MnaElemCap ----------------- */

class MnaElemCap: public MnaElem
{
public:
	MnaElemCap()=default;
	MnaElemCap(ElemCap* elem);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemCap* elem;
};


/* ------------------ class MnaElemInd ----------------- */

class MnaElemInd: public MnaElem
{
public:
	MnaElemInd()=default;
	MnaElemInd(ElemInd* elem);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	double dValue;
	ElemInd* elem;
};


/* ------------------ class MnaElemIsrc ----------------- */

class MnaElemIsrc: public MnaElem
{
public:
	MnaElemIsrc()=default;
	MnaElemIsrc(ElemIsrc* elem);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemIsrc* elem;
};


/* ------------------ class MnaElemVCCS ----------------- */

class MnaElemVCCS: public MnaElem
{
public:
	MnaElemVCCS()=default;
	MnaElemVCCS(ElemVCCS* elem);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemVCCS* elem;
};

/* ------------------ class MnaElemGsc ----------------- */

class MnaElemGsc: public MnaElem
{
public:
	MnaElemGsc()=default;
	MnaElemGsc(ElemGsc* elem);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemGsc* elem;
};


/* ------------------ class MnaElemVsrc ----------------- */

class MnaElemVsrc: public MnaElem
{
public:
	MnaElemVsrc()=default;
	MnaElemVsrc(ElemVsrc* elem);
	void Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet);
	void BkInc(int& rk,int& ck);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemVsrc* elem;
	int rowk;
};


/* ------------------ class MnaElemVCVS ----------------- */

class MnaElemVCVS: public MnaElem
{
public:
	MnaElemVCVS()=default;
	MnaElemVCVS(ElemVCVS* elem);
	void Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet);
	void BkInc(int& rk,int& ck);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemVCVS* elem;
	int rowk;
};


/* ------------------ class MnaElemCCVS ----------------- */

class MnaElemCCVS: public MnaElem
{
public:
	MnaElemCCVS()=default;
	MnaElemCCVS(ElemCCVS* elem);
	void Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet);
	void BkInc(int& rk,int& ck);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemCCVS* elem;
	int rowk;
	int colk;
};


/* ------------------ class MnaElemCCCS ----------------- */

class MnaElemCCCS: public MnaElem
{
public:
	MnaElemCCCS()=default;
	MnaElemCCCS(ElemCCCS* elem);
	void Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet);
	void BkInc(int& rk,int& ck);
	void AcLoad(MNA<Cplx>& mna, Cplx s, SuperNodeSet& rowSet,
			SuperNodeSet& colSet);
private:
	ElemCCCS* elem;
	int colk;
};


/* ------------------ class MnaElemOpamp ----------------- */

class MnaElemOpamp: public MnaElem
{
public:
	MnaElemOpamp()=default;
	MnaElemOpamp(ElemOpamp* elem);
	void Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet);
private:
	ElemOpamp* elem;
};


/* ------------------ class MnaElemSwitch ----------------- */

class MnaElemSwitch: public MnaElem
{
public:
	MnaElemSwitch()=default;
	MnaElemSwitch(ElemSwitch* elem);
	void Sup(SuperNodeSet& rowSet, SuperNodeSet& colSet);
private:
	ElemSwitch* elem;
};

#endif /* MNAELEM_H_ */
