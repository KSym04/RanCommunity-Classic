#pragma once
#include <string>

#include "./RCryptByteDefine.h"
#include "./RCryptBase.h"

namespace NSCRYPT_BYTE
{
	extern UCHAR ENCRYPT_ARRAY[EMRCRYPT_BYTE_TYPE_SIZE][RCRYPT_BYTE_ARRAY_SIZE];
	extern UCHAR DECRYPT_ARRAY[EMRCRYPT_BYTE_TYPE_SIZE][RCRYPT_BYTE_ARRAY_SIZE];

	extern BOOL bVALIDARRAY;

	/*void byte_encode ( BYTE* pbuffer, DWORD dwSize, EMRCRYPT_BYTE_TYPE emCRYPT );
	void byte_decode ( BYTE* pbuffer, DWORD dwSize, EMRCRYPT_BYTE_TYPE emCRYPT );*/
};

class RCryptByte : public CRCryptBase
{
private:
	EMRCRYPT_BYTE_TYPE m_emType;

public:
	virtual bool Initialize();
	virtual void SetType(int nType);
	virtual bool Decrypt(BYTE* pbuffer, DWORD dwSize);
	virtual bool Encrypt(BYTE* pbuffer, DWORD dwSize);

public:
	RCryptByte();
	virtual ~RCryptByte();
};
