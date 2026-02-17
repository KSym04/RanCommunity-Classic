
// EditorBattlePassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorBattlePass.h"
#include "./EditorBattlePassDlg.h"

#include "../Lib_Engine/DxResponseMan.h"
#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"
#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"

#include "../Lib_Client/G-Logic/GLBattlePass.h"
#include "../Lib_Helper/HLibDataConvert.h"
#include "../Lib_Helper/EtcFunction.h"

#include "../Lib_Client/G-Logic/GLCrowData.h"
#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLItemDef.h"
#include "../Lib_Client/G-Logic/GLQuestMan.h"

#include "./DlgFindCrow.h"
#include "./DlgFindMap.h"
#include "./DlgFindItem.h"

#include <boost/filesystem.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorBattlePassDlg dialog




CEditorBattlePassDlg::CEditorBattlePassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorBattlePassDlg::IDD, pParent)
	, m_bAdd(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorBattlePassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BATTLEPASS, m_List );
}

BEGIN_MESSAGE_MAP(CEditorBattlePassDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FILE_LOAD, OnBnClickedButtonFileLoad)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAVE, OnBnClickedButtonFileSave)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BATTLEPASS, OnNMDblclkListBattlePass)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_MOB_KILL_MID, OnEnChangeEditProgressMobKillMid)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_MOB_KILL_SID, OnEnChangeEditProgressMobKillSid)
	ON_BN_CLICKED(IDC_BUTTON_MOB_KILL, OnBnClickedButtonMobKill)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_MAP_KILL_MID, OnEnChangeEditProgressMapKillMid)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_MAP_KILL_SID, OnEnChangeEditProgressMapKillSid)
	ON_BN_CLICKED(IDC_BUTTON_MAP_KILL, OnBnClickedButtonMapKill)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_MAP_REACH_MID, OnEnChangeEditProgressMapReachMid)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_MAP_REACH_SID, OnEnChangeEditProgressMapReachSid)
	ON_BN_CLICKED(IDC_BUTTON_MAP_REACH, OnBnClickedButtonMapReach)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_TAKE_ITEM_MID, OnEnChangeEditProgressTakeItemMid)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_TAKE_ITEM_SID, OnEnChangeEditProgressTakeItemSid)
	ON_BN_CLICKED(IDC_BUTTON_TAKE_ITEM, OnBnClickedButtonTakeItem)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_USE_ITEM_MID, OnEnChangeEditProgressUseItemMid)
	ON_EN_CHANGE(IDC_EDIT_PROGRESS_USE_ITEM_SID, OnEnChangeEditProgressUseItemSid)
	ON_BN_CLICKED(IDC_BUTTON_USE_ITEM, OnBnClickedButtonUseItem)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnBnClickedButtonCopy)
END_MESSAGE_MAP()


// CEditorBattlePassDlg message handlers

BOOL CEditorBattlePassDlg::OnInitDialog()
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
	m_List.InsertColumn( 0, "ID", LVCFMT_LEFT, ( 7* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 1, "Type", LVCFMT_LEFT, ( 13* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 2, "Title", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 3, "Description", LVCFMT_LEFT, ( 13* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 4, "Point", LVCFMT_LEFT, ( 8* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 5, "Info", LVCFMT_LEFT, ( 24* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 6, "", LVCFMT_LEFT, ( 2* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	
	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	GLBattlePass::GetInstance().LoadFile( "BattlePass.bp", FALSE );

	m_mapList.CleanUp();
	m_mapList.LoadMapsListFile( "mapslist.mst" );

	SetWin_Combo_Init( this, IDC_COMBO_TYPE, COMMENT::BATTLEPASS_MID, EMBATTLEPASS_TYPE_SIZE );

	DataShow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorBattlePassDlg::OnPaint()
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
HCURSOR CEditorBattlePassDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorBattlePassDlg::PostNcDestroy()
{
	GLBattlePass::GetInstance().CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}

BOOL CEditorBattlePassDlg::DataShow()
{
	m_List.DeleteAllItems();
	m_List.SetRedraw( FALSE );

	DWORD dwTotal = 0;

	SBATTLEPASS_FILE_DATA_MAP &map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;
	int i=0;
	for( SBATTLEPASS_FILE_DATA_MAP_ITER iter = map_battlepass.begin(); 
		iter != map_battlepass.end(); iter++, i++ )
	{
		const SBATTLEPASS_FILE_DATA &sdata = (*iter).second;

		m_List.InsertItem( i, _HLIB::cstring_dword(sdata.dwBattlePassID).GetString() );
		m_List.SetItemText( i, 1, COMMENT::BATTLEPASS_MID[sdata.emType].c_str() );
		m_List.SetItemText( i, 2, sdata.strBattlePassTitle.c_str() );
		m_List.SetItemText( i, 3, sdata.strBattlePassDescription.c_str() );
		m_List.SetItemText( i, 4, _HLIB::cstring_dword(sdata.dwRewardPoint).GetString() );

		if ( sdata.emType == EMBATTLEPASS_TYPE_KILL_MOB )
		{
			SCROWDATA* pcrow_data = GLCrowDataMan::GetInstance().GetCrowData( sdata.sidMobKill );
			m_List.SetItemText( i, 5, _HLIB::cstringformat( "[%03d~%03d][%03d]%s", 
				sdata.sidMobKill.wMainID, sdata.sidMobKill.wSubID, sdata.wProgressMobKill, 
				pcrow_data? pcrow_data->GetName() : "unknown crow" ).GetString() );
		}
		else if ( sdata.emType == EMBATTLEPASS_TYPE_KILL_PLAYER )
		{
			SMAPNODE* pmapnode = m_mapList.FindMapNode( sdata.sidMapKill );
			m_List.SetItemText( i, 5, _HLIB::cstringformat( "[%03d~%03d][%03d]%s", 
				sdata.sidMapKill.wMainID, sdata.sidMapKill.wSubID, sdata.wProgressMapKill, 
				pmapnode? pmapnode->strMapName.c_str() : "unknown map" ).GetString() );
		}
		else if ( sdata.emType == EMBATTLEPASS_TYPE_REACH_MAP )
		{
			SMAPNODE* pmapnode = m_mapList.FindMapNode( sdata.sidMapReach );
			m_List.SetItemText( i, 5, _HLIB::cstringformat( "[%03d~%03d][%03d]%s", 
				sdata.sidMapReach.wMainID, sdata.sidMapReach.wSubID, sdata.wProgressMapReach, 
				pmapnode? pmapnode->strMapName.c_str() : "unknown map" ).GetString() );
		}
		else if ( sdata.emType == EMBATTLEPASS_TYPE_TAKE_ITEM )
		{
			SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sdata.sidItemGet );
			m_List.SetItemText( i, 5, _HLIB::cstringformat( "[%03d~%03d][%03d]%s", 
				sdata.sidItemGet.wMainID, sdata.sidItemGet.wSubID, sdata.wProgressItemGet, 
				pitem_data? pitem_data->GetName() : "unknown item" ).GetString() );
		}
		else if ( sdata.emType == EMBATTLEPASS_TYPE_USE_ITEM )
		{
			SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sdata.sidItemUse );
			m_List.SetItemText( i, 5, _HLIB::cstringformat( "[%03d~%03d][%03d]%s", 
				sdata.sidItemUse.wMainID, sdata.sidItemUse.wSubID, sdata.wProgressItemUse, 
				pitem_data? pitem_data->GetName() : "unknown item" ).GetString() );
		}

		m_List.SetItemData( i, sdata.dwBattlePassID );

		dwTotal += sdata.dwRewardPoint;
	}

	m_List.SetRedraw( TRUE );

	SBATTLEPASS_FILE_DATA sbattlepass;
	BattlePassShow( sbattlepass, FALSE, FALSE );

	SetWin_Num_int( this, IDC_EDIT_TOTAL_BATTLEPASS, dwTotal );

	return TRUE;
}

void CEditorBattlePassDlg::OnBnClickedButtonFileLoad()
{
    CString szFilter = "BattlePass Setting File (*.bp)|*.bp|";
    CFileDialog dlg( TRUE, ".bp", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

    dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();
    if ( dlg.DoModal() == IDOK )
    {
        HRESULT hr = GLBattlePass::GetInstance().LoadFile ( dlg.GetFileName().GetString(), FALSE );
        if ( FAILED ( hr ) )
        {
            CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load File" );
        }

        DataShow();
    }
}

void CEditorBattlePassDlg::OnBnClickedButtonFileSave() {
    CString initialDir = GLOGIC::GetPath();

    CString searchPattern;
    searchPattern.Format("%s\\*.bp", initialDir);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPattern, & findFileData);
    bool hasBpFile = (hFind != INVALID_HANDLE_VALUE);
    if (hasBpFile) FindClose(hFind); // Đóng handle

    CString szFilter = "BattlePass Setting File (*.bp)|*.bp|";
    CString defaultFileName = hasBpFile ? findFileData.cFileName : "BattlePass.bp";

    CFileDialog fileDlg(FALSE, ".bp", defaultFileName,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
        szFilter, this);

    fileDlg.m_ofn.lpstrInitialDir = initialDir;

    if (fileDlg.DoModal() == IDOK) {
        CString fullPath = fileDlg.GetPathName();

        if (fullPath.Right(3).CompareNoCase(".bp") != 0) {
            fullPath += ".bp";
        }

        HRESULT hr = GLBattlePass::GetInstance().SaveFile(fullPath.GetString());
        if (FAILED(hr)) {
            CDebugSet::MsgBox(GetSafeHwnd(), "Failure to Save File");
            return;
        }
    }
}

void CEditorBattlePassDlg::OnBnClickedButtonAdd()
{
	SBATTLEPASS_FILE_DATA_MAP &map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;

	DWORD dwNewKey = UINT_MAX;
	for( DWORD dwKey = 0; dwKey<GLBattlePass::BATTLEPASS_MAXID; ++dwKey )
	{
		SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(dwKey);
		if ( iterpos == map_battlepass.end() )
		{
			dwNewKey = dwKey;
			break;
		}
	}

	if ( dwNewKey == UINT_MAX || dwNewKey >= GLBattlePass::BATTLEPASS_MAXID )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Unable to generate new ID" );
		return;
	}

	SBATTLEPASS_FILE_DATA sbattlepass;
	sbattlepass.dwBattlePassID = dwNewKey;
	BattlePassShow( sbattlepass, TRUE, TRUE );
}

void CEditorBattlePassDlg::OnBnClickedButtonEdit()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );

	SBATTLEPASS_FILE_DATA_MAP &map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;
	SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(dwID);
	if ( iterpos != map_battlepass.end() )
	{
		SBATTLEPASS_FILE_DATA &sbattlepass = (*iterpos).second;
		BattlePassShow( sbattlepass, TRUE, FALSE );
	}
}

void CEditorBattlePassDlg::OnBnClickedButtonDelete()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Delete BattlePass ID:%02d", dwID ) !=  IDYES )
		return;

	SBATTLEPASS_FILE_DATA_MAP &map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;
	SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(dwID);
	if ( iterpos != map_battlepass.end() )
	{
		map_battlepass.erase( iterpos );
		DataShow();
	}

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List.EnsureVisible( nNewSelect, TRUE );
	m_List.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CEditorBattlePassDlg::OnBnClickedButtonClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear BattlePass?" ) ==  IDYES )
	{
		GLBattlePass::GetInstance().m_mapBattlePass.clear();
		DataShow();
	}
}

void CEditorBattlePassDlg::OnBnClickedButtonSave()
{
	SBATTLEPASS_FILE_DATA sbattlepass_data;

	sbattlepass_data.dwBattlePassID = GetWin_Num_int( this, IDC_EDIT_ID );
	sbattlepass_data.strBattlePassTitle = GetWin_Text( this, IDC_EDIT_TITLE ).GetString();
	sbattlepass_data.strBattlePassDescription = GetWin_Text( this, IDC_EDIT_DESC ).GetString();

	sbattlepass_data.dwRewardPoint = GetWin_Num_int( this, IDC_EDIT_REWARD_POINT );

	sbattlepass_data.emType = static_cast<EMBATTLEPASS_TYPE> ( GetWin_Combo_Sel( this, IDC_COMBO_TYPE ) );

	//mobkill
	sbattlepass_data.sidMobKill.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_MID );
	sbattlepass_data.sidMobKill.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_SID );
	sbattlepass_data.wProgressMobKill = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_NUM );

	//mapkill
	sbattlepass_data.sidMapKill.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_MID );
	sbattlepass_data.sidMapKill.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_SID );
	sbattlepass_data.wProgressMapKill = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_NUM );

	//map reach
	sbattlepass_data.sidMapReach.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_MID );
	sbattlepass_data.sidMapReach.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_SID );
	sbattlepass_data.wProgressMapReach = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_NUM );

	//take item
	sbattlepass_data.sidItemGet.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_MID );
	sbattlepass_data.sidItemGet.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_SID );
	sbattlepass_data.wProgressItemGet = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_NUM );

	//use item
	sbattlepass_data.sidItemUse.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_MID );
	sbattlepass_data.sidItemUse.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_SID );
	sbattlepass_data.wProgressItemUse = GetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_NUM );

	//trade
	sbattlepass_data.wProgressTrade = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TRADE_PLAYER );
	
	//whisper
	sbattlepass_data.wProgressWhisper = GetWin_Num_int( this, IDC_EDIT_PROGRESS_WHISPER_PLAYER );

	//vend time
	sbattlepass_data.wProgressPrivateMarket = GetWin_Num_int( this, IDC_EDIT_PROGRESS_PRIVATE_MARKET );

	//playtime
	sbattlepass_data.wProgressGainPlayTime = GetWin_Num_int( this, IDC_EDIT_PROGRESS_GAIN_PLAYTIME );

	//gold
	sbattlepass_data.wProgressGainGold = GetWin_Num_int( this, IDC_EDIT_PROGRESS_GAIN_GOLD );

	//contrib points
	sbattlepass_data.wProgressGainContriPoint = GetWin_Num_int( this, IDC_EDIT_PROGRESS_GAIN_CONTRIBPOINT );

	if ( sbattlepass_data.dwBattlePassID == UINT_MAX || sbattlepass_data.dwBattlePassID >= GLBattlePass::BATTLEPASS_MAXID )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Invalid ID %d", sbattlepass_data.dwBattlePassID );
		return;
	}

	if ( sbattlepass_data.strBattlePassTitle.empty() )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Title Empty!" );
		return;
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_KILL_MOB )
	{
		SCROWDATA* pcrowdata = GLCrowDataMan::GetInstance().GetCrowData( sbattlepass_data.sidMobKill );
		if ( !pcrowdata )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Crow %d~%d", sbattlepass_data.sidMobKill.wMainID, sbattlepass_data.sidMobKill.wSubID );
			return;
		}

		if ( sbattlepass_data.wProgressMobKill < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Mob Num %d", sbattlepass_data.wProgressMobKill );
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_KILL_PLAYER )
	{
		SMAPNODE* pmap_node = m_mapList.FindMapNode( sbattlepass_data.sidMapKill );
		if ( !pmap_node )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Map %d~%d", sbattlepass_data.sidMapKill.wMainID, sbattlepass_data.sidMapKill.wSubID );
			return;
		}

		if ( sbattlepass_data.wProgressMapKill < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Kill Num %d", sbattlepass_data.wProgressMobKill );
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_REACH_MAP )
	{
		SMAPNODE* pmap_node = m_mapList.FindMapNode( sbattlepass_data.sidMapReach );
		if ( !pmap_node )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Map %d~%d", sbattlepass_data.sidMapReach.wMainID, sbattlepass_data.sidMapReach.wSubID );
			return;
		}

		if ( sbattlepass_data.wProgressMapReach < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid reach Num %d", sbattlepass_data.wProgressMapReach);
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_TAKE_ITEM )
	{
		SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sbattlepass_data.sidItemGet );
		if ( !pitem_data )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid item %d~%d", sbattlepass_data.sidItemGet.wMainID, sbattlepass_data.sidItemGet.wSubID );
			return;
		}

		if ( sbattlepass_data.wProgressItemGet < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid item Num %d", sbattlepass_data.wProgressItemGet);
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_USE_ITEM )
	{
		SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sbattlepass_data.sidItemUse );
		if ( !pitem_data )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid item %d~%d", sbattlepass_data.sidItemUse.wMainID, sbattlepass_data.sidItemUse.wSubID );
			return;
		}

		if ( sbattlepass_data.wProgressItemUse < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid item Num %d", sbattlepass_data.wProgressItemUse);
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_TRADE_PLAYER )
	{
		if ( sbattlepass_data.wProgressTrade < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid trade player count %d", sbattlepass_data.wProgressTrade );
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_WHISPER_PLAYER )
	{
		if ( sbattlepass_data.wProgressWhisper < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid whisper player count %d", sbattlepass_data.wProgressWhisper );
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_PRIVATE_MARKET )
	{
		if ( sbattlepass_data.wProgressPrivateMarket < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid private market online time %d", sbattlepass_data.wProgressPrivateMarket );
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_GAIN_PLAYTIME )
	{
		if ( sbattlepass_data.wProgressGainPlayTime < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid gain play time %d", sbattlepass_data.wProgressGainPlayTime );
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_GAIN_GOLD )
	{
		if ( sbattlepass_data.wProgressGainGold < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid gain gold %d", sbattlepass_data.wProgressGainGold );
			return;
		}
	}

	if ( sbattlepass_data.emType == EMBATTLEPASS_TYPE_GAIN_CONTRIPOINT )
	{
		if ( sbattlepass_data.wProgressGainContriPoint < 1 )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Invalid gain contribution point %d", sbattlepass_data.wProgressGainContriPoint );
			return;
		}
	}

	SBATTLEPASS_FILE_DATA_MAP &map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;

	if ( m_bAdd )
	{
		SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(sbattlepass_data.dwBattlePassID);
		if ( iterpos != map_battlepass.end() )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "ID Exist ID:%d", sbattlepass_data.dwBattlePassID );
			return;
		}

		map_battlepass.insert( std::make_pair( sbattlepass_data.dwBattlePassID, sbattlepass_data ) );
		DataShow();
		BattlePassShow( sbattlepass_data, TRUE, FALSE );
	}
	else
	{
		SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(sbattlepass_data.dwBattlePassID);
		if ( iterpos != map_battlepass.end() )
		{
			SBATTLEPASS_FILE_DATA* pData = &(*iterpos).second;
			*pData = sbattlepass_data;

			DataShow();
			BattlePassShow( *pData, TRUE, FALSE );
		}else{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to save ID :%d", sbattlepass_data.dwBattlePassID );

		}
	}

	for( int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		DWORD dwID = m_List.GetItemData( nItem );
		if( dwID == sbattlepass_data.dwBattlePassID )
		{
			m_List.EnsureVisible( nItem, TRUE );
			m_List.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}

void CEditorBattlePassDlg::OnCbnSelchangeComboType()
{
	EMBATTLEPASS_TYPE emType = static_cast<EMBATTLEPASS_TYPE> ( GetWin_Combo_Sel( this, IDC_COMBO_TYPE ) );
	BOOL bKillMob = BOOL( emType== EMBATTLEPASS_TYPE_KILL_MOB );
	BOOL bKillPlayer = BOOL( emType == EMBATTLEPASS_TYPE_KILL_PLAYER );
	BOOL bReachMap = BOOL( emType == EMBATTLEPASS_TYPE_REACH_MAP );
	BOOL bTakeItem = BOOL( emType == EMBATTLEPASS_TYPE_TAKE_ITEM );
	BOOL bUseItem = BOOL( emType == EMBATTLEPASS_TYPE_USE_ITEM );
	BOOL bTradePlayer = BOOL ( emType == EMBATTLEPASS_TYPE_TRADE_PLAYER );
	BOOL bWhisperPlayer = BOOL ( emType == EMBATTLEPASS_TYPE_WHISPER_PLAYER );
	BOOL bPrivateMarket = BOOL ( emType == EMBATTLEPASS_TYPE_PRIVATE_MARKET );
	BOOL bGainPlayTime = BOOL ( emType == EMBATTLEPASS_TYPE_GAIN_PLAYTIME );
	BOOL bGainGold = BOOL ( emType == EMBATTLEPASS_TYPE_GAIN_GOLD );
	BOOL bGainContriPoint = BOOL ( emType == EMBATTLEPASS_TYPE_GAIN_CONTRIPOINT );

	SetWin_Enable( this, IDC_BUTTON_MOB_KILL, bKillMob );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MOB_KILL_MID, bKillMob );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MOB_KILL_SID, bKillMob );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MOB_KILL_NUM, bKillMob );

	SetWin_Enable( this, IDC_BUTTON_MAP_KILL, bKillPlayer );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_KILL_MID, bKillPlayer );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_KILL_SID, bKillPlayer );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_KILL_NUM, bKillPlayer );

	SetWin_Enable( this, IDC_BUTTON_MAP_REACH, bReachMap );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_REACH_MID, bReachMap );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_REACH_SID, bReachMap );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_REACH_NUM, bReachMap );

	SetWin_Enable( this, IDC_BUTTON_TAKE_ITEM, bTakeItem );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_TAKE_ITEM_MID, bTakeItem );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_TAKE_ITEM_SID, bTakeItem );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_TAKE_ITEM_NUM, bTakeItem );

	SetWin_Enable( this, IDC_BUTTON_USE_ITEM, bUseItem );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_USE_ITEM_MID, bUseItem );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_USE_ITEM_SID, bUseItem );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_USE_ITEM_NUM, bUseItem );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_TRADE_PLAYER, bTradePlayer );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_WHISPER_PLAYER, bWhisperPlayer );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_PRIVATE_MARKET, bPrivateMarket );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_GAIN_PLAYTIME, bGainPlayTime );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_GAIN_GOLD, bGainGold );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_GAIN_CONTRIBPOINT, bGainContriPoint );
}

void CEditorBattlePassDlg::BattlePassShow( SBATTLEPASS_FILE_DATA &sdata, BOOL bEnable, BOOL bAdd )
{
	m_bAdd = bAdd;

	SetWin_Num_int( this, IDC_EDIT_ID, sdata.dwBattlePassID );
	SetWin_Text( this, IDC_EDIT_TITLE, sdata.strBattlePassTitle.c_str() );
	SetWin_Text( this, IDC_EDIT_DESC, sdata.strBattlePassDescription.c_str() );

	SetWin_Num_int( this, IDC_EDIT_REWARD_POINT, sdata.dwRewardPoint );

	SetWin_Combo_Sel( this, IDC_COMBO_TYPE, sdata.emType );

	//mobkill
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_MID, sdata.sidMobKill.wMainID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_SID, sdata.sidMobKill.wSubID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_NUM, sdata.wProgressMobKill );

	//mapkill
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_MID, sdata.sidMapKill.wMainID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_SID, sdata.sidMapKill.wSubID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_NUM, sdata.wProgressMapKill );

	//mapreach
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_MID, sdata.sidMapReach.wMainID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_SID, sdata.sidMapReach.wSubID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_NUM, sdata.wProgressMapReach );

	//take item
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_MID, sdata.sidItemGet.wMainID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_SID, sdata.sidItemGet.wSubID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_NUM, sdata.wProgressItemGet );

	//use item
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_MID, sdata.sidItemUse.wMainID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_SID, sdata.sidItemUse.wSubID );
	SetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_NUM, sdata.wProgressItemUse );

	SetWin_Num_int( this, IDC_EDIT_PROGRESS_TRADE_PLAYER, sdata.wProgressTrade );

	SetWin_Num_int( this, IDC_EDIT_PROGRESS_WHISPER_PLAYER, sdata.wProgressWhisper );

	SetWin_Num_int( this, IDC_EDIT_PROGRESS_PRIVATE_MARKET, sdata.wProgressPrivateMarket );

	SetWin_Num_int( this, IDC_EDIT_PROGRESS_GAIN_PLAYTIME, sdata.wProgressGainPlayTime );

	SetWin_Num_int( this, IDC_EDIT_PROGRESS_GAIN_GOLD, sdata.wProgressGainGold );

	SetWin_Num_int( this, IDC_EDIT_PROGRESS_GAIN_CONTRIBPOINT, sdata.wProgressGainContriPoint );

	SetWin_Enable( this, IDC_EDIT_ID, bEnable && bAdd );
	SetWin_Enable( this, IDC_EDIT_TITLE, bEnable );
	SetWin_Enable( this, IDC_EDIT_DESC, bEnable );

	SetWin_Enable( this, IDC_EDIT_REWARD_POINT, bEnable );

	SetWin_Enable( this, IDC_COMBO_TYPE, bEnable );

	//mobkill
	SetWin_Enable( this, IDC_BUTTON_MOB_KILL, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MOB_KILL_MID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MOB_KILL_SID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MOB_KILL_NUM, bEnable );

	//mapkill
	SetWin_Enable( this, IDC_BUTTON_MAP_KILL, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_KILL_MID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_KILL_SID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_KILL_NUM, bEnable );

	//mapreach
	SetWin_Enable( this, IDC_BUTTON_MAP_REACH, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_REACH_MID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_REACH_SID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_MAP_REACH_NUM, bEnable );

	//take item
	SetWin_Enable( this, IDC_BUTTON_TAKE_ITEM, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_TAKE_ITEM_MID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_TAKE_ITEM_SID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_TAKE_ITEM_NUM, bEnable );

	//use item
	SetWin_Enable( this, IDC_BUTTON_USE_ITEM, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_USE_ITEM_MID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_USE_ITEM_SID, bEnable );
	SetWin_Enable( this, IDC_EDIT_PROGRESS_USE_ITEM_NUM, bEnable );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_TRADE_PLAYER, bEnable );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_WHISPER_PLAYER, bEnable );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_PRIVATE_MARKET, bEnable );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_GAIN_PLAYTIME, bEnable );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_GAIN_GOLD, bEnable );

	SetWin_Enable( this, IDC_EDIT_PROGRESS_GAIN_CONTRIBPOINT, bEnable );

	SetWin_Enable( this, IDC_BUTTON_SAVE, bEnable );

	if ( bEnable )	OnCbnSelchangeComboType();
}

void CEditorBattlePassDlg::OnNMDblclkListBattlePass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );

	SBATTLEPASS_FILE_DATA_MAP &map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;
	SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(dwID);
	if ( iterpos != map_battlepass.end() )
	{
		SBATTLEPASS_FILE_DATA &sbattlepass = (*iterpos).second;
		BattlePassShow( sbattlepass, TRUE, FALSE );
	}
}

void CEditorBattlePassDlg::OnEnChangeEditProgressMobKillMid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_SID );

	SCROWDATA* pcrowdata = GLCrowDataMan::GetInstance().GetCrowData( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_MOB_KILL_NAME, pcrowdata? pcrowdata->GetName(): "unknown crow" );
}

void CEditorBattlePassDlg::OnEnChangeEditProgressMobKillSid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_SID );

	SCROWDATA* pcrowdata = GLCrowDataMan::GetInstance().GetCrowData( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_MOB_KILL_NAME, pcrowdata? pcrowdata->GetName(): "unknown crow" );
}

void CEditorBattlePassDlg::OnBnClickedButtonMobKill()
{
	CDlgFindCrow dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_MID, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_MOB_KILL_SID, dlg.m_sSELECTED.wSubID );
	}
}

void CEditorBattlePassDlg::OnEnChangeEditProgressMapKillMid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_SID );

	SMAPNODE* pmap_node = m_mapList.FindMapNode( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_MAP_KILL_NAME, pmap_node? pmap_node->strMapName.c_str(): "unknown map" );
}

void CEditorBattlePassDlg::OnEnChangeEditProgressMapKillSid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_SID );

	SMAPNODE* pmap_node = m_mapList.FindMapNode( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_MAP_KILL_NAME, pmap_node? pmap_node->strMapName.c_str(): "unknown map" );
}

void CEditorBattlePassDlg::OnBnClickedButtonMapKill()
{
	CDlgFindMap dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_MID, dlg.m_sSELECTED.sMAPID.wMainID );
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_KILL_SID, dlg.m_sSELECTED.sMAPID.wSubID );
	}
}

void CEditorBattlePassDlg::OnEnChangeEditProgressMapReachMid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_SID );

	SMAPNODE* pmap_node = m_mapList.FindMapNode( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_MAP_REACH_NAME, pmap_node? pmap_node->strMapName.c_str(): "unknown map" );
}

void CEditorBattlePassDlg::OnEnChangeEditProgressMapReachSid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_SID );

	SMAPNODE* pmap_node = m_mapList.FindMapNode( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_MAP_REACH_NAME, pmap_node? pmap_node->strMapName.c_str(): "unknown map" );
}

void CEditorBattlePassDlg::OnBnClickedButtonMapReach()
{
	CDlgFindMap dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_MID, dlg.m_sSELECTED.sMAPID.wMainID );
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_MAP_REACH_SID, dlg.m_sSELECTED.sMAPID.wSubID );
	}
}

void CEditorBattlePassDlg::OnEnChangeEditProgressTakeItemMid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_SID );

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_TAKE_ITEM_NAME, pitem_data? pitem_data->GetName(): "unknown item" );
}

void CEditorBattlePassDlg::OnEnChangeEditProgressTakeItemSid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_SID );

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_TAKE_ITEM_NAME, pitem_data? pitem_data->GetName(): "unknown item" );
}

void CEditorBattlePassDlg::OnBnClickedButtonTakeItem()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_MID, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_TAKE_ITEM_SID, dlg.m_sSELECTED.wSubID );
	}
}

void CEditorBattlePassDlg::OnEnChangeEditProgressUseItemMid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_SID );

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_USE_ITEM_NAME, pitem_data? pitem_data->GetName(): "unknown item" );
}

void CEditorBattlePassDlg::OnEnChangeEditProgressUseItemSid()
{
	SNATIVEID sid;
	sid.wMainID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_MID );
	sid.wSubID = GetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_SID );

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sid );
	SetWin_Text( this, IDC_EDIT_PROGRESS_USE_ITEM_NAME, pitem_data? pitem_data->GetName(): "unknown item" );
}

void CEditorBattlePassDlg::OnBnClickedButtonUseItem()
{
	CDlgFindItem dlg(this);
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_MID, dlg.m_sSELECTED.wMainID );
		SetWin_Num_int( this, IDC_EDIT_PROGRESS_USE_ITEM_SID, dlg.m_sSELECTED.wSubID );
	}
}

void CEditorBattlePassDlg::OnBnClickedButtonCopy()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwID = m_List.GetItemData( nSelect );

	SBATTLEPASS_FILE_DATA_MAP &map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;

	DWORD dwNewKey = UINT_MAX;
	for( DWORD dwKey = 0; dwKey<GLBattlePass::BATTLEPASS_MAXID; ++dwKey )
	{
		SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(dwKey);
		if ( iterpos == map_battlepass.end() )
		{
			dwNewKey = dwKey;
			break;
		}
	}

	if ( dwNewKey == UINT_MAX || dwNewKey >= GLBattlePass::BATTLEPASS_MAXID )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Unable to generate new ID" );
		return;
	}

	SBATTLEPASS_FILE_DATA_MAP_ITER iterpos = map_battlepass.find(dwID);
	if ( iterpos != map_battlepass.end() )
	{
		SBATTLEPASS_FILE_DATA sbattlepass = (*iterpos).second;
		sbattlepass.dwBattlePassID = dwNewKey;
		BattlePassShow( sbattlepass, TRUE, TRUE );
	}
}
