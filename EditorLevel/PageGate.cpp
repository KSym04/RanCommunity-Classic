// PageGate.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorLevel.h"
#include "./PageGate.h"
#include "./SheetWithTab.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./EditorLevelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageGate dialog

IMPLEMENT_DYNAMIC(CPageGate, CPropertyPage)

CPageGate::CPageGate(LOGFONT logfont)
	: CPropertyPage(CPageGate::IDD)
	, m_pFont ( NULL )
	, m_pGLLand( NULL )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageGate::~CPageGate()
{
	SAFE_DELETE ( m_pFont );
}

void CPageGate::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CPageGate, CPropertyPage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnNMDblclkList)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_REBUILDNAME, OnBnClickedButtonRebuildname)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_GATE_LOAD_CSV, &CPageGate::OnBnClickedButtonGateLoadCsv)
	ON_BN_CLICKED(IDC_BUTTON_GATE_SAVE_CSV, &CPageGate::OnBnClickedButtonGateSaveCsv)
END_MESSAGE_MAP()


// CPageGate message handlers
BOOL CPageGate::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	RECT rectCtrl;
	LONG lnWidth;
	LV_COLUMN lvColumn;

	m_List.SetExtendedStyle ( m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );
	m_List.GetClientRect ( &rectCtrl );
	lnWidth = rectCtrl.right - rectCtrl.left;

	const int nColumnCount = 5;	
	char* szColumnName[nColumnCount] = { "", "Num", "Gate ID", "Gate Name", "ToMap" };
	int nColumnWidthPercent[nColumnCount] = { 5, 6, 12, 31, 46 };

	for ( int i = 0; i < nColumnCount; i++ )
	{		
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.pszText = szColumnName[i];
		lvColumn.iSubItem = i;
		lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
		m_List.InsertColumn (i, &lvColumn );
	}

	return TRUE;  
}

void CPageGate::DataSet( GLLandMan* pLand )
{
	m_pGLLand = pLand;
	DataShow();
}

void CPageGate::ResetEditor()
{
	m_pGLLand = NULL;
}

void CPageGate::DataShow()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	m_List.DeleteAllItems();

	if ( !m_pGLLand )	return;

	char szTempChar[512];
	int i = 0;

	m_List.SetRedraw( FALSE );

	PDXLANDGATE pLandGate = m_pGLLand->GetLandGateMan().GetListLandGate();
	while ( pLandGate )
	{
		m_List.InsertItem( i, "" );
		sprintf( szTempChar, "%d", i );
		m_List.SetItemText( i, 1, szTempChar );
		sprintf( szTempChar, "%d", pLandGate->GetGateID());
		m_List.SetItemText( i, 2, szTempChar );
		m_List.SetItemText( i, 3, pLandGate->GetName() );
		
		/* multi gate out, Juver, 2020/11/18 */
		if ( pLandGate->GetNewSystem() )
		{
			memset( szTempChar, 0, sizeof(szTempChar) );

			for ( DWORD ii=0; ii<pLandGate->GetToMapNum(); ++ii )
			{
				char szTempChar2[512];

				SNATIVEID sToMapID = pLandGate->GetToMapID(ii);
				DWORD dwToGateID = pLandGate->GetToGateID(ii);

				SMAPNODE *pMapNode = CEditorLevelView::GetView()->m_sMap.FindMapNode ( sToMapID );
				if ( pMapNode )
				{
					DWORD dwNameINDEX = pLandGate->GetNameINDEX(ii);
					std::string strNameKEY = pLandGate->GetNameKEY(ii);

					if ( dwNameINDEX != DxLandGate::GATE_OUT_INDEX_NULL && strNameKEY.size() )
						sprintf( szTempChar2, "[%d/%d]%s(%d) [%d]", sToMapID.wMainID, sToMapID.wSubID, strNameKEY.c_str(), dwNameINDEX, dwToGateID );
					else
						sprintf( szTempChar2, "[%d/%d]%s [%d]", sToMapID.wMainID, sToMapID.wSubID, pMapNode->strMapName.c_str(), dwToGateID );
				}

				strcat_s( szTempChar, szTempChar2 );
			}	

			m_List.SetItemText( i, 4, szTempChar );
		}
		else
		{
			std::string strMAPNAME = "Unknown Map";

			SNATIVEID sToMapID = pLandGate->GetToMapID( DxLandGate::DEFAULT_GATE_OUT_INDEX );
			DWORD dwToGateID = pLandGate->GetToGateID( DxLandGate::DEFAULT_GATE_OUT_INDEX );

			SMAPNODE* pMap = CEditorLevelView::GetView()->m_sMap.FindMapNode( sToMapID );
			if ( pMap )
				strMAPNAME = pMap->strMapName.c_str();

			sprintf( szTempChar, "[%d/%d]%s [%d]", sToMapID.wMainID, sToMapID.wSubID, strMAPNAME.c_str(), dwToGateID );

			m_List.SetItemText( i, 4, szTempChar );
		}
		

		m_List.SetItemData( i, (DWORD_PTR)pLandGate );
		i++;

		pLandGate = pLandGate->m_pNext;
	}

	m_List.SetRedraw( TRUE );

	if ( nSelect == -1 && m_List.GetItemCount() > 0 )
		nSelect = 0;

	if ( nSelect != -1 )
	{
		m_List.EnsureVisible( nSelect, TRUE );
		m_List.SetItemState( nSelect, LVIS_SELECTED, LVIS_SELECTED );
	}
}

void CPageGate::JumpTo( D3DXVECTOR3 vMax, D3DXVECTOR3 vMin )
{
	//newbie math
	D3DXVECTOR3	vec3Center;
	vec3Center.x=((vMin.x + vMax.x ) / 2);
	vec3Center.y=((vMin.y + vMax.y ) / 2);
	vec3Center.z=((vMin.z + vMax.z ) / 2);
	DxViewPort::GetInstance().CameraJump( vec3Center );
}

void CPageGate::SelectGate( std::string strName )
{
	if ( !m_pGLLand )	return;

	for( int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		m_List.SetItemState( nItem, ~LVIS_SELECTED, LVIS_SELECTED  ) ;	
		m_List.SetCheck( nItem, false );
	}

	for( int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		std::string strGATE = m_List.GetItemText( nItem, 3 );
		if ( !strcmp( strGATE.c_str(), strName.c_str() ) )
		{
			m_List.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED ) ;
			m_List.EnsureVisible( nItem, TRUE );
			m_List.SetCheck( nItem, true );
			break;
		}	
	}
}

void CPageGate::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( !m_pGLLand )	return;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	PDXLANDGATE pLandGate = (PDXLANDGATE)m_List.GetItemData( nSelect );
	if ( pLandGate )
	{
		JumpTo( pLandGate->GetMax(), pLandGate->GetMin() );
	}
}

void CPageGate::OnBnClickedButtonNew()
{
	if ( !m_pGLLand )	return;

	if ( m_pSheetTab )
		m_pSheetTab->PageEditGate( NULL );
}

void CPageGate::OnBnClickedButtonEdit()
{
	if ( !m_pGLLand )	return;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	PDXLANDGATE pLandGate = (PDXLANDGATE)m_List.GetItemData( nSelect );
	if ( pLandGate )
	{
		if ( m_pSheetTab )
			m_pSheetTab->PageEditGate( pLandGate );
	}
}

void CPageGate::OnBnClickedButtonDelete()
{
	if ( !m_pGLLand )	return;

	CEditorLevelView::GetView()->m_pSelectedGate = NULL;
	CEditorLevelView::GetView()->m_pSelectSched = NULL;

	int nCHECKED = 0;
	for( int nItem = 0; nItem < m_List.GetItemCount(); nItem++ )
	{
		BOOL bChecked = m_List.GetCheck( nItem );
		if( bChecked )
		{
			PDXLANDGATE pLandGate = (PDXLANDGATE)m_List.GetItemData( nItem );
			if ( pLandGate )
			{
				std::string strName = pLandGate->GetName();
				if ( strName.size() != 0 )
				{
					m_pGLLand->GetLandGateMan().DelLandGate( pLandGate->GetName() );
				}else{
					m_pGLLand->GetLandGateMan().DelLandGate( pLandGate->GetGateID() );
				}	
			}
			
			nCHECKED++;
		}	
	}

	m_pGLLand->GetLandGateMan().BuildTree();

	if ( nCHECKED == 0 )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Nothing selected Use Check boxes to select" );
	}

	DataShow();
}

void CPageGate::OnBnClickedButtonRebuildname()
{
	if ( !m_pGLLand )	return;

	int nINDEX = 0;
	PDXLANDGATE pLandGate = m_pGLLand->GetLandGateMan().GetListLandGate();
	while ( pLandGate )
	{
		char szTempChar[DxLandGate::MAX_SZNAME];
		sprintf( szTempChar, "GATE_%04d", nINDEX );
		pLandGate->SetName( szTempChar );
		nINDEX++;

		pLandGate = pLandGate->m_pNext;
	}

	DataShow();
}

void CPageGate::OnBnClickedButtonSave()
{
	if ( m_pSheetTab )
		m_pSheetTab->PageActive( LEVELEDIT_PAGE_MAIN );
}


void CPageGate::OnBnClickedButtonGateLoadCsv()
{
	if ( !m_pGLLand )	return;

	CEditorLevelView::GetView()->m_pSelectedGate = NULL;
	CEditorLevelView::GetView()->m_pSelectSched = NULL;

	CFileDialog dlg( TRUE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "csv file (*.csv)|*.csv|", AfxGetApp()->m_pMainWnd );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath();
	if ( dlg.DoModal() != IDOK )
		return;

	CStringFile StrFile( 10240 );
	if( !StrFile.Open ( dlg.GetPathName().GetString() ) )
		return;

	STRUTIL::ClearSeparator();
	STRUTIL::RegisterSeparator( "," );

	CString strLine;
	CStringArray StrArray;
	std::string strTemp;
	char szLandGateName[ DxLandGate::MAX_SZNAME ];

	// Skip Csv Head
	StrFile.GetNextLine( strLine );

	while ( StrFile.GetNextLine( strLine ) )
	{
		int iCsvCur = 0;
		STRUTIL::StringSeparate( strLine, StrArray );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTemp );
		StringCchCopy(szLandGateName, DxLandGate::MAX_SZNAME, strTemp.c_str() );

		PDXLANDGATE pLandGate = m_pGLLand->GetLandGateMan().FindLandGate( szLandGateName );
		if ( !pLandGate ) continue;

		pLandGate->LoadCsv( StrArray );
		pLandGate->MakeStartPos( m_pGLLand->GetNavi() );
	}

	m_pGLLand->GetLandGateMan().BuildTree();

	DataShow();
}


void CPageGate::OnBnClickedButtonGateSaveCsv()
{
	if ( !m_pGLLand )	return;

	CEditorLevelView::GetView()->m_pSelectedGate = NULL;
	CEditorLevelView::GetView()->m_pSelectSched = NULL;

	CFileDialog dlg( FALSE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "csv file (*.csv)|*.csv|", (CWnd*)this );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() != IDOK ) return;

	std::fstream streamFILE;
	streamFILE.open ( dlg.GetPathName().GetString(), std::ios_base::out );

	DxLandGate::SaveCsvHead( streamFILE );

	PDXLANDGATE pLandGate = m_pGLLand->GetLandGateMan().GetListLandGate();
	while ( pLandGate )
	{
		pLandGate->SaveCsv( streamFILE );

		pLandGate = pLandGate->m_pNext;
	}

	streamFILE.close();

	DataShow();
}
