// LandmanSky.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./LandmanSky.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./ToolPages.h"
#include "./SelColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLandmanSky dialog

IMPLEMENT_DYNAMIC(CLandmanSky, CPropertyPage)

CLandmanSky::CLandmanSky(LOGFONT logfont)
	: CPropertyPage(CLandmanSky::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanSky::~CLandmanSky()
{
	SAFE_DELETE( m_pFont );
}

void CLandmanSky::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLandmanSky, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_CLOSE, &CLandmanSky::OnBnClickedBtnWldClose)
	ON_BN_CLICKED(IDC_BTN_WLD_APPLY, &CLandmanSky::OnBnClickedBtnWldApply)
END_MESSAGE_MAP()


// CLandmanSky message handlers
BOOL CLandmanSky::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CLandmanTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	ShowData();

	return TRUE;  
}

void CLandmanSky::SetData( DxLandMan* pData )
{
	m_pData = pData;

	ShowData();
}

void CLandmanSky::ResetData()
{
	m_pData = NULL;
}

void CLandmanSky::OnBnClickedBtnWldClose()
{
	if ( m_pTab )
		m_pTab->PageActive( EMWLD_PAGE_MAIN );
}

void CLandmanSky::OnBnClickedBtnWldApply()
{
	SaveData();
}

BOOL CLandmanSky::ShowData()
{
	if ( !m_pData )	return FALSE;

	SKY_PROPERTY& prop = m_pData->GetSkyProperty();
	SetWin_Check( this, IDC_CHK_SKY_ENABLE, prop.m_bSkyEnable );
	SetWin_Check( this, IDC_CHK_CLOUD_ENABLE, prop.m_bCloudEnable );
	SetWin_Num_float( this, IDC_EDT_AXIS_VALUE, prop.m_fAxisValue );
	SetWin_Num_int( this, IDC_EDT_RADIO_AXIS, prop.m_nRadioAxis );

	SetWin_Check( this, IDC_CHK_UNKNOWN_DATA1, prop.unknown_data1 );
	SetWin_Check( this, IDC_CHK_UNKNOWN_DATA2, prop.unknown_data2 );
	
	return TRUE;
}

BOOL CLandmanSky::SaveData()
{
	if ( !m_pData )	return FALSE;

	SKY_PROPERTY& prop = m_pData->GetSkyProperty();
	prop.m_bSkyEnable = GetWin_Check( this, IDC_CHK_SKY_ENABLE );
	prop.m_bCloudEnable = GetWin_Check( this, IDC_CHK_CLOUD_ENABLE );
	prop.m_fAxisValue = GetWin_Num_float( this, IDC_EDT_AXIS_VALUE );
	prop.m_nRadioAxis = GetWin_Num_int( this, IDC_EDT_RADIO_AXIS );

	prop.unknown_data1 = GetWin_Check( this, IDC_CHK_UNKNOWN_DATA1 );
	prop.unknown_data2 = GetWin_Check( this, IDC_CHK_UNKNOWN_DATA2 );

	DxSkyMan::GetInstance().SetProperty( prop );
	

	return TRUE;
}