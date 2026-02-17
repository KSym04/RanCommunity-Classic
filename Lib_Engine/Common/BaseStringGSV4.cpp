#include "stdafx.h"
#include "./BaseStringGSV4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBaseStringGSV4::CBaseStringGSV4 () :
	m_dwIndex(0),
	m_dwLine(0),
	m_bDecode(FALSE),
	m_nVersion(-1),
	m_nVersionDate(-1)
{
	SecureZeroMemory ( m_szFileName, sizeof(m_szFileName) );
}

CBaseStringGSV4::~CBaseStringGSV4 ()
{
}
