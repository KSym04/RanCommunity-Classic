
// EditorSkillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorSkill.h"
#include "./EditorSkillDlg.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"

#include "../Lib_Engine/DxResponseMan.h"

#include "../Lib_Client/G-Logic/GLSkill.h"
#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"
#include "../Lib_Client/G-Logic/GLStringTable.h"

#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorSkillDlg dialog




CEditorSkillDlg::CEditorSkillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorSkillDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorSkillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditorSkillDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_LOAD_SKILL, OnBnClickedLoadSkill)
	ON_BN_CLICKED(ID_SAVE_SKILL, OnBnClickedSaveSkill)
	ON_BN_CLICKED(ID_LOAD_CSV, OnBnClickedLoadCsv)
	ON_BN_CLICKED(ID_SAVE_CSV, OnBnClickedSaveCsv)
	ON_BN_CLICKED(ID_SEARCH, OnBnClickedSearch)
	ON_BN_CLICKED(ID_BTN_EFFECTS_COPY, &CEditorSkillDlg::OnBnClickedBtnEffectsCopy)
	ON_BN_CLICKED(ID_SEARCH2, &CEditorSkillDlg::OnBnClickedSearch2)
END_MESSAGE_MAP()


// CEditorSkillDlg message handlers

BOOL CEditorSkillDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GLSkillMan::GetInstance().OneTimeSceneInit();

	GMTOOL::Create ( RPATH::getAppPath() );

	CRect SheetRect;
	CWnd *pWnd;
	pWnd = GetDlgItem ( IDC_STATIC_TOOL );
	pWnd->GetWindowRect ( SheetRect );
	ScreenToClient ( &SheetRect );
	LOGFONT logfont = {0};

	CFont* pFont = GetFont();
	if ( pFont ) pFont->GetLogFont( &logfont );

	m_pSheetTab = new CsheetWithTab ( SheetRect, logfont, this );
	m_pSheetTab->Create( this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0 );
	m_pSheetTab->ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_pSheetTab->SetActivePage ( PAGE_TREE );

	m_vec.clear();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorSkillDlg::OnPaint()
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
HCURSOR CEditorSkillDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorSkillDlg::PostNcDestroy()
{
	SAFE_DELETE ( m_pSheetTab );
	GLSkillMan::GetInstance().FinalCleanup ();
	DxResponseMan::GetInstance().FinalCleanup ();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}

bool CEditorSkillDlg::DataLoad()
{
	CString szFilter = "Skill Setting File (*.ssf)|*.ssf|";
	CFileDialog dlg( TRUE, ".ssf", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = GLSkillMan::GetInstance().LoadFile ( dlg.GetFileName().GetString(), TRUE );
		if ( FAILED ( hr ) )
		{
			MessageBox ( "Failure to load Skill Setting File." );
		}

		GLStringTable::GetInstance().CLEAR( GLStringTable::SKILL );
		if( !GLStringTable::GetInstance().LOADFILE( GLSkillMan::GetInstance()._STRINGTABLE, GLStringTable::SKILL ) )
		{
			MessageBox ( "Failure to load Skill String Table." );
		}

		m_vec.clear();

		if ( m_pSheetTab )
			m_pSheetTab->ActivePageTree( TRUE );
	}
	return true;
}

bool CEditorSkillDlg::DataSave()
{
	CString szFilter = "Skill Setting File (*.ssf)|*.ssf|";
	CFileDialog dlg( FALSE, ".ssf", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr;
		hr = GLSkillMan::GetInstance().SaveFile ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			MessageBox ( "Failure to Save File" );
			return false;
		}

		if ( !GLStringTable::GetInstance().SAVEFILE( GLStringTable::SKILL ) )
		{
			MessageBox ( "Failure to Save File" );
			return false;
		}

		return true;
	}

	return false;
}

void CEditorSkillDlg::OnClose()
{
	if ( GLSkillMan::GetInstance().IsModify() )
	{
		int nret = MessageBox ( "Press the 'YES' Button to Save.", "Caution", MB_YESNOCANCEL );
		switch ( nret )
		{
		case IDYES:
			if ( !DataSave () )	return;
			break;

		case IDNO:
			break;

		case IDCANCEL:
			return;
		};
	}

	CDialog::OnClose();
}

void CEditorSkillDlg::OnBnClickedLoadSkill()
{
	DataLoad();
}

void CEditorSkillDlg::OnBnClickedSaveSkill()
{
	DataSave();
}

void CEditorSkillDlg::OnBnClickedLoadCsv()
{
	if( GLSkillMan::GetInstance().LoadCsvFile( this ) == S_OK )
	{
		m_vec.clear();

		if ( m_pSheetTab )
			m_pSheetTab->ActivePageTree( TRUE );

		GLStringTable::GetInstance().CLEAR( GLStringTable::SKILL );
		GLStringTable::GetInstance().LOADFILE( GLSkillMan::GetInstance()._STRINGTABLE, GLStringTable::SKILL );
		MessageBox( _T("CSV Load Success"), _T("Success"), MB_OK );
	}
}

void CEditorSkillDlg::OnBnClickedSaveCsv()
{
	if( GLSkillMan::GetInstance().SaveCsvFile( this ) == S_OK )
		MessageBox( _T("CSV Save Success"), _T("Success"), MB_OK );
}

void CEditorSkillDlg::OnBnClickedSearch()
{
	if ( m_pSheetTab )
	{
		m_SearchDlg.SetContainer( &m_vec );
		if ( m_SearchDlg.DoModal() == IDOK )
		{
			if ( m_pSheetTab )
			{
				if ( m_pSheetTab->GetActivePage() != &m_pSheetTab->m_PageTree )
				{
					m_pSheetTab->m_PageEdit1.SkillClear();
					m_pSheetTab->m_PageEdit2.SkillClear();
					m_pSheetTab->ActivePageTree( TRUE );
				}

				m_pSheetTab->m_PageTree.SelectTree( m_SearchDlg.m_sID.wMainID, m_SearchDlg.m_sID.wSubID );
			}
		}
	}
}


void CEditorSkillDlg::OnBnClickedBtnEffectsCopy()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "this will fill blank effect slots with data from 'spirit effect', continue?") != IDYES )	return;

	for ( int mid=0; mid<EMSKILLCLASS_NSIZE; mid++ )
	{
		for ( int sid=0; sid<GLSkillMan::MAX_CLASSSKILL; sid++ )
		{        		
			PGLSKILL pskill = GLSkillMan::GetInstance().GetData( mid, sid );
			if ( pskill )
			{
				for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
				{
					if ( pskill->m_sEXT_DATA.strSELFZONE01[i].length() == 0 && pskill->m_sEXT_DATA.strSELFZONE01[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strSELFZONE01[i] = pskill->m_sEXT_DATA.strSELFZONE01[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strSELFZONE02[i].length() == 0 && pskill->m_sEXT_DATA.strSELFZONE02[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strSELFZONE02[i] = pskill->m_sEXT_DATA.strSELFZONE02[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strSELFZONE03[i].length() == 0 && pskill->m_sEXT_DATA.strSELFZONE03[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strSELFZONE03[i] = pskill->m_sEXT_DATA.strSELFZONE03[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strTARG[i].length() == 0 && pskill->m_sEXT_DATA.strTARG[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strTARG[i] = pskill->m_sEXT_DATA.strTARG[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strTARGZONE01[i].length() == 0 && pskill->m_sEXT_DATA.strTARGZONE01[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strTARGZONE01[i] = pskill->m_sEXT_DATA.strTARGZONE01[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strTARGZONE02[i].length() == 0 && pskill->m_sEXT_DATA.strTARGZONE02[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strTARGZONE02[i] = pskill->m_sEXT_DATA.strTARGZONE02[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strTARGZONE03[i].length() == 0 && pskill->m_sEXT_DATA.strTARGZONE03[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strTARGZONE03[i] = pskill->m_sEXT_DATA.strTARGZONE03[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strTARGBODY01[i].length() == 0 && pskill->m_sEXT_DATA.strTARGBODY01[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strTARGBODY01[i] = pskill->m_sEXT_DATA.strTARGBODY01[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strTARGBODY02[i].length() == 0 && pskill->m_sEXT_DATA.strTARGBODY02[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strTARGBODY02[i] = pskill->m_sEXT_DATA.strTARGBODY02[EMELEMENT_SPIRIT];

					if ( pskill->m_sEXT_DATA.strSELFBODY[i].length() == 0 && pskill->m_sEXT_DATA.strSELFBODY[EMELEMENT_SPIRIT].length() != 0 )	
						pskill->m_sEXT_DATA.strSELFBODY[i] = pskill->m_sEXT_DATA.strSELFBODY[EMELEMENT_SPIRIT];

				}	
			}
		}
	}

	
}


void CEditorSkillDlg::OnBnClickedSearch2()
{
	if ( m_pSheetTab )
	{
		m_Search2Dlg.SetContainer( &m_vec );
		if ( m_Search2Dlg.DoModal() == IDOK )
		{
			if ( m_pSheetTab )
			{
				if ( m_pSheetTab->GetActivePage() != &m_pSheetTab->m_PageTree )
				{
					m_pSheetTab->m_PageEdit1.SkillClear();
					m_pSheetTab->m_PageEdit2.SkillClear();
					m_pSheetTab->ActivePageTree( TRUE );
				}

				m_pSheetTab->m_PageTree.SelectTree( m_Search2Dlg.m_sID.wMainID, m_Search2Dlg.m_sID.wSubID );
			}
		}
	}
}
