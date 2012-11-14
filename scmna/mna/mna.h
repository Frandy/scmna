/*
 * mna.h
 *
 *  Created on: Oct 13, 2012
 *      Author: chjd
 */

#ifndef MNA_H_
#define MNA_H_

#include <iostream>
using std::clog;
using std::endl;

#include <armadillo>
using namespace arma;

enum class AnaType
{
	DC,
	TRAN,
	AC
};

template<class ValueType>
class MNA
{
public:
	MNA()=default;
	MNA(int n);
	void CellSet(int row, int col, ValueType value);
	void CellAdd(int row,int col,ValueType value);
	void RhsSet(int row,ValueType value);
	void RhsAdd(int row,ValueType value);
	bool Solve();
	ValueType GetX(int row);
	void Clear()
	{
		// set all elements to be zero
		mna.zeros();
		rhs.zeros();
		x.zeros();
	}
public:
	int N;
	Mat<ValueType> mna;
	Col<ValueType> rhs;
	Col<ValueType> x;
//	Mat<ValueType> mrhs;
//	Mat<ValueType> mx;
};

template<class ValueType>
MNA<ValueType>::MNA(int n) :
	N(n)
{
	mna = Mat<ValueType>(N, N);
	rhs = Col<ValueType>(N);
	x = Col<ValueType>(N);
}
template<class ValueType>
void MNA<ValueType>::CellSet(int row, int col, ValueType value)
{
	int m = row - 1;
	int n = col - 1;
	if (m < 0 || n < 0)
		return;
	mna(m, n) = value;
}
template<class ValueType>
void MNA<ValueType>::CellAdd(int row, int col, ValueType value)
{
	int m = row - 1;
	int n = col - 1;
	if (m < 0 || n < 0)
		return;
	mna(m, n) += value;
}
template<class ValueType>
void MNA<ValueType>::RhsSet(int row, ValueType value)
{
	int m = row - 1;
	if (m < 0)
		return;
	rhs(m) = value;
}
template<class ValueType>
void MNA<ValueType>::RhsAdd(int row, ValueType value)
{
	int m = row - 1;
	if (m < 0)
		return;
	rhs(m) += value;
}
template<class ValueType>
bool MNA<ValueType>::Solve()
{
/*	clog << "begin to solve mna" << endl;
	clog << "mna:\n" << mna << endl;
	clog << "rhs:\n" << rhs << endl;
*/
	bool ok = solve(x, mna, rhs);
/*
	clog << "solve done." << endl;
	clog << "x:\n" << x << endl;
*/
	return ok;
}

template<class ValueType>
ValueType MNA<ValueType>::GetX(int row)
{
	int m = row - 1;
	if(m<0)
		return ValueType(0);
	else
		return x(m);
}



#endif /* MNA_H_ */
