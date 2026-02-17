// LandmanBasic.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./LandmanBasic.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"
#include "ToolPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLandmanBasic dialog

IMPLEMENT_DYNAMIC(CLandmanBasic, CPropertyPage)

CLandmanBasic::CLandmanBasic(LOGFONT logfont)
	: CPropertyPage(CLandmanBasic::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanBasic::~CLandmanBasic()
{
	SAFE_DELETE( m_pFont );
}

void CLandmanBasic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLandmanBasic, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_CLOSE, &CLandmanBasic::OnBnClickedBtnWldClose)
	ON_BN_CLICKED(IDC_BTN_WLD_APPLY, &CLandmanBasic::OnBnClickedBtnWldApply)
END_MESSAGE_MAP()


// CLandmanBasic message handlers
BOOL CLandmanBasic::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CLandmanTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	ShowData();

	return TRUE;  
}

void CLandmanBasic::SetData( DxLandMan* pData )
{
	m_pData = pData;

	ShowData();
}

void CLandmanBasic::ResetData()
{
	m_pData = NULL;
}

void CLandmanBasic::OnBnClickedBtnWldClose()
{
	if ( m_pTab )
		m_pTab->PageActive( EMWLD_PAGE_MAIN );
}

void CLandmanBasic::OnBnClickedBtnWldApply()
{
	SaveData();
}

BOOL CLandmanBasic::ShowData()
{
	if ( !m_pData )	return FALSE;

	SetWin_Check( this, IDC_CHK_BASIC_LEGACY_SHADOW, m_pData->m_sbasic.m_blegacy_shadow );
	SetWin_Num_float( this, IDC_EDT_BASIC_VERTEX_MULTI, m_pData->m_sbasic.m_fVS_1_1_ColorMulti );
	
	return TRUE;
}

BOOL CLandmanBasic::SaveData()
{
	if ( !m_pData )	return FALSE;

	m_pData->m_sbasic.m_blegacy_shadow = GetWin_Check( this, IDC_CHK_BASIC_LEGACY_SHADOW );
	m_pData->m_sbasic.m_fVS_1_1_ColorMulti = GetWin_Num_float( this, IDC_EDT_BASIC_VERTEX_MULTI );
	
	return TRUE;
}