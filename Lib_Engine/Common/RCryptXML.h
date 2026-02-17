#ifndef RCRYPTXML_H__INCLUDED
#define RCRYPTXML_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define XML_XOR_KEY_SIZE 256

#include "./RCryptBase.h"

namespace XML_CRYPT_ARRAY
{
	extern UCHAR ENCRYPT_ARRAY[CRYPT_BYTE_ARRAY_SIZE];
	extern UCHAR DECRYPT_ARRAY[CRYPT_BYTE_ARRAY_SIZE];
	extern bool	bValidArray;

	void Init();
};

class CRCryptXML : public CRCryptBase
{
public:
	virtual bool Initialize();
	virtual void SetType( int nType );
	virtual bool Decrypt( BYTE* pbuffer, DWORD dwSize );
	virtual bool Encrypt( BYTE* pbuffer, DWORD dwSize );

public:
	CRCryptXML();
	virtual ~CRCryptXML();
};


#endif // RCRYPTXML_H__INCLUDED
