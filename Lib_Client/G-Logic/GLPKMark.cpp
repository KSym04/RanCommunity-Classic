#include "stdafx.h"
#include "./GLPKMark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


SPKMARK_INFO::SPKMARK_INFO()
	: m_dwCharIDA(GAEAID_NULL)
	, m_dwCharIDB(GAEAID_NULL)
	, m_wSchoolA(0)
	, m_wSchoolB(0)
	, m_emClassA(GLCI_NUM_8CLASS)
	, m_emClassB(GLCI_NUM_8CLASS)
	, m_emAttack(ITEMATT_NSIZE)
	, m_fAge(0.0f)
	, m_dwClubA(CLUB_NULL)
	, m_dwClubB(CLUB_NULL)
	, m_dwClubMarkVerA(0)
	, m_dwClubMarkVerB(0)
{
	SecureZeroMemory ( m_szNameA, sizeof(m_szNameA) );
	SecureZeroMemory ( m_szNameB, sizeof(m_szNameB) );
}

SPKMARK_INFO::~SPKMARK_INFO()
{

}

void SPKMARK_INFO::SetDataA( 
	DWORD dwCharID, 
	const char* szName, 
	WORD wSchool,
	EMCHARINDEX emClass,
	DWORD dwClub,
	DWORD dwClubMark )
{
	m_dwCharIDA = dwCharID;
	m_wSchoolA = wSchool;
	m_emClassA = emClass;
	m_dwClubA = dwClub;
	m_dwClubMarkVerA = dwClubMark;

	if ( szName )
		StringCchCopy ( m_szNameA, CHAR_SZNAME, szName );

}

void SPKMARK_INFO::SetDataB( 
	DWORD dwCharID,
	const char* szName, 
	WORD wSchool, 
	EMCHARINDEX emClass,
	DWORD dwClub,
	DWORD dwClubMark )
{
	m_dwCharIDB = dwCharID;
	m_wSchoolB = wSchool;
	m_emClassB = emClass;
	m_dwClubB = dwClub;
	m_dwClubMarkVerB = dwClubMark;

	if ( szName )
		StringCchCopy ( m_szNameB, CHAR_SZNAME, szName );
}

void SPKMARK_INFO::SetWeapon( GLITEM_ATT emAttack )
{
	m_emAttack = emAttack;
}

void SPKMARK_INFO::FrameMove( float fElapsedTime )
{
	m_fAge += fElapsedTime;
}

BOOL SPKMARK_INFO::isDeleteTime()
{
	if ( m_fAge >= PKMARK_DELETE_TIME )
		return TRUE;
	else 
		return FALSE;
}


GLPKMark& GLPKMark::GetInstance ()
{
	static GLPKMark Instance;
	return Instance;
}


GLPKMark::GLPKMark()
{

}

GLPKMark::~GLPKMark()
{

}

void GLPKMark::CleanUp()
{
	m_vecMark.clear();
}

void GLPKMark::FrameMove( float fElapsedTime )
{
	//update timer
	for ( VEC_PKMARK_INFO_ITER it=m_vecMark.begin(); 
		it!=m_vecMark.end(); 
		it++ )
	{
		SPKMARK_INFO& sData = (*it);
		sData.FrameMove( fElapsedTime );
	}

	//check if data is needed to be deleted
	for ( VEC_PKMARK_INFO_ITER it=m_vecMark.begin(); 
		it!=m_vecMark.end(); )
	{
		SPKMARK_INFO& sData = (*it);
		if ( sData.isDeleteTime() == TRUE )
		{
			it = m_vecMark.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GLPKMark::AddData( const SPKMARK_INFO& sData )
{
	m_vecMark.push_back( sData );

	//when size is greater than whats possible to display in the ui
	//delete the very first element
	//this will look very bad when there is lots of kills happening 
	//a good solution is to increase the expire time of existing elements when new elements is needed to be displayed
	//this will speed up the deletion time instead of just rapid appear and disappear of elements
	//but it will come in a performance penalty as we have to iterate the existing data and modify its expire time

	if ( m_vecMark.size() > PKMARK_UI_ARRAY_SIZE )
	{
		m_vecMark.erase( m_vecMark.begin() );
	}
}