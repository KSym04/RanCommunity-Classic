/*!
 * \file PSDefine.h
 *
 * \author Juver
 * \date 2022/05/18
 *
 * 
 */

#ifndef PSDEFINE_H_INCLUDED__
#define PSDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined( BUILD_Z )
//use psgg
#define USE_PSGG
#endif // BUILD_X

#ifdef USE_PSGG

namespace PSGGGlobal
{

};

#endif


#endif // PSDEFINE_H_INCLUDED__
