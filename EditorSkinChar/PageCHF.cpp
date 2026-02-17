// PageCHF.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorSkinChar.h"
#include "./EditorSkinCharView.h"
#include "./PageCHF.h"

#include "../Lib_Engine/Meshs/DxSkinDataDummy.h"
#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/FileDialogFilters.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageCHF dialog

IMPLEMENT_DYNAMIC(CPageCHF, CPropertyPage)

CPageCHF::CPageCHF(LOGFONT logfont)
	: CPropertyPage(CPageCHF::IDD)
	, m_pFont( NULL )
	, m_pSkin( NULL )
	, m_pData( NULL )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageCHF::~CPageCHF()
{
	SAFE_DELETE( m_pFont );
}

void CPageCHF::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHF_ANIM, m_List_Anim );
	DDX_Control(pDX, IDC_LIST_CHF_PIECE, m_List_Piece );
}


BEGIN_MESSAGE_MAP(CPageCHF, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_CHF_NEW, OnBnClickedButtonChfNew)
	ON_BN_CLICKED(IDC_BUTTON_CHF_LOAD, OnBnClickedButtonChfLoad)
	ON_BN_CLICKED(IDC_BUTTON_CHF_SAVE, OnBnClickedButtonChfSave)
	ON_BN_CLICKED(IDC_BUTTON_CHF_TEST, OnBnClickedButtonChfTest)
	ON_BN_CLICKED(IDC_BUTTON_CHF_PIECE_CPS, OnBnClickedButtonChfPieceCps)
	ON_BN_CLICKED(IDC_BUTTON_CHF_PIECE_RESET, OnBnClickedButtonChfPieceReset)
	ON_BN_CLICKED(IDC_BUTTON_CHF_ANIM_PLAY, OnBnClickedButtonChfAnimPlay)
	ON_BN_CLICKED(IDC_BUTTON_CHF_ANIM_STOP, OnBnClickedButtonChfAnimStop)
	ON_BN_CLICKED(IDC_BUTTON_CHF_ANIM_ADDCFG, OnBnClickedButtonChfAnimAddcfg)
	ON_BN_CLICKED(IDC_BUTTON_CHF_ANIM_ADDCFG_MULTI, OnBnClickedButtonChfAnimAddcfgMulti)
	ON_BN_CLICKED(IDC_BUTTON_CHF_ANIM_DELETE, OnBnClickedButtonChfAnimDelete)
	ON_BN_CLICKED(IDC_BUTTON_CHF_ANIM_CLEAR, OnBnClickedButtonChfAnimClear)
	ON_BN_CLICKED(IDC_BUTTON_CHF_SKELETON, OnBnClickedButtonChfSkeleton)
	ON_BN_CLICKED(IDC_BUTTON_CHF_APPLY, OnBnClickedButtonChfApply)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHF_ANIM, OnNMDblclkListChfAnim)
	ON_BN_CLICKED(IDC_BUTTON_CHF_PIECE_ABL, OnBnClickedButtonChfPieceAbl)
	ON_BN_CLICKED(IDC_BUTTON_CHF_PIECE_VCF, OnBnClickedButtonChfPieceVcf)
END_MESSAGE_MAP()


// CPageCHF message handlers
BOOL CPageCHF::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	RECT rectCtrl;
	m_List_Anim.SetExtendedStyle ( m_List_Anim.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List_Anim.GetClientRect ( &rectCtrl );
	m_List_Anim.InsertColumn( 0, "Num", LVCFMT_LEFT, ( 15* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List_Anim.InsertColumn( 1, "Animation", LVCFMT_LEFT, ( 60* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List_Anim.InsertColumn( 2, "Main/Sub", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	m_List_Piece.SetExtendedStyle ( m_List_Piece.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List_Piece.GetClientRect ( &rectCtrl );
	m_List_Piece.InsertColumn( 0, "Piece", LVCFMT_LEFT, ( 40* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List_Piece.InsertColumn( 1, "File", LVCFMT_LEFT, ( 60* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	ResetTool();

	return TRUE;  
}

void CPageCHF::OnBnClickedButtonChfNew()
{
	CEditorSkinCharView::GetView()->CHFNew();
	
	m_pSkin = CEditorSkinCharView::GetView()->m_pSkinChar;
	m_pData = CEditorSkinCharView::GetView()->m_pSkinCharData;

	if ( m_pSkin && m_pData )
		DataShow();
}

void CPageCHF::OnBnClickedButtonChfLoad()
{
	CFileDialog dlg ( TRUE, NULL, DxSkinCharDataContainer::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterContainerCHF, this );
	dlg.m_ofn.lpstrInitialDir = DxSkinCharDataContainer::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	BOOL bOK = CEditorSkinCharView::GetView()->CHFLoad( dlg.GetFileName().GetString() );
	if ( bOK )
	{
		m_pSkin = CEditorSkinCharView::GetView()->m_pSkinChar;
		m_pData = CEditorSkinCharView::GetView()->m_pSkinCharData;

		if ( m_pSkin && m_pData )
			DataShow();
	}
}

void CPageCHF::OnBnClickedButtonChfSave()
{
	if( !m_pData )	return;

	if( !m_pData->GetSkeleton() )	
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Skeleton!" );
		return;
	}

	if( !m_pData->GetAniList().size() )	
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Animation Empty!" );
		return;
	}

	CString strsavefile = DxSkinCharDataContainer::GetInstance().GetPath();
	strsavefile += m_pData->GetFileName();
	CFileDialog dlg( FALSE, NULL, strsavefile.GetString(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterContainerCHF, this );
	dlg.m_ofn.lpstrInitialDir = DxSkinCharDataContainer::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	BOOL bOK = CEditorSkinCharView::GetView()->CHFSave( dlg.GetFileName().GetString() );
	if ( bOK )
	{
		if ( m_pSkin && m_pData )
			DataShow();
	}
}

void CPageCHF::OnBnClickedButtonChfTest()
{
	CEditorSkinCharView::GetView()->ResetData();

	CFileDialog dlg ( TRUE, NULL, DxSkinCharDataContainer::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterContainerCHF, this );
	dlg.m_ofn.lpstrInitialDir = DxSkinCharDataContainer::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	std::string strFileName = dlg.GetFileName();

	DxSkinDataDummy dxSkinDummy;
	BOOL bOK = dxSkinDummy.LoadFile ( strFileName.c_str() );
	if ( !bOK )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Error Load File:%s", strFileName.c_str() );
		return;
	}

	SetWin_Text( this, IDC_EDIT_CHF_FILENAME, strFileName.c_str() );
	SetWin_Text( this, IDC_EDIT_CHF_SKELETON, dxSkinDummy.m_strSkeleton.c_str() );
	SetWin_Num_float( this, IDC_EDIT_CHF_SCALE, dxSkinDummy.m_fScale );
	SetWin_Num_float( this, IDC_EDIT_CHF_HEIGHT, 1.0f );
	SetWin_Num_float( this, IDC_EDIT_CHF_RADIUS, 1.0f );
	SetWin_Check( this, IDC_CHECK_CHF_WORLDOBJECT, FALSE );
	SetWin_Check( this, IDC_CHECK_CHF_FORCE_SAFE_SLOT, FALSE );

	m_List_Anim.DeleteAllItems();
	m_List_Piece.DeleteAllItems();

	m_List_Anim.SetRedraw( FALSE );
	m_List_Piece.SetRedraw( FALSE );

	for ( size_t i = 0; i < dxSkinDummy.m_vecANIMINFO.size(); i++ )
	{
		char szTempChar[512];
		sprintf( szTempChar, "%d", i );
		m_List_Anim.InsertItem( i, szTempChar );
		m_List_Anim.SetItemText( i, 1, dxSkinDummy.m_vecANIMINFO[i]->m_szName );

		sprintf( szTempChar, "(%d)(%d)", dxSkinDummy.m_vecANIMINFO[i]->m_MainType, dxSkinDummy.m_vecANIMINFO[i]->m_SubType );
		m_List_Anim.SetItemText( i, 2, szTempChar );
	}

	for ( int i= 0; i<PIECE_SIZE; ++ i )
	{
		m_List_Piece.InsertItem( i, SKINTYPE_STRING::m_szPIECETYPE[i] );
		m_List_Piece.SetItemText( i, 1, dxSkinDummy.m_strPIECE[i].c_str());
	}

	m_List_Anim.SetRedraw( TRUE );
	m_List_Piece.SetRedraw( TRUE );
}

void CPageCHF::ResetTool()
{
	m_pSkin = NULL;
	m_pData = NULL;

	SetWin_Text( this, IDC_EDIT_CHF_FILENAME, "" );
	SetWin_Text( this, IDC_EDIT_CHF_SKELETON, "" );
	SetWin_Num_float( this, IDC_EDIT_CHF_SCALE, 1.0f );
	SetWin_Num_float( this, IDC_EDIT_CHF_HEIGHT, 1.0f );
	SetWin_Num_float( this, IDC_EDIT_CHF_RADIUS, 1.0f );
	SetWin_Check( this, IDC_CHECK_CHF_WORLDOBJECT, FALSE );
	SetWin_Check( this, IDC_CHECK_CHF_FORCE_SAFE_SLOT, FALSE );

	m_List_Anim.DeleteAllItems();
	m_List_Piece.DeleteAllItems();
}

void CPageCHF::DataShow()
{
	if ( !m_pData )	return;

	SetWin_Text( this, IDC_EDIT_CHF_FILENAME, m_pData->GetFileName() );
	SetWin_Text( this, IDC_EDIT_CHF_SKELETON, m_pData->GetSkeletonName() );
	SetWin_Num_float( this, IDC_EDIT_CHF_SCALE, m_pData->m_fScale );
	SetWin_Num_float( this, IDC_EDIT_CHF_HEIGHT, m_pData->m_fHeight );
	SetWin_Num_float( this, IDC_EDIT_CHF_RADIUS, m_pData->m_fRadius );
	SetWin_Check( this, IDC_CHECK_CHF_WORLDOBJECT, m_pData->m_bWorldObj );
	SetWin_Check( this, IDC_CHECK_CHF_FORCE_SAFE_SLOT, m_pData->m_bForceSafeSlot );

	DataShowAnim();
	DataShowPiece();
}

void CPageCHF::DataShowAnim()
{
	m_List_Anim.DeleteAllItems();

	if ( !m_pData )	return;

	m_List_Anim.SetRedraw( FALSE );

	DxSkinAniControl::VECNODE vecAnim = m_pData->GetAniList ();
	for ( int i= 0; i<(int)vecAnim.size(); ++ i )
	{
		char szTempChar[512];
		sprintf( szTempChar, "%d", i );
		m_List_Anim.InsertItem( i, szTempChar );
		m_List_Anim.SetItemText( i, 1, vecAnim[i]->pAnimCont->m_szName );

		sprintf( szTempChar, "(%d)(%d)", vecAnim[i]->pAnimCont->m_MainType, vecAnim[i]->pAnimCont->m_SubType );
		m_List_Anim.SetItemText( i, 2, szTempChar );
	}

	m_List_Anim.SetRedraw( TRUE );
}

void CPageCHF::DataShowPiece()
{
	m_List_Piece.DeleteAllItems();

	if ( !m_pData )	return;

	m_List_Piece.SetRedraw( FALSE );

	for ( int i= 0; i<PIECE_SIZE; ++ i )
	{
		m_List_Piece.InsertItem( i, SKINTYPE_STRING::m_szPIECETYPE[i] );
		
		if ( m_pData->GetAttBone(i) )
		{
			m_List_Piece.SetItemText( i, 1, m_pData->GetAttBone(i)->GetFileName() );
		}
		else if ( m_pData->GetPiece(i) )
		{
			m_List_Piece.SetItemText( i, 1, m_pData->GetPiece(i)->m_szFileName );
		}
	}

	m_List_Piece.SetRedraw( TRUE );
}

void CPageCHF::ResetAnimation()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	m_pSkin->ResetAnimationCore();
	m_pData->ResetAnimationCore();

	if ( m_pSkin->GetSkeleton() && m_pSkin->GetSkeleton()->pBoneRoot )
		m_pSkin->GetSkeleton()->pBoneRoot->ResetBone();

	if ( m_pData->GetSkeleton() && m_pData->GetSkeleton()->pBoneRoot )
		m_pData->GetSkeleton()->pBoneRoot->ResetBone();
}

void CPageCHF::OnBnClickedButtonChfPieceCps()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	CFileDialog dlg( true, NULL, DxSkinPieceContainer::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterPieceCPS, this );
	dlg.m_ofn.lpstrInitialDir = DxSkinPieceContainer::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	std::string strFILE = dlg.GetFileName();
	PDXSKINPIECE pPiece = new DxSkinPiece;
	HRESULT hr = pPiece->LoadPiece( strFILE.c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), TRUE );
	if ( FAILED(hr) )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s Error Loading Piece", strFILE.c_str() );
		SAFE_DELETE( pPiece );
		return;
	}

	//skeleton check
	CString strFile1( m_pData->GetSkeletonName() );
	strFile1.MakeLower();

	CString strFile2( pPiece->m_szSkeleton );
	strFile2.MakeLower();

	if( _tcscmp(strFile1.GetString(),strFile2.GetString()) != 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s Invalid Piece Skeleton Mismatch", strFILE.c_str() );
		SAFE_DELETE( pPiece );
		return;
	}

	m_pSkin->ResetPiece( pPiece->m_emType );
	m_pData->ResetPiece( CEditorSkinCharView::GetView()->Get3DDevice(), pPiece->m_emType );
	
	SAFE_DELETE( pPiece );
	DxSkinPieceContainer::GetInstance().DeletePiece ( strFILE.c_str() );

	hr = m_pData->SetPiece( strFILE.c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), TRUE );
	if ( FAILED(hr) )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s Invalid Piece", strFILE.c_str() );
		return;
	}

	hr = m_pSkin->SetPiece( strFILE.c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), 0, 0, TRUE );
	if ( FAILED(hr) )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s Invalid Piece", strFILE.c_str() );
		return;
	}
	
	DataShowPiece();

	for( int nItem = 0; nItem < m_List_Piece.GetItemCount(); nItem++)
	{
		std::string str = m_List_Piece.GetItemText( nItem, 1 );
		if( _tcscmp( strFILE.c_str(), str.c_str() ) == 0 )
		{
			m_List_Piece.EnsureVisible( nItem, TRUE );
			m_List_Piece.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}

void CPageCHF::OnBnClickedButtonChfPieceReset()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	int nSelect = m_List_Piece.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	m_pSkin->ResetPiece( nSelect );
	m_pData->ResetPiece( CEditorSkinCharView::GetView()->Get3DDevice(), nSelect );
	
	DataShowPiece();
}

void CPageCHF::OnBnClickedButtonChfAnimPlay()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	int nSelect = m_List_Anim.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string str = m_List_Anim.GetItemText( nSelect, 1 );
	m_pSkin->SELECTANI( str.c_str() );
	m_pData->SELECTANI( str.c_str() );
}

void CPageCHF::OnBnClickedButtonChfAnimStop()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	ResetAnimation();
}

void CPageCHF::OnBnClickedButtonChfAnimAddcfg()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	if ( !m_pData->GetSkeleton() )	
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Skeleton!" );
		return;
	}

	CFileDialog dlg( TRUE, NULL, DxSkinAniMan::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterAnimationCFG, this );
	dlg.m_ofn.lpstrInitialDir = DxSkinAniMan::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	boost::filesystem::path path = dlg.GetPathName().GetString();
	path.replace_extension( ".x" );

	PSANIMCONTAINER pAnim = DxSkinAniMan::GetInstance().LoadAnimContainer ( path.filename().string().c_str(), m_pData->GetSkeletonName(), CEditorSkinCharView::GetView()->Get3DDevice(), FALSE );
	if ( !pAnim )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Error Load CFG :%s", path.string().c_str() );
		return;
	}

	ResetAnimation();

	BOOL bOK = m_pSkin->LoadAnimation( path.filename().string().c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), FALSE );
	if ( !bOK )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Error Load Animation :%s", path.string().c_str() );
		return;
	}

	bOK = m_pData->LoadAnimation( path.filename().string().c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), FALSE );
	if ( !bOK )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Error Load Animation :%s", path.string().c_str() );
		return;
	}

	DataShowAnim();

	for( int nItem = 0; nItem < m_List_Anim.GetItemCount(); nItem++)
	{
		std::string str = m_List_Anim.GetItemText( nItem, 1 );
		if( _tcscmp( path.filename().string().c_str(), str.c_str() ) == 0 )
		{
			m_List_Anim.EnsureVisible( nItem, TRUE );
			m_List_Anim.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}

void CPageCHF::OnBnClickedButtonChfAnimAddcfgMulti()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	if ( !m_pData->GetSkeleton() )	
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Invalid Skeleton!" );
		return;
	}

	//prepare file buffer
	const DWORD dwMaxFileNames = 100;
	const DWORD dwMaxFileLength = MAX_PATH + 1;
	const DWORD dwBufferSize = (dwMaxFileNames * dwMaxFileLength) + 1;
	TCHAR* szFileNamesBuffer = new TCHAR[dwBufferSize];
	szFileNamesBuffer[0] = NULL;
	szFileNamesBuffer[dwBufferSize-1] = NULL;

	CFileDialog dlg( TRUE, NULL, DxSkinAniMan::GetInstance().GetPath(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, RFILE_DIALOG_FILTERS::strFilterAnimationCFG, this );
	dlg.m_ofn.lpstrInitialDir = DxSkinAniMan::GetInstance().GetPath();
	dlg.m_ofn.lpstrFile = szFileNamesBuffer;
	dlg.m_ofn.nMaxFile = dwBufferSize;
	
	if ( dlg.DoModal() != IDOK ) return;

	ResetAnimation();

	WORD wInserted = 0;
	WORD wFailed = 0;

	POSITION pos ( dlg.GetStartPosition() );
	while( pos )
	{
		CString strFilePath = dlg.GetNextPathName(pos);

		boost::filesystem::path path = strFilePath.GetString();
		path.replace_extension( ".x" );

		PSANIMCONTAINER pAnim = DxSkinAniMan::GetInstance().LoadAnimContainer ( path.filename().string().c_str(), m_pData->GetSkeletonName(), CEditorSkinCharView::GetView()->Get3DDevice(), FALSE );
		if ( !pAnim )
		{
			wFailed ++;
			continue;
		}

		BOOL bOK = m_pSkin->LoadAnimation( path.filename().string().c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), FALSE );
		if ( !bOK )
		{
			wFailed ++;
			continue;
		}

		bOK = m_pData->LoadAnimation( path.filename().string().c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), FALSE );
		if ( !bOK )
		{
			wFailed ++;
			continue;
		}

		wInserted ++;
	}

	delete[] szFileNamesBuffer;

	CDebugSet::MsgBox( GetSafeHwnd(), "inserted:%d failed :%d", wInserted, wFailed );
	DataShowAnim();
}

void CPageCHF::OnBnClickedButtonChfAnimDelete()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	int nSelect = m_List_Anim.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string str = m_List_Anim.GetItemText( nSelect, 1 );

	ResetAnimation();

	m_pSkin->ReleaseAnimation( str.c_str() );
	m_pData->ReleaseAnimation( str.c_str() );

	DataShowAnim();

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List_Anim.EnsureVisible( nNewSelect, TRUE );
	m_List_Anim.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CPageCHF::OnBnClickedButtonChfAnimClear()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	int nRETURN = CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear all animations?" );
	if ( nRETURN != IDYES )	return;
	
	ResetAnimation();

	DxSkinAniControl::VECNODE vecNode = m_pSkin->GetAniList();
	for( size_t i=0; i<vecNode.size(); ++i )
	{
		std::string strAniName = vecNode[i]->pAnimCont->m_szName;
		m_pSkin->ReleaseAnimation( strAniName.c_str() );
	}

	vecNode = m_pData->GetAniList();
	for( size_t i=0; i<vecNode.size(); ++i )
	{
		std::string strAniName = vecNode[i]->pAnimCont->m_szName;
		m_pData->ReleaseAnimation( strAniName.c_str() );
	}

	DataShowAnim();
}

void CPageCHF::OnBnClickedButtonChfSkeleton()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	int nRETURN = CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Change Skeleton? Piece and animations will be cleared!" );
	if ( nRETURN != IDYES )	return;

	CFileDialog dlg( true, NULL, DxBoneCollector::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterSkin, this );
	dlg.m_ofn.lpstrInitialDir = DxBoneCollector::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	std::string strSkeleton = dlg.GetFileName();
	DxSkeleton* pSkeleton = DxBoneCollector::GetInstance().Load( strSkeleton.c_str(), CEditorSkinCharView::GetView()->Get3DDevice() );
	if ( pSkeleton )
	{
		ResetAnimation();

		m_pData->ClearAll();
		m_pSkin->ClearAll();
		
		BOOL bOK = m_pData->LoadSkeleton( strSkeleton.c_str(), CEditorSkinCharView::GetView()->Get3DDevice() );
		if ( !bOK )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Error Load Skeleton :%s", strSkeleton.c_str() );
			return;
		}

		bOK = m_pSkin->LoadSkeleton( strSkeleton.c_str(), CEditorSkinCharView::GetView()->Get3DDevice() );
		if ( !bOK )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Error Load Skeleton :%s", strSkeleton.c_str() );
			return;
		}

		DataShow();
	}
}

void CPageCHF::OnBnClickedButtonChfApply()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	m_pData->m_fScale = GetWin_Num_float( this, IDC_EDIT_CHF_SCALE );
	m_pData->m_fHeight = GetWin_Num_float( this, IDC_EDIT_CHF_HEIGHT );
	m_pData->m_fRadius = GetWin_Num_float( this, IDC_EDIT_CHF_RADIUS );
	m_pData->m_bWorldObj = GetWin_Check( this, IDC_CHECK_CHF_WORLDOBJECT );
	m_pData->m_bForceSafeSlot = GetWin_Check( this, IDC_CHECK_CHF_FORCE_SAFE_SLOT );

	m_pData->SetRadiusHeight();

	m_pSkin->SetScale( m_pData->m_fScale );
	m_pSkin->SetRadiusHeight( m_pData );
	m_pSkin->SetWorldObj( m_pData->m_bWorldObj );
	m_pSkin->SetForceSafeSlot( m_pData->m_bForceSafeSlot );
}


void CPageCHF::OnNMDblclkListChfAnim(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	int nSelect = m_List_Anim.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string str = m_List_Anim.GetItemText( nSelect, 1 );
	m_pSkin->SELECTANI( str.c_str() );
	m_pData->SELECTANI( str.c_str() );
}

/*ABL system, Juver, 2017/06/01 */
void CPageCHF::OnBnClickedButtonChfPieceAbl()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	CFileDialog dlg( true, NULL, DxAttBoneLinkContainer::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterBoneLinkABL, this );
	dlg.m_ofn.lpstrInitialDir = DxAttBoneLinkContainer::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	std::string strFILE = dlg.GetFileName();

	DxAttBoneLink* pBoneLink = DxAttBoneLinkContainer::GetInstance().LoadData( strFILE.c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), TRUE );
	if ( !pBoneLink )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s Error Loading ABL", strFILE.c_str() );
		return;
	}

	m_pSkin->SetAttBone( pBoneLink, CEditorSkinCharView::GetView()->Get3DDevice(), 0, 0, TRUE );
	m_pData->SetAttBone( pBoneLink, CEditorSkinCharView::GetView()->Get3DDevice(), TRUE, 0 );

	DataShowPiece();

	for( int nItem = 0; nItem < m_List_Piece.GetItemCount(); nItem++)
	{
		std::string str = m_List_Piece.GetItemText( nItem, 1 );
		if( _tcscmp( strFILE.c_str(), str.c_str() ) == 0 )
		{
			m_List_Piece.EnsureVisible( nItem, TRUE );
			m_List_Piece.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}

/*vehicle system, Juver, 2017/08/04 */
void CPageCHF::OnBnClickedButtonChfPieceVcf()
{
	if ( !m_pSkin )	return;
	if ( !m_pData )	return;

	CFileDialog dlg( true, NULL, DxVehicleDataContainer::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, RFILE_DIALOG_FILTERS::strFilterContainerVCF, this );
	dlg.m_ofn.lpstrInitialDir = DxVehicleDataContainer::GetInstance().GetPath();
	if ( dlg.DoModal() != IDOK ) return;

	std::string strFILE = dlg.GetFileName();

	DxVehicleData* pVehicleData = DxVehicleDataContainer::GetInstance().LoadData( strFILE.c_str(), CEditorSkinCharView::GetView()->Get3DDevice(), TRUE );
	if ( !pVehicleData )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "%s Error Loading VCF", strFILE.c_str() );
		return;
	}

	m_pSkin->SetVehicle( pVehicleData, CEditorSkinCharView::GetView()->Get3DDevice(), 0, 0, TRUE );
	m_pData->SetVehicle( pVehicleData, CEditorSkinCharView::GetView()->Get3DDevice(), TRUE, 0 );

	DataShowPiece();

	for( int nItem = 0; nItem < m_List_Piece.GetItemCount(); nItem++)
	{
		std::string str = m_List_Piece.GetItemText( nItem, 1 );
		if( _tcscmp( strFILE.c_str(), str.c_str() ) == 0 )
		{
			m_List_Piece.EnsureVisible( nItem, TRUE );
			m_List_Piece.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}
