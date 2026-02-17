// EditorRandomOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorRandomOption.h"
#include "./EditorRandomOptionDlg.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"

#include "../Lib_Engine/DxResponseMan.h"

#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"

#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorRandomOptionDlg dialog



CEditorRandomOptionDlg::CEditorRandomOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorRandomOptionDlg::IDD, pParent)
	, m_bAddGen(FALSE)
	, m_bAddSet(FALSE)
	, m_bSetR(FALSE)
	, m_bAddSection(FALSE)
	, m_dwSectionEdit(0)
{
	m_RandomOptionGen = SRANDOM_OPTION_GEN();
	m_RandomOptionSet = SRANDOM_OPTION_SET();
	m_RandomOptionSection = SRANDOM_OPTION_SECTION();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorRandomOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RANDOM_OPTION, m_List );
	DDX_Control(pDX, IDC_LIST_GEN_SET, m_ListSet );
	DDX_Control(pDX, IDC_LIST_GEN_SETR, m_ListSetR );
	DDX_Control(pDX, IDC_LIST_SET_SECTION, m_ListSection );
}

BEGIN_MESSAGE_MAP(CEditorRandomOptionDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SAVE, OnBnClickedButtonGenSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RANDOM_OPTION, OnNMDblclkListRandomOption)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_ADD1, OnBnClickedButtonGenSetAdd1)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_EDIT1, OnBnClickedButtonGenSetEdit1)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_DEL1, OnBnClickedButtonGenSetDel1)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_CLEAR1, OnBnClickedButtonGenSetClear1)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_COPY1, OnBnClickedButtonGenSetCopy1)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_ADD2, OnBnClickedButtonGenSetAdd2)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_EDIT2, OnBnClickedButtonGenSetEdit2)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_DEL2, OnBnClickedButtonGenSetDel2)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_CLEAR2, OnBnClickedButtonGenSetClear2)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SET_COPY2, OnBnClickedButtonGenSetCopy2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GEN_SET, OnNMDblclkListGenSet)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GEN_SETR, OnNMDblclkListGenSetr)
	ON_BN_CLICKED(IDC_BUTTON_SET_GENERATE, OnBnClickedButtonSetGenerate)
	ON_BN_CLICKED(IDC_BUTTON_SET_SAVE, OnBnClickedButtonSetSave)
	ON_BN_CLICKED(IDC_BUTTON_SET_SECTION_ADD, OnBnClickedButtonSetSectionAdd)
	ON_BN_CLICKED(IDC_BUTTON_SET_SECTION_EDIT, OnBnClickedButtonSetSectionEdit)
	ON_BN_CLICKED(IDC_BUTTON_SET_SECTION_DEL, OnBnClickedButtonSetSectionDel)
	ON_BN_CLICKED(IDC_BUTTON_SET_SECTION_CLEAR, OnBnClickedButtonSetSectionClear)
	ON_BN_CLICKED(IDC_BUTTON_SECTION_SAVE, OnBnClickedButtonSectionSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SET_SECTION, OnNMDblclkListSetSection)
	ON_BN_CLICKED(IDC_BUTTON_GEN_COPY_SET_TO_SETR, &CEditorRandomOptionDlg::OnBnClickedButtonGenCopySetToSetr)
	ON_BN_CLICKED(IDC_BUTTON_LOAD2, &CEditorRandomOptionDlg::OnBnClickedButtonLoad2)
	ON_BN_CLICKED(IDC_BUTTON_SAVE2, &CEditorRandomOptionDlg::OnBnClickedButtonSave2)
END_MESSAGE_MAP()


// CEditorRandomOptionDlg message handlers

BOOL CEditorRandomOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	RECT rectCtrl;
	m_List.SetExtendedStyle ( m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List.GetClientRect ( &rectCtrl );
	m_List.InsertColumn( 0, "Num", LVCFMT_LEFT, ( 10* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 1, "Name", LVCFMT_LEFT, ( 75* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 2, "Check", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	m_ListSet.SetExtendedStyle ( m_ListSet.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ListSet.GetClientRect ( &rectCtrl );
	m_ListSet.InsertColumn( 0, "Type", LVCFMT_LEFT, ( 55* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSet.InsertColumn( 1, "Rate", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSet.InsertColumn( 2, "Low", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSet.InsertColumn( 3, "High", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	m_ListSetR.SetExtendedStyle ( m_ListSetR.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ListSetR.GetClientRect ( &rectCtrl );
	m_ListSetR.InsertColumn( 0, "Type", LVCFMT_LEFT, ( 55* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSetR.InsertColumn( 1, "Rate", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSetR.InsertColumn( 2, "Low", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSetR.InsertColumn( 3, "High", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	m_ListSection.SetExtendedStyle ( m_ListSection.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ListSection.GetClientRect ( &rectCtrl );
	m_ListSection.InsertColumn( 0, "Rate", LVCFMT_LEFT, ( 40* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSection.InsertColumn( 1, "Low", LVCFMT_LEFT, ( 30* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListSection.InsertColumn( 2, "High", LVCFMT_LEFT, ( 30* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	SetWin_Combo_Init( this, IDC_COMBO_SET_TYPE, COMMENT::ITEM_RANDOM_OPT, EMR_OPT_SIZE );

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	GLRandomOptionManager::GetInstance().LoadFile( "RandomOption.rsf" );
	
	DataShow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorRandomOptionDlg::OnPaint() 
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
HCURSOR CEditorRandomOptionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorRandomOptionDlg::PostNcDestroy()
{
	GLRandomOptionManager::GetInstance().CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}

void CEditorRandomOptionDlg::OnBnClickedButtonLoad()
{
	CString szFilter = "Random Option File (*.rsf)|*.rsf|";
	CFileDialog dlg( TRUE, ".rsf", GLOGIC::GetServerPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath();

	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = GLRandomOptionManager::GetInstance().LoadFile ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load File" );
		}

		DataShow();
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonSave()
{
	CString szFilter = "Random Option File (*.rsf)|*.rsf|";
	CFileDialog dlg( FALSE, ".rsf", GLOGIC::GetServerPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath();

	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = GLRandomOptionManager::GetInstance().SaveFile ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Save File" );
			return;
		}
	}
}


void CEditorRandomOptionDlg::OnBnClickedButtonLoad2()
{
	bool bOK = GLRandomOptionManager::GetInstance().CSVLoad(this);
	if ( bOK )
		DataShow();
}

void CEditorRandomOptionDlg::OnBnClickedButtonSave2()
{
	GLRandomOptionManager::GetInstance().CSVSave(this);
}

void CEditorRandomOptionDlg::OnBnClickedButtonImport()
{
	CString szFilter = "Random Option File (*.bin)|*.bin|";
	CFileDialog dlg( TRUE, ".bin", GLOGIC::GetServerPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath();

	if ( dlg.DoModal() == IDOK )
	{
		SRANDOM_OPTION_GEN* prandom_option = GLRandomOptionManager::GetInstance().GetRandomOption( dlg.GetFileName().GetString() );
		if ( prandom_option )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "%s data already exist", dlg.GetFileName().GetString() );
			return;
		}

		HRESULT hr = GLRandomOptionManager::GetInstance().Import ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Import File" );
		}

		DataShow();
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonExport()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Export to :%s ?", GLOGIC::GetServerPath() ) == IDYES )
	{
		DWORD dwNum = GLRandomOptionManager::GetInstance().Export();
		CDebugSet::MsgBox( GetSafeHwnd(), "Export :%d", dwNum );
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonAdd()
{
	m_RandomOptionGen.Reset();
	RandomOptionGenShow( TRUE, TRUE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonEdit()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );

	SRANDOM_OPTION_GEN* prandom_option_gen = GLRandomOptionManager::GetInstance().GetRandomOption( strtext.c_str() );
	if ( !prandom_option_gen )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s failure to get data", strtext.c_str() );
		return;
	}

	m_RandomOptionGen = *prandom_option_gen;
	RandomOptionGenShow( TRUE, FALSE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonDelete()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "delete %s", strtext.c_str() ) !=  IDYES )
		return;

	SRANDOM_OPTION_GEN_MAP &map_random_option = GLRandomOptionManager::GetInstance().m_mapRandomOption;
	SRANDOM_OPTION_GEN_MAP_ITER iterpos = map_random_option.find(strtext);
	if ( iterpos != map_random_option.end() )
	{
		map_random_option.erase( iterpos );
		DataShow();
	}

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List.EnsureVisible( nNewSelect, TRUE );
	m_List.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CEditorRandomOptionDlg::OnBnClickedButtonCopy()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );

	SRANDOM_OPTION_GEN* prandom_option_gen = GLRandomOptionManager::GetInstance().GetRandomOption( strtext.c_str() );
	if ( !prandom_option_gen )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s failure to get data", strtext.c_str() );
		return;
	}

	m_RandomOptionGen = *prandom_option_gen;
	RandomOptionGenShow( TRUE, TRUE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "clear all random option?" ) == IDYES )
	{
		GLRandomOptionManager::GetInstance().CleanUp();
		DataShow();
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSave()
{
	std::string strName = GetWin_Text( this, IDC_EDIT_GEN_NAME );
	float fRate = GetWin_Num_float( this, IDC_EDIT_GEN_RATE );
	float fDPoint = GetWin_Num_float( this, IDC_EDIT_GEN_DPOINT );
	float fSValue = GetWin_Num_float( this, IDC_EDIT_GEN_SVALUE );

	WORD wMinGain = GetWin_Num_int( this, IDC_EDIT_GEN_MIN_GAIN );
	WORD wMaxGain = GetWin_Num_int( this, IDC_EDIT_GEN_MAX_GAIN );

	WORD wMaxGenD = GetWin_Num_int( this, IDC_EDIT_GEN_MAX_GEN_D );
	WORD wMaxGenR = GetWin_Num_int( this, IDC_EDIT_GEN_MAX_GEN_R );

	if ( strName.size() <= 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid Name" );
		return;
	}

	if ( fRate <= 0.0f || fRate > 100.0f )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid rate" );
		return;
	}

	if ( m_RandomOptionGen.GetSetSize() == 0 && m_RandomOptionGen.GetSetRSize() == 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "option empty" );
		return;
	}

	m_RandomOptionGen.strFileName = strName;
	m_RandomOptionGen.fRATE = fRate;
	m_RandomOptionGen.fD_point = fDPoint;
	m_RandomOptionGen.fS_value = fSValue;
	m_RandomOptionGen.wMinGain = wMinGain;
	m_RandomOptionGen.wMaxGain = wMaxGain;

	m_RandomOptionGen.wMaxGenD = wMaxGenD;
	m_RandomOptionGen.wMaxGenR = wMaxGenR;

	m_RandomOptionGen.dwMapSetSize = m_RandomOptionGen.mapSET.size();
	m_RandomOptionGen.dwMapSetRSize = m_RandomOptionGen.mapSETR.size();

	std::transform ( m_RandomOptionGen.strFileName.begin(), m_RandomOptionGen.strFileName.end(), m_RandomOptionGen.strFileName.begin(), tolower );

	SRANDOM_OPTION_GEN_MAP &map_random_option = GLRandomOptionManager::GetInstance().m_mapRandomOption;

	if ( m_bAddGen )
	{
		SRANDOM_OPTION_GEN_MAP_ITER iter = map_random_option.find( m_RandomOptionGen.strFileName );
		if ( iter == map_random_option.end() )
		{
			map_random_option.insert( std::make_pair( m_RandomOptionGen.strFileName, m_RandomOptionGen ) );
		}
		else
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "failed to insert random option, data exist" );
			return;
		}
	}
	else
	{
		SRANDOM_OPTION_GEN_MAP_ITER iter = map_random_option.find( m_RandomOptionGen.strFileName );
		if ( iter != map_random_option.end() )
		{
			(*iter).second = m_RandomOptionGen;
		}
		else
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "failed to update random option" );
			return;
		}
	}

	DataShow();


	for( int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		DWORD dwID = m_List.GetItemData( nItem );
		std::string strBinName = m_List.GetItemText( nItem, 1 );
		if( strBinName == strName )
		{
			m_List.EnsureVisible( nItem, TRUE );
			m_List.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
			GotoDlgCtrl(GetDlgItem(m_List.GetDlgCtrlID())); 
		}
	}
}

void CEditorRandomOptionDlg::OnNMDblclkListRandomOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );

	SRANDOM_OPTION_GEN* prandom_option_gen = GLRandomOptionManager::GetInstance().GetRandomOption( strtext.c_str() );
	if ( !prandom_option_gen )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s failure to get data", strtext.c_str() );
		return;
	}

	m_RandomOptionGen = *prandom_option_gen;
	RandomOptionGenShow( TRUE, FALSE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetAdd1()
{
	m_RandomOptionSet.Reset();
	RandomOptionSetShow( TRUE, TRUE, FALSE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetEdit1()
{
	int nSelect = m_ListSet.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSet.GetItemData( nSelect );

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSET.find(dwID);
	if ( iter != m_RandomOptionGen.mapSET.end() )
	{
		SRANDOM_OPTION_SET &random_option_set = (*iter).second;
		m_RandomOptionSet = random_option_set;
		RandomOptionSetShow( TRUE, FALSE, FALSE );
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetDel1()
{
	int nSelect = m_ListSet.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSet.GetItemData( nSelect );
	std::string strtext = m_ListSet.GetItemText( nSelect, 0 );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "delete %s", strtext.c_str() ) !=  IDYES )
		return;

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSET.find(dwID);
	if ( iter != m_RandomOptionGen.mapSET.end() )
	{
		m_RandomOptionGen.mapSET.erase( iter );
		ShowListSet();
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetClear1()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear all Set?" ) == IDYES )
	{
		m_RandomOptionGen.mapSET.clear();
		ShowListSet();
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetCopy1()
{
	int nSelect = m_ListSet.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSet.GetItemData( nSelect );

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSET.find(dwID);
	if ( iter != m_RandomOptionGen.mapSET.end() )
	{
		SRANDOM_OPTION_SET &random_option_set = (*iter).second;
		m_RandomOptionSet = random_option_set;
		RandomOptionSetShow( TRUE, TRUE, FALSE );
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetAdd2()
{
	m_RandomOptionSet.Reset();
	RandomOptionSetShow( TRUE, TRUE, TRUE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetEdit2()
{
	int nSelect = m_ListSetR.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSetR.GetItemData( nSelect );

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSETR.find(dwID);
	if ( iter != m_RandomOptionGen.mapSETR.end() )
	{
		SRANDOM_OPTION_SET &random_option_set = (*iter).second;
		m_RandomOptionSet = random_option_set;
		RandomOptionSetShow( TRUE, FALSE, TRUE );
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetDel2()
{
	int nSelect = m_ListSetR.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSetR.GetItemData( nSelect );
	std::string strtext = m_ListSetR.GetItemText( nSelect, 0 );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "delete %s", strtext.c_str() ) !=  IDYES )
		return;

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSETR.find(dwID);
	if ( iter != m_RandomOptionGen.mapSETR.end() )
	{
		m_RandomOptionGen.mapSETR.erase( iter );
		ShowListSetR();
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetClear2()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear all SetR?" ) == IDYES )
	{
		m_RandomOptionGen.mapSETR.clear();
		ShowListSetR();
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonGenSetCopy2()
{
	int nSelect = m_ListSetR.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSetR.GetItemData( nSelect );

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSETR.find(dwID);
	if ( iter != m_RandomOptionGen.mapSETR.end() )
	{
		SRANDOM_OPTION_SET &random_option_set = (*iter).second;
		m_RandomOptionSet = random_option_set;
		RandomOptionSetShow( TRUE, TRUE, TRUE );
	}
}

void CEditorRandomOptionDlg::OnNMDblclkListGenSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int nSelect = m_ListSet.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSet.GetItemData( nSelect );

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSET.find(dwID);
	if ( iter != m_RandomOptionGen.mapSET.end() )
	{
		SRANDOM_OPTION_SET &random_option_set = (*iter).second;
		m_RandomOptionSet = random_option_set;
		RandomOptionSetShow( TRUE, FALSE, FALSE );
	}
}

void CEditorRandomOptionDlg::OnNMDblclkListGenSetr(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int nSelect = m_ListSetR.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSetR.GetItemData( nSelect );

	SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSETR.find(dwID);
	if ( iter != m_RandomOptionGen.mapSETR.end() )
	{
		SRANDOM_OPTION_SET &random_option_set = (*iter).second;
		m_RandomOptionSet = random_option_set;
		RandomOptionSetShow( TRUE, FALSE, TRUE );
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonSetGenerate()
{
	m_RandomOptionSet.GenerateRange();
	SetWin_Num_float( this, IDC_EDIT_SET_LOW, m_RandomOptionSet.fLOW );
	SetWin_Num_float( this, IDC_EDIT_SET_HIGH, m_RandomOptionSet.fHIGH );
}

void CEditorRandomOptionDlg::OnBnClickedButtonSetSave()
{
	EMRANDOM_OPT emtype = static_cast<EMRANDOM_OPT> ( GetWin_Combo_Sel( this, IDC_COMBO_SET_TYPE ) );
	float fRate = GetWin_Num_float( this, IDC_EDIT_SET_RATE );
	float fLow = GetWin_Num_float( this, IDC_EDIT_SET_LOW );
	float fHigh = GetWin_Num_float( this, IDC_EDIT_SET_HIGH );

	if ( emtype == EMR_OPT_NULL )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid type" );
		return;
	}

	if ( fRate <= 0.0f || fRate > 100.0f )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid rate" );
		return;
	}

	if ( fLow > fHigh || fHigh < fLow )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "LOW-HIGH value reversed" );
		return;
	}

	if ( m_RandomOptionSet.vecSECT.size() <= 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "section empty" );
		return;
	}

	m_RandomOptionSet.emTYPE = emtype;
	m_RandomOptionSet.fRATE = fRate;
	m_RandomOptionSet.fLOW = fLow;
	m_RandomOptionSet.fHIGH = fHigh;

	if ( m_bAddSet )
	{
		if ( m_bSetR )
		{
			SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSETR.find( m_RandomOptionSet.emTYPE );
			if ( iter == m_RandomOptionGen.mapSETR.end() )
			{
				m_RandomOptionGen.mapSETR.insert( std::make_pair( m_RandomOptionSet.emTYPE, m_RandomOptionSet ) );
			}
			else
			{
				CDebugSet::MsgBox( GetSafeHwnd(), "failed to insert random option set, type exist" );
				return;
			}
		}
		else
		{
			SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSET.find( m_RandomOptionSet.emTYPE );
			if ( iter == m_RandomOptionGen.mapSET.end() )
			{
				m_RandomOptionGen.mapSET.insert( std::make_pair( m_RandomOptionSet.emTYPE, m_RandomOptionSet ) );
			}
			else
			{
				CDebugSet::MsgBox( GetSafeHwnd(), "failed to insert random option set, type exist" );
				return;
			}
		}
	}
	else
	{
		if ( m_bSetR )
		{
			SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSETR.find( m_RandomOptionSet.emTYPE );
			if ( iter != m_RandomOptionGen.mapSETR.end() )
			{
				(*iter).second = m_RandomOptionSet;
			}
			else
			{
				CDebugSet::MsgBox( GetSafeHwnd(), "failed to update random option set" );
				return;
			}
		}
		else
		{
			SRANDOM_OPTION_SET_MAP_ITER iter = m_RandomOptionGen.mapSET.find( m_RandomOptionSet.emTYPE );
			if ( iter != m_RandomOptionGen.mapSET.end() )
			{
				(*iter).second = m_RandomOptionSet;
			}
			else
			{
				CDebugSet::MsgBox( GetSafeHwnd(), "failed to update random option set" );
				return;
			}
		}
	}

	if ( m_bSetR )
	{
		ShowListSetR();

		for( int nItem = 0; nItem < m_ListSetR.GetItemCount(); nItem++)
		{
			DWORD dwID = m_ListSetR.GetItemData( nItem );
			if( dwID == (DWORD)emtype )
			{
				m_ListSetR.EnsureVisible( nItem, TRUE );
				m_ListSetR.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
				GotoDlgCtrl(GetDlgItem(m_ListSetR.GetDlgCtrlID())); 
			}
		}
	}
	else
	{
		ShowListSet();

		for( int nItem = 0; nItem < m_ListSet.GetItemCount(); nItem++)
		{
			DWORD dwID = m_ListSet.GetItemData( nItem );
			if( dwID == (DWORD)emtype )
			{
				m_ListSet.EnsureVisible( nItem, TRUE );
				m_ListSet.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
				GotoDlgCtrl(GetDlgItem(m_ListSet.GetDlgCtrlID())); 
			}
		}
	}
}

void CEditorRandomOptionDlg::OnBnClickedButtonSetSectionAdd()
{
	m_RandomOptionSection.Reset();
	RandomOptionSectionShow( TRUE, TRUE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonSetSectionEdit()
{
	int nSelect = m_ListSection.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSection.GetItemData( nSelect );

	if ( dwID >= (DWORD)m_RandomOptionSet.vecSECT.size() )	return;

	m_dwSectionEdit = dwID;
	SRANDOM_OPTION_SECTION &random_option_section = m_RandomOptionSet.vecSECT[dwID];
	m_RandomOptionSection = random_option_section;
	RandomOptionSectionShow( TRUE, FALSE );
}

void CEditorRandomOptionDlg::OnBnClickedButtonSetSectionDel()
{
	int nSelect = m_ListSection.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSection.GetItemData( nSelect );
	m_RandomOptionSet.vecSECT.erase( m_RandomOptionSet.vecSECT.begin() + dwID );
	ShowListSection();
}

void CEditorRandomOptionDlg::OnBnClickedButtonSetSectionClear()
{
	m_RandomOptionSet.vecSECT.clear();
	ShowListSection();
}

void CEditorRandomOptionDlg::OnBnClickedButtonSectionSave()
{
	float fRate = GetWin_Num_float( this, IDC_EDIT_SECTION_RATE );
	float fLow = GetWin_Num_float( this, IDC_EDIT_SECTION_LOW );
	float fHigh = GetWin_Num_float( this, IDC_EDIT_SECTION_HIGH );

	if ( fRate <= 0.0f || fRate > 100.0f )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid rate" );
		return;
	}

	if ( fLow > fHigh || fHigh < fLow )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "LOW-HIGH value reversed" );
		return;
	}

	m_RandomOptionSection.fRATE = fRate;
	m_RandomOptionSection.fLOW = fLow;
	m_RandomOptionSection.fHIGH = fHigh;

	if ( m_bAddSection )
	{
		m_RandomOptionSet.vecSECT.push_back( m_RandomOptionSection );
	}
	else
	{
		if ( m_dwSectionEdit >= m_RandomOptionSet.vecSECT.size() )	return;
		m_RandomOptionSet.vecSECT[m_dwSectionEdit] = m_RandomOptionSection;
	}

	ShowListSection();
}

void CEditorRandomOptionDlg::OnNMDblclkListSetSection(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int nSelect = m_ListSection.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_ListSection.GetItemData( nSelect );

	if ( dwID >= (DWORD)m_RandomOptionSet.vecSECT.size() )	return;

	m_dwSectionEdit = dwID;
	SRANDOM_OPTION_SECTION &random_option_section = m_RandomOptionSet.vecSECT[dwID];
	m_RandomOptionSection = random_option_section;
	RandomOptionSectionShow( TRUE, FALSE );
}


void CEditorRandomOptionDlg::OnBnClickedButtonGenCopySetToSetr()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "all setR contents will be replaced with data from set, Continue?" ) != IDYES )
		return;

	m_RandomOptionGen.mapSETR.clear();
	m_RandomOptionGen.mapSETR = m_RandomOptionGen.mapSET;

	ShowListSet();
	ShowListSetR();
}
