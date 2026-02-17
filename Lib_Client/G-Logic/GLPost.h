/*!
 * \file GLPost.h
 *
 * \author Juver
 * \date 2021/02/05
 *
 * 
 */

#ifndef GLPOST_H_INCLUDED__
#define GLPOST_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GLPost
{
public:
	GLPost ();
	virtual ~GLPost();

public:
	static LONGLONG llPOST_EXPIRE_DURATION_SEC;

public:
	static const LONGLONG GetCommision ( const BOOL IsAttachDesign, const BOOL IsAttachItem, const LONGLONG llAttachMoney );

};



#endif // GLPOST_H_INCLUDED__
