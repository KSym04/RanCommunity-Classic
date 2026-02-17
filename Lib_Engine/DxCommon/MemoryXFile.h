/*!
 * \file MemoryXFile.h
 *
 * \author Juver
 * \date August 2017
 *
 * 
 */
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMemoryXFile
{
public:
	CMemoryXFile();
	~CMemoryXFile();

	enum 
	{
		SKIN_HEADER_SIZE = 12,
		SKIN_VERSION	= 0x201,

		//used in version 0x0100
		SKIN_XOR_DATA	= 0x48,
		SKIN_DIFF_DATA	= 0x04,

		//key size
		XOR_KEY_SIZE	= 256,
	};

	static TBYTE m_skin_file_xor_key_v2[XOR_KEY_SIZE];
	static TBYTE m_skin_file_xor_key_v3[XOR_KEY_SIZE];

public:
	BYTE* m_Buffer;
	int m_nSize;

public:
	bool LoadFileDec( const char* szFileName );
	bool SaveFileEnc( const char* szFileName );
	bool DecryptSkin( FILE* pFile );
	bool EncryptSkin( FILE* pFile, const char* szFileName );
	bool DecryptSkinToTile( FILE* pFile, const char* szFileName );
	BYTE* GetData() { return m_Buffer; }
	int	GetSize() { return m_nSize; }
};