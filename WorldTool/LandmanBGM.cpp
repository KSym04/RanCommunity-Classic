// LandmanBGM.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./LandmanBGM.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./ToolPages.h"
#include "./SelColorDlg.h"
#include "../Lib_Engine/DxSound/BgmSound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLandmanBGM dialog

IMPLEMENT_DYNAMIC(CLandmanBGM, CPropertyPage)

CLandmanBGM::CLandmanBGM(LOGFONT logfont)
	: CPropertyPage(CLandmanBGM::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanBGM::~CLandmanBGM()
{
	SAFE_DELETE( m_pFont );
}

void CLandmanBGM::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLandmanBGM, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_CLOSE, &CLandmanBGM::OnBnClickedBtnWldClose)
	ON_BN_CLICKED(IDC_BTN_WLD_APPLY, &CLandmanBGM::OnBnClickedBtnWldApply)
	ON_BN_CLICKED(IDC_BTN_BGM, &CLandmanBGM::OnBnClickedBtnBgm)
END_MESSAGE_MAP()


// CLandmanBGM message handlers
BOOL CLandmanBGM::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CLandmanTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	ShowData();

	return TRUE;  
}

void CLandmanBGM::SetData( DxLandMan* pData )
{
	m_pData = pData;

	ShowData();
}

void CLandmanBGM::ResetData()
{
	m_pData = NULL;
}

void CLandmanBGM::OnBnClickedBtnWldClose()
{
	if ( m_pTab )
		m_pTab->PageActive( EMWLD_PAGE_MAIN );
}

void CLandmanBGM::OnBnClickedBtnWldApply()
{
	SaveData();
}

BOOL CLandmanBGM::ShowData()
{
	if ( !m_pData )	return FALSE;

	SetWin_Text( this, IDC_EDT_BGM, m_pData->GetBgmFile().GetString() );
	
	return TRUE;
}

BOOL CLandmanBGM::SaveData()
{
	if ( !m_pData )	return FALSE;

	CString strText = GetWin_Text( this, IDC_EDT_BGM ).GetString();
	m_pData->SetBgmFile( strText.GetString() );
	
	
	DxBgmSound::GetInstance().SetFile ( strText.GetString() );
	DxBgmSound::GetInstance().Stop();
	DxBgmSound::GetInstance().Play ();

	return TRUE;
}

void CLandmanBGM::OnBnClickedBtnBgm()
{
	CString szFilter = "BGM (*.ogg)|*.OGG|";
	CFileDialog dlg(TRUE,".ogg",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = DxBgmSound::GetInstance().GetPath();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDT_BGM, dlg.GetFileName().GetString() );
}
