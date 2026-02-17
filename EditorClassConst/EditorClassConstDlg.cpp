
// EditorClassConstDlg.cpp : implementation file
//

#include "stdafx.h"

#include "./EditorClassConst.h"
#include "./EditorClassConstDlg.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"

#include "../Lib_Engine/DxResponseMan.h"

#include "../Lib_Engine/G-Logic/GLOGIC.h"
#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Helper/EtcFunction.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorClassConstDlg dialog




CEditorClassConstDlg::CEditorClassConstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorClassConstDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorClassConstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CEditorClassConstDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON_FILE_LOAD, &CEditorClassConstDlg::OnBnClickedButtonFileLoad)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAVE, &CEditorClassConstDlg::OnBnClickedButtonFileSave)
	ON_BN_CLICKED(IDC_BUTTON_FILE_IMPORT, &CEditorClassConstDlg::OnBnClickedButtonFileImport)
	ON_BN_CLICKED(IDC_BUTTON_FILE_EXPORT, &CEditorClassConstDlg::OnBnClickedButtonFileExport)
END_MESSAGE_MAP()


// CEditorClassConstDlg message handlers

BOOL CEditorClassConstDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();

	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	DataShow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorClassConstDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEditorClassConstDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorClassConstDlg::PostNcDestroy()
{
	//GLTaxiStation::GetInstance().CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}

void CEditorClassConstDlg::OnBnClickedButtonFileLoad()
{
	CString szFilter = "classconst (*.classconst)|*.classconst|";
	CFileDialog dlg( TRUE, ".classconst", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		if( m_CharClass.LOADFILE_STREAM( dlg.GetFileName().GetString() ) )
		{
			SetWin_Text( this, IDC_EDIT_FILE, dlg.GetPathName().GetString() );
			m_strFileName = dlg.GetFileName().GetString();
			DataShow();
		}
		else
			CDebugSet::MsgBox( GetSafeHwnd(), "LOADFILE_STREAM Failed: %s", dlg.GetPathName().GetString() );
	}
}

void CEditorClassConstDlg::OnBnClickedButtonFileSave()
{
	CString szFilter = "classconst (*.classconst)|*.classconst|";

	CString strsavefile = GLOGIC::GetPath();
	strsavefile += m_strFileName.c_str();

	CFileDialog dlg( FALSE, ".classconst", strsavefile.GetString(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		DataSave();

		if( m_CharClass.SAVEFILE_STREAM( dlg.GetFileName().GetString() ) )
		{
			SetWin_Text( this, IDC_EDIT_FILE, dlg.GetPathName().GetString() );
			m_strFileName = dlg.GetFileName().GetString();
			DataShow();
		}
		else
			CDebugSet::MsgBox( GetSafeHwnd(), "SAVEFILE_STREAM Failed: %s", dlg.GetPathName().GetString() );
	}
}

void CEditorClassConstDlg::OnBnClickedButtonFileImport()
{
	CString szFilter = "classconst (*.classconst)|*.classconst|";
	CFileDialog dlg( TRUE, ".classconst", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		if( m_CharClass.LOADFILE_STRING( dlg.GetFileName().GetString() ) )
		{
			SetWin_Text( this, IDC_EDIT_FILE, dlg.GetPathName().GetString() );
			m_strFileName = dlg.GetFileName().GetString();
			DataShow();
		}
		else
			CDebugSet::MsgBox( GetSafeHwnd(), "LOADFILE_STRING Failed: %s", dlg.GetPathName().GetString() );
	}
}

void CEditorClassConstDlg::DataShow()
{
	SetWin_Num_float( this, IDC_EDIT_WALK_VELO, m_CharClass.fWALKVELO );
	SetWin_Num_float( this, IDC_EDIT_RUN_VELO, m_CharClass.fRUNVELO );

	SetWin_Num_float( this, IDC_EDIT_HP_STR, m_CharClass.fHP_STR );
	SetWin_Num_float( this, IDC_EDIT_MP_SPI, m_CharClass.fMP_SPI );
	SetWin_Num_float( this, IDC_EDIT_SP_STA, m_CharClass.fSP_STA );

	SetWin_Num_float( this, IDC_EDIT_HIT_DEX, m_CharClass.fHIT_DEX );
	SetWin_Num_float( this, IDC_EDIT_AVOID_DEX, m_CharClass.fAVOID_DEX );
	SetWin_Num_float( this, IDC_EDIT_DEFENSE_DEX, m_CharClass.fDEFENSE_DEX );

	SetWin_Num_float( this, IDC_EDIT_PA_POW, m_CharClass.fPA_POW );
	SetWin_Num_float( this, IDC_EDIT_PA_DEX, m_CharClass.fPA_DEX );

	SetWin_Num_float( this, IDC_EDIT_SA_POW, m_CharClass.fSA_POW );
	SetWin_Num_float( this, IDC_EDIT_SA_DEX, m_CharClass.fSA_DEX );

	SetWin_Num_float( this, IDC_EDIT_MA_DEX, m_CharClass.fMA_DEX );
	SetWin_Num_float( this, IDC_EDIT_MA_SPI, m_CharClass.fMA_SPI );
	SetWin_Num_float( this, IDC_EDIT_MA_INT, m_CharClass.fMA_INT );

	SetWin_Num_float( this, IDC_EDIT_CONV_AP, m_CharClass.fCONV_AP );
	SetWin_Num_float( this, IDC_EDIT_CONV_DP, m_CharClass.fCONV_DP );
	SetWin_Num_float( this, IDC_EDIT_CONV_PA, m_CharClass.fCONV_PA );
	SetWin_Num_float( this, IDC_EDIT_CONV_SA, m_CharClass.fCONV_SA );

	SetWin_Num_int( this, IDC_EDIT_BEGIN_AP, m_CharClass.wBEGIN_AP );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_DP, m_CharClass.wBEGIN_DP );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_PA, m_CharClass.wBEGIN_PA );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_SA, m_CharClass.wBEGIN_SA );

	SetWin_Num_float( this, IDC_EDIT_LEVELUP_AP, m_CharClass.fLVLUP_AP );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_DP, m_CharClass.fLVLUP_DP );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_PA, m_CharClass.fLVLUP_PA );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_SA, m_CharClass.fLVLUP_SA );

	SetWin_Num_int( this, IDC_EDIT_BEGIN_POW, m_CharClass.sBEGIN_STATS.wPow );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_STR, m_CharClass.sBEGIN_STATS.wStr );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_SPI, m_CharClass.sBEGIN_STATS.wSpi );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_DEX, m_CharClass.sBEGIN_STATS.wDex );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_INT, m_CharClass.sBEGIN_STATS.wInt );
	SetWin_Num_int( this, IDC_EDIT_BEGIN_STA, m_CharClass.sBEGIN_STATS.wSta );

	SetWin_Num_float( this, IDC_EDIT_LEVELUP_POW, m_CharClass.sLVLUP_STATS.fPow );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_STR, m_CharClass.sLVLUP_STATS.fStr );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_SPI, m_CharClass.sLVLUP_STATS.fSpi );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_DEX, m_CharClass.sLVLUP_STATS.fDex );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_INT, m_CharClass.sLVLUP_STATS.fInt );
	SetWin_Num_float( this, IDC_EDIT_LEVELUP_STA, m_CharClass.sLVLUP_STATS.fSta );

	SetWin_Num_int( this, IDC_EDIT_MAX_HEAD, m_CharClass.dwHEADNUM );
	SetWin_Num_int( this, IDC_EDIT_SEL_HEAD, m_CharClass.dwHEADNUM_SELECT );
	SetWin_Text( this, IDC_EDIT_HEAD_00, m_CharClass.strHEAD_CPS[0].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_01, m_CharClass.strHEAD_CPS[1].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_02, m_CharClass.strHEAD_CPS[2].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_03, m_CharClass.strHEAD_CPS[3].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_04, m_CharClass.strHEAD_CPS[4].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_05, m_CharClass.strHEAD_CPS[5].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_06, m_CharClass.strHEAD_CPS[6].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_07, m_CharClass.strHEAD_CPS[7].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_08, m_CharClass.strHEAD_CPS[8].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_09, m_CharClass.strHEAD_CPS[9].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_10, m_CharClass.strHEAD_CPS[10].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_11, m_CharClass.strHEAD_CPS[11].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_12, m_CharClass.strHEAD_CPS[12].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_13, m_CharClass.strHEAD_CPS[13].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_14, m_CharClass.strHEAD_CPS[14].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_15, m_CharClass.strHEAD_CPS[15].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_16, m_CharClass.strHEAD_CPS[16].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_17, m_CharClass.strHEAD_CPS[17].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_18, m_CharClass.strHEAD_CPS[18].c_str() );
	SetWin_Text( this, IDC_EDIT_HEAD_19, m_CharClass.strHEAD_CPS[19].c_str() );

	SetWin_Num_int( this, IDC_EDIT_MAX_HAIR, m_CharClass.dwHAIRNUM );
	SetWin_Num_int( this, IDC_EDIT_SEL_HAIR, m_CharClass.dwHAIRNUM_SELECT );
	SetWin_Text( this, IDC_EDIT_HAIR_00, m_CharClass.strHAIR_CPS[0].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_01, m_CharClass.strHAIR_CPS[1].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_02, m_CharClass.strHAIR_CPS[2].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_03, m_CharClass.strHAIR_CPS[3].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_04, m_CharClass.strHAIR_CPS[4].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_05, m_CharClass.strHAIR_CPS[5].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_06, m_CharClass.strHAIR_CPS[6].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_07, m_CharClass.strHAIR_CPS[7].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_08, m_CharClass.strHAIR_CPS[8].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_09, m_CharClass.strHAIR_CPS[9].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_10, m_CharClass.strHAIR_CPS[10].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_11, m_CharClass.strHAIR_CPS[11].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_12, m_CharClass.strHAIR_CPS[12].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_13, m_CharClass.strHAIR_CPS[13].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_14, m_CharClass.strHAIR_CPS[14].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_15, m_CharClass.strHAIR_CPS[15].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_16, m_CharClass.strHAIR_CPS[16].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_17, m_CharClass.strHAIR_CPS[17].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_18, m_CharClass.strHAIR_CPS[18].c_str() );
	SetWin_Text( this, IDC_EDIT_HAIR_19, m_CharClass.strHAIR_CPS[19].c_str() );

	SetWin_Text( this, IDC_EDIT_CLASS_EFFECT, m_CharClass.strCLASS_EFFECT.c_str() );
}

void CEditorClassConstDlg::DataSave()
{
	m_CharClass.fWALKVELO = GetWin_Num_float( this, IDC_EDIT_WALK_VELO );
	m_CharClass.fRUNVELO = GetWin_Num_float( this, IDC_EDIT_RUN_VELO );

	m_CharClass.fHP_STR = GetWin_Num_float( this, IDC_EDIT_HP_STR );
	m_CharClass.fMP_SPI = GetWin_Num_float( this, IDC_EDIT_MP_SPI );
	m_CharClass.fSP_STA = GetWin_Num_float( this, IDC_EDIT_SP_STA );

	m_CharClass.fHIT_DEX = GetWin_Num_float( this, IDC_EDIT_HIT_DEX );
	m_CharClass.fAVOID_DEX = GetWin_Num_float( this, IDC_EDIT_AVOID_DEX );
	m_CharClass.fDEFENSE_DEX = GetWin_Num_float( this, IDC_EDIT_DEFENSE_DEX );

	m_CharClass.fPA_POW = GetWin_Num_float( this, IDC_EDIT_PA_POW );
	m_CharClass.fPA_DEX = GetWin_Num_float( this, IDC_EDIT_PA_DEX );

	m_CharClass.fSA_POW = GetWin_Num_float( this, IDC_EDIT_SA_POW );
	m_CharClass.fSA_DEX = GetWin_Num_float( this, IDC_EDIT_SA_DEX );

	m_CharClass.fMA_DEX = GetWin_Num_float( this, IDC_EDIT_MA_DEX );
	m_CharClass.fMA_SPI = GetWin_Num_float( this, IDC_EDIT_MA_SPI );
	m_CharClass.fMA_INT = GetWin_Num_float( this, IDC_EDIT_MA_INT );

	m_CharClass.fCONV_AP = GetWin_Num_float( this, IDC_EDIT_CONV_AP );
	m_CharClass.fCONV_DP = GetWin_Num_float( this, IDC_EDIT_CONV_DP );
	m_CharClass.fCONV_PA = GetWin_Num_float( this, IDC_EDIT_CONV_PA );
	m_CharClass.fCONV_SA = GetWin_Num_float( this, IDC_EDIT_CONV_SA );

	m_CharClass.wBEGIN_AP = GetWin_Num_int( this, IDC_EDIT_BEGIN_AP );
	m_CharClass.wBEGIN_DP = GetWin_Num_int( this, IDC_EDIT_BEGIN_DP );
	m_CharClass.wBEGIN_PA = GetWin_Num_int( this, IDC_EDIT_BEGIN_PA );
	m_CharClass.wBEGIN_SA = GetWin_Num_int( this, IDC_EDIT_BEGIN_SA );

	m_CharClass.fLVLUP_AP = GetWin_Num_float( this, IDC_EDIT_LEVELUP_AP );
	m_CharClass.fLVLUP_DP = GetWin_Num_float( this, IDC_EDIT_LEVELUP_DP );
	m_CharClass.fLVLUP_PA = GetWin_Num_float( this, IDC_EDIT_LEVELUP_PA );
	m_CharClass.fLVLUP_SA = GetWin_Num_float( this, IDC_EDIT_LEVELUP_SA );

	m_CharClass.sBEGIN_STATS.wPow = GetWin_Num_int( this, IDC_EDIT_BEGIN_POW );
	m_CharClass.sBEGIN_STATS.wStr = GetWin_Num_int( this, IDC_EDIT_BEGIN_STR );
	m_CharClass.sBEGIN_STATS.wSpi = GetWin_Num_int( this, IDC_EDIT_BEGIN_SPI );
	m_CharClass.sBEGIN_STATS.wDex = GetWin_Num_int( this, IDC_EDIT_BEGIN_DEX );
	m_CharClass.sBEGIN_STATS.wInt = GetWin_Num_int( this, IDC_EDIT_BEGIN_INT );
	m_CharClass.sBEGIN_STATS.wSta = GetWin_Num_int( this, IDC_EDIT_BEGIN_STA );

	m_CharClass.sLVLUP_STATS.fPow = GetWin_Num_float( this, IDC_EDIT_LEVELUP_POW );
	m_CharClass.sLVLUP_STATS.fStr = GetWin_Num_float( this, IDC_EDIT_LEVELUP_STR );
	m_CharClass.sLVLUP_STATS.fSpi = GetWin_Num_float( this, IDC_EDIT_LEVELUP_SPI );
	m_CharClass.sLVLUP_STATS.fDex = GetWin_Num_float( this, IDC_EDIT_LEVELUP_DEX );
	m_CharClass.sLVLUP_STATS.fInt = GetWin_Num_float( this, IDC_EDIT_LEVELUP_INT );
	m_CharClass.sLVLUP_STATS.fSta = GetWin_Num_float( this, IDC_EDIT_LEVELUP_STA );

	m_CharClass.dwHEADNUM = GetWin_Num_int( this, IDC_EDIT_MAX_HEAD );
	m_CharClass.dwHEADNUM_SELECT = GetWin_Num_int( this, IDC_EDIT_SEL_HEAD );
	m_CharClass.strHEAD_CPS[0] = GetWin_Text( this, IDC_EDIT_HEAD_00 ).GetString();
	m_CharClass.strHEAD_CPS[1] = GetWin_Text( this, IDC_EDIT_HEAD_01 ).GetString();
	m_CharClass.strHEAD_CPS[2] = GetWin_Text( this, IDC_EDIT_HEAD_02 ).GetString();
	m_CharClass.strHEAD_CPS[3] = GetWin_Text( this, IDC_EDIT_HEAD_03 ).GetString();
	m_CharClass.strHEAD_CPS[4] = GetWin_Text( this, IDC_EDIT_HEAD_04 ).GetString();
	m_CharClass.strHEAD_CPS[5] = GetWin_Text( this, IDC_EDIT_HEAD_05 ).GetString();
	m_CharClass.strHEAD_CPS[6] = GetWin_Text( this, IDC_EDIT_HEAD_06 ).GetString();
	m_CharClass.strHEAD_CPS[7] = GetWin_Text( this, IDC_EDIT_HEAD_07 ).GetString();
	m_CharClass.strHEAD_CPS[8] = GetWin_Text( this, IDC_EDIT_HEAD_08 ).GetString();
	m_CharClass.strHEAD_CPS[9] = GetWin_Text( this, IDC_EDIT_HEAD_09 ).GetString();
	m_CharClass.strHEAD_CPS[10] = GetWin_Text( this, IDC_EDIT_HEAD_10 ).GetString();
	m_CharClass.strHEAD_CPS[11] = GetWin_Text( this, IDC_EDIT_HEAD_11 ).GetString();
	m_CharClass.strHEAD_CPS[12] = GetWin_Text( this, IDC_EDIT_HEAD_12 ).GetString();
	m_CharClass.strHEAD_CPS[13] = GetWin_Text( this, IDC_EDIT_HEAD_13 ).GetString();
	m_CharClass.strHEAD_CPS[14] = GetWin_Text( this, IDC_EDIT_HEAD_14 ).GetString();
	m_CharClass.strHEAD_CPS[15] = GetWin_Text( this, IDC_EDIT_HEAD_15 ).GetString();
	m_CharClass.strHEAD_CPS[16] = GetWin_Text( this, IDC_EDIT_HEAD_16 ).GetString();
	m_CharClass.strHEAD_CPS[17] = GetWin_Text( this, IDC_EDIT_HEAD_17 ).GetString();
	m_CharClass.strHEAD_CPS[18] = GetWin_Text( this, IDC_EDIT_HEAD_18 ).GetString();
	m_CharClass.strHEAD_CPS[19] = GetWin_Text( this, IDC_EDIT_HEAD_19 ).GetString();

	m_CharClass.dwHAIRNUM = GetWin_Num_int( this, IDC_EDIT_MAX_HAIR );
	m_CharClass.dwHAIRNUM_SELECT = GetWin_Num_int( this, IDC_EDIT_SEL_HAIR );
	m_CharClass.strHAIR_CPS[0] = GetWin_Text( this, IDC_EDIT_HAIR_00 ).GetString();
	m_CharClass.strHAIR_CPS[1] = GetWin_Text( this, IDC_EDIT_HAIR_01 ).GetString();
	m_CharClass.strHAIR_CPS[2] = GetWin_Text( this, IDC_EDIT_HAIR_02 ).GetString();
	m_CharClass.strHAIR_CPS[3] = GetWin_Text( this, IDC_EDIT_HAIR_03 ).GetString();
	m_CharClass.strHAIR_CPS[4] = GetWin_Text( this, IDC_EDIT_HAIR_04 ).GetString();
	m_CharClass.strHAIR_CPS[5] = GetWin_Text( this, IDC_EDIT_HAIR_05 ).GetString();
	m_CharClass.strHAIR_CPS[6] = GetWin_Text( this, IDC_EDIT_HAIR_06 ).GetString();
	m_CharClass.strHAIR_CPS[7] = GetWin_Text( this, IDC_EDIT_HAIR_07 ).GetString();
	m_CharClass.strHAIR_CPS[8] = GetWin_Text( this, IDC_EDIT_HAIR_08 ).GetString();
	m_CharClass.strHAIR_CPS[9] = GetWin_Text( this, IDC_EDIT_HAIR_09 ).GetString();
	m_CharClass.strHAIR_CPS[10] = GetWin_Text( this, IDC_EDIT_HAIR_10 ).GetString();
	m_CharClass.strHAIR_CPS[11] = GetWin_Text( this, IDC_EDIT_HAIR_11 ).GetString();
	m_CharClass.strHAIR_CPS[12] = GetWin_Text( this, IDC_EDIT_HAIR_12 ).GetString();
	m_CharClass.strHAIR_CPS[13] = GetWin_Text( this, IDC_EDIT_HAIR_13 ).GetString();
	m_CharClass.strHAIR_CPS[14] = GetWin_Text( this, IDC_EDIT_HAIR_14 ).GetString();
	m_CharClass.strHAIR_CPS[15] = GetWin_Text( this, IDC_EDIT_HAIR_15 ).GetString();
	m_CharClass.strHAIR_CPS[16] = GetWin_Text( this, IDC_EDIT_HAIR_16 ).GetString();
	m_CharClass.strHAIR_CPS[17] = GetWin_Text( this, IDC_EDIT_HAIR_17 ).GetString();
	m_CharClass.strHAIR_CPS[18] = GetWin_Text( this, IDC_EDIT_HAIR_18 ).GetString();
	m_CharClass.strHAIR_CPS[19] = GetWin_Text( this, IDC_EDIT_HAIR_19 ).GetString();

	m_CharClass.strCLASS_EFFECT = GetWin_Text( this, IDC_EDIT_CLASS_EFFECT ).GetString();

	if ( m_CharClass.dwHEADNUM >= MAX_HEAD )
		m_CharClass.dwHEADNUM = MAX_HEAD;

	if ( m_CharClass.dwHEADNUM_SELECT >= MAX_HEAD )		
		m_CharClass.dwHEADNUM_SELECT = MAX_HEAD;

	if ( m_CharClass.dwHAIRNUM >= MAX_HAIR )		
		m_CharClass.dwHAIRNUM = MAX_HAIR;

	if ( m_CharClass.dwHAIRNUM_SELECT >= MAX_HAIR )		
		m_CharClass.dwHAIRNUM_SELECT = MAX_HAIR;
}

void CEditorClassConstDlg::OnBnClickedButtonFileExport()
{
	CString szFilter = "classconst (*.classconst)|*.classconst|";

	CString strsavefile = GLOGIC::GetPath();
	strsavefile += m_strFileName.c_str();

	CFileDialog dlg( FALSE, ".classconst", strsavefile.GetString(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		DataSave();

		if( m_CharClass.SAVEFILE_STRING( dlg.GetPathName().GetString() ) )
		{
			SetWin_Text( this, IDC_EDIT_FILE, dlg.GetPathName().GetString() );
			m_strFileName = dlg.GetFileName().GetString();
			DataShow();
		}
		else
			CDebugSet::MsgBox( GetSafeHwnd(), "SAVEFILE_STRING Failed: %s", dlg.GetPathName().GetString() );
	}
}
