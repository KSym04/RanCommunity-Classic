/*!
 * \file MaxRvPreviewPageSlot.h
 *
 * \author Juver
 * \date 2022/02/25
 *
 * 
 */

#ifndef MAXRVPREVIEWPAGESLOT_H_INCLUDED__
#define MAXRVPREVIEWPAGESLOT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLMaxRvPreview.h"

class CBasicTextBoxEx;

class CMaxRvPreviewPageSlot : public CUIGroup
{
public:
	CMaxRvPreviewPageSlot ();
	virtual	~CMaxRvPreviewPageSlot ();

public:
	CBasicTextBoxEx*	m_pTextOption;
	CBasicTextBoxEx*	m_pTextValueMin;
	CBasicTextBoxEx*	m_pTextValueMax;

public:
	void	CreateSubControl ();
	void	ResetText();
	void	SetOption( EMRANDOM_OPT emOption, float fMin, float fMax );
	float	GetOptionValue( EMRANDOM_OPT emOption, float fValue );
	BOOL	IsOptionPer( EMRANDOM_OPT emOption );
};

#endif // MAXRVPREVIEWPAGESLOT_H_INCLUDED__