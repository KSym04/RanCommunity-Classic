/*!
 * \file DxShaderData.h
 *
 * \author Juver
 * \date May 2018
 *
 * 
 */

#if !defined(_DXSHADER_DATA_INCLUDED_H_)
#define _DXSHADER_DATA_INCLUDED_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/SerialFile.h"

class DxShaderData
{
	enum 
	{
		SHADER_DATA_VERSION = 0x0200,
		VERSION_ENCODE		= 0x0200,
	};
public:
	typedef std::map<std::string,std::string>		SHADER_DATA_MAP;
	typedef SHADER_DATA_MAP::iterator				SHADER_DATA_MAP_ITER;

	std::string		m_strPATH;

	SHADER_DATA_MAP	m_mapShader;

public:
	void SetPath ( std::string strPATH )	{ m_strPATH = strPATH; }
	std::string GetPath ()					{ return m_strPATH.c_str(); }

	BOOL LoadFile( std::string strfile );
	BOOL SaveFile( std::string strfile );
	void CleanUp();

protected:
	DxShaderData();

public:
	~DxShaderData();

public:
	static DxShaderData& GetInstance();
};

#endif // !defined(_DXSHADER_DATA_INCLUDED_H_)
