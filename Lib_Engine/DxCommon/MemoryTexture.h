/*!
 * \file MemoryTexture.h
 *
 * \author Juver
 * \date August 2017
 *
 * 
 */
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMemoryTexture
{
public:
	CMemoryTexture();
	~CMemoryTexture();

public:
	enum 
	{
		TEXTURE_DDS		= 0,
		TEXTURE_TGA		= 1,
		TEXTURE_BMP		= 2,
		TEXTURE_TYPE	= 3,
	};

	enum 
	{
		TEX_HEADER_SIZE = 12,
		TEX_VERSION		= 0x202,

		//used in version 0x0100
		TEX_XOR_DATA	= 0x32,
		TEX_DIFF_DATA	= 0x08,

		//key size
		XOR_KEY_SIZE	= 256,
	};

	static char m_szFileExt[TEXTURE_TYPE][MAX_PATH];

	static TBYTE m_texture_xor_key_v2[XOR_KEY_SIZE];
	static TBYTE m_texture_xor_key_v3[XOR_KEY_SIZE];
	static TBYTE m_texture_xor_key_v4[XOR_KEY_SIZE];

public:
	BYTE* m_Buffer;
	int m_nSize;

public:
	bool LoadFileDec( const char* szFileName );
	bool SaveFileEnc( const char* szFileName );
	bool DecryptTexture( FILE* pFile, int&nFileType );
	bool EncryptTexture( FILE* pFile, const char* szFileName );
	bool DecryptTextureToFile( FILE* pFile, const char* szFileName );
	BYTE* GetData() { return m_Buffer; }
	int	GetSize() { return m_nSize; }
};