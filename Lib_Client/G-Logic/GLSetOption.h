/*!
 * \file GLSetOption.h
 *
 * \author Juver
 * \date 2021/07/30
 *
 * 
 */

#ifndef GLSETOPTION_H_INCLUDED__
#define GLSETOPTION_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>

#include "./GLSetOptionDefine.h"

class GLSetOptionData
{
public:
	GLSetOptionData();
	~GLSetOptionData();

	static GLSetOptionData& GetInstance ();

	enum
	{
		VERSION				= 0x0100, 
		VERSION_ENCODE		= 0x0100,
	};

public:
	static const char*	_FILEHEAD;

	std::string			m_strFileName;
	SET_OPTION_DATA_MAP	m_mapSetOptionData;

public:
	void CleanUp();
	BOOL LoadSetOptionData( std::string strFile );
	BOOL SaveSetOptionData( std::string strFile );
	const bool	CSVLoad(CWnd* pWnd);
	const bool	CSVSave(CWnd* pWnd);

	SSET_OPTION_DATA*	GetData( WORD wID );
};

class GLSetOptionGen
{
public:
	GLSetOptionGen();
	~GLSetOptionGen();

	static GLSetOptionGen& GetInstance ();

	enum
	{
		VERSION				= 0x0100, 
		VERSION_ENCODE		= 0x0100,
	};

public:
	static const char*	_FILEHEAD;

	std::string			m_strFileName;
	SET_OPTION_GEN_MAP	m_mapSetOptionGen;

public:
	void CleanUp();
	BOOL LoadSetOptionGen( std::string strFile );
	BOOL SaveSetOptionGen( std::string strFile );
	const bool	CSVLoad(CWnd* pWnd);
	const bool	CSVSave(CWnd* pWnd);

	SSET_OPTION_GEN*	GetData( std::string strName );

	WORD GenerateSetOptionID( std::string strFile );
};


#endif // GLSETOPTION_H_INCLUDED__
