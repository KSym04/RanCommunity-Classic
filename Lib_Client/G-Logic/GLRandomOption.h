/*!
 * \file GLRandomOption.h
 *
 * \author Juver
 * \date February 2018
 *
 * 
 */

#ifndef GLRANDOM_OPTION_H
#define GLRANDOM_OPTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>

#include "../../Lib_Engine/Common/ByteStream.h"
#include "../../Lib_Engine/Common/SerialFile.h"

#include "./GLItemDef.h"

struct SRANDOM_OPTION_SECTION
{
	float	fRATE;	
	float	fHIGH;	
	float	fLOW;	

	SRANDOM_OPTION_SECTION()
		: fRATE(0.0f)
		, fHIGH(0.0f)
		, fLOW(0.0f)
	{
	}

	SRANDOM_OPTION_SECTION ( const SRANDOM_OPTION_SECTION &value )
	{
		operator=(value);
	}

	SRANDOM_OPTION_SECTION& operator = ( const SRANDOM_OPTION_SECTION& rvalue )
	{
		fRATE	= rvalue.fRATE;
		fHIGH	= rvalue.fHIGH;
		fLOW	= rvalue.fLOW;

		return *this;
	}

	void Reset()
	{
		fRATE	= 0.0f;
		fHIGH	= 0.0f;
		fLOW	= 0.0f;
	}

	void LoadSection_1_2 ( basestream &SFile, EMRANDOM_OPT emTYPE );
	void LoadSection ( basestream &SFile );
	void SaveSection ( CSerialFile &SFile );
};

typedef std::vector<SRANDOM_OPTION_SECTION>		SRANDOM_OPTION_SECTION_VEC;


struct SRANDOM_OPTION_SET
{
	EMRANDOM_OPT	emTYPE;
	float			fRATE;
	float			fHIGH;
	float			fLOW;
	SRANDOM_OPTION_SECTION_VEC	vecSECT;

	SRANDOM_OPTION_SET()
		: emTYPE(EMR_OPT_NULL)
		, fRATE(0.0f)
		, fHIGH(0.0f)
		, fLOW(0.0f)
	{
	}

	SRANDOM_OPTION_SET ( const SRANDOM_OPTION_SET &value )
	{
		operator=(value);
	}

	SRANDOM_OPTION_SET& operator = ( const SRANDOM_OPTION_SET& rvalue )
	{
		emTYPE	= rvalue.emTYPE;
		fRATE	= rvalue.fRATE;
		fHIGH	= rvalue.fHIGH;
		fLOW	= rvalue.fLOW;
		vecSECT = rvalue.vecSECT;

		return *this;
	}

	void Reset()
	{
		emTYPE	= EMR_OPT_NULL;
		fRATE	= 0.0f;
		fHIGH	= 0.0f;
		fLOW	= 0.0f;
		vecSECT.clear();
	}

	DWORD GetSectionSize() { return (DWORD)vecSECT.size(); }

	void LoadSet_1_2 ( basestream &SFile );
	void LoadSet ( basestream &SFile );
	void SaveSet ( CSerialFile &SFile );
	const bool saveCsv(std::fstream& SFile);
	const bool loadCsv(CStringArray& StrArray);

	void GenerateRange();
	float GetTotalRate();
};

typedef std::map<DWORD,SRANDOM_OPTION_SET>		SRANDOM_OPTION_SET_MAP;
typedef SRANDOM_OPTION_SET_MAP::iterator		SRANDOM_OPTION_SET_MAP_ITER;
typedef SRANDOM_OPTION_SET_MAP::const_iterator	SRANDOM_OPTION_SET_MAP_CITER;

 struct SRANDOM_OPTION_GEN
 {
	enum { MAX_OPT = 4 };

	std::string strFileName;
	float	fRATE;
	float	fD_point;
	float	fS_value;

	WORD	wMinGain;
	WORD	wMaxGain;
	DWORD	dwMapSetSize;
	DWORD	dwMapSetRSize;

	WORD	wMaxGenD;
	WORD	wMaxGenR;

	SRANDOM_OPTION_SET_MAP	mapSET;
	SRANDOM_OPTION_SET_MAP	mapSETR;

	SRANDOM_OPTION_GEN()
		: fRATE(20.0f)
		, fD_point(0.0f)
		, fS_value(1.0f)
		, wMinGain(0)
		, wMaxGain(0)
		, dwMapSetSize(0)
		, dwMapSetRSize(0)
		, wMaxGenD(MAX_OPT)
		, wMaxGenR(MAX_OPT)
	{
		strFileName = "";
	}

	SRANDOM_OPTION_GEN ( const SRANDOM_OPTION_GEN &value )
	{
		operator=(value);
	}

	SRANDOM_OPTION_GEN& operator = ( const SRANDOM_OPTION_GEN& rvalue )
	{
		strFileName	= rvalue.strFileName;
		fRATE		= rvalue.fRATE;
		fD_point	= rvalue.fD_point;
		fS_value	= rvalue.fS_value;

		wMinGain = rvalue.wMinGain;
		wMaxGain = rvalue.wMaxGain;
		dwMapSetSize = rvalue.dwMapSetSize;
		dwMapSetRSize = rvalue.dwMapSetRSize;

		wMaxGenD = rvalue.wMaxGenD;
		wMaxGenR = rvalue.wMaxGenR;

		mapSET		= rvalue.mapSET;
		mapSETR		= rvalue.mapSETR;

		return *this;
	}

	void Reset()
	{
		strFileName	= "";
		fRATE		= 20.0f;
		fD_point	= 0.0f;
		fS_value	= 0.0f;

		wMinGain = 0;
		wMaxGain = 0;
		dwMapSetSize = 0;
		dwMapSetRSize = 0;

		wMaxGenD = MAX_OPT;
		wMaxGenR = MAX_OPT;

		mapSET.clear();
		mapSETR.clear();
	}


	 DWORD GetSetSize()		{ return (DWORD)mapSET.size(); }
	 DWORD GetSetRSize()	{ return (DWORD)mapSETR.size(); } 

	 WORD getMaxGenD() { return wMaxGenD; }
	 WORD getMaxGenR() { return wMaxGenR; }

	 void LoadGen_2 ( basestream &SFile );
	 void LoadGen_1 ( basestream &SFile );
	 void LoadGen_200 ( basestream &SFile );
	 void LoadGen ( basestream &SFile );
	 void SaveGen ( CSerialFile &SFile );

	 const bool loadCsv(CStringArray& StrArray);
	 const bool saveCsv(std::fstream& SFile);
	
	 BOOL CheckData();
 };

 typedef std::map<std::string,SRANDOM_OPTION_GEN>	SRANDOM_OPTION_GEN_MAP;
 typedef SRANDOM_OPTION_GEN_MAP::iterator			SRANDOM_OPTION_GEN_MAP_ITER;


 class GLRandomOptionManager
 {
 public:
	 enum
	 {
		 VERSION			= 0x0203,	

		 ENCODE_VER_BYTE_CRYPT_2021_RANDOM_OPTION_V1	= 0x0202,

		 VERSION_ENCODE2	= 0x0201,
		 VERSION_ENCODE		= 0x0200,
	 };

 public:
	 GLRandomOptionManager();
	 virtual ~GLRandomOptionManager();

 public:
	 static const char*	_FILEHEAD;
	 char			m_szFileName[MAX_PATH];

 public:
	 SRANDOM_OPTION_GEN_MAP		m_mapRandomOption;
	
 public:
	 void	CleanUp();

	 HRESULT LoadFile ( const char* szFile );
	 HRESULT SaveFile ( const char* szFile );
	 const bool	CSVLoad(CWnd* pWnd);
	 const bool	CSVSave(CWnd* pWnd);
	 void		saveCsvHead(std::fstream& SFile) const;

	 HRESULT Import ( const char* szFile );
	 DWORD	Export ();

 public:
	 SRANDOM_OPTION_GEN*	GetRandomOption( std::string strFile );

 public:
	 static GLRandomOptionManager& GetInstance ();
 };

#endif // GLRANDOM_OPTION_H
