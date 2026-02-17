/*!
 * \file GLWTF.h
 *
 * \author Juver
 * \date 2021/06/19
 *
 * 
 */

#ifndef GLWTF_H_INCLUDED__
#define GLWTF_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined( BUILD_CH ) || defined( BUILD_CHY )

namespace NSWTF
{

};

class GLWTF
{
public:
	GLWTF();
	~GLWTF();

public:
	float fTimeOut;

	BOOL bActionCheck1;
	BOOL bActionCheck2;
	BOOL bActionCheck3;

public:
	void SetKey();
	BOOL CheckMsg( std::string strName, std::string strMsg );
	void FrameMove( float fElapsedTime );

	void DoAction();
};

#endif

#endif // GLWTF_H_INCLUDED__

