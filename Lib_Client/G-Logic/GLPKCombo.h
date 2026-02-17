/*!
 * \file GLPKCombo.h
 *
 * \author EJCode
 * \date November 2018
 *
 * 
 */

#ifndef _GLPK_COMBO_
#define _GLPK_COMBO_

#include "./GLPKComboDefine.h"

class GLPKCombo
{
public:
	BOOL		bPKComboUse;

	std::string strPKComboSoundText[EMPK_COMBO_TYPE_SIZE];
	std::string strPKComboSoundFile[EMPK_COMBO_TYPE_SIZE];
	float		fPKComboDelayTime[EMPK_COMBO_TYPE_SIZE];

public:
	void CleanUp();
	void LoadFile ( std::string file_name );

	void LoadSound();

public:
	GLPKCombo();
	~GLPKCombo();

public:
	static GLPKCombo& GetInstance ();

};

#endif