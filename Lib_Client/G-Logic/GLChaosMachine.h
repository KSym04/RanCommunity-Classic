/*!
 * \file GLChaosMachine.h
 *
 * \author Juver
 * \date 2021/07/07
 *
 * 
 */

#ifndef GLCHAOSMACHINE_H_INCLUDED__
#define GLCHAOSMACHINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLChaosMachineDefine.h"

class GLChaosMachine
{
public:
	SCHAOS_MACHINE_DATA_MAP	m_mapData;

public:
	void ClearData();
	void LoadData();
	void CopyItemResult();

	SCHAOS_MACHINE_DATA* GetData( DWORD dwID );

public:
	GLChaosMachine();
	~GLChaosMachine();

	static GLChaosMachine& GetInstance ();
};

#endif // GLCHAOSMACHINE_H_INCLUDED__
