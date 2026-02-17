
// EditorMapsListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorMapsList.h"
#include "./EditorMapsListDlg.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"

#include "../Lib_Engine/DxResponseMan.h"

#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Engine/DxOctree/DxLandDef.h"
#include "../Lib_Engine/DxCommon/TextureManager.h"
#include "../Lib_Engine/DxSound/BgmSound.h"
#include "../Lib_Helper/HLibDataConvert.h"

#include "../Lib_Engine/GUInterface/BasicTextLoader.h"

#include <boost/filesystem.hpp>
#include "LuaPlus.h"

#include "../Lib_Helper/standard_string_convert.h"
#include "./LuaHelper_Object.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorMapsListDlg dialog


CEditorMapsListDlg::CEditorMapsListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorMapsListDlg::IDD, pParent)
	, m_bAdd( FALSE )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditorMapsListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAP, m_ListMap );
}

BEGIN_MESSAGE_MAP(CEditorMapsListDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FILE_LOAD, OnBnClickedButtonFileLoad)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAVE, OnBnClickedButtonFileSave)
	ON_BN_CLICKED(IDC_BUTTON_FILE_IMPORT, OnBnClickedButtonFileImport)
	ON_BN_CLICKED(IDC_BUTTON_MAPSLIST_ADD, OnBnClickedButtonMapslistAdd)
	ON_BN_CLICKED(IDC_BUTTON_MAPSLIST_EDIT, OnBnClickedButtonMapslistEdit)
	ON_BN_CLICKED(IDC_BUTTON_MAPSLIST_DELETE, OnBnClickedButtonMapslistDelete)
	ON_BN_CLICKED(IDC_BUTTON_MAPSLIST_CLEAR, OnBnClickedButtonMapslistClear)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAP, OnNMDblclkListMap)
	ON_BN_CLICKED(IDC_BUTTON_MAP_LEVELFILE, OnBnClickedButtonMapLevelfile)
	ON_BN_CLICKED(IDC_BUTTON_MAP_BGMFILE, OnBnClickedButtonMapBgmfile)
	ON_BN_CLICKED(IDC_BUTTON_MAP_LOADINGIMAGE, OnBnClickedButtonMapLoadingimage)
	ON_BN_CLICKED(IDC_BUTTON_MAP_SAVE, OnBnClickedButtonMapSave)
	ON_BN_CLICKED(IDC_BUTTON_MAPSLIST_SETFIELD_0, OnBnClickedButtonMapslistSetfield0)
	ON_BN_CLICKED(IDC_BUTTON_MAPSLIST_COPY, OnBnClickedButtonMapslistCopy)
	ON_BN_CLICKED(IDC_BUTTON_MAPSLIST_CREATE_LIST, &CEditorMapsListDlg::OnBnClickedButtonMapslistCreateList)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_FROM_LUA, &CEditorMapsListDlg::OnBnClickedButtonCreateFromLua)
	ON_BN_CLICKED(IDC_BUTTON_CSV_LOAD, &CEditorMapsListDlg::OnBnClickedButtonCsvLoad)
	ON_BN_CLICKED(IDC_BUTTON_CSV_SAVE, &CEditorMapsListDlg::OnBnClickedButtonCsvSave)
END_MESSAGE_MAP()


// CEditorMapsListDlg message handlers

BOOL CEditorMapsListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	RECT rectCtrl;
	m_ListMap.SetExtendedStyle ( m_ListMap.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ListMap.GetClientRect ( &rectCtrl );
	m_ListMap.InsertColumn( 0, "Use", LVCFMT_LEFT, ( 3* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 1, "ID", LVCFMT_LEFT, ( 8* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 2, "Field", LVCFMT_LEFT, ( 4* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 3, "MapName", LVCFMT_LEFT, ( 14* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 4, "LevFile", LVCFMT_LEFT, ( 14* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 5, "BGM", LVCFMT_LEFT, ( 11* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 6, "Loading", LVCFMT_LEFT, ( 11* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 7, "Flags", LVCFMT_LEFT, ( 33* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListMap.InsertColumn( 8, "", LVCFMT_LEFT, ( 2* ( rectCtrl.right - rectCtrl.left ) ) / 100 );


	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	m_mapList.CleanUp();
	m_mapList.LoadMapsListFile( "mapslist.mst", NULL, 0, TRUE );

	UpdateList();

#ifndef _DEBUG
	SetWin_ShowWindow(this, IDC_BUTTON_CREATE_FROM_LUA, FALSE);
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorMapsListDlg::OnPaint()
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
HCURSOR CEditorMapsListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorMapsListDlg::PostNcDestroy()
{
	m_mapList.CleanUp();
	DxResponseMan::GetInstance().FinalCleanup();
	GMTOOL::CleanUp ();
	CDialog::PostNcDestroy();
}

void CEditorMapsListDlg::UpdateList()
{
	m_ListMap.DeleteAllItems();
	m_ListMap.SetRedraw( FALSE );

	GLMapList::FIELDMAP mapData = m_mapList.GetMapList();

	int nIndex = 0;
	for( int nMID = 0; nMID < MAXLANDMID; ++ nMID )
	{
		for( int nSID = 0; nSID < MAXLANDSID; ++ nSID )
		{
			SNATIVEID sMapID( nMID, nSID );
			GLMapList::FIELDMAP_ITER iterpos = mapData.find( sMapID.dwID );
			if ( iterpos != mapData.end() )
			{
				const SMAPNODE sMapNode = (*iterpos).second;

				m_ListMap.InsertItem( nIndex, _HLIB::cstring_int(sMapNode.bUsed).GetString());
				m_ListMap.SetItemText( nIndex, 1, _HLIB::cstringformat( "%03d~%03d", sMapNode.sNativeID.wMainID, sMapNode.sNativeID.wSubID ).GetString());
				m_ListMap.SetItemText( nIndex, 2, _HLIB::cstring_dword(sMapNode.dwFieldSID).GetString() );
				m_ListMap.SetItemText( nIndex, 3, sMapNode.strMapName.c_str() );
				m_ListMap.SetItemText( nIndex, 4, sMapNode.strFile.c_str() );
				m_ListMap.SetItemText( nIndex, 5, sMapNode.strBGM.c_str() );
				m_ListMap.SetItemText( nIndex, 6, sMapNode.strLoadingImageName.c_str() );
				m_ListMap.SetItemText( nIndex, 7, _HLIB::cstringformat(
					"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", 
					sMapNode.bPeaceZone,
					sMapNode.bCommission,
					sMapNode.bPKZone,
					sMapNode.bFreePK,
					sMapNode.bItemDrop,
					sMapNode.bMove,
					sMapNode.bRestart,
					sMapNode.bPetActivity,
					sMapNode.bDECEXP,
					sMapNode.bVehicleActivity,
					sMapNode.bClubBattleZone,
					sMapNode.bInstantMap,
					sMapNode.bQBoxEnable,
					sMapNode.bLunchBoxForbid,
					sMapNode.bCPReset,
					sMapNode.bCPResetDeath,
					sMapNode.bPKMap,
					sMapNode.bUIMapSelect,
					sMapNode.bUIMapInfo,
					sMapNode.bClubPKRecord,
					sMapNode.bOpenPrivateMarket, /* map private market setting, Juver, 2017/10/02 */
					sMapNode.bPartySparring,
					
					/*map move settings, Juver, 2017/11/25 */
					sMapNode.bBlockTaxi,
					sMapNode.bBlockFriendCard,
					sMapNode.bBlockRecall,
					sMapNode.bBlockTeleport,

					/*global buffs map setting, Juver, 2018/01/23 */
					sMapNode.bDisableSystemBuffs,

					/*hp potion map setting, Juver, 2018/01/23 */
					sMapNode.bBlockHPPotion,
					

					/*map party setting, Juver, 2018/06/29 */
					sMapNode.bBlockParty,

					/*force revive, Juver, 2018/07/09 */
					sMapNode.bForceRevive,

					/* open pk function, Juver, 2019/12/23 */
					sMapNode.bOpenPK,

					/* death hide, Juver, 2020/02/02 */
					sMapNode.bDeathHide,

					/* map entry user verified, Juver, 2020/02/27 */
					sMapNode.bUserVerifiedMapEntry,

					/* costume combine map restriction, Juver, 2020/09/11 */
					sMapNode.bCostumeCombineRestrictMoveSpeed,
					sMapNode.bCostumeCombineRestrictAttackSpeed,

					/* trade restricted, Juver, 2021/06/07 */
					sMapNode.bTradeRestricted,

					sMapNode.bAllowHideDead,

					sMapNode.bShowPkMark,

					sMapNode.bUseAutoPotion,
					sMapNode.bAutoPotionManual,

					sMapNode.bUseAutoPilot,

					sMapNode.bPKMapCount,

					sMapNode.bUseCaptcha,

					sMapNode.bBlockGeneralChat,
					sMapNode.bResetSkillDelayOnDie,
					sMapNode.bBossMap

					).GetString() );

				m_ListMap.SetItemData( nIndex, sMapNode.sNativeID.dwID );

				nIndex++;
			}
		}
	}

	m_ListMap.SetRedraw( TRUE );

	SMAPNODE sMapNode;
	ShowMapData( sMapNode, FALSE, FALSE );
}

void CEditorMapsListDlg::OnBnClickedButtonFileLoad()
{
	CString szFilter = "MapsList Setting File (*.mst)|*.mst|";
	CFileDialog dlg( TRUE, ".mst", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = m_mapList.LoadMapsListFile( dlg.GetFileName().GetString(), NULL, 0, TRUE );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load MapsList Setting File." );
		}

		UpdateList();
	}
}

void CEditorMapsListDlg::OnBnClickedButtonFileSave()
{
	CString szFilter = "MapsList Setting File (*.mst)|*.mst|";
	CFileDialog dlg( FALSE, ".mst", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = m_mapList.SaveMapsListFile ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Save File" );
			return;
		}
	}
}

void CEditorMapsListDlg::OnBnClickedButtonFileImport()
{
	CString szFilter = "MapsList Ini File (*.ini)|*.ini|";
	CFileDialog dlg( TRUE, ".ini", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = m_mapList.LoadMapList_OLD( dlg.GetPathName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to import MapsList ini File." );
		}

		UpdateList();
	}
}

void CEditorMapsListDlg::OnBnClickedButtonMapslistAdd()
{
	GLMapList::FIELDMAP mapData = m_mapList.GetMapList();

	BOOL bAdded = FALSE;
	for( int i = 0; i < MAXLANDMID; ++ i )
	{
		SNATIVEID sID( i, 0 );
		GLMapList::FIELDMAP_ITER iterpos = mapData.find( sID.dwID );
		if ( iterpos == mapData.end() )
		{
			SMAPNODE sMapNode;
			sMapNode.sNativeID = sID;
			ShowMapData( sMapNode, TRUE, TRUE );
			bAdded = TRUE;
			break;
		}
	}

	if ( !bAdded )
	{
		for( int nMID = 0; nMID < MAXLANDMID; ++ nMID )
		{
			for( int nSID = 0; nSID < MAXLANDSID; ++ nSID )
			{
				SNATIVEID sID( nMID, nSID );
				GLMapList::FIELDMAP_ITER iterpos = mapData.find( sID.dwID );
				if ( iterpos == mapData.end() )
				{
					SMAPNODE sMapNode;
					sMapNode.sNativeID = sID;
					ShowMapData( sMapNode, TRUE, TRUE );
					bAdded = TRUE;
					break;
				}
			}

			if ( bAdded )
				break;
		}
	}
}

void CEditorMapsListDlg::OnBnClickedButtonMapslistEdit()
{
	MapEdit();
}

void CEditorMapsListDlg::OnBnClickedButtonMapslistDelete()
{
	int nSelect = m_ListMap.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwMapID = m_ListMap.GetItemData( nSelect );

	SNATIVEID sID( dwMapID );
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Delete MapID ID:%02d~%02d", sID.wMainID, sID.wSubID ) !=  IDYES )
		return;

	GLMapList::FIELDMAP &mapData = m_mapList.GetMapList();
	GLMapList::FIELDMAP_ITER iterpos = mapData.find( dwMapID );
	if ( iterpos != mapData.end() )
	{
		mapData.erase( iterpos );

		UpdateList();

		int nNewSelect = nSelect > 1? nSelect-1:nSelect;
		m_ListMap.EnsureVisible( nNewSelect, TRUE );
		m_ListMap.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
	}
}

void CEditorMapsListDlg::OnBnClickedButtonMapslistClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear MapsList?" ) ==  IDYES )
	{
		GLMapList::FIELDMAP &mapData = m_mapList.GetMapList();
		mapData.clear();
		UpdateList();
	}
}

void CEditorMapsListDlg::OnNMDblclkListMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	MapEdit();
}

void CEditorMapsListDlg::MapEdit()
{
	int nSelect = m_ListMap.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwMapID = m_ListMap.GetItemData( nSelect );

	SNATIVEID sID( dwMapID );

	GLMapList::FIELDMAP mapData = m_mapList.GetMapList();
	GLMapList::FIELDMAP_ITER iterpos = mapData.find( dwMapID );
	if ( iterpos != mapData.end() )
	{
		const SMAPNODE sMapNode = (*iterpos).second;
		ShowMapData( sMapNode, TRUE, FALSE );
	}
}

void CEditorMapsListDlg::ShowMapData( SMAPNODE sMapNode, BOOL bEnable, BOOL bAdd )
{
	m_bAdd = bAdd;

	SetWin_Check( this, IDC_CHECK_MAP_USE, sMapNode.bUsed );
	SetWin_Num_int( this, IDC_EDIT_MAP_FIELD, sMapNode.dwFieldSID );
	SetWin_Num_int( this, IDC_EDIT_MAP_MID, sMapNode.sNativeID.wMainID );
	SetWin_Num_int( this, IDC_EDIT_MAP_SID, sMapNode.sNativeID.wSubID );

	SetWin_Text( this, IDC_EDIT_MAP_NAME, sMapNode.strMapName.c_str() );
	SetWin_Text( this, IDC_EDIT_MAP_LEVELFILE, sMapNode.strFile.c_str() );
	SetWin_Text( this, IDC_EDIT_MAP_BGM, sMapNode.strBGM.c_str() );
	SetWin_Text( this, IDC_EDIT_MAP_LOADING, sMapNode.strLoadingImageName.c_str() );

	SetWin_Check( this, IDC_CHECK_MAP_PEACEZONE, sMapNode.bPeaceZone );
	SetWin_Check( this, IDC_CHECK_MAP_COMMISSION, sMapNode.bCommission );
	SetWin_Check( this, IDC_CHECK_MAP_PKZONE, sMapNode.bPKZone );
	SetWin_Check( this, IDC_CHECK_MAP_FREEPK, sMapNode.bFreePK );
	SetWin_Check( this, IDC_CHECK_MAP_ITEMDROP, sMapNode.bItemDrop );
	SetWin_Check( this, IDC_CHECK_MAP_MOVE, sMapNode.bMove );
	SetWin_Check( this, IDC_CHECK_MAP_RESTART, sMapNode.bRestart );
	SetWin_Check( this, IDC_CHECK_MAP_PETACTIVITY, sMapNode.bPetActivity );
	SetWin_Check( this, IDC_CHECK_MAP_DECEXP, sMapNode.bDECEXP );
	SetWin_Check( this, IDC_CHECK_MAP_VEHICLEACTIVITY, sMapNode.bVehicleActivity );
	SetWin_Check( this, IDC_CHECK_MAP_CLUBBATTLEZONE, sMapNode.bClubBattleZone );
	SetWin_Check( this, IDC_CHECK_MAP_INSTANTMAP, sMapNode.bInstantMap );
	SetWin_Check( this, IDC_CHECK_MAP_QBOXENABLE, sMapNode.bQBoxEnable );
	SetWin_Check( this, IDC_CHECK_MAP_LUNCHBOXFORBID, sMapNode.bLunchBoxForbid );
	SetWin_Check( this, IDC_CHECK_MAP_CPRESET, sMapNode.bCPReset );
	SetWin_Check( this, IDC_CHECK_MAP_CPRESET_DEATH, sMapNode.bCPResetDeath );
	SetWin_Check( this, IDC_CHECK_MAP_PKMAP, sMapNode.bPKMap );

	SetWin_Check( this, IDC_CHECK_MAP_UI_MAP_SELECT, sMapNode.bUIMapSelect );
	SetWin_Check( this, IDC_CHECK_MAP_UI_MAP_INFO, sMapNode.bUIMapInfo );
	SetWin_Check( this, IDC_CHECK_MAP_CLUB_PK_RECORD, sMapNode.bClubPKRecord );
	SetWin_Check( this, IDC_CHECK_MAP_OPEN_PRIVATE_MARKET, sMapNode.bOpenPrivateMarket ); /* map private market setting, Juver, 2017/10/02 */
	SetWin_Check( this, IDC_CHECK_MAP_PARTY_SPARRING, sMapNode.bPartySparring );

	/*map move settings, Juver, 2017/11/25 */
	SetWin_Check( this, IDC_CHECK_MAP_BLOCK_TAXI, sMapNode.bBlockTaxi );
	SetWin_Check( this, IDC_CHECK_MAP_BLOCK_FRIENDCARD, sMapNode.bBlockFriendCard );
	SetWin_Check( this, IDC_CHECK_MAP_BLOCK_RECALL, sMapNode.bBlockRecall );
	SetWin_Check( this, IDC_CHECK_MAP_BLOCK_TELEPORT, sMapNode.bBlockTeleport );

	/*global buffs map setting, Juver, 2018/01/23 */
	SetWin_Check( this, IDC_CHECK_MAP_DISABLE_SYSTEM_BUFFS, sMapNode.bDisableSystemBuffs );

	/*hp potion map setting, Juver, 2018/01/23 */
	SetWin_Check( this, IDC_CHECK_MAP_BLOCK_HPPOTION, sMapNode.bBlockHPPotion );


	/*map party setting, Juver, 2018/06/29 */
	SetWin_Check( this, IDC_CHECK_MAP_BLOCK_PARTY, sMapNode.bBlockParty );

	/*force revive, Juver, 2018/07/09 */
	SetWin_Check( this, IDC_CHECK_MAP_FORCE_REVIVE, sMapNode.bForceRevive );

	/* open pk function, Juver, 2019/12/23 */
	SetWin_Check( this, IDC_CHECK_MAP_OPEN_PK, sMapNode.bOpenPK );

	/* death hide, Juver, 2020/02/02 */
	SetWin_Check( this, IDC_CHECK_MAP_DEATH_HIDE, sMapNode.bDeathHide );

	/* map entry user verified, Juver, 2020/02/27 */
	SetWin_Check( this, IDC_CHECK_MAP_USER_VERIFIED_MAP_ENTRY, sMapNode.bUserVerifiedMapEntry );

	/* costume combine map restriction, Juver, 2020/09/11 */
	SetWin_Check( this, IDC_CHECK_COSTUME_COMBINE_RESTRICT_MOVESPEED, sMapNode.bCostumeCombineRestrictMoveSpeed );
	SetWin_Check( this, IDC_CHECK_COSTUME_COMBINE_RESTRICT_ATTACKSPEED, sMapNode.bCostumeCombineRestrictAttackSpeed );

	/* trade restricted, Juver, 2021/06/07 */
	SetWin_Check( this, IDC_CHECK_MAP_TRADE_RESTRICTED, sMapNode.bTradeRestricted );

	SetWin_Check( this, IDC_CHECK_MAP_ALLOW_HIDE_DEAD, sMapNode.bAllowHideDead );

	SetWin_Check( this, IDC_CHECK_MAP_SHOW_PK_MARK, sMapNode.bShowPkMark );

	SetWin_Check( this, IDC_CHECK_MAP_USE_AUTO_POTION, sMapNode.bUseAutoPotion );
	SetWin_Check( this, IDC_CHECK_MAP_AUTO_POTION_MANUAL, sMapNode.bAutoPotionManual );

	SetWin_Check( this, IDC_CHECK_MAP_USE_AUTO_PILOT, sMapNode.bUseAutoPilot );

	SetWin_Check( this, IDC_CHECK_MAP_DISABLE_GENERAL_CHAT, sMapNode.bBlockGeneralChat );

	SetWin_Check( this, IDC_CHECK_MAP_PK_MAP_COUNT, sMapNode.bPKMapCount );

	SetWin_Check( this, IDC_CHECK_MAP_USE_CAPTCHA, sMapNode.bUseCaptcha );

	SetWin_Check( this, IDC_CHECK_MAP_RESET_SKILL_WHENDIE, sMapNode.bResetSkillDelayOnDie );
	
	SetWin_Check( this, IDC_CHECK_MAP_BOSS_TRACKER, sMapNode.bBossMap );
	
	SetWin_Enable( this, IDC_CHECK_MAP_USE, bEnable );
	SetWin_Enable( this, IDC_EDIT_MAP_FIELD, bEnable );
	SetWin_Enable( this, IDC_EDIT_MAP_MID, bEnable && m_bAdd );
	SetWin_Enable( this, IDC_EDIT_MAP_SID, bEnable && m_bAdd );

	SetWin_Enable( this, IDC_EDIT_MAP_NAME, bEnable );
	SetWin_Enable( this, IDC_EDIT_MAP_LEVELFILE, bEnable );
	SetWin_Enable( this, IDC_EDIT_MAP_BGM, bEnable );
	SetWin_Enable( this, IDC_EDIT_MAP_LOADING, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_PEACEZONE, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_COMMISSION, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_PKZONE, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_FREEPK, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_ITEMDROP, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_MOVE, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_RESTART, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_PETACTIVITY, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_DECEXP, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_VEHICLEACTIVITY, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_CLUBBATTLEZONE, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_INSTANTMAP, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_QBOXENABLE, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_LUNCHBOXFORBID, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_CPRESET, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_CPRESET_DEATH, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_PKMAP, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_UI_MAP_SELECT, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_UI_MAP_INFO, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_CLUB_PK_RECORD, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_OPEN_PRIVATE_MARKET, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_PARTY_SPARRING, bEnable );

	/*map move settings, Juver, 2017/11/25 */
	SetWin_Enable( this, IDC_CHECK_MAP_BLOCK_TAXI, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_BLOCK_FRIENDCARD, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_BLOCK_RECALL, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_BLOCK_TELEPORT, bEnable );

	/*global buffs map setting, Juver, 2018/01/23 */
	SetWin_Enable( this, IDC_CHECK_MAP_DISABLE_SYSTEM_BUFFS, bEnable );

	/*hp potion map setting, Juver, 2018/01/23 */
	SetWin_Enable( this, IDC_CHECK_MAP_BLOCK_HPPOTION, bEnable );

	/*map party setting, Juver, 2018/06/29 */
	SetWin_Enable( this, IDC_CHECK_MAP_BLOCK_PARTY, bEnable );

	/*force revive, Juver, 2018/07/09 */
	SetWin_Enable( this, IDC_CHECK_MAP_FORCE_REVIVE, bEnable );

	/* open pk function, Juver, 2019/12/23 */
	SetWin_Enable( this, IDC_CHECK_MAP_OPEN_PK, bEnable );

	/* death hide, Juver, 2020/02/02 */
	SetWin_Enable( this, IDC_CHECK_MAP_DEATH_HIDE, bEnable );

	/* map entry user verified, Juver, 2020/02/27 */
	SetWin_Enable( this, IDC_CHECK_MAP_USER_VERIFIED_MAP_ENTRY, bEnable );

	/* costume combine map restriction, Juver, 2020/09/11 */
	SetWin_Enable( this, IDC_CHECK_COSTUME_COMBINE_RESTRICT_MOVESPEED, bEnable );
	SetWin_Enable( this, IDC_CHECK_COSTUME_COMBINE_RESTRICT_ATTACKSPEED, bEnable );

	/* trade restricted, Juver, 2021/06/07 */
	SetWin_Enable( this, IDC_CHECK_MAP_TRADE_RESTRICTED, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_ALLOW_HIDE_DEAD, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_SHOW_PK_MARK, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_USE_AUTO_POTION, bEnable );
	SetWin_Enable( this, IDC_CHECK_MAP_AUTO_POTION_MANUAL, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_USE_AUTO_PILOT, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_DISABLE_GENERAL_CHAT, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_PK_MAP_COUNT, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_USE_CAPTCHA, bEnable );

	SetWin_Enable( this, IDC_CHECK_MAP_RESET_SKILL_WHENDIE, bEnable );
	
	SetWin_Enable( this, IDC_CHECK_MAP_BOSS_TRACKER, bEnable );

	SetWin_Enable( this, IDC_BUTTON_MAP_LEVELFILE, bEnable );
	SetWin_Enable( this, IDC_BUTTON_MAP_BGMFILE, bEnable );
	SetWin_Enable( this, IDC_BUTTON_MAP_LOADINGIMAGE, bEnable );
	SetWin_Enable( this, IDC_BUTTON_MAP_SAVE, bEnable );
}

void CEditorMapsListDlg::OnBnClickedButtonMapLevelfile()
{
	CString szFilter = "lev|*.LEV|";
	CFileDialog dlg ( TRUE, ".LEV", GLLevelFile::GetLevelPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = GLLevelFile::GetLevelPath();
	if ( dlg.DoModal() == IDOK )
		SetWin_Text( this, IDC_EDIT_MAP_LEVELFILE, dlg.GetFileName() );
}

void CEditorMapsListDlg::OnBnClickedButtonMapBgmfile()
{
	CString szFilter = "ogg|*.OGG|";
	CFileDialog dlg ( TRUE, ".ogg", DxBgmSound::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = DxBgmSound::GetInstance().GetPath();
	if ( dlg.DoModal() == IDOK )
		SetWin_Text( this, IDC_EDIT_MAP_BGM, dlg.GetFileName() );
}

void CEditorMapsListDlg::OnBnClickedButtonMapLoadingimage()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg( TRUE, ".", TextureManager::GetTexturePath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	
		SetWin_Text ( this, IDC_EDIT_MAP_LOADING, dlg.GetFileName().GetString() );
}

void CEditorMapsListDlg::OnBnClickedButtonMapSave()
{
	SMAPNODE sDATA;
	sDATA.bUsed = GetWin_Check( this, IDC_CHECK_MAP_USE )? TRUE:FALSE;
	sDATA.dwFieldSID = GetWin_Num_int( this, IDC_EDIT_MAP_FIELD );
	sDATA.sNativeID.wMainID = GetWin_Num_int( this, IDC_EDIT_MAP_MID );
	sDATA.sNativeID.wSubID = GetWin_Num_int( this, IDC_EDIT_MAP_SID );

	sDATA.strMapName = GetWin_Text( this, IDC_EDIT_MAP_NAME );
	sDATA.strFile = GetWin_Text( this, IDC_EDIT_MAP_LEVELFILE );
	sDATA.strBGM = GetWin_Text( this, IDC_EDIT_MAP_BGM );
	sDATA.strLoadingImageName = GetWin_Text( this, IDC_EDIT_MAP_LOADING );

	sDATA.bPeaceZone = GetWin_Check( this, IDC_CHECK_MAP_PEACEZONE )? TRUE:FALSE;
	sDATA.bCommission = GetWin_Check( this, IDC_CHECK_MAP_COMMISSION )? TRUE:FALSE;
	sDATA.bPKZone = GetWin_Check( this, IDC_CHECK_MAP_PKZONE )? TRUE:FALSE;
	sDATA.bFreePK = GetWin_Check( this, IDC_CHECK_MAP_FREEPK )? TRUE:FALSE;
	sDATA.bItemDrop = GetWin_Check( this, IDC_CHECK_MAP_ITEMDROP )? TRUE:FALSE;
	sDATA.bMove = GetWin_Check( this, IDC_CHECK_MAP_MOVE )? TRUE:FALSE;
	sDATA.bRestart = GetWin_Check( this, IDC_CHECK_MAP_RESTART )? TRUE:FALSE;
	sDATA.bPetActivity = GetWin_Check( this, IDC_CHECK_MAP_PETACTIVITY )? TRUE:FALSE;
	sDATA.bDECEXP = GetWin_Check( this, IDC_CHECK_MAP_DECEXP )? TRUE:FALSE;
	sDATA.bVehicleActivity = GetWin_Check( this, IDC_CHECK_MAP_VEHICLEACTIVITY )? TRUE:FALSE;
	sDATA.bClubBattleZone = GetWin_Check( this, IDC_CHECK_MAP_CLUBBATTLEZONE )? TRUE:FALSE;
	sDATA.bInstantMap = GetWin_Check( this, IDC_CHECK_MAP_INSTANTMAP )? TRUE:FALSE;
	sDATA.bQBoxEnable = GetWin_Check( this, IDC_CHECK_MAP_QBOXENABLE )? TRUE:FALSE;
	sDATA.bLunchBoxForbid = GetWin_Check( this, IDC_CHECK_MAP_LUNCHBOXFORBID )? TRUE:FALSE;
	sDATA.bCPReset = GetWin_Check( this, IDC_CHECK_MAP_CPRESET )? TRUE:FALSE;
	sDATA.bCPResetDeath = GetWin_Check( this, IDC_CHECK_MAP_CPRESET_DEATH )? TRUE:FALSE;
	sDATA.bPKMap = GetWin_Check( this, IDC_CHECK_MAP_PKMAP )? TRUE:FALSE;

	sDATA.bUIMapSelect = GetWin_Check( this, IDC_CHECK_MAP_UI_MAP_SELECT )? TRUE:FALSE;
	sDATA.bUIMapInfo = GetWin_Check( this, IDC_CHECK_MAP_UI_MAP_INFO )? TRUE:FALSE;
	sDATA.bClubPKRecord = GetWin_Check( this, IDC_CHECK_MAP_CLUB_PK_RECORD )? TRUE:FALSE;
	sDATA.bOpenPrivateMarket = GetWin_Check( this, IDC_CHECK_MAP_OPEN_PRIVATE_MARKET )? TRUE:FALSE; /* map private market setting, Juver, 2017/10/02 */
	sDATA.bPartySparring = GetWin_Check( this, IDC_CHECK_MAP_PARTY_SPARRING )? TRUE:FALSE;

	/*map move settings, Juver, 2017/11/25 */
	sDATA.bBlockTaxi = GetWin_Check( this, IDC_CHECK_MAP_BLOCK_TAXI )? TRUE:FALSE;
	sDATA.bBlockFriendCard = GetWin_Check( this, IDC_CHECK_MAP_BLOCK_FRIENDCARD )? TRUE:FALSE;
	sDATA.bBlockRecall = GetWin_Check( this, IDC_CHECK_MAP_BLOCK_RECALL )? TRUE:FALSE;
	sDATA.bBlockTeleport = GetWin_Check( this, IDC_CHECK_MAP_BLOCK_TELEPORT )? TRUE:FALSE;

	/*global buffs map setting, Juver, 2018/01/23 */
	sDATA.bDisableSystemBuffs = GetWin_Check( this, IDC_CHECK_MAP_DISABLE_SYSTEM_BUFFS )? TRUE:FALSE;

	/*hp potion map setting, Juver, 2018/01/23 */
	sDATA.bBlockHPPotion = GetWin_Check( this, IDC_CHECK_MAP_BLOCK_HPPOTION )? TRUE:FALSE;

	/*map party setting, Juver, 2018/06/29 */
	sDATA.bBlockParty = GetWin_Check( this, IDC_CHECK_MAP_BLOCK_PARTY )? TRUE:FALSE;

	/*force revive, Juver, 2018/07/09 */
	sDATA.bForceRevive = GetWin_Check( this, IDC_CHECK_MAP_FORCE_REVIVE )? TRUE:FALSE;

	/* open pk function, Juver, 2019/12/23 */
	sDATA.bOpenPK = GetWin_Check( this, IDC_CHECK_MAP_OPEN_PK )? TRUE:FALSE;

	/* death hide, Juver, 2020/02/02 */
	sDATA.bDeathHide = GetWin_Check( this, IDC_CHECK_MAP_DEATH_HIDE )? TRUE:FALSE;

	/* map entry user verified, Juver, 2020/02/27 */
	sDATA.bUserVerifiedMapEntry = GetWin_Check( this, IDC_CHECK_MAP_USER_VERIFIED_MAP_ENTRY )? TRUE:FALSE;

	/* costume combine map restriction, Juver, 2020/09/11 */
	sDATA.bCostumeCombineRestrictMoveSpeed = GetWin_Check( this, IDC_CHECK_COSTUME_COMBINE_RESTRICT_MOVESPEED )? TRUE:FALSE;
	sDATA.bCostumeCombineRestrictAttackSpeed = GetWin_Check( this, IDC_CHECK_COSTUME_COMBINE_RESTRICT_ATTACKSPEED )? TRUE:FALSE;

	/* trade restricted, Juver, 2021/06/07 */
	sDATA.bTradeRestricted = GetWin_Check( this, IDC_CHECK_MAP_TRADE_RESTRICTED )? TRUE:FALSE;

	sDATA.bAllowHideDead = GetWin_Check( this, IDC_CHECK_MAP_ALLOW_HIDE_DEAD )? TRUE:FALSE;

	sDATA.bShowPkMark = GetWin_Check( this, IDC_CHECK_MAP_SHOW_PK_MARK )? TRUE:FALSE;

	sDATA.bUseAutoPotion = GetWin_Check( this, IDC_CHECK_MAP_USE_AUTO_POTION )? TRUE:FALSE;
	sDATA.bAutoPotionManual = GetWin_Check( this, IDC_CHECK_MAP_AUTO_POTION_MANUAL )? TRUE:FALSE;

	sDATA.bUseAutoPilot = GetWin_Check( this, IDC_CHECK_MAP_USE_AUTO_PILOT )? TRUE:FALSE;

	sDATA.bBlockGeneralChat = GetWin_Check( this, IDC_CHECK_MAP_DISABLE_GENERAL_CHAT )? TRUE:FALSE;

	sDATA.bPKMapCount = GetWin_Check( this, IDC_CHECK_MAP_PK_MAP_COUNT )? TRUE:FALSE;

	sDATA.bUseCaptcha = GetWin_Check( this, IDC_CHECK_MAP_USE_CAPTCHA )? TRUE:FALSE;

	sDATA.bResetSkillDelayOnDie = GetWin_Check( this, IDC_CHECK_MAP_RESET_SKILL_WHENDIE )? TRUE:FALSE;

	sDATA.bBossMap = GetWin_Check( this, IDC_CHECK_MAP_BOSS_TRACKER )? TRUE:FALSE;
	
	if( sDATA.strMapName.size() > MAP_NAME_MAX )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Max Name Length is only: %d", MAP_NAME_MAX );
		return;
	}

	if ( sDATA.strMapName.size() <= 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Name Empty!" );
		return;
	}

	if ( sDATA.sNativeID.wMainID >= MAXLANDMID )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Max MID is only: %d", MAXLANDMID );
		return;
	}

	if ( sDATA.sNativeID.wSubID >= MAXLANDSID )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Max MID is only: %d", MAXLANDSID );
		return;
	}

	if ( sDATA.strLoadingImageName.size() <= 0 )
		sDATA.strLoadingImageName = "null";

	GLMapList::FIELDMAP &mapData = m_mapList.GetMapList();

	if ( m_bAdd )
	{
		GLMapList::FIELDMAP_ITER iter = mapData.begin ();
		GLMapList::FIELDMAP_ITER iter_end = mapData.end ();

		for ( ; iter!=iter_end; ++iter )
		{
			const SMAPNODE &sMapNode = (*iter).second;

			if ( sMapNode.sNativeID==sDATA.sNativeID )
			{
				CDebugSet::MsgBox( GetSafeHwnd(), "Map ID Exist:%d~%d", sDATA.sNativeID.wMainID, sDATA.sNativeID.wSubID );
				return;
			}

			if ( sMapNode.strFile==sDATA.strFile )
			{
				CDebugSet::MsgBox( GetSafeHwnd(), "Map Lev FileName Used %s", sDATA.strFile.c_str() );
				return;
			}
		}

		mapData.insert ( std::make_pair( sDATA.sNativeID.dwID, sDATA ) );
		UpdateList();
		ShowMapData( sDATA, TRUE, FALSE );
	}
	else
	{
		GLMapList::FIELDMAP_ITER iter = mapData.begin ();
		GLMapList::FIELDMAP_ITER iter_end = mapData.end ();
		for ( ; iter!=iter_end; ++iter )
		{
			const SMAPNODE &sMapNode = (*iter).second;

			if ( sMapNode.sNativeID!=sDATA.sNativeID && sMapNode.strFile==sDATA.strFile )
			{
				CDebugSet::MsgBox( GetSafeHwnd(), "Map Lev FileName Used %s", sDATA.strFile.c_str() );
				return;
			}
		}

		GLMapList::FIELDMAP_ITER iterpos = mapData.find( sDATA.sNativeID.dwID );
		if ( iterpos != mapData.end() )
		{
			SMAPNODE &sMapNode = (*iterpos).second;
			sMapNode = sDATA;
			UpdateList();
			ShowMapData( sMapNode, TRUE, FALSE );
		}
		else
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to save ID:%d~%d", sDATA.sNativeID.wMainID, sDATA.sNativeID.wSubID );
		}
	}

	for( int nItem = 0; nItem < m_ListMap.GetItemCount(); nItem++)
	{
		DWORD dwMapID = m_ListMap.GetItemData( nItem );
		if( dwMapID == sDATA.sNativeID.dwID )
		{
			m_ListMap.EnsureVisible( nItem, TRUE );
			m_ListMap.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}

void CEditorMapsListDlg::OnBnClickedButtonMapslistSetfield0()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Set all FieldID to 0?" ) == IDYES )
	{
		GLMapList::FIELDMAP &mapData = m_mapList.GetMapList();
		for ( GLMapList::FIELDMAP_ITER iterpos =  mapData.begin();
			iterpos != mapData.end();
			++ iterpos )
		{
			SMAPNODE &sMapNode = (*iterpos).second;
			sMapNode.dwFieldSID = 0;
		}

		UpdateList();
	}
}

void CEditorMapsListDlg::OnBnClickedButtonMapslistCopy()
{
	int nSelect = m_ListMap.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	DWORD dwMapID = m_ListMap.GetItemData( nSelect );

	SNATIVEID snewid(false);

	GLMapList::FIELDMAP mapData = m_mapList.GetMapList();

	BOOL bAdded = FALSE;
	for( int i=0; i<MAXLANDMID; ++i )
	{
		SNATIVEID sID( i, 0 );
		GLMapList::FIELDMAP_ITER iterpos = mapData.find( sID.dwID );
		if ( iterpos == mapData.end() )
		{
			snewid = sID;
			bAdded = TRUE;
			break;
		}
	}

	if ( !bAdded )
	{
		for( int nMID=0; nMID<MAXLANDMID; ++nMID )
		{
			for( int nSID=0; nSID<MAXLANDSID; ++nSID )
			{
				SNATIVEID sID( nMID, nSID );
				GLMapList::FIELDMAP_ITER iterpos = mapData.find( sID.dwID );
				if ( iterpos == mapData.end() )
				{
					snewid = sID;
					bAdded = TRUE;
					break;
				}
			}

			if ( bAdded )	break;
		}
	}

	if ( snewid == NATIVEID_NULL() )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "no free id" );
		return;
	}

	GLMapList::FIELDMAP_ITER iterfind = mapData.find( dwMapID );
	if ( iterfind != mapData.end() )
	{
		SMAPNODE sMapNode = (*iterfind).second;
		sMapNode.sNativeID = snewid;
		ShowMapData( sMapNode, TRUE, TRUE );
	}
}


void CEditorMapsListDlg::OnBnClickedButtonMapslistCreateList()
{

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	CString strAppPath = szPath;

	if ( !strAppPath.IsEmpty() )
	{
		DWORD dwFind = strAppPath.ReverseFind ( '\\' );
		if ( dwFind != -1 )
		{
			CString strAppPath2 = strAppPath.Left ( dwFind );

			if ( !strAppPath2.IsEmpty() )
				if ( strAppPath2.GetAt(0) == '"' )
					strAppPath2 = strAppPath2.Right ( strAppPath2.GetLength() - 1 );

			strAppPath = strAppPath2;
		}
	}

	std::string filepath = strAppPath;
	filepath += "\\";
	filepath += "listmap.txt";

	FILE* file=NULL;
	file = fopen ( filepath.c_str(), "wt" );
	if ( !file )	return;


	GLMapList::FIELDMAP mapData = m_mapList.GetMapList();

	for( int nMID = 0; nMID < MAXLANDMID; ++ nMID )
	{
		for( int nSID = 0; nSID < MAXLANDSID; ++ nSID )
		{
			SNATIVEID sMapID( nMID, nSID );
			GLMapList::FIELDMAP_ITER iterpos = mapData.find( sMapID.dwID );
			if ( iterpos != mapData.end() )
			{
				const SMAPNODE sMapNode = (*iterpos).second;
				if ( !sMapNode.bUsed )	continue;

				fprintf ( file, "%d %d    %s \n", sMapNode.sNativeID.wMainID, sMapNode.sNativeID.wSubID, sMapNode.strMapName.c_str() );
			}
		}
	}

	fclose ( file );

	CDebugSet::MsgBox( GetSafeHwnd(), "saved %s", filepath.c_str() );
}

void CEditorMapsListDlg::OnBnClickedButtonCreateFromLua()
{
#ifdef _DEBUG
	std::string strLuaFilePath("");
	std::string strGUIFilePath("");

	

	{
		CString szFilter = "(*.lua)|*.lua|";
		CFileDialog dlg(TRUE, ".lua", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

		if (dlg.DoModal() == IDOK)
			strLuaFilePath = dlg.GetPathName().GetString();
	}

	{
		CString szFilter = "(*.xml)|*.xml|";
		CFileDialog dlg(TRUE, ".xml", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

		if (dlg.DoModal() == IDOK)
			strGUIFilePath = dlg.GetPathName().GetString();
	}

	CBasicTextLoader cBasicLoader;
	BOOL bLoadXML = cBasicLoader.LoadText(strGUIFilePath.c_str(), TRUE);
	if (!bLoadXML)
	{
		CDebugSet::MsgBox("Failure to load :%s", strGUIFilePath.c_str());
		return;
	}

	
	LuaPlus::LuaStateOwner state;
	if (state->DoFile(strLuaFilePath.c_str()) != 0)
	{
		CDebugSet::MsgBox("Lua:%s", LuaPlus::LuaStackObject(state, -1).GetString());
		return;
	}

	GLMapList::FIELDMAP &mapData = m_mapList.GetMapList();
	mapData.clear();

	DWORD dwVersion = 0;
	LuaPlus::LuaObject luaObjectVersion = state->GetGlobals()["VERSION"];
	if ( luaObjectVersion.IsInteger() )
	{
		dwVersion = luaObjectVersion.GetInteger();
	}
	else if ( luaObjectVersion.IsTable()  )
	{
		for (int i = 1; ; ++i)
		{
			LuaPlus::LuaObject entryObj = luaObjectVersion[i];
			if (entryObj.IsNil())
				break;
			if (entryObj.IsInteger())
			{
				dwVersion = entryObj.GetInteger();
			}
		}
	}
	
	if ( dwVersion == 256 )
	{
		LuaPlus::LuaObject testTableObj = state->GetGlobals()["maplist"];
		for (int i = 1; ; ++i)
		{
			LuaPlus::LuaObject entryObj = testTableObj[i];
			if (entryObj.IsNil())
				break;
			if (entryObj.IsTable())
			{
				LuaPlus::LuaObject keyObj;
				LuaPlus::LuaObject valueObj;
				char output[256];

				SMAPNODE sDATA;

				keyObj.AssignString(state, "wMID");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.sNativeID.wMainID = valueObj.GetInteger();

				keyObj.AssignString(state, "wSID");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.sNativeID.wSubID = valueObj.GetInteger();

				keyObj.AssignString(state, "wFieldServerID");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.dwFieldSID = valueObj.GetInteger();

				keyObj.AssignString(state, "strBGMName");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sprintf(output, "%ws", valueObj.GetWString());
				sDATA.strBGM = output;

				keyObj.AssignString(state, "strLodingImag");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sprintf(output, "%ws", valueObj.GetWString());
				sDATA.strLoadingImageName = output;

				keyObj.AssignString(state, "strLevelFileName");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sprintf(output, "%ws", valueObj.GetWString());
				sDATA.strFile = output;

				keyObj.AssignString(state, "bUse");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bUsed = valueObj.GetBoolean();

				keyObj.AssignString(state, "bPeaceZone");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bPeaceZone = valueObj.GetBoolean();

				keyObj.AssignString(state, "bCommission");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bCommission = valueObj.GetBoolean();

				keyObj.AssignString(state, "bPKZone");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bPKZone = valueObj.GetBoolean();

				keyObj.AssignString(state, "bFreePK");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bFreePK = valueObj.GetBoolean();

				keyObj.AssignString(state, "bItemDrop");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bItemDrop = valueObj.GetBoolean();

				keyObj.AssignString(state, "bFriendMove");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bMove = valueObj.GetBoolean();

				keyObj.AssignString(state, "bRestart");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bRestart = valueObj.GetBoolean();

				keyObj.AssignString(state, "bPetActivity");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bPetActivity = valueObj.GetBoolean();

				keyObj.AssignString(state, "bDecExp");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bDECEXP = valueObj.GetBoolean();

				keyObj.AssignString(state, "bVehicleActivity");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bVehicleActivity = valueObj.GetBoolean();

				keyObj.AssignString(state, "bClubBattleZone");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bClubBattleZone = valueObj.GetBoolean();


				keyObj.AssignString(state, "nInstantMap");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				WORD wInstanceType = valueObj.GetInteger();
				if (wInstanceType == 1)
					sDATA.bInstantMap = true;
				else
					sDATA.bInstantMap = false;

				keyObj.AssignString(state, "bQBoxDrop");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bQBoxEnable = valueObj.GetBoolean();

				keyObj.AssignString(state, "bLunchBoxForbid");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bLunchBoxForbid = valueObj.GetBoolean();

				keyObj.AssignString(state, "bCPReset");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bCPReset = valueObj.GetBoolean();

				keyObj.AssignString(state, "bPKMap");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bPKMap = valueObj.GetBoolean();

				keyObj.AssignString(state, "bUIMapSelect");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bUIMapSelect = valueObj.GetBoolean();

				keyObj.AssignString(state, "bUIMapInfo");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bUIMapInfo = valueObj.GetBoolean();


				//keyObj.AssignString(state, "bPartyMap");
				//valueObj = entryObj[keyObj];
				//if (valueObj.IsNil())	continue;
				//sDATA.bPartyMap = valueObj.GetBoolean();

				//keyObj.AssignString(state, "bExpeditionMap");
				//valueObj = entryObj[keyObj];
				//if (valueObj.IsNil())	continue;
				//sDATA.bExpeditionMap = valueObj.GetBoolean();

				keyObj.AssignString(state, "bClubPKRecord");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bClubPKRecord = valueObj.GetBoolean();

				keyObj.AssignString(state, "bPrivateMarketOpenable");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bOpenPrivateMarket = valueObj.GetBoolean();

				keyObj.AssignString(state, "bSparring");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				sDATA.bPartySparring = valueObj.GetBoolean();

				//keyObj.AssignString(state, "bCullByObjectMap");
				//valueObj = entryObj[keyObj];
				//if (valueObj.IsNil())	continue;
				//sDATA.bCullByObjectMap = valueObj.GetBoolean();


				keyObj.AssignString(state, "ProhidibtedItems");
				valueObj = entryObj[keyObj];
				if (valueObj.IsNil())	continue;
				if (valueObj.IsTable())
				{
					LuaPlus::LuaObject keyObj2;
					LuaPlus::LuaObject valueObj2;

					keyObj2.AssignString(state, "Taxi_Card_Block");
					valueObj2 = valueObj[keyObj2];
					if (valueObj2.IsNil())	continue;
					sDATA.bBlockTaxi = valueObj2.GetBoolean();

					keyObj2.AssignString(state, "Teleport_Card_Block");
					valueObj2 = valueObj[keyObj2];
					if (valueObj2.IsNil())	continue;
					sDATA.bBlockTeleport = valueObj2.GetBoolean();

					keyObj2.AssignString(state, "Recall_Block");
					valueObj2 = valueObj[keyObj2];
					if (valueObj2.IsNil())	continue;
					sDATA.bBlockRecall = valueObj2.GetBoolean();
					sDATA.bBlockFriendCard = sDATA.bBlockRecall;

					keyObj2.AssignString(state, "Cure_Block");
					valueObj2 = valueObj[keyObj2];
					if (valueObj2.IsNil())	continue;
					sDATA.bBlockHPPotion = valueObj2.GetBoolean();
				}

				//keyObj.AssignString(state, "bRebirthBlock");
				//valueObj = entryObj[keyObj];
				//if (valueObj.IsNil())	continue;
				//sDATA.bRebirthBlock = valueObj.GetBoolean();


				CString strText;
				strText.Format("MAP_%u_%u", sDATA.sNativeID.wMainID, sDATA.sNativeID.wSubID);

				sDATA.strMapName = cBasicLoader.GetText(strText.GetString(), 0).GetString();


				if (sDATA.strMapName.size() > MAP_NAME_MAX)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Max Name Length is only: %d", MAP_NAME_MAX);
					continue;
				}

				if (sDATA.strMapName.size() <= 0)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Name Empty!");
					continue;
				}

				if (sDATA.sNativeID.wMainID >= MAXLANDMID)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Max MID is only: %d", MAXLANDMID);
					continue;
				}

				if (sDATA.sNativeID.wSubID >= MAXLANDSID)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Max MID is only: %d", MAXLANDSID);
					continue;
				}

				if (sDATA.strLoadingImageName.size() <= 0)
					sDATA.strLoadingImageName = "null";

				GLMapList::FIELDMAP &mapData = m_mapList.GetMapList();


				GLMapList::FIELDMAP_ITER iter = mapData.begin();
				GLMapList::FIELDMAP_ITER iter_end = mapData.end();

				for (; iter != iter_end; ++iter)
				{
					const SMAPNODE &sMapNode = (*iter).second;

					if (sMapNode.sNativeID == sDATA.sNativeID)
					{
						CDebugSet::MsgBox(GetSafeHwnd(), "Map ID Exist:%d~%d", sDATA.sNativeID.wMainID, sDATA.sNativeID.wSubID);
						continue;
					}

					if (sMapNode.strFile == sDATA.strFile)
					{
						CDebugSet::MsgBox(GetSafeHwnd(), "Map Lev FileName Used %s", sDATA.strFile.c_str());
						continue;
					}
				}

				mapData.insert(std::make_pair(sDATA.sNativeID.dwID, sDATA));
			}
		}
	}
	else
	{
		//older version of mapslist lua used by official
		//not stable since its using array loop instead of key lookup
		LuaPlus::LuaObject luaMapslistTables = state->GetGlobals()["maplist"];
		for (int i = 1; ; ++i)
		{
			LuaPlus::LuaObject luaMapslistTable = luaMapslistTables[i];
			if (luaMapslistTable.IsNil())
				break;

			if (luaMapslistTable.IsTable())
			{
				SMAPNODE sDATA;
				char output[256];
				LuaPlus::LuaObject luaObject;

				int index = 1;
				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsInteger() )
					sDATA.sNativeID.wMainID = luaObject.GetInteger();
				
				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsInteger() )
					sDATA.sNativeID.wSubID = luaObject.GetInteger();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsInteger() )
					sDATA.dwFieldSID = luaObject.GetInteger();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.GetWString() )
				{
					sprintf(output, "%ws", luaObject.GetWString());
					sDATA.strBGM = output;
				}

				luaObject = luaMapslistTable[index++];
				if ( luaObject.GetWString() )
				{
					sprintf(output, "%ws", luaObject.GetWString());
					sDATA.strLoadingImageName = output;
				}

				luaObject = luaMapslistTable[index++];
				if ( luaObject.GetWString() )
				{
					sprintf(output, "%ws", luaObject.GetWString());
					sDATA.strFile = output;
				}

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bUsed = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bPeaceZone = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bCommission = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bPKZone = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bFreePK = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bItemDrop = luaObject.GetBoolean();
			
				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bMove = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bRestart = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bPetActivity = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bDECEXP = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bVehicleActivity = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bClubBattleZone = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bInstantMap = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bQBoxEnable = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bLunchBoxForbid = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bCPReset = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bPKMap = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bUIMapSelect = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bUIMapInfo = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bClubPKRecord = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bOpenPrivateMarket = luaObject.GetBoolean();

				luaObject = luaMapslistTable[index++];
				if ( luaObject.IsBoolean() )
					sDATA.bPartySparring = luaObject.GetBoolean();

			
				CString strText;
				strText.Format("MAP_%u_%u", sDATA.sNativeID.wMainID, sDATA.sNativeID.wSubID);

				sDATA.strMapName = cBasicLoader.GetText(strText.GetString(), 0).GetString();


				if (sDATA.strMapName.size() > MAP_NAME_MAX)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Max Name Length is only: %d", MAP_NAME_MAX);
					continue;
				}

				if (sDATA.strMapName.size() <= 0)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Name Empty!");
					continue;
				}

				if (sDATA.sNativeID.wMainID >= MAXLANDMID)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Max MID is only: %d", MAXLANDMID);
					continue;
				}

				if (sDATA.sNativeID.wSubID >= MAXLANDSID)
				{
					CDebugSet::MsgBox(GetSafeHwnd(), "Max MID is only: %d", MAXLANDSID);
					continue;
				}

				if (sDATA.strLoadingImageName.size() <= 0)
					sDATA.strLoadingImageName = "null";

				GLMapList::FIELDMAP &mapData = m_mapList.GetMapList();


				GLMapList::FIELDMAP_ITER iter = mapData.begin();
				GLMapList::FIELDMAP_ITER iter_end = mapData.end();

				for (; iter != iter_end; ++iter)
				{
					const SMAPNODE &sMapNode = (*iter).second;

					if (sMapNode.sNativeID == sDATA.sNativeID)
					{
						CDebugSet::MsgBox(GetSafeHwnd(), "Map ID Exist:%d~%d", sDATA.sNativeID.wMainID, sDATA.sNativeID.wSubID);
						continue;
					}

					if (sMapNode.strFile == sDATA.strFile)
					{
						CDebugSet::MsgBox(GetSafeHwnd(), "Map Lev FileName Used %s", sDATA.strFile.c_str());
						continue;
					}
				}

				mapData.insert(std::make_pair(sDATA.sNativeID.dwID, sDATA));

				
			}
		}
	}


	

	
	
	UpdateList();
	SMAPNODE sDATA;
	ShowMapData(sDATA, TRUE, FALSE);

#endif

}


void CEditorMapsListDlg::OnBnClickedButtonCsvLoad()
{
	CString szFilter = "Maps List Csv File (*.csv)|*.csv|";

	CFileDialog dlg(TRUE,".csv",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, (CEditorMapsListDlg*)this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		std::string strFileName = dlg.GetPathName();
		bool bOK = m_mapList.LoadCsv( strFileName );
		if ( !bOK )
		{
			MessageBox ( "LoadCsv Error", "", MB_OK );
		}
		else
		{
			MessageBox ( "LoadCsv Done", "", MB_OK );
		}

		UpdateList();
	}	
}


void CEditorMapsListDlg::OnBnClickedButtonCsvSave()
{
	CString szFilter = "Maps List Csv File (*.csv)|*.csv|";

	CFileDialog dlg(FALSE,".csv",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, (CEditorMapsListDlg*)this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		std::string strFileName = dlg.GetPathName();

		bool bOK = m_mapList.SaveCsv( strFileName );
		if ( !bOK )
		{
			MessageBox ( "SaveCsv Error", "", MB_OK );
		}
		else
		{
			MessageBox ( "SaveCsv Done", "", MB_OK );
		}
	}
}
