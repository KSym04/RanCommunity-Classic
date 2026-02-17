// EditorSetOptionDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorSetOptionData.h"
#include "./EditorSetOptionDataDlg.h"

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


// CEditorSetOptionDataDlg dialog



CEditorSetOptionDataDlg::CEditorSetOptionDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorSetOptionDataDlg::IDD, pParent)
	, m_bAdd(FALSE)
{
	m_sDataEdit = SSET_OPTION_DATA();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorSetOptionDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SET_OPTION, m_List );

}

BEGIN_MESSAGE_MAP(CEditorSetOptionDataDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CEditorSetOptionDataDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEditorSetOptionDataDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_CSV, &CEditorSetOptionDataDlg::OnBnClickedButtonLoadCsv)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CSV, &CEditorSetOptionDataDlg::OnBnClickedButtonSaveCsv)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEditorSetOptionDataDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CEditorSetOptionDataDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEditorSetOptionDataDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CEditorSetOptionDataDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CEditorSetOptionDataDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SET_OPTION_SAVE, &CEditorSetOptionDataDlg::OnBnClickedButtonSetOptionSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SET_OPTION, &CEditorSetOptionDataDlg::OnNMDblclkListSetOption)
	
END_MESSAGE_MAP()


// CEditorSetOptionDataDlg message handlers

BOOL CEditorSetOptionDataDlg::OnInitDialog()
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
	m_List.InsertColumn( 0, "ID", LVCFMT_LEFT, ( 30* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 1, "Name", LVCFMT_LEFT, ( 70* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	

	

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	GLSetOptionData::GetInstance().LoadSetOptionData( "SetOptionData.sdsf" );
	

	SetWin_Combo_Init( this, IDC_COMBO_OPTION_1, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_2, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_3, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_4, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_5, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_6, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_7, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_8, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_9, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_10, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_11, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_OPTION_12, COMMENT::SET_OPTION_VAR, EMSET_OPTION_VAR_SIZE );

	DataShow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorSetOptionDataDlg::OnPaint() 
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
HCURSOR CEditorSetOptionDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorSetOptionDataDlg::PostNcDestroy()
{
	GLSetOptionData::GetInstance().CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}





BOOL CEditorSetOptionDataDlg::DataShow()
{
	m_List.DeleteAllItems();
	m_List.SetRedraw( FALSE );

	SET_OPTION_DATA_MAP &mapData = GLSetOptionData::GetInstance().m_mapSetOptionData;
	int i=0;
	for( SET_OPTION_DATA_MAP_ITER iter = mapData.begin(); iter != mapData.end(); iter++, i++ )
	{
		SSET_OPTION_DATA &sData = (*iter).second;
		m_List.InsertItem( i, _HLIB::cstring_dword(sData.wID).GetString() );
		m_List.SetItemText( i, 1, sData.strName.c_str() );
		m_List.SetItemData( i, sData.wID );
	}

	m_List.SetRedraw( TRUE );

	m_sDataEdit = SSET_OPTION_DATA();
	SetOptionShow( FALSE, FALSE );

	SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM, (DWORD)mapData.size() );

	return TRUE;
}

void CEditorSetOptionDataDlg::SetOptionShow( BOOL bEnable, BOOL bAdd )
{
	m_bAdd = bAdd;

	SetWin_Num_int( this, IDC_EDIT_SET_OPTION_ID, m_sDataEdit.wID );
	SetWin_Text( this, IDC_EDIT_SET_OPTION_NAME, m_sDataEdit.strName.c_str() );

	SetWin_Check( this, IDC_CHECK_USE_PART_0, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_HEADGEAR] );
	SetWin_Check( this, IDC_CHECK_USE_PART_1, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_UPPER] );
	SetWin_Check( this, IDC_CHECK_USE_PART_2, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_LOWER] );
	SetWin_Check( this, IDC_CHECK_USE_PART_3, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_HAND] );
	SetWin_Check( this, IDC_CHECK_USE_PART_4, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_FOOT] );
	SetWin_Check( this, IDC_CHECK_USE_PART_5, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_WEAPON] );
	SetWin_Check( this, IDC_CHECK_USE_PART_6, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_NECK] );
	SetWin_Check( this, IDC_CHECK_USE_PART_7, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_WRIST] );
	SetWin_Check( this, IDC_CHECK_USE_PART_8, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_FINGER] );
	SetWin_Check( this, IDC_CHECK_USE_PART_9, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_BELT] );
	SetWin_Check( this, IDC_CHECK_USE_PART_10, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_EARRING] );
	SetWin_Check( this, IDC_CHECK_USE_PART_11, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_ACCESSORY] );
	SetWin_Check( this, IDC_CHECK_USE_PART_12, m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_DECORATION] );


	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_1, m_sDataEdit.emOptionType[0] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_2, m_sDataEdit.emOptionType[1] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_3, m_sDataEdit.emOptionType[2] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_4, m_sDataEdit.emOptionType[3] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_5, m_sDataEdit.emOptionType[4] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_6, m_sDataEdit.emOptionType[5] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_7, m_sDataEdit.emOptionType[6] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_8, m_sDataEdit.emOptionType[7] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_9, m_sDataEdit.emOptionType[8] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_10, m_sDataEdit.emOptionType[9] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_11, m_sDataEdit.emOptionType[10] );
	SetWin_Combo_Sel( this, IDC_COMBO_OPTION_12, m_sDataEdit.emOptionType[11] );

	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_1, m_sDataEdit.fOptionVar[0] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_2, m_sDataEdit.fOptionVar[1] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_3, m_sDataEdit.fOptionVar[2] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_4, m_sDataEdit.fOptionVar[3] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_5, m_sDataEdit.fOptionVar[4] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_6, m_sDataEdit.fOptionVar[5] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_7, m_sDataEdit.fOptionVar[6] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_8, m_sDataEdit.fOptionVar[7] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_9, m_sDataEdit.fOptionVar[8] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_10, m_sDataEdit.fOptionVar[9] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_11, m_sDataEdit.fOptionVar[10] );
	SetWin_Num_float( this, IDC_EDIT_OPTION_VAR_12, m_sDataEdit.fOptionVar[11] );

	SetWin_Enable( this, IDC_EDIT_SET_OPTION_ID, bEnable && m_bAdd );
	SetWin_Enable( this, IDC_EDIT_SET_OPTION_NAME, bEnable );

	SetWin_Enable( this, IDC_CHECK_USE_PART_0, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_1, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_2, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_3, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_4, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_5, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_6, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_7, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_8, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_9, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_10, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_11, bEnable );
	SetWin_Enable( this, IDC_CHECK_USE_PART_12, bEnable );

	SetWin_Enable( this, IDC_COMBO_OPTION_1, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_2, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_3, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_4, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_5, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_6, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_7, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_8, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_9, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_10, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_11, bEnable );
	SetWin_Enable( this, IDC_COMBO_OPTION_12, bEnable );

	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_1, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_2, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_3, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_4, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_5, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_6, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_7, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_8, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_9, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_10, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_11, bEnable );
	SetWin_Enable( this, IDC_EDIT_OPTION_VAR_12, bEnable );

	SetWin_Enable( this, IDC_BUTTON_SET_OPTION_SAVE, bEnable );
}

void CEditorSetOptionDataDlg::OnBnClickedButtonLoad()
{
	CString szFilter = "Set Option File (*.sdsf)|*.sdsf|";
	CFileDialog dlg( TRUE, ".sdsf", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();

	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = GLSetOptionData::GetInstance().LoadSetOptionData ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load File" );
		}

		DataShow();
	}
}

void CEditorSetOptionDataDlg::OnBnClickedButtonSave()
{
	CString szFilter = "Set Option File (*.sdsf)|*.sdsf|";
	CFileDialog dlg( FALSE, ".sdsf", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();

	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = GLSetOptionData::GetInstance().SaveSetOptionData ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Save File" );
			return;
		}
	}
}

void CEditorSetOptionDataDlg::OnBnClickedButtonLoadCsv()
{
	bool bOK = GLSetOptionData::GetInstance().CSVLoad(this);
	if ( bOK )
		DataShow();
}

void CEditorSetOptionDataDlg::OnBnClickedButtonSaveCsv()
{
	GLSetOptionData::GetInstance().CSVSave(this);
}


void CEditorSetOptionDataDlg::OnBnClickedButtonAdd()
{
	m_sDataEdit = SSET_OPTION_DATA();
	SetOptionShow( TRUE, TRUE );
}


void CEditorSetOptionDataDlg::OnBnClickedButtonEdit()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );
	WORD wID = static_cast<WORD>(dwID);

	SSET_OPTION_DATA* pData = GLSetOptionData::GetInstance().GetData( wID );
	if ( !pData )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "[%d]%s failure to get data", wID, strtext.c_str() );
		return;
	}

	m_sDataEdit = *pData;
	SetOptionShow( TRUE, FALSE );
}


void CEditorSetOptionDataDlg::OnBnClickedButtonDelete()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );
	WORD wID = static_cast<WORD>(dwID);

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "delete [%d]%s", wID, strtext.c_str() ) !=  IDYES )
		return;

	SET_OPTION_DATA_MAP &mapData = GLSetOptionData::GetInstance().m_mapSetOptionData;
	SET_OPTION_DATA_MAP_ITER iterpos = mapData.find(wID);
	if ( iterpos != mapData.end() )
	{
		mapData.erase( iterpos );
		DataShow();
	}

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List.EnsureVisible( nNewSelect, TRUE );
	m_List.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CEditorSetOptionDataDlg::OnBnClickedButtonCopy()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );
	WORD wID = static_cast<WORD>(dwID);

	SSET_OPTION_DATA* pData = GLSetOptionData::GetInstance().GetData( wID );
	if ( !pData )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "[%d]%s failure to get data", wID, strtext.c_str() );
		return;
	}

	m_sDataEdit = *pData;
	SetOptionShow( TRUE, TRUE );
}


void CEditorSetOptionDataDlg::OnBnClickedButtonClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "clear all set option data?" ) == IDYES )
	{
		GLSetOptionData::GetInstance().CleanUp();
		DataShow();
	}
}


void CEditorSetOptionDataDlg::OnBnClickedButtonSetOptionSave()
{
	WORD wID = GetWin_Num_int( this, IDC_EDIT_SET_OPTION_ID );
	std::string strName = GetWin_Text( this, IDC_EDIT_SET_OPTION_NAME );

	if ( wID == SET_OPTION_NULL )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid id" );
		return;
	}

	if ( strName.size() <= 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid Name" );
		return;
	}

	m_sDataEdit.wID = wID;
	m_sDataEdit.strName = strName;

	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_HEADGEAR] = GetWin_Check( this, IDC_CHECK_USE_PART_0 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_UPPER] = GetWin_Check( this, IDC_CHECK_USE_PART_1 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_LOWER] = GetWin_Check( this, IDC_CHECK_USE_PART_2 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_HAND] = GetWin_Check( this, IDC_CHECK_USE_PART_3 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_FOOT] = GetWin_Check( this, IDC_CHECK_USE_PART_4 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_WEAPON] = GetWin_Check( this, IDC_CHECK_USE_PART_5 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_NECK] = GetWin_Check( this, IDC_CHECK_USE_PART_6 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_WRIST] = GetWin_Check( this, IDC_CHECK_USE_PART_7 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_FINGER] = GetWin_Check( this, IDC_CHECK_USE_PART_8 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_BELT] = GetWin_Check( this, IDC_CHECK_USE_PART_9 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_EARRING] = GetWin_Check( this, IDC_CHECK_USE_PART_10 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_ACCESSORY] = GetWin_Check( this, IDC_CHECK_USE_PART_11 )? true:false;
	m_sDataEdit.bUseParts[EMSET_OPTION_SLOT_DECORATION] = GetWin_Check( this, IDC_CHECK_USE_PART_12 )? true:false;

	m_sDataEdit.emOptionType[0] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_1 );
	m_sDataEdit.emOptionType[1] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_2 );
	m_sDataEdit.emOptionType[2] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_3 );
	m_sDataEdit.emOptionType[3] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_4 );
	m_sDataEdit.emOptionType[4] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_5 );
	m_sDataEdit.emOptionType[5] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_6 );
	m_sDataEdit.emOptionType[6] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_7 );
	m_sDataEdit.emOptionType[7] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_8 );
	m_sDataEdit.emOptionType[8] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_9 );
	m_sDataEdit.emOptionType[9] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_10 );
	m_sDataEdit.emOptionType[10] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_11 );
	m_sDataEdit.emOptionType[11] = (EMSET_OPTION_VAR)GetWin_Combo_Sel( this, IDC_COMBO_OPTION_12 );

	m_sDataEdit.fOptionVar[0] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_1 );
	m_sDataEdit.fOptionVar[1] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_2 );
	m_sDataEdit.fOptionVar[2] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_3 );
	m_sDataEdit.fOptionVar[3] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_4 );
	m_sDataEdit.fOptionVar[4] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_5 );
	m_sDataEdit.fOptionVar[5] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_6 );
	m_sDataEdit.fOptionVar[6] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_7 );
	m_sDataEdit.fOptionVar[7] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_8 );
	m_sDataEdit.fOptionVar[8] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_9 );
	m_sDataEdit.fOptionVar[9] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_10 );
	m_sDataEdit.fOptionVar[10] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_11 );
	m_sDataEdit.fOptionVar[11] = GetWin_Num_float( this, IDC_EDIT_OPTION_VAR_12 );



	SET_OPTION_DATA_MAP &mapData = GLSetOptionData::GetInstance().m_mapSetOptionData;

	if ( m_bAdd )
	{
		SET_OPTION_DATA_MAP_ITER iter = mapData.find( m_sDataEdit.wID );
		if ( iter == mapData.end() )
		{
			mapData.insert( std::make_pair( m_sDataEdit.wID, m_sDataEdit ) );
		}
		else
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "failed to insert set option, data exist" );
			return;
		}
	}
	else
	{
		SET_OPTION_DATA_MAP_ITER iter = mapData.find( m_sDataEdit.wID );
		if ( iter != mapData.end() )
		{
			(*iter).second = m_sDataEdit;
		}
		else
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "failed to update set option" );
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


void CEditorSetOptionDataDlg::OnNMDblclkListSetOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 1 );
	WORD wID = static_cast<WORD>(dwID);

	SSET_OPTION_DATA* pData = GLSetOptionData::GetInstance().GetData( wID );
	if ( !pData )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "[%d]%s failure to get data", wID, strtext.c_str() );
		return;
	}

	m_sDataEdit = *pData;
	SetOptionShow( TRUE, FALSE );
}


