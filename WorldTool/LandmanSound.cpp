// LandmanSound.cpp : implementation file
//

#include "stdafx.h"
#include "./WorldTool.h"
#include "./LandmanSound.h"
#include "./LandmanTab.h"
#include "./WorldToolView.h"
#include "../Lib_Helper/EtcFunction.h"
#include "./ToolPages.h"
#include "./SelColorDlg.h"
#include "../Lib_Engine/DxSound/StaticSoundMan.h"
#include "../Lib_Helper/HLibDataConvert.h"
#include "../Lib_Engine/DxCommon/DxViewPort.h"
#include "../Lib_Engine/DxSound/DxSoundMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLandmanSound dialog

IMPLEMENT_DYNAMIC(CLandmanSound, CPropertyPage)

CLandmanSound::CLandmanSound(LOGFONT logfont)
	: CPropertyPage(CLandmanSound::IDD)
	, m_pFont(NULL)
	, m_pData(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CLandmanSound::~CLandmanSound()
{
	SAFE_DELETE( m_pFont );
}

void CLandmanSound::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SOUND, m_List );
}


BEGIN_MESSAGE_MAP(CLandmanSound, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WLD_CLOSE, &CLandmanSound::OnBnClickedBtnWldClose)
	ON_BN_CLICKED(IDC_BTN_SOUND_ADD, &CLandmanSound::OnBnClickedBtnSoundAdd)
	ON_BN_CLICKED(IDC_BTN_SOUND_DEL, &CLandmanSound::OnBnClickedBtnSoundDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SOUND, &CLandmanSound::OnNMDblclkListSound)
	ON_BN_CLICKED(IDC_BTN_EDIT_SAVE, &CLandmanSound::OnBnClickedBtnEditSave)
	ON_BN_CLICKED(IDC_BTN_EDIT_CANCEL, &CLandmanSound::OnBnClickedBtnEditCancel)
	ON_BN_CLICKED(IDC_BTN_SOUND_FILE, &CLandmanSound::OnBnClickedBtnSoundFile)
END_MESSAGE_MAP()


// CLandmanSound message handlers
BOOL CLandmanSound::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CLandmanTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	RECT rectCtrl;
	m_List.SetExtendedStyle ( m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List.GetClientRect ( &rectCtrl );
	m_List.InsertColumn( 0, "Name", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 1, "File", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 2, "Range", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_List.InsertColumn( 3, "Loop", LVCFMT_LEFT, ( 25* ( rectCtrl.right - rectCtrl.left ) ) / 100 );


	ShowData();

	return TRUE;  
}

void CLandmanSound::SetData( DxLandMan* pData )
{
	m_pData = pData;

	ShowData();
}

void CLandmanSound::ResetData()
{
	m_pData = NULL;
}

void CLandmanSound::OnBnClickedBtnWldClose()
{
	if ( m_pTab )
		m_pTab->PageActive( EMWLD_PAGE_MAIN );
}

BOOL CLandmanSound::ShowData()
{
	m_List.DeleteAllItems();

	if ( !m_pData )		return FALSE;
	CStaticSoundMan* psoundman = m_pData->GetSoundMan();
	if ( !psoundman )	return FALSE;

	m_List.SetRedraw( FALSE );

	int i=0;
	SVecSound* pVecSound = psoundman->GetVecSoundHead();
	while ( pVecSound )
	{
		m_List.InsertItem( i, pVecSound->m_szSoundName );
		m_List.SetItemText( i, 1, pVecSound->m_strFileName.c_str() );
		m_List.SetItemText( i, 2, _HLIB::cstringformat( "%g~%g", pVecSound->m_MinRange, pVecSound->m_MaxRange ).GetString() );
		m_List.SetItemText( i, 3, _HLIB::cstringformat( "(%d)%g", pVecSound->m_bLoop, pVecSound->m_LoopTime ).GetString() );
		
		m_List.SetItemData( i, (DWORD_PTR)pVecSound );

		pVecSound = pVecSound->m_pNext;

		i++;
	}


	m_List.SetRedraw( TRUE );

	ShowEdit( FALSE );
	
	return TRUE;
}

void CLandmanSound::JumpTo( D3DXMATRIX sMat )
{
	D3DXVECTOR3 vPos = D3DXVECTOR3( sMat._41, sMat._42, sMat._43 );
	vPos.y += 10.0f;
	DxViewPort::GetInstance().CameraJump( vPos );
}

void CLandmanSound::OnBnClickedBtnSoundAdd()
{
	ShowEdit( TRUE );
}

void CLandmanSound::OnBnClickedBtnSoundDel()
{
	if ( !m_pData )		return;
	CStaticSoundMan* psoundman = m_pData->GetSoundMan();
	if ( !psoundman )	return;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	SVecSound* psound = (SVecSound*)m_List.GetItemData( nSelect );
	if ( psound )
	{
		BOOL deleted = psoundman->DelVecSound( psound->m_szSoundName );
		if ( deleted )
			ShowData();
	}
}

void CLandmanSound::OnNMDblclkListSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	SVecSound* psound = (SVecSound*)m_List.GetItemData( nSelect );
	if ( psound )
	{
		JumpTo(psound->m_matWorld);
	}
}

void CLandmanSound::ShowEdit( BOOL bShow )
{
	SetWin_Enable( this, IDC_EDT_EDIT_SOUND_NAME, bShow );
	SetWin_Enable( this, IDC_EDT_SOUND_FILE, bShow );
	SetWin_Enable( this, IDC_EDT_SOUND_RANGE_MIN, bShow );
	SetWin_Enable( this, IDC_EDT_SOUND_RANGE_MAX, bShow );
	SetWin_Enable( this, IDC_EDT_SOUND_LOOP_TIME, bShow );
	SetWin_Enable( this, IDC_EDIT_CHK_LOOP, bShow );

	SetWin_Enable( this, IDC_BTN_SOUND_FILE, bShow );
	SetWin_Enable( this, IDC_BTN_EDIT_CANCEL, bShow );
	SetWin_Enable( this, IDC_BTN_EDIT_SAVE, bShow );
}

void CLandmanSound::OnBnClickedBtnEditSave()
{
	if ( !m_pData )		return;
	CStaticSoundMan* psoundman = m_pData->GetSoundMan();
	if ( !psoundman )	return;

	std::string soundname = GetWin_Text( this, IDC_EDT_EDIT_SOUND_NAME );
	std::string soundfile = GetWin_Text( this, IDC_EDT_SOUND_FILE );

	if ( soundfile.empty() )
	{
		CDebugSet::MsgBox( "sound name empty!" );
		return;
	}

	if ( soundfile.empty() )
	{
		CDebugSet::MsgBox( "sound file empty!" );
		return;
	}

	SVecSound* soundfind = psoundman->GetVecSound( soundname.c_str() );
	if ( soundfind )
	{
		CDebugSet::MsgBox( "%s sound exist!", soundname.c_str() );
		return;
	}

	SVecSound* psound = new SVecSound;
	D3DXMatrixIdentity( &psound->m_matWorld );
	StringCchCopy( psound->m_szSoundName, 256, soundname.c_str() );
	psound->Load( soundfile.c_str(), GetWin_Check( this, IDC_EDIT_CHK_LOOP )? true:false );
	psound->m_MinRange = GetWin_Num_float( this, IDC_EDT_SOUND_RANGE_MIN );
	psound->m_MaxRange = GetWin_Num_float( this, IDC_EDT_SOUND_RANGE_MAX );
	psound->m_LoopTime = GetWin_Num_float( this, IDC_EDT_SOUND_LOOP_TIME );
	
	D3DXVECTOR3 pos = CWorldToolView::GetView()->m_vPos;
	psound->m_matWorld._41 = pos.x;
	psound->m_matWorld._42 = pos.y;
	psound->m_matWorld._43 = pos.z;

	psound->Play();
	psoundman->AddVecSound( psound );
	psoundman->EnableSound(TRUE);

	ShowData();
}

void CLandmanSound::OnBnClickedBtnEditCancel()
{
	ShowEdit( FALSE );
}

void CLandmanSound::OnBnClickedBtnSoundFile()
{
	CString szFilter = "Sound (*.wav)|*.WAV|";
	CFileDialog dlg(TRUE,".wav",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = DxSoundMan::GetInstance().GetPath();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDT_SOUND_FILE, dlg.GetFileName().GetString() );
}
