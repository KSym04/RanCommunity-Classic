#include "StdAfx.h"
#include "./GLNote.h"
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLNote& GLNote::operator= ( const GLNote &value )
{
	m_bSelected = value.m_bSelected;
	StringCchCopy ( m_szSendChar, CHAR_SZNAME, value.m_szSendChar );
	StringCchCopy ( m_szReciChar, CHAR_SZNAME, value.m_szReciChar );
	StringCchCopy ( m_szSentence, CHAR_SZNOTE, value.m_szSentence );

	return *this;
}
