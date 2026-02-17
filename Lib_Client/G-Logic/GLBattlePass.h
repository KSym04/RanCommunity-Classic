#ifndef GLBATTLEPASS_H_
#define GLBATTLEPASS_H_

/*12-9-14, Battle Pass - CNDev*/

#if _MSC_VER > 1000
#pragma once
#endif

#include "./GLBattlePassData.h"

#include "../../Lib_Engine/Common/ByteStream.h"
#include "../../Lib_Engine/Common/SerialFile.h"

class GLBattlePass
{
public:
	enum
	{
		VERSION_ENCODE_2024		= 0x0001,
		VERSION_ENCODE			= 0x0001,

		BATTLEPASS_MAXID		= 1024,
	};

public:
	GLBattlePass();
	virtual ~GLBattlePass();

public:
	static const char*	_FILEHEAD;
	char				m_szFileName[MAX_PATH];

protected:
	std::string			m_strPATH;

public:
	SBATTLEPASS_FILE_DATA_MAP		m_mapBattlePass;

public:
	void SetPath ( std::string strPATH )	{ m_strPATH = strPATH; }
	std::string GetPath ()					{ return m_strPATH.c_str(); }
	void	CleanUp();

	HRESULT LoadFile ( const char* szFile, BOOL bServer );
	HRESULT SaveFile ( const char* szFile );

public:
	SBATTLEPASS_FILE_DATA*	GetBattlePass( DWORD dwID );

public:
	static GLBattlePass& GetInstance ();
};

#endif // GLBATTLEPASS_H_