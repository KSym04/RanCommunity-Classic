/*!
 * \file CommonContainer.h
 *
 * \author Juver
 * \date 2019/09/16
 *
 * 
 */

#ifndef COMMONCONTAINER_H_INCLUDED__
#define COMMONCONTAINER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

//don't call when container elements are created with "new"
template<typename T>
inline void common_vector_free(T& p_container)
{
	T empty;
	using std::swap;
	swap(p_container, empty);
}

#endif // COMMONCONTAINER_H_INCLUDED__
