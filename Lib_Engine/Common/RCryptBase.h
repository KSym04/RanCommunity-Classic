#ifndef RCRYPTBASE_H__INCLUDED
#define RCRYPTBASE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CRYPT_BYTE_ARRAY_SIZE 256

class CRCryptBase
{
public:	
	bool	m_bValid;
	int		m_nType;

public:
	virtual bool Initialize() { return true; }
	virtual void SetType( int nType ) { m_nType = nType; }
	virtual bool Decrypt( BYTE* pbuffer, DWORD dwSize ) { return true; }
	virtual bool Encrypt( BYTE* pbuffer, DWORD dwSize ) { return true; }

public:
	CRCryptBase()
		: m_bValid(false)
		, m_nType(0)
	{
	}

	virtual ~CRCryptBase()
	{
	}

};

#endif // RCRYPTBASE_H__INCLUDED


