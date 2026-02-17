#ifndef GLLEVEL_HEAD_H
#define GLLEVEL_HEAD_H

#if _MSC_VER > 1000
#pragma once
#endif


#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLMapAxisInfo.h"
#include "../../Lib_Engine/G-Logic/GLQuadTree.h"
#include "../../Lib_Engine/DxOctree/DxLandDef.h"
#include "./GLCharDefine.h"

struct SLEVEL_HEAD_100
{
	char					m_szWldFile[MAX_PATH];
	char					m_szMapName[MAXLANDNAME];
	SNATIVEID				m_MapID;
	EMDIVISION				m_eDivision;
};

struct SLEVEL_HEAD
{
	enum { VERSION = 0x0103, };

	std::string				m_strMapName;
	std::string				m_strWldFile;
	std::string				m_strNaviFile;
	EMDIVISION				m_eDivision;
	EMBRIGHT				m_emBright;
	
	SLEVEL_HEAD () :
		m_eDivision(QUAD_DEFAULT_DIV),
		m_emBright(BRIGHT_BOTH)
	{
	}

	BOOL SAVE ( CSerialFile &SFile );
	BOOL LOAD ( basestream &SFile );

	void LOAD_0101 ( basestream &SFile, DWORD dwSIZE );
	void LOAD_0102 ( basestream &SFile, DWORD dwSIZE );
	void LOAD_0103 ( basestream &SFile, DWORD dwSIZE );

	void Assign ( const SLEVEL_HEAD_100 &sOldData );
};

#endif // GLLEVEL_HEAD_H