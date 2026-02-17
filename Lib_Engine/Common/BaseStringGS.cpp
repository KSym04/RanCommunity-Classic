#include "stdafx.h"
#include "./BaseStringGS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBaseStringGS::CBaseStringGS () :
	m_dwIndex(0),
	m_dwLine(0),
	m_bDecode(FALSE),
	m_nVersion(-1),
	m_nVersionDate(-1)
{
	SecureZeroMemory ( m_szFileName, sizeof(m_szFileName) );
}

CBaseStringGS::~CBaseStringGS ()
{
}
