
// EditorShaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorShader.h"
#include "./EditorShaderDlg.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"

#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Engine/DxCommon/DxShaderData.h"

#include "../Lib_Engine/DxResponseMan.h"
#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Helper/HLibDataConvert.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorShaderDlg dialog


CEditorShaderDlg::CEditorShaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorShaderDlg::IDD, pParent)
	, m_bAdd(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CEditorShaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST, m_List );
	
}

BEGIN_MESSAGE_MAP(CEditorShaderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CEditorShaderDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEditorShaderDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEditorShaderDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CEditorShaderDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEditorShaderDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DATA_SAVE, &CEditorShaderDlg::OnBnClickedButtonDataSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CEditorShaderDlg::OnNMDblclkList)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CEditorShaderDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CEditorShaderDlg message handlers

BOOL CEditorShaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	RECT rectCtrl;
	m_List.SetExtendedStyle ( m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List.GetClientRect ( &rectCtrl );
	m_List.InsertColumn( 0, "ID", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 1, "Name", LVCFMT_LEFT, ( 85* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	//increase edit control size limit
	CWnd* pWnd = GetDlgItem(IDC_EDIT_SHADER);
	CEdit *edit = static_cast<CEdit*>(pWnd); 
	if(edit != 0)
	{
		edit->SetLimitText(0);
	}

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	DxShaderData::GetInstance().LoadFile( "GameShader.shsf" );

	ShowData();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorShaderDlg::OnPaint()
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
HCURSOR CEditorShaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorShaderDlg::PostNcDestroy()
{
	DxShaderData::GetInstance().CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	CDialog::PostNcDestroy();
}

void CEditorShaderDlg::OnBnClickedButtonLoad()
{
	CString szFilter = "Shader Setting File (*.shsf)|*.shsf|";
	CFileDialog dlg( TRUE, ".shsf", DxShaderData::GetInstance().GetPath().c_str(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxShaderData::GetInstance().GetPath().c_str();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = DxShaderData::GetInstance().LoadFile ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load File." );
		}

		ShowData();
	}
}

void CEditorShaderDlg::OnBnClickedButtonSave()
{
	CString szFilter = "Shader Setting File (*.shsf)|*.shsf|";
	CFileDialog dlg( FALSE, ".shsf", DxShaderData::GetInstance().GetPath().c_str(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxShaderData::GetInstance().GetPath().c_str();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = DxShaderData::GetInstance().SaveFile ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Save File" );
			return;
		}
	}
}

void CEditorShaderDlg::OnBnClickedButtonAdd()
{
	ShaderShow( "ShaderFileName.Fx", "//Shader Data", TRUE, TRUE );
}

void CEditorShaderDlg::OnBnClickedButtonEdit()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string strFileName = m_List.GetItemText( nSelect, 1 );

	DxShaderData::SHADER_DATA_MAP &mapdata = DxShaderData::GetInstance().m_mapShader;
	DxShaderData::SHADER_DATA_MAP_ITER it = mapdata.find(strFileName);
	if ( it != mapdata.end() )
	{
		ShaderShow( (*it).first, (*it).second, TRUE, FALSE );
	}
}

void CEditorShaderDlg::OnBnClickedButtonDelete()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string strFileName = m_List.GetItemText( nSelect, 1 );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Delete Shader %s", strFileName.c_str() ) !=  IDYES )
		return;

	DxShaderData::SHADER_DATA_MAP &mapdata = DxShaderData::GetInstance().m_mapShader;
	DxShaderData::SHADER_DATA_MAP_ITER it = mapdata.find(strFileName);
	if ( it != mapdata.end() )
	{
		mapdata.erase( it );
		ShowData();
	}

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List.EnsureVisible( nNewSelect, TRUE );
	m_List.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CEditorShaderDlg::ShowData()
{
	m_List.DeleteAllItems();
	m_List.SetRedraw( FALSE );

	DxShaderData::SHADER_DATA_MAP& mapdata = DxShaderData::GetInstance().m_mapShader;
	DxShaderData::SHADER_DATA_MAP_ITER it = mapdata.begin();
	int idx = 0;
	for ( ; it != mapdata.end(); ++it, ++idx )
	{
		m_List.InsertItem( idx, _HLIB::cstring_dword( idx ).GetString() );
		m_List.SetItemText( idx, 1, (*it).first.c_str() );
	}
	
	m_List.SetRedraw( TRUE );

	ShaderShow( "", "", FALSE, FALSE );
}

void CEditorShaderDlg::ShaderShow( std::string strTitle, std::string strShader, BOOL bENABLE, BOOL bADD )
{
	m_bAdd = bADD;

	SetWin_Text( this, IDC_EDIT_FILENAME, strTitle.c_str() );
	SetWin_Text( this, IDC_EDIT_SHADER, strShader.c_str() );

	SetWin_Enable( this, IDC_EDIT_FILENAME, bENABLE && bADD );
	SetWin_Enable( this, IDC_EDIT_SHADER, bENABLE );

	SetWin_Enable( this, IDC_BUTTON_DATA_SAVE, bENABLE );
}

void CEditorShaderDlg::OnBnClickedButtonDataSave()
{
	std::string strFileName = GetWin_Text( this, IDC_EDIT_FILENAME );
	std::string strShader = GetWin_Text( this, IDC_EDIT_SHADER );

	if ( strFileName.empty() )	
	{
		CDebugSet::MsgBox( "FileName Empty" );
		return;
	}

	if ( strShader.empty() )	
	{
		CDebugSet::MsgBox( "Shader Empty" );
		return;
	}

	DxShaderData::SHADER_DATA_MAP &mapdata = DxShaderData::GetInstance().m_mapShader;

	if ( m_bAdd )
	{
		DxShaderData::SHADER_DATA_MAP_ITER it = mapdata.find(strFileName);
		if ( it != mapdata.end() )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "FileName Exist %s", strFileName.c_str() );
			return;
		}

		mapdata.insert( std::make_pair( strFileName, strShader ) );
		ShowData();

		ShaderShow( strFileName, strShader, TRUE, FALSE );
	}
	else
	{
		DxShaderData::SHADER_DATA_MAP_ITER it = mapdata.find(strFileName);
		if ( it != mapdata.end() )
		{
			(*it).second = strShader;

			ShowData();

			ShaderShow( strFileName, strShader, TRUE, FALSE );
		}else{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to save FileName Exist  %s", strFileName.c_str() );

		}
	}

	for( int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		std::string strtext = m_List.GetItemText( nItem, 1 );
		if( strcmp( strtext.c_str(), strFileName.c_str() ) == 0  )
		{
			m_List.EnsureVisible( nItem, TRUE );
			m_List.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}

void CEditorShaderDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string strFileName = m_List.GetItemText( nSelect, 1 );

	DxShaderData::SHADER_DATA_MAP &mapdata = DxShaderData::GetInstance().m_mapShader;
	DxShaderData::SHADER_DATA_MAP_ITER it = mapdata.find(strFileName);
	if ( it != mapdata.end() )
	{
		ShaderShow( (*it).first, (*it).second, TRUE, FALSE );
	}
}

void CEditorShaderDlg::OnBnClickedButtonClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear All Shader" ) !=  IDYES )
		return;

	DxShaderData::SHADER_DATA_MAP &mapdata = DxShaderData::GetInstance().m_mapShader;
	mapdata.clear();

	ShowData();
}
