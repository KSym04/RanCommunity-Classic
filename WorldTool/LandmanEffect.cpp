// LandmanEffect.cpp : implementation file
//

#include "stdafx.h"
#include "WorldTool.h"
#include "LandmanEffect.h"
#include "afxdialogex.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "ToolPages.h"

#include "../Lib_Engine/DxOctree/DXLANDEFF.h"
#include "../Lib_Engine/DxCommon/DxViewPort.h"

#include "../Lib_Helper/EtcFunction.h"

// CLandmanEffect dialog

IMPLEMENT_DYNAMIC(CLandmanEffect, CPropertyPage)

CLandmanEffect::CLandmanEffect(LOGFONT logfont)
	: CPropertyPage(CLandmanEffect::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanEffect::~CLandmanEffect()
{
	SAFE_DELETE( m_pFont );
}

void CLandmanEffect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EFFECT, m_List );
}


BEGIN_MESSAGE_MAP(CLandmanEffect, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_CLOSE, &CLandmanEffect::OnBnClickedBtnWldClose)
	ON_BN_CLICKED(IDC_BTN_EFFECT_DEL, &CLandmanEffect::OnBnClickedBtnEffectDel)
	ON_BN_CLICKED(IDC_BTN_EFFECT_ADD, &CLandmanEffect::OnBnClickedBtnEffectAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EFFECT, &CLandmanEffect::OnNMDblclkListEffect)
	
	ON_BN_CLICKED(IDC_BTN_EFFECT_EDIT_MATRIX, &CLandmanEffect::OnBnClickedBtnEffectEditMatrix)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_FILE, &CLandmanEffect::OnBnClickedButtonEffectFile)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_SAVE, &CLandmanEffect::OnBnClickedButtonEffectSave)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_CANCEL, &CLandmanEffect::OnBnClickedButtonEffectCancel)
	ON_BN_CLICKED(IDC_BTN_EFFECT_EDIT_MATRIX_CANCEL, &CLandmanEffect::OnBnClickedBtnEffectEditMatrixCancel)
END_MESSAGE_MAP()


// CLandmanEffect message handlers
BOOL CLandmanEffect::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CLandmanTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	RECT rectCtrl;
	m_List.SetExtendedStyle ( m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List.GetClientRect ( &rectCtrl );
	m_List.InsertColumn( 0, "Name", LVCFMT_LEFT, ( 50* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 1, "File", LVCFMT_LEFT, ( 50* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	ShowData();

	return TRUE;  
}

void CLandmanEffect::SetData( DxLandMan* pData )
{
	m_pData = pData;

	ShowData();
}

void CLandmanEffect::ResetData()
{
	m_pData = NULL;
}

void CLandmanEffect::OnBnClickedBtnWldClose()
{
	if ( m_pTab )
		m_pTab->PageActive( EMWLD_PAGE_MAIN );
}

BOOL CLandmanEffect::ShowData()
{
	m_List.DeleteAllItems();

	if ( !m_pData )		return FALSE;

	m_List.SetRedraw( FALSE );

	int i=0;
	PLANDEFF pLandEff = m_pData->GetLandEffList();
	while ( pLandEff )
	{
		m_List.InsertItem( i, pLandEff->m_szName );
		m_List.SetItemText( i, 1, pLandEff->m_szFileName );

		m_List.SetItemData( i, (DWORD_PTR)pLandEff );

		pLandEff = pLandEff->m_pNext;

		i++;
	}


	m_List.SetRedraw( TRUE );

	ShowEdit( FALSE );

	return TRUE;
}

BOOL CLandmanEffect::SaveData()
{
	if ( !m_pData )	return FALSE;

	return TRUE;
}

void CLandmanEffect::JumpTo( const D3DXMATRIX& sMat )
{
	D3DXVECTOR3 vPos = D3DXVECTOR3( sMat._41, sMat._42, sMat._43 );
	vPos.y += 10.0f;
	DxViewPort::GetInstance().CameraJump( vPos );
}

void CLandmanEffect::ShowEdit( BOOL bShow )
{
	SetWin_ShowWindow( this, IDC_STATIC_EFFECT_NAME, bShow );
	SetWin_ShowWindow( this, IDC_EDIT_EFFECT_NAME, bShow );

	SetWin_ShowWindow( this, IDC_BUTTON_EFFECT_FILE, bShow );
	SetWin_ShowWindow( this, IDC_EDIT_EFFECT_FILE, bShow );

	SetWin_ShowWindow( this, IDC_BUTTON_EFFECT_CANCEL, bShow );
	SetWin_ShowWindow( this, IDC_BUTTON_EFFECT_SAVE, bShow );
}

void CLandmanEffect::OnBnClickedBtnEffectAdd()
{
	ShowEdit( TRUE );
}

void CLandmanEffect::OnBnClickedBtnEffectDel()
{
	if ( !m_pData )		return;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	PLANDEFF pLandEff = (PLANDEFF)m_List.GetItemData( nSelect );
	if ( pLandEff )
	{
		m_pData->DelLandEff(pLandEff);
		m_pData->BuildSingleEffTree();
		ShowData();
	}
}

void CLandmanEffect::OnNMDblclkListEffect(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( !m_pData )		return;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	PLANDEFF pLandEff = (PLANDEFF)m_List.GetItemData( nSelect );
	if ( pLandEff )
	{
		JumpTo(pLandEff->m_matWorld);
	}
}

void CLandmanEffect::OnBnClickedBtnEffectEditMatrix()
{
	if ( !m_pData )		return;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	PLANDEFF pLandEff = (PLANDEFF)m_List.GetItemData( nSelect );
	if ( pLandEff )
	{
		JumpTo(pLandEff->m_matWorld);

		CWorldToolView::GetView()->EditMRSSet( TRUE );
		CWorldToolView::GetView()->m_DxEditMRS.SetMatrix( &pLandEff->m_matWorld );
	}
}

void CLandmanEffect::OnBnClickedButtonEffectFile()
{
	CString szFilter = "Effect File (*.egp)|*.egp|";
	CFileDialog dlg( TRUE, ".egp", DxEffSinglePropGMan::GetInstance().GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = DxEffSinglePropGMan::GetInstance().GetPath();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_EFFECT_FILE, dlg.GetFileName().GetString() );
}

void CLandmanEffect::OnBnClickedButtonEffectSave()
{
	if ( !m_pData )		return;

	std::string strName = GetWin_Text( this, IDC_EDIT_EFFECT_NAME );
	if ( strName.empty() )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Effect Name Emmpty!" );
		return;
	}


	std::string strFile = GetWin_Text( this, IDC_EDIT_EFFECT_FILE );
	if ( strFile.empty() )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Effect File Empty!" );
		return;
	}

	PLANDEFF pLandEffNew = new DXLANDEFF;
	StringCchCopy( pLandEffNew->m_szName, DXLANDEFF::LANDEFF_MAXSZ, strName.c_str() );
	StringCchCopy( pLandEffNew->m_szFileName, MAX_PATH, strFile.c_str() );
	
	D3DXVECTOR3 pos = CWorldToolView::GetView()->m_vPos;
	D3DXMatrixIdentity( &pLandEffNew->m_matWorld );
	pLandEffNew->m_matWorld._41 = pos.x;
	pLandEffNew->m_matWorld._42 = pos.y;
	pLandEffNew->m_matWorld._43 = pos.z;

	pLandEffNew->m_pPropGroup = DxEffSinglePropGMan::GetInstance().LoadEffectGProp ( pLandEffNew->m_szFileName );
	if ( pLandEffNew->m_pPropGroup )
	{
		pLandEffNew->m_vMax = pLandEffNew->m_pPropGroup->m_vMax;
		pLandEffNew->m_vMin = pLandEffNew->m_pPropGroup->m_vMin;
		COLLISION::TransformAABB ( pLandEffNew->m_vMax, pLandEffNew->m_vMin, pLandEffNew->m_matWorld );
	}
	else
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "Effect File Load Failed!" );
		SAFE_DELETE( pLandEffNew );
		return;
	}


	m_pData->AddLandEff(pLandEffNew);
	m_pData->BuildSingleEffTree();

	ShowData();

	for( int nItem=0; nItem<m_List.GetItemCount(); nItem++)
	{
		PLANDEFF pLandEff = (PLANDEFF)m_List.GetItemData( nItem );
		if ( pLandEff )
		{
			if ( !strcmp( pLandEff->m_szName, pLandEffNew->m_szName ) &&
				!strcmp( pLandEff->m_szFileName, pLandEffNew->m_szFileName ) &&
				pLandEff->m_matWorld == pLandEffNew->m_matWorld )
			{
				m_List.EnsureVisible( nItem, TRUE );
				m_List.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
				GotoDlgCtrl(GetDlgItem(m_List.GetDlgCtrlID()));
			}
		}
	}
}

void CLandmanEffect::OnBnClickedButtonEffectCancel()
{
	ShowData();
}

void CLandmanEffect::OnBnClickedBtnEffectEditMatrixCancel()
{
	CWorldToolView::GetView()->EditMRSSet( FALSE );
}
