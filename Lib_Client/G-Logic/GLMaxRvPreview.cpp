#include "stdafx.h"
#include "./GLMaxRvPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


SMAXRV_OPTION::SMAXRV_OPTION()
	: m_emOption(EMR_OPT_NULL)
	, m_fMin(0.0f)
	, m_fMax(0.0f)
{

}

SMAXRV_PREVIEW::SMAXRV_PREVIEW()
	: m_sItemID(false)
	, m_dwOptionMobGenWait(0)
	, m_dwOptionRebuildWait(0)
{

}

SMAXRV_PREVIEW::~SMAXRV_PREVIEW()
{

}

void SMAXRV_PREVIEW::Reset()
{
	m_sItemID.ResetNativeID();
	m_dwOptionMobGenWait = 0;
	m_dwOptionRebuildWait = 0;

	m_vecOptionMobGen.clear();
	m_vecOptionRebuild.clear();

	m_sItemCustom = SITEMCUSTOM(false);
}

BOOL SMAXRV_PREVIEW::CheckComplete() const
{
	if ( m_dwOptionMobGenWait != 0 && 
		m_dwOptionMobGenWait != m_vecOptionMobGen.size() )
		return FALSE;

	if ( m_dwOptionRebuildWait != 0 && 
		m_dwOptionRebuildWait != m_vecOptionRebuild.size() )
		return FALSE;

	return TRUE;
}