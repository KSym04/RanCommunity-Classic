/*!
 * \file GLPKMark.h
 *
 * \author Juver
 * \date 2022/02/14
 *
 * 
 */

#ifndef GLPKMARK_H_INCLUDED__
#define GLPKMARK_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLCharDefine.h"
#include "./GLItemDef.h"

#define PKMARK_UI_ARRAY_SIZE 10
#define PKMARK_DELETE_TIME 4.0f

struct SPKMARK_INFO
{
	DWORD m_dwCharIDA;
	DWORD m_dwCharIDB;

	char m_szNameA[CHAR_SZNAME];
	char m_szNameB[CHAR_SZNAME];

	WORD m_wSchoolA;
	WORD m_wSchoolB;

	EMCHARINDEX m_emClassA;
	EMCHARINDEX m_emClassB;

	DWORD m_dwClubA;
	DWORD m_dwClubB;

	DWORD m_dwClubMarkVerA;
	DWORD m_dwClubMarkVerB;

	GLITEM_ATT m_emAttack;

	float m_fAge;

	SPKMARK_INFO();
	~SPKMARK_INFO();

	void SetDataA( 
		DWORD dwCharID, 
		const char* szName, 
		WORD wSchool,
		EMCHARINDEX emClass,
		DWORD dwClub,
		DWORD dwClubMark );

	void SetDataB( 
		DWORD dwCharID,
		const char* szName, 
		WORD wSchool, 
		EMCHARINDEX emClass,
		DWORD dwClub,
		DWORD dwClubMark );

	void SetWeapon( GLITEM_ATT emAttack );

	void FrameMove( float fElapsedTime );
	BOOL isDeleteTime();
};

typedef std::vector<SPKMARK_INFO> VEC_PKMARK_INFO;
typedef VEC_PKMARK_INFO::iterator VEC_PKMARK_INFO_ITER;

class GLPKMark
{
public:
	GLPKMark();
	~GLPKMark();

	static GLPKMark& GetInstance ();

public:
	VEC_PKMARK_INFO	m_vecMark;

	void CleanUp();

	void FrameMove( float fElapsedTime );

	void AddData( const SPKMARK_INFO& sData );

};

#endif // GLPKMARK_H_INCLUDED__
