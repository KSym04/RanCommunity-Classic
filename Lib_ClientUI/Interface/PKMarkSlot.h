/*!
 * \file PKMarkSlot.h
 *
 * \author Juver
 * \date 2022/02/14
 *
 * 
 */

#ifndef PKMARKSLOT_H_INCLUDED__
#define PKMARKSLOT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLItemDef.h"
#include "../../Lib_Client/G-Logic/GLPKMark.h"

class CBasicTextBoxEx;
class CBasicLineBox;

class CPKMarkSlot : public CUIGroup
{
	enum
	{
		PKMARK_SLOT_IMAGE_CLASS		= GLCI_NUM_8CLASS,
		PKMARK_SLOT_IMAGE_SCHOOL	= 3,
		PKMARK_SLOT_IMAGE_WEAPON	= ITEMATT_NSIZE,
	};

public:
	CPKMarkSlot();
	virtual ~CPKMarkSlot();
	virtual	HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void CreateSubControl();

public:
	CUIControl*	m_pImageClassA[PKMARK_SLOT_IMAGE_CLASS];
	CUIControl*	m_pImageSchoolA[PKMARK_SLOT_IMAGE_SCHOOL];
	CUIControl*	m_pImageClubMarkA;

	CUIControl*	m_pImageClassB[PKMARK_SLOT_IMAGE_CLASS];
	CUIControl*	m_pImageSchoolB[PKMARK_SLOT_IMAGE_SCHOOL];
	CUIControl*	m_pImageClubMarkB;

	CBasicTextBoxEx* m_pTextNameA;
	CBasicTextBoxEx* m_pTextNameB;
	CBasicLineBox* m_pLineBoxBG;

	CUIControl*	m_pWeaponImage[PKMARK_SLOT_IMAGE_WEAPON];

	SPKMARK_INFO m_sMarkData;
public:
	void ResetData();
	void SetData( const SPKMARK_INFO& sData );
};


#endif // PKMARKSLOT_H_INCLUDED__
