
// EditorSetOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorSetOption.h"
#include "./EditorSetOptionDlg.h"

#include "./DlgSetOptionEdit.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"

#include "../Lib_Engine/DxResponseMan.h"

#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"

#include "../Lib_Client/G-Logic/GLSetOption.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorSetOptionDlg dialog


CEditorSetOptionDlg::CEditorSetOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorSetOptionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorSetOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SET_OPTION, m_List );

}

BEGIN_MESSAGE_MAP(CEditorSetOptionDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FILE_LOAD, &CEditorSetOptionDlg::OnBnClickedButtonFileLoad)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAVE, &CEditorSetOptionDlg::OnBnClickedButtonFileSave)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEditorSetOptionDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CEditorSetOptionDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEditorSetOptionDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CEditorSetOptionDlg::OnBnClickedButtonClear)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SET_OPTION, &CEditorSetOptionDlg::OnNMDblclkListSetOption)
END_MESSAGE_MAP()


// CEditorSetOptionDlg message handlers

BOOL CEditorSetOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	RECT rectCtrl;
	m_List.SetExtendedStyle ( m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List.GetClientRect ( &rectCtrl );
	m_List.InsertColumn( 0, "ID", LVCFMT_LEFT, ( 20* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 1, "Name", LVCFMT_LEFT, ( 75* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 2, "", LVCFMT_LEFT, ( 5* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	GLSetOptionManager::GetInstance().Load( SET_OPTION_FILE_NAME );

	DataShow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorSetOptionDlg::OnPaint()
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
HCURSOR CEditorSetOptionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorSetOptionDlg::PostNcDestroy()
{
	GLSetOptionManager::GetInstance().CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}

BOOL CEditorSetOptionDlg::DataShow()
{
	m_List.DeleteAllItems();
	m_List.SetRedraw( FALSE );

	DWORD dwTotal = 0;

	const SET_OPTION_DATA_MAP &mapData = GLSetOptionManager::GetInstance().m_mapSetOption;
	int i=0;
	for( SET_OPTION_DATA_CITER it = mapData.begin(); it != mapData.end(); it++, i++ )
	{
		const SET_OPTION_DATA &OptionData = it->second;

		m_List.InsertItem( i, _HLIB::cstring_dword(OptionData.dwID).GetString() );
		m_List.SetItemText( i, 1, OptionData.strName.c_str() );
		
		m_List.SetItemData( i, OptionData.dwID );
	}

	m_List.SetRedraw( TRUE );

	SetWin_Num_int( this, IDC_EDIT_TOTAL_NUM, static_cast<int>(mapData.size()) );

	return TRUE;
}

void CEditorSetOptionDlg::OnBnClickedButtonFileLoad()
{
	CString szFilter = "OptionFile (*.bin)|*.bin|";
	CFileDialog dlg( TRUE, ".bin", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();
	if ( dlg.DoModal() == IDOK )
	{
		BOOL bOK = GLSetOptionManager::GetInstance().Load ( dlg.GetFileName().GetString() );
		if ( !bOK )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load File" );
		}

		DataShow();
	}
}

void CEditorSetOptionDlg::OnBnClickedButtonFileSave()
{
	CString szFilter = "OptionFile (*.bin)|*.bin|";
	CFileDialog dlg( FALSE, ".bin", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();
	if ( dlg.DoModal() == IDOK )
	{
		BOOL bOK = GLSetOptionManager::GetInstance().Save ( dlg.GetPathName().GetString() );
		if ( !bOK )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Save File" );
			return;
		}
	}
}

void CEditorSetOptionDlg::OnBnClickedButtonAdd()
{
	SET_OPTION_DATA_MAP &mapData = GLSetOptionManager::GetInstance().m_mapSetOption;

	DWORD dwNewKey = UINT_MAX;
	for( DWORD dwKey = 1; dwKey<SET_OPTION_MAXID; ++dwKey )
	{
		SET_OPTION_DATA_ITER it = mapData.find(dwKey);
		if ( it == mapData.end() )
		{
			dwNewKey = dwKey;
			break;
		}
	}

	if ( dwNewKey == UINT_MAX || dwNewKey >= SET_OPTION_MAXID )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Unable to generate new ID" );
		return;
	}

	SET_OPTION_DATA OptionData;
	OptionData.dwID = dwNewKey;
	CDlgSetOptionEdit dlg(this, &OptionData);
	if ( dlg.DoModal() == IDOK )
	{
		mapData.insert(std::make_pair(OptionData.dwID, OptionData));
		DataShow();
	}
	
}

void CEditorSetOptionDlg::OnBnClickedButtonEdit()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) 
		return;

	DWORD dwID = m_List.GetItemData( nSelect );

	SET_OPTION_DATA_MAP &mapData = GLSetOptionManager::GetInstance().m_mapSetOption;
	SET_OPTION_DATA_ITER it = mapData.find(dwID);
	if ( it != mapData.end() )
	{
		CDlgSetOptionEdit dlg(this, &it->second);
		if ( dlg.DoModal() == IDOK )
		{
			DataShow();
		}
	}
}

void CEditorSetOptionDlg::OnBnClickedButtonDelete()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) 
		return;

	DWORD dwID = m_List.GetItemData( nSelect );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Delete ID:%02d", dwID ) !=  IDYES )
		return;

	SET_OPTION_DATA_MAP &mapData = GLSetOptionManager::GetInstance().m_mapSetOption;
	SET_OPTION_DATA_ITER it = mapData.find(dwID);
	if ( it != mapData.end() )
	{
		mapData.erase( it );
		DataShow();
	}

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List.EnsureVisible( nNewSelect, TRUE );
	m_List.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CEditorSetOptionDlg::OnBnClickedButtonClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear All?" ) ==  IDYES )
	{
		SET_OPTION_DATA_MAP &mapData = GLSetOptionManager::GetInstance().m_mapSetOption;
		mapData.clear();
		DataShow();
	}
}

void CEditorSetOptionDlg::OnNMDblclkListSetOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OnBnClickedButtonEdit();
}
