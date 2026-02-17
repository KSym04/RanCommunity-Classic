#pragma once

#include "BaseStringGSV4.h"

class SFileSystem;

class CStringMemoryGSV4 : public CBaseStringGSV4
{
public:
	CStringMemoryGSV4 ( UINT nBufSize = 4096 ); // 버퍼를 1024로 제안해야 하는가...?
	virtual ~CStringMemoryGSV4 ();
	
public:
	BOOL Open ( LPCSTR _szZipFileName, 
				LPCSTR _szFileName, 
				BOOL bDecode = FALSE, 
				BOOL bPack = FALSE );

	virtual void Close ();

public:
	virtual void Reset (void);

public:
	virtual DWORD GetNextLine ( LPSTR szLine,UINT iLineSize );
	virtual DWORD GetNextLine ( PSTR &szString );
	virtual DWORD GetNextLine ( CString &strString );

protected:
	DWORD		m_dwOffSet; // 메모리 Offset;
	UINT		m_nBufSize;
	BYTE*		m_pBufferMem;
	UINT		m_nBufSizeTemp;
};
