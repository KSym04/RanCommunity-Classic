/*!
 * \file GLPartyFinder.h
 *
 * \author Juver
 * \date 2019/12/29
 *
 * 
 */

#ifndef GLPARTYFINDER_H_INCLUDED__
#define GLPARTYFINDER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLPartyFinderDef.h"

class GLPartyFinder
{
public:
	SPARTY_FINDER_MAP_DATA_MAP	m_mapPartyFinderMap;

public:
	void CleanData();
	void LoadData();

	SPARTY_FINDER_MAP_DATA* GetData( DWORD dwID );

public:
	static GLPartyFinder& GetInstance ();

	GLPartyFinder();
	~GLPartyFinder();
};

#endif // GLPARTYFINDER_H_INCLUDED__
