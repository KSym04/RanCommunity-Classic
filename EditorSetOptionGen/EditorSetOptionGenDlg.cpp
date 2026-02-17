// EditorSetOptionGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorSetOptionGen.h"
#include "./EditorSetOptionGenDlg.h"

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


// CEditorSetOptionGenDlg dialog



CEditorSetOptionGenDlg::CEditorSetOptionGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorSetOptionGenDlg::IDD, pParent)
	, m_bAdd(FALSE)
{
	m_sDataEdit = SSET_OPTION_GEN();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorSetOptionGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SET_OPTION, m_List );
	DDX_Control(pDX, IDC_LIST_GEN, m_ListGen );

}

BEGIN_MESSAGE_MAP(CEditorSetOptionGenDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CEditorSetOptionGenDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEditorSetOptionGenDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_CSV, &CEditorSetOptionGenDlg::OnBnClickedButtonLoadCsv)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CSV, &CEditorSetOptionGenDlg::OnBnClickedButtonSaveCsv)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEditorSetOptionGenDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CEditorSetOptionGenDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEditorSetOptionGenDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CEditorSetOptionGenDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CEditorSetOptionGenDlg::OnBnClickedButtonClear)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SET_OPTION, &CEditorSetOptionGenDlg::OnNMDblclkListSetOption)
	ON_BN_CLICKED(IDC_BUTTON_GEN_SAVE, &CEditorSetOptionGenDlg::OnBnClickedButtonGenSave)
	ON_BN_CLICKED(IDC_BUTTON_GEN_DEL, &CEditorSetOptionGenDlg::OnBnClickedButtonGenDel)
	ON_BN_CLICKED(IDC_BUTTON_GEN_ADD, &CEditorSetOptionGenDlg::OnBnClickedButtonGenAdd)
	ON_BN_CLICKED(IDC_BUTTON_GEN_ADD_SELECT, &CEditorSetOptionGenDlg::OnBnClickedButtonGenAddSelect)
	ON_BN_CLICKED(IDC_BUTTON_GEN_ADD_SAVE, &CEditorSetOptionGenDlg::OnBnClickedButtonGenAddSave)
	ON_EN_CHANGE(IDC_EDIT_GEN_ADD_ID, &CEditorSetOptionGenDlg::OnEnChangeEditGenAddId)
	ON_BN_CLICKED(IDC_BUTTON_GEN_TEST, &CEditorSetOptionGenDlg::OnBnClickedButtonGenTest)
END_MESSAGE_MAP()


// CEditorSetOptionGenDlg message handlers

BOOL CEditorSetOptionGenDlg::OnInitDialog()
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
	m_List.InsertColumn( 0, "Name", LVCFMT_LEFT, ( 100* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	m_ListGen.SetExtendedStyle ( m_ListGen.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ListGen.GetClientRect ( &rectCtrl );
	m_ListGen.InsertColumn( 0, "ID", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListGen.InsertColumn( 1, "Name", LVCFMT_LEFT, ( 50* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListGen.InsertColumn( 2, "Chance", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );


	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	GLSetOptionGen::GetInstance().LoadSetOptionGen( "SetOptionGen.sgsf" );
	GLSetOptionData::GetInstance().LoadSetOptionData( "SetOptionData.sdsf" );

	

	DataShow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorSetOptionGenDlg::OnPaint() 
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
HCURSOR CEditorSetOptionGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorSetOptionGenDlg::PostNcDestroy()
{
	GLSetOptionGen::GetInstance().CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}

BOOL CEditorSetOptionGenDlg::DataShow()
{
	m_List.DeleteAllItems();
	m_List.SetRedraw( FALSE );

	SET_OPTION_GEN_MAP &mapData = GLSetOptionGen::GetInstance().m_mapSetOptionGen;
	int i=0;
	for( SET_OPTION_GEN_MAP_ITER iter = mapData.begin(); iter != mapData.end(); iter++, i++ )
	{
		SSET_OPTION_GEN &sData = (*iter).second;
		m_List.InsertItem( i, sData.strGenName.c_str() );
		m_List.SetItemData( i, i );
	}

	m_List.SetRedraw( TRUE );

	m_sDataEdit = SSET_OPTION_GEN();
	SetOptionShow( FALSE, FALSE );

	SetWin_Num_int( this, IDC_EDIT_SET_OPTION_NUM, (DWORD)mapData.size() );

	return TRUE;
}

void CEditorSetOptionGenDlg::SetOptionShow( BOOL bEnable, BOOL bAdd )
{
	m_bAdd = bAdd;

	SetWin_Text( this, IDC_EDIT_GEN_NAME, m_sDataEdit.strGenName.c_str() );

	SetWin_Num_float( this, IDC_EDIT_GEN_CHANCE, m_sDataEdit.fChance );

	
	SetWin_Enable( this, IDC_EDIT_GEN_NAME, bEnable && m_bAdd );
	SetWin_Enable( this, IDC_EDIT_GEN_CHANCE, bEnable );

	SetWin_Enable( this, IDC_BUTTON_GEN_ADD, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_DEL, bEnable );
	SetWin_Enable( this, IDC_STATIC_GEN_CHANCE_TOTAL, bEnable );
	SetWin_Enable( this, IDC_EDIT_GEN_TOTAL_CHANCE, bEnable );

	SetWin_Enable( this, IDC_BUTTON_GEN_SAVE, bEnable );
	SetWin_Enable( this, IDC_BUTTON_GEN_TEST, bEnable );

	GenShow();
}

void CEditorSetOptionGenDlg::GenShow()
{
	m_ListGen.DeleteAllItems();
	m_ListGen.SetRedraw( FALSE );

	float fTotal = 0.0f;
	for ( DWORD i=0; i<m_sDataEdit.vecGen.size(); ++i )
	{
		SSET_OPTION_GEN_CHANCE& sGenChance = m_sDataEdit.vecGen[i];

		CString strTemp;
		strTemp.Format( "%d", sGenChance.wGenID );
		m_ListGen.InsertItem( i, strTemp.GetString() );

		SSET_OPTION_DATA* pData = GLSetOptionData::GetInstance().GetData( sGenChance.wGenID );
		if ( pData )
			strTemp.Format( "%s", pData->strName.c_str() );
		else
			strTemp.Format( "%s", "unknown data" );

		m_ListGen.SetItemText( i, 1, strTemp.GetString() );

		strTemp.Format( "%g", sGenChance.fGenChance );
		m_ListGen.SetItemText( i, 2, strTemp.GetString() );

		m_ListGen.SetItemData( i, sGenChance.wGenID );

		fTotal += sGenChance.fGenChance;
	}
	
	m_ListGen.SetRedraw( TRUE );

	SetWin_Enable( this, IDC_BUTTON_GEN_ADD_SELECT, FALSE );
	SetWin_Enable( this, IDC_EDIT_GEN_ADD_ID, FALSE );
	SetWin_Enable( this, IDC_EDIT_GEN_ADD_NAME, FALSE );
	SetWin_Enable( this, IDC_EDIT_GEN_ADD_CHANCE, FALSE );
	SetWin_Enable( this, IDC_BUTTON_GEN_ADD_SAVE, FALSE );
	SetWin_Enable( this, IDC_STATIC_GEN_ADD_CHANCE, FALSE );

	SetWin_Num_float( this, IDC_EDIT_GEN_TOTAL_CHANCE, fTotal );
}

void CEditorSetOptionGenDlg::OnBnClickedButtonLoad()
{
	CString szFilter = "Set Option File (*.sgsf)|*.sgsf|";
	CFileDialog dlg( TRUE, ".sdsf", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();

	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = GLSetOptionGen::GetInstance().LoadSetOptionGen ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load File" );
		}

		DataShow();
	}
}

void CEditorSetOptionGenDlg::OnBnClickedButtonSave()
{
	CString szFilter = "Set Option File (*.sgsf)|*.sgsf|";
	CFileDialog dlg( FALSE, ".sgsf", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();

	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = GLSetOptionGen::GetInstance().SaveSetOptionGen ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Save File" );
			return;
		}
	}
}

void CEditorSetOptionGenDlg::OnBnClickedButtonLoadCsv()
{
	bool bOK = GLSetOptionGen::GetInstance().CSVLoad(this);
	if ( bOK )
		DataShow();
}

void CEditorSetOptionGenDlg::OnBnClickedButtonSaveCsv()
{
	GLSetOptionGen::GetInstance().CSVSave(this);
}





void CEditorSetOptionGenDlg::OnBnClickedButtonAdd()
{
	m_sDataEdit = SSET_OPTION_GEN();
	SetOptionShow( TRUE, TRUE );
}

void CEditorSetOptionGenDlg::OnBnClickedButtonEdit()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 0 );

	SSET_OPTION_GEN* pData = GLSetOptionGen::GetInstance().GetData( strtext );
	if ( !pData )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s failure to get gen data", strtext.c_str() );
		return;
	}

	m_sDataEdit = *pData;
	SetOptionShow( TRUE, FALSE );
}

void CEditorSetOptionGenDlg::OnBnClickedButtonDelete()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 0 );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "delete %s", strtext.c_str() ) !=  IDYES )
		return;

	SET_OPTION_GEN_MAP &mapData = GLSetOptionGen::GetInstance().m_mapSetOptionGen;
	SET_OPTION_GEN_MAP_ITER iterpos = mapData.find(strtext);
	if ( iterpos != mapData.end() )
	{
		mapData.erase( iterpos );
		DataShow();
	}

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List.EnsureVisible( nNewSelect, TRUE );
	m_List.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CEditorSetOptionGenDlg::OnBnClickedButtonCopy()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 0 );

	SSET_OPTION_GEN* pData = GLSetOptionGen::GetInstance().GetData( strtext );
	if ( !pData )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s failure to get gen data", strtext.c_str() );
		return;
	}

	m_sDataEdit = *pData;
	SetOptionShow( TRUE, TRUE );
}

void CEditorSetOptionGenDlg::OnBnClickedButtonClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "clear all set option gen?" ) == IDYES )
	{
		GLSetOptionGen::GetInstance().CleanUp();
		DataShow();
	}
}

void CEditorSetOptionGenDlg::OnNMDblclkListSetOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );
	std::string strtext = m_List.GetItemText( nSelect, 0 );

	SSET_OPTION_GEN* pData = GLSetOptionGen::GetInstance().GetData( strtext );
	if ( !pData )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s failure to get data", strtext.c_str() );
		return;
	}

	m_sDataEdit = *pData;
	SetOptionShow( TRUE, FALSE );
}


void CEditorSetOptionGenDlg::OnBnClickedButtonGenSave()
{
	std::string strName = GetWin_Text( this, IDC_EDIT_GEN_NAME );

	if ( strName.size() <= 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "invalid Name" );
		return;
	}

	m_sDataEdit.strGenName = strName;
	m_sDataEdit.fChance = GetWin_Num_float( this, IDC_EDIT_GEN_CHANCE );

	SET_OPTION_GEN_MAP &mapData = GLSetOptionGen::GetInstance().m_mapSetOptionGen;

	if ( m_bAdd )
	{
		SET_OPTION_GEN_MAP_ITER iter = mapData.find( m_sDataEdit.strGenName );
		if ( iter == mapData.end() )
		{
			mapData.insert( std::make_pair( m_sDataEdit.strGenName, m_sDataEdit ) );
		}
		else
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "failed to insert set option, data exist" );
			return;
		}
	}
	else
	{
		SET_OPTION_GEN_MAP_ITER iter = mapData.find( m_sDataEdit.strGenName );
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

void CEditorSetOptionGenDlg::OnBnClickedButtonGenDel()
{
	int nSelect = m_ListGen.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	WORD wID = (WORD)m_ListGen.GetItemData( nSelect );
	std::string strtext = m_ListGen.GetItemText( nSelect, 1 );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "delete %s", strtext.c_str() ) !=  IDYES )
		return;

	m_sDataEdit.vecGen.erase( m_sDataEdit.vecGen.begin() + nSelect );

	std::sort( m_sDataEdit.vecGen.begin(), m_sDataEdit.vecGen.end() );

	GenShow();
}

void CEditorSetOptionGenDlg::OnBnClickedButtonGenAdd()
{
	SetWin_Enable( this, IDC_BUTTON_GEN_ADD_SELECT, TRUE );
	SetWin_Enable( this, IDC_EDIT_GEN_ADD_ID, TRUE );
	SetWin_Enable( this, IDC_EDIT_GEN_ADD_NAME, TRUE );
	SetWin_Enable( this, IDC_EDIT_GEN_ADD_CHANCE, TRUE );
	SetWin_Enable( this, IDC_BUTTON_GEN_ADD_SAVE, TRUE );
	SetWin_Enable( this, IDC_STATIC_GEN_ADD_CHANCE, TRUE );

	SetWin_Num_int( this, IDC_EDIT_GEN_ADD_ID, 0 );
	SetWin_Num_float( this, IDC_EDIT_GEN_ADD_CHANCE, 0.0f );
}

void CEditorSetOptionGenDlg::OnBnClickedButtonGenAddSelect()
{
	// TODO: Add your control notification handler code here
}

void CEditorSetOptionGenDlg::OnBnClickedButtonGenAddSave()
{
	SSET_OPTION_GEN_CHANCE sGenChance;
	sGenChance.wGenID = GetWin_Num_int( this, IDC_EDIT_GEN_ADD_ID );
	sGenChance.fGenChance = GetWin_Num_float( this, IDC_EDIT_GEN_ADD_CHANCE );

	SSET_OPTION_DATA* pData = GLSetOptionData::GetInstance().GetData( sGenChance.wGenID );
	if ( !pData )
	{
		MessageBox( "invalid id" );
		return;
	}

	if ( sGenChance.fGenChance <= 0.0f )
	{
		MessageBox( "invalid chance" );
		return;
	}

	m_sDataEdit.vecGen.push_back( sGenChance );

	std::sort( m_sDataEdit.vecGen.begin(), m_sDataEdit.vecGen.end() );

	GenShow();
}

void CEditorSetOptionGenDlg::OnEnChangeEditGenAddId()
{
	WORD wGen = GetWin_Num_int( this, IDC_EDIT_GEN_ADD_ID );

	SSET_OPTION_DATA* pData = GLSetOptionData::GetInstance().GetData( wGen );
	if ( pData )
		SetWin_Text( this, IDC_EDIT_GEN_ADD_NAME, pData->strName.c_str() );
	else
		SetWin_Text( this, IDC_EDIT_GEN_ADD_NAME, "unknown data" );
}


void CEditorSetOptionGenDlg::OnBnClickedButtonGenTest()
{
	WORD wID = GLSetOptionGen::GetInstance().GenerateSetOptionID( m_sDataEdit.strGenName );

	CString strTemp;
	strTemp.Format( "%s generated %d", m_sDataEdit.strGenName.c_str(), wID );
	MessageBox( strTemp.GetString() );
}
