/*
 * macro.h
 *
 *  Created on: Oct 15, 2012
 *      Author: chjd
 */

#ifndef MACRO_H_
#define MACRO_H_

#define foreach(container,it) \
    for(auto it = (container).begin(),et=(container).end();it!=et;++it)

#endif /* MACRO_H_ */
