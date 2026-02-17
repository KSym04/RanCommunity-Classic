// LandmanFog.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./LandmanFog.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./ToolPages.h"
#include "./SelColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLandmanFog dialog

IMPLEMENT_DYNAMIC(CLandmanFog, CPropertyPage)

CLandmanFog::CLandmanFog(LOGFONT logfont)
	: CPropertyPage(CLandmanFog::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanFog::~CLandmanFog()
{
	SAFE_DELETE( m_pFont );
}

void CLandmanFog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLandmanFog, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_CLOSE, &CLandmanFog::OnBnClickedBtnWldClose)
	ON_BN_CLICKED(IDC_BTN_WLD_APPLY, &CLandmanFog::OnBnClickedBtnWldApply)
	ON_BN_CLICKED(IDC_BTN_FOG_COLOR, &CLandmanFog::OnBnClickedBtnFogColor)
	ON_BN_CLICKED(IDC_BTN_FOG_COLOR_DAY, &CLandmanFog::OnBnClickedBtnFogColorDay)
	ON_BN_CLICKED(IDC_BTN_FOG_COLOR_NIGHT, &CLandmanFog::OnBnClickedBtnFogColorNight)
END_MESSAGE_MAP()


// CLandmanFog message handlers
BOOL CLandmanFog::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CLandmanTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	ShowData();

	return TRUE;  
}

void CLandmanFog::SetData( DxLandMan* pData )
{
	m_pData = pData;

	ShowData();
}

void CLandmanFog::ResetData()
{
	m_pData = NULL;
}

void CLandmanFog::OnBnClickedBtnWldClose()
{
	if ( m_pTab )
		m_pTab->PageActive( EMWLD_PAGE_MAIN );
}

void CLandmanFog::OnBnClickedBtnWldApply()
{
	SaveData();
}

BOOL CLandmanFog::ShowData()
{
	if ( !m_pData )	return FALSE;

	FOG_PROPERTY& prop = m_pData->GetFogProperty();

	SetWin_Check( this, IDC_CHK_NEW_RANGE, prop.m_bNewRange );
	SetWin_Check( this, IDC_CHK_SKY_FOG, prop.m_bSkyFogEnable );
	SetWin_Check( this, IDC_CHK_STATIC_RANGE, prop.m_bFogStaticRange );

	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_A, RGBA_GETALPHA( prop.m_cFogColor ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_R, RGBA_GETRED( prop.m_cFogColor ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_G, RGBA_GETGREEN( prop.m_cFogColor ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_B, RGBA_GETBLUE( prop.m_cFogColor ) );
	
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_A, RGBA_GETALPHA( prop.m_cFogColor_Day ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_R, RGBA_GETRED( prop.m_cFogColor_Day ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_G, RGBA_GETGREEN( prop.m_cFogColor_Day ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_B, RGBA_GETBLUE( prop.m_cFogColor_Day ) );
	
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_A, RGBA_GETALPHA( prop.m_cFogColor_Night ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_R, RGBA_GETRED( prop.m_cFogColor_Night ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_G, RGBA_GETGREEN( prop.m_cFogColor_Night ) );
	SetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_B, RGBA_GETBLUE( prop.m_cFogColor_Night ) );
	
	SetWin_Num_float( this, IDC_EDT_FOG_START, prop.m_fFogStart );
	SetWin_Num_float( this, IDC_EDT_FOG_END, prop.m_fFogEnd );

	return TRUE;
}

BOOL CLandmanFog::SaveData()
{
	if ( !m_pData )	return FALSE;

	FOG_PROPERTY& prop = m_pData->GetFogProperty();
	prop.m_bNewRange = GetWin_Check( this, IDC_CHK_NEW_RANGE );
	prop.m_bSkyFogEnable = GetWin_Check( this, IDC_CHK_SKY_FOG );
	prop.m_bFogStaticRange = GetWin_Check( this, IDC_CHK_STATIC_RANGE );

	prop.m_cFogColor = RGBA_MAKE(
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_R ), 
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_G ), 
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_B ),
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_A ));

	prop.m_cFogColor_Day = RGBA_MAKE(
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_R ), 
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_G ), 
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_B ),
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_A ));

	prop.m_cFogColor_Night = RGBA_MAKE(
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_R ), 
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_G ), 
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_B ),
		GetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_A ));

	prop.m_fFogStart = GetWin_Num_float( this, IDC_EDT_FOG_START );
	prop.m_fFogEnd = GetWin_Num_float( this, IDC_EDT_FOG_END );

	DxFogMan::GetInstance().SetProperty( m_pData->GetFogProperty() );

	return TRUE;
}

void CLandmanFog::OnBnClickedBtnFogColor()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_R );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_G );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_B );

	CSelColorDlg dlg( nCUR_R, nCUR_G, nCUR_B );
	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_R, GetRValue( crNewColor ) );
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_G, GetGValue( crNewColor ) );
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_B, GetBValue( crNewColor ) );
	}
}

void CLandmanFog::OnBnClickedBtnFogColorDay()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_R );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_G );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_B );

	CSelColorDlg dlg( nCUR_R, nCUR_G, nCUR_B );
	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_R, GetRValue( crNewColor ) );
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_G, GetGValue( crNewColor ) );
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_DAY_B, GetBValue( crNewColor ) );
	}
}

void CLandmanFog::OnBnClickedBtnFogColorNight()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_R );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_G );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_B );

	CSelColorDlg dlg( nCUR_R, nCUR_G, nCUR_B );
	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_R, GetRValue( crNewColor ) );
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_G, GetGValue( crNewColor ) );
		SetWin_Num_int( this, IDC_EDT_FOG_COLOR_NIGHT_B, GetBValue( crNewColor ) );
	}
}
