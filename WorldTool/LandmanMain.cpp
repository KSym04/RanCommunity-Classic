// LandmanMain.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./LandmanMain.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./ToolPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLandmanMain dialog

IMPLEMENT_DYNAMIC(CLandmanMain, CPropertyPage)

CLandmanMain::CLandmanMain(LOGFONT logfont)
	: CPropertyPage(CLandmanMain::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanMain::~CLandmanMain()
{
	SAFE_DELETE( m_pFont );
}

void CLandmanMain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLandmanMain, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_SELECT, &CLandmanMain::OnBnClickedBtnWldSelect)
	ON_BN_CLICKED(IDC_BTN_MAKE_SHADOW_MESH, &CLandmanMain::OnBnClickedBtnMakeShadowMesh)
	ON_BN_CLICKED(IDC_BTN_MAKE_SHADOW_NAVI, &CLandmanMain::OnBnClickedBtnMakeShadowNavi)
	ON_BN_CLICKED(IDC_BTN_MAKE_SHADOW_NAVI2, &CLandmanMain::OnBnClickedBtnMakeShadowNavi2)
	ON_BN_CLICKED(IDC_BTN_NAVIGATION_MESH_REPLACE, &CLandmanMain::OnBnClickedBtnNavigationMeshReplace)
END_MESSAGE_MAP()


// CLandmanMain message handlers
BOOL CLandmanMain::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CLandmanTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	std::string strpages[EMWLD_PAGE_SIZE] = 
	{
		"Main Page",
		"Basic Page",
		"Fog Property Page",
		"Sky Property Page",
		"BGM Property Page",
		"SoundMan Page",
		"Effect Page",
	};

	SetWin_Combo_Init( this, IDC_CMB_WLD_SELECT, strpages, EMWLD_PAGE_SIZE );

	return TRUE;  
}

void CLandmanMain::SetData( DxLandMan* pData )
{
	m_pData = pData;

}

void CLandmanMain::ResetData()
{
	m_pData = NULL;
}

void CLandmanMain::OnBnClickedBtnWldSelect()
{
	int nPAGE = GetWin_Combo_Sel( this, IDC_CMB_WLD_SELECT );

	if ( m_pTab )
		m_pTab->PageActive( nPAGE );
}

void CLandmanMain::OnBnClickedBtnMakeShadowMesh()
{
	if ( m_pData )
		m_pData->CopyMeshAndCreateShadowMesh( CWorldToolView::GetView()->Get3DDevice() );
}

void CLandmanMain::OnBnClickedBtnMakeShadowNavi()
{
	if ( m_pData )
		m_pData->CopyNaviAndCreateShadowMesh( CWorldToolView::GetView()->Get3DDevice() );
}


void CLandmanMain::OnBnClickedBtnMakeShadowNavi2()
{
	CString szFilter = "navi|*.navi|";
	CFileDialog dlg ( TRUE,".navi", DxLandMan::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = DxLandMan::GetPath ();
	if ( dlg.DoModal() != IDOK )	return;

	std::string strFile = dlg.GetFileName().GetString();

	if ( m_pData )
		m_pData->NaviFileCreateShadowMesh( CWorldToolView::GetView()->Get3DDevice(), strFile.c_str() );
}


void CLandmanMain::OnBnClickedBtnNavigationMeshReplace()
{
	CString szFilter = "navi|*.navi|";
	CFileDialog dlg(TRUE, ".navi", DxLandMan::GetPath(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = DxLandMan::GetPath();
	if (dlg.DoModal() != IDOK)	return;

	std::string strFile = dlg.GetPathName().GetString();

	if (m_pData)
	{
		BOOL bOK = m_pData->ReplaceNagivationMesh(strFile, CWorldToolView::GetView()->Get3DDevice());
		if (bOK)
			CDebugSet::MsgBox("Replaced Done :%s", strFile.c_str());
		else
			CDebugSet::MsgBox("Replaced Failed :%s", strFile.c_str());
	}
}
