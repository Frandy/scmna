/*
 * hashMap.h
 *
 *  Created on: Oct 15, 2012
 *      Author: chjd
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <string>
using std::string;
#include <unordered_map>
using std::unordered_map;

// since c++11 support string hash, no need for this

class StrHash{
public:
    size_t operator()(const string& s) const {
        return JSHash(s.c_str());
    }
};

class StrCompare{
public:
    bool operator()(const string& a,const string& b) const {
        return (a==b);
    }
};


#endif /* HASHMAP_H_ */
