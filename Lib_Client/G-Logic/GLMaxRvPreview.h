/*!
 * \file GLMaxRvPreview.h
 *
 * \author Juver
 * \date 2022/02/25
 *
 * 
 */

#ifndef GLMAXRVPREVIEW_H_INCLUDED__
#define GLMAXRVPREVIEW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLItemDef.h"
#include "./GLItem.h"

enum EMMAXRV_PREVIEW_TYPE
{
	EMMAXRV_PREVIEW_NONE	= 0,
	EMMAXRV_PREVIEW_MOBGEN	= 1,
	EMMAXRV_PREVIEW_REBUILD = 2,
};

struct SMAXRV_OPTION
{
	EMRANDOM_OPT m_emOption;
	float m_fMin;
	float m_fMax;

	SMAXRV_OPTION();
};

typedef std::vector<SMAXRV_OPTION> SMAXRV_OPTION_VEC;

struct SMAXRV_PREVIEW
{
	SNATIVEID m_sItemID;

	DWORD m_dwOptionMobGenWait;
	DWORD m_dwOptionRebuildWait;
	SMAXRV_OPTION_VEC m_vecOptionMobGen;
	SMAXRV_OPTION_VEC m_vecOptionRebuild;

	SITEMCUSTOM	m_sItemCustom;

	SMAXRV_PREVIEW();
	~SMAXRV_PREVIEW();

	void Reset();

	BOOL CheckComplete() const;
};


#endif // GLMAXRVPREVIEW_H_INCLUDED__
