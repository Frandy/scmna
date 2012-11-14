/*
 * stringenc.h
 *
 *  Created on: Oct 4, 2012
 *      Author: chjd
 */

#ifndef STRINGENC_H_
#define STRINGENC_H_

#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>
using std::string;
using std::stringstream;
using std::vector;

vector<string> split(string& str,const char* c=" \t");

string& ltrim(string& str);
string& rtrim(string& str);
string& trim(string& str);

string int2str(int n);
string float2str(double f);

/*
template<class Type>
string toString(Type a);
*/
template<class Type>
string toString(Type a)
{
    stringstream ss;
    string s;
    ss << a;
    ss >> s;
    return s;
}

/*
string toString(char c);
*/
int str2int(string s);
double str2float(string s);

template<class ToType,class FromType>
ToType strconvert(FromType t);

string& stringtoupper(string& s);
string strtoupper(string s);
string& stringtolower(string& s);
string strtolower(string s);

unsigned int JSHash(const char *str);

#endif /* STRINGENC_H_ */
