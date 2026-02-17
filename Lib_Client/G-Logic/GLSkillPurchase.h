/*!
 * \file GLSkillPurchase.h
 *
 * \author Juver
 * \date 2022/03/08
 *
 * 
 */

#ifndef GLSKILLPURCHASE_H_INCLUDED__
#define GLSKILLPURCHASE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"

struct SSKILL_PURCHASE
{
	SNATIVEID sSkillID;
	DWORD	dwCost;

	SSKILL_PURCHASE()
		: sSkillID(false)
		, dwCost(0)
	{
	}
};

typedef std::map<DWORD, SSKILL_PURCHASE>	SKILL_PURCHASE_MAP;
typedef SKILL_PURCHASE_MAP::iterator		SKILL_PURCHASE_MAP_ITER;


class GLSkillPurchase
{
public:
	GLSkillPurchase();
	~GLSkillPurchase();

	static GLSkillPurchase& GetInstance ();

public:
	SKILL_PURCHASE_MAP	m_mapSkillPurchase;

public:
	void CleanUp();
	void LoadFile( std::string strFileName );

	SSKILL_PURCHASE* getSkillPurchase( SNATIVEID sSkillID );
};


#endif // GLSKILLPURCHASE_H_INCLUDED__
