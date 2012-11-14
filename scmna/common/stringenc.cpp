/*
 * stringenc.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: chjd
 */

/*
 * stringenc.cpp
 * 2012-04-24
 * some useful function for string manipulations,
 * including:
 *         split,            // split string by delim(such as " .,/")
 *         int2str,        // convert int to string
 *         float2str,        // convert double to string
 *         str2int,        // convert string to int
 *         str2float,        // convert string to double
 *         strtoupper,        // all to upper case
 *         strtolower,        // all to lower case
 *         //strtocapital,        // capital first character
 *
 */

#include "stringenc.h"

/**
 * @brief split a string by delim
 *
 * @param str string to be splited
 * @param c delimiter, const char*, just like " .,/", white space, dot, comma, splash
 *
 * @return a string vector saved all the splited world
 */
vector<string> split(string& str,const char* c)
{
    char *cstr, *p;
    vector<string> res;
    cstr = new char[str.size()+1];
    strcpy(cstr,str.c_str());
    p = strtok(cstr,c);
    while(p!=NULL)
    {
        res.push_back(p);
        p = strtok(NULL,c);
    }
    return res;
}

/*
ref code, http://codereview.stackexchange.com/questions/11203/most-efficient-way-in-c-to-strip-strings
Andrey's solution

 string strip(string in)
 {
 	 in.erase(std::remove_if(in.begin(), in.end(), \
 	 	 [] (string::value_type ch){ return !isalpha(ch); }),\
 	 	 in.end());
 	 return in;
 }

stackoverflow version, http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */


// trim from start
string& ltrim(string& s) {
	s.erase(
			s.begin(),
			std::find_if(s.begin(), s.end(),
					std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
string& rtrim(string& s) {
	s.erase(
			std::find_if(s.rbegin(), s.rend(),
					std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
			s.end());
	return s;
}

// trim from both ends
string& trim(string &s) {
	return ltrim(rtrim(s));
}


/**
 * @brief convert a integer into string through stringstream
 *
 * @param n a integer
 *
 * @return the string form of n
 */
string int2str(int n)
{
    stringstream ss;
    string s;
    ss << n;
    ss >> s;
    return s;
}

string float2str(double f)
{
    stringstream ss;
    string s;
    ss << f;
    ss >> s;
    return s;
}

/**
 * @brief convert something to string form through stringstream
 *
 * @tparam Type Type can be int,float,double
 * @param a
 *
 * @return the string form of param a
 */
/*
template<class Type>
string toString(Type a)
{
    stringstream ss;
    string s;
    ss << a;
    ss >> s;
    return s;
}
*/
/*
string toString(char c){
	string s="";
	s.push_back(c);
}
*/

/**
 * @brief convert string to int by atoi
 *
 * @param s string
 *
 * @return the integer result
 */
int str2int(string s)
{
    return atoi(s.c_str());
}

double str2float(string s)
{
    return atof(s.c_str());
}

/**
 * @brief do string convert through stringstream from FromType to ToType
 *
 * @tparam ToType target type
 * @tparam FromType source type
 * @param t to be converted param
 *
 * @return the target form of param t
 */
template<class ToType,class FromType>
ToType strconvert(FromType t)
{
    stringstream ss;
    ToType a;
    ss << t;
    ss >> a;
    return a;
}

/**
 * @brief convert string to upper case throught transform method, also can use transform method directly
 *
 * @param s
 *
 * @return the upper case result saved still in s
 */
string& stringtoupper(string& s)
{
    std::transform(s.begin(),s.end(),s.begin(),::toupper);
    return s;
}

/**
 * @brief convert string to upper case through toupper, which transform a char into upper case
 *
 * @param s
 *
 * @return the upper case result string
 */
string strtoupper(string s)
{
    string t = s;
    int i = -1;
    while(t[i++])
    {
        t[i] = toupper(t[i]);
    }
    return t;
}

/**
 * @brief convert string to lower case throught transform method, also can use transform method directly
 *
 * @param s
 *
 * @return the lower case result saved still in s
 */
string& stringtolower(string& s)
{
    std::transform(s.begin(),s.end(),s.begin(),::tolower);
    return s;
}

/**
 * @brief convert string to lower case through tolower, which transform a char into lower case
 *
 * @param s
 *
 * @return the lower case result string
 */
string strtolower(string s)
{
    string t = s;
    int i = -1;
    while(t[i++])
    {
        t[i] = tolower(t[i]);
    }
    return t;
}

unsigned int JSHash(const char *str)
{
    unsigned int hash = 1315423911;
    while (*str){
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }
    return (hash & 0x7FFFFFFF);
}
