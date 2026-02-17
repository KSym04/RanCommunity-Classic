
// EditorMaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./EditorMaterial.h"
#include "./EditorMaterialDlg.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRLog.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"

#include "../Lib_Helper/EtcFunction.h"

#include "../Lib_Engine/DxResponseMan.h"
#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Helper/HLibDataConvert.h"
#include "../Lib_Engine/DxCommon/DxMaterialManager.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorMaterialDlg dialog


CEditorMaterialDlg::CEditorMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorMaterialDlg::IDD, pParent)
	, m_bAdd(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CEditorMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST, m_List );
	
}

BEGIN_MESSAGE_MAP(CEditorMaterialDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CEditorMaterialDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEditorMaterialDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_LIST, &CEditorMaterialDlg::OnBnClickedButtonImportList)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CEditorMaterialDlg::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEditorMaterialDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CEditorMaterialDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEditorMaterialDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CEditorMaterialDlg::OnBnClickedButtonClear)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CEditorMaterialDlg::OnNMDblclkList)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CEditorMaterialDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_DATA_SAVE, &CEditorMaterialDlg::OnBnClickedButtonDataSave)
END_MESSAGE_MAP()


// CEditorMaterialDlg message handlers

BOOL CEditorMaterialDlg::OnInitDialog()
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

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);

	RPATH::SetPath( szPath );
	RLOG::Init();

	RPARAM::Init();
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	GMTOOL::Create ( RPATH::getAppPath() );

	DxMaterialManager::GetInstance().LoadFile( NULL, "GameMaterial.mmsf" );

	ShowData();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditorMaterialDlg::OnPaint()
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
HCURSOR CEditorMaterialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditorMaterialDlg::PostNcDestroy()
{
	DxMaterialManager::GetInstance().m_mapMaterial.clear();
	DxResponseMan::GetInstance().FinalCleanup();
	CDialog::PostNcDestroy();
}

void CEditorMaterialDlg::OnBnClickedButtonLoad()
{
	CString szFilter = "Material Setting File (*.mmsf)|*.mmsf|";
	CFileDialog dlg( TRUE, ".mmsf", DxMaterialManager::GetInstance().GetPath().c_str(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxMaterialManager::GetInstance().GetPath().c_str();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = DxMaterialManager::GetInstance().LoadFile ( NULL, dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to load File." );
		}

		ShowData();
	}
}

void CEditorMaterialDlg::OnBnClickedButtonSave()
{
	CString szFilter = "Material Setting File (*.mmsf)|*.mmsf|";
	CFileDialog dlg( FALSE, ".mmsf", DxMaterialManager::GetInstance().GetPath().c_str(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxMaterialManager::GetInstance().GetPath().c_str();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = DxMaterialManager::GetInstance().SaveFile ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to Save File" );
			return;
		}
	}
}

void CEditorMaterialDlg::OnBnClickedButtonImportList()
{
	CString szFilter = "Material list (*.ini)|*.ini|";
	CFileDialog dlg( TRUE, ".ini", DxMaterialManager::GetInstance().GetPath().c_str(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxMaterialManager::GetInstance().GetPath().c_str();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = DxMaterialManager::GetInstance().ImportMaterialList ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to import File." );
		}

		ShowData();
	}
}

void CEditorMaterialDlg::OnBnClickedButtonImport()
{
	CString szFilter = "Material (*.ini)|*.ini|";
	CFileDialog dlg( TRUE, ".ini", DxMaterialManager::GetInstance().GetPath().c_str(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = DxMaterialManager::GetInstance().GetPath().c_str();
	if ( dlg.DoModal() == IDOK )
	{
		HRESULT hr = DxMaterialManager::GetInstance().ImportMaterial ( dlg.GetFileName().GetString() );
		if ( FAILED ( hr ) )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to import File." );
		}

		ShowData();
	}
}

void CEditorMaterialDlg::OnBnClickedButtonAdd()
{
	SMATERIAL_STATIC material;
	MaterialShow( material, TRUE, TRUE );
}

void CEditorMaterialDlg::OnBnClickedButtonEdit()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string strFileName = m_List.GetItemText( nSelect, 1 );

	DxMaterialManager::SMATERIAL_STATIC_MAP& mapdata = DxMaterialManager::GetInstance().m_mapMaterial;
	DxMaterialManager::SMATERIAL_STATIC_MAP_ITER it = mapdata.find(strFileName);
	if ( it != mapdata.end() )
	{
		MaterialShow( (*it).second, TRUE, FALSE );
	}
}

void CEditorMaterialDlg::OnBnClickedButtonDelete()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string strFileName = m_List.GetItemText( nSelect, 1 );

	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Delete Material? %s", strFileName.c_str() ) !=  IDYES )
		return;

	DxMaterialManager::SMATERIAL_STATIC_MAP& mapdata = DxMaterialManager::GetInstance().m_mapMaterial;
	DxMaterialManager::SMATERIAL_STATIC_MAP_ITER it = mapdata.find(strFileName);
	if ( it != mapdata.end() )
	{
		mapdata.erase( it );
		ShowData();
	}

	int nNewSelect = nSelect > 1? nSelect-1:nSelect;
	m_List.EnsureVisible( nNewSelect, TRUE );
	m_List.SetItemState( nNewSelect, LVIS_SELECTED, LVIS_SELECTED );
}

void CEditorMaterialDlg::OnBnClickedButtonClear()
{
	if ( CDebugSet::MsgBoxYesNo( GetSafeHwnd(), "Clear All Materials?" ) !=  IDYES )
		return;

	DxMaterialManager::SMATERIAL_STATIC_MAP& mapdata = DxMaterialManager::GetInstance().m_mapMaterial;
	mapdata.clear();

	ShowData();
}

void CEditorMaterialDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string strFileName = m_List.GetItemText( nSelect, 1 );

	DxMaterialManager::SMATERIAL_STATIC_MAP& mapdata = DxMaterialManager::GetInstance().m_mapMaterial;
	DxMaterialManager::SMATERIAL_STATIC_MAP_ITER it = mapdata.find(strFileName);
	if ( it != mapdata.end() )
	{
		MaterialShow( (*it).second, TRUE, FALSE );
	}
}

void CEditorMaterialDlg::OnBnClickedButtonCopy()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;
	std::string strFileName = m_List.GetItemText( nSelect, 1 );

	DxMaterialManager::SMATERIAL_STATIC_MAP& mapdata = DxMaterialManager::GetInstance().m_mapMaterial;
	DxMaterialManager::SMATERIAL_STATIC_MAP_ITER it = mapdata.find(strFileName);
	if ( it != mapdata.end() )
	{
		SMATERIAL_STATIC material = (*it).second;
		std::string strnewname = "copy_";
		strnewname += material.strMaterialName;
		material.strMaterialName = strnewname;
		MaterialShow( material, TRUE, TRUE );
	}
}

void CEditorMaterialDlg::ShowData()
{
	m_List.DeleteAllItems();
	m_List.SetRedraw( FALSE );

	DxMaterialManager::SMATERIAL_STATIC_MAP& mapdata = DxMaterialManager::GetInstance().m_mapMaterial;
	DxMaterialManager::SMATERIAL_STATIC_MAP_ITER it = mapdata.begin();
	int idx = 0;
	for ( ; it != mapdata.end(); ++it, ++idx )
	{
		m_List.InsertItem( idx, _HLIB::cstring_dword( idx ).GetString() );

		const SMATERIAL_STATIC& material = (*it).second;
		m_List.SetItemText( idx, 1, material.strMaterialName.c_str() );
	}

	m_List.SetRedraw( TRUE );

	SMATERIAL_STATIC material;
	MaterialShow( material, FALSE, FALSE );
}

void CEditorMaterialDlg::MaterialShow( const SMATERIAL_STATIC& material, BOOL bENABLE, BOOL bADD )
{
	m_bAdd = bADD;

	SetWin_Text( this, IDC_EDIT_MATERIAL_NAME, material.strMaterialName.c_str() );
	SetWin_Text( this, IDC_EDIT_SHADER_FILE, material.strShaderFile.c_str() );
	SetWin_Text( this, IDC_EDIT_TECHNIQUE_LOW, material.strTechniqueLow.c_str() );
	SetWin_Text( this, IDC_EDIT_TECHNIQUE_HIGH, material.strTechniqueHigh.c_str() );

	SetWin_Text( this, IDC_EDIT_BASE_TEXTURE, material.g_BaseTexture.c_str() );
	SetWin_Text( this, IDC_EDIT_BASE_TEXTURE2, material.g_BaseTexture2.c_str() );
	SetWin_Text( this, IDC_EDIT_NORMAL_TEXTURE, material.g_NormalTexture.c_str() );
	SetWin_Text( this, IDC_EDIT_CUBE_TEXTURE, material.g_CubeTexture.c_str() );
	SetWin_Text( this, IDC_EDIT_REFLECT_TEXTURE, material.g_ReflectTexRT.c_str() );

	SetWin_Num_float( this, IDC_EDIT_ROTATE_UV, material.g_fRotate_UV );
	SetWin_Num_float( this, IDC_EDIT_SCALE_FACTOR, material.g_fScaleFactor );
	SetWin_Num_float( this, IDC_EDIT_SPECULAR_POWER_1, material.g_fSpecularPower1 );
	SetWin_Num_float( this, IDC_EDIT_SPECULAR_INTENSITY_1, material.g_fSpecularIntensity1 );
	SetWin_Num_float( this, IDC_EDIT_SPECULAR_POWER_2, material.g_fSpecularPower2 );
	SetWin_Num_float( this, IDC_EDIT_SPECULAR_INTENSITY_2, material.g_fSpecularIntensity2 );

	SetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_X, material.g_vReflectPower.x );
	SetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_Y, material.g_vReflectPower.y );
	SetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_Z, material.g_vReflectPower.z );
	SetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_W, material.g_vReflectPower.w );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_X, material.g_vMoveSpeed.x );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_Y, material.g_vMoveSpeed.y );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_Z, material.g_vMoveSpeed.z );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_W, material.g_vMoveSpeed.w );
	SetWin_Num_float( this, IDC_EDIT_WAVE_DENSITY, material.g_fWaveDensity );
	SetWin_Num_float( this, IDC_EDIT_WAVE_SCALE, material.g_fWaveScale );
	SetWin_Num_float( this, IDC_EDIT_WAVE_SPEED, material.g_fWaveSpeed );

	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_X, material.g_vMoveSpeed0.x );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_Y, material.g_vMoveSpeed0.y );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_Z, material.g_vMoveSpeed0.z );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_W, material.g_vMoveSpeed0.w );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_X, material.g_vMoveSpeed1.x );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_Y, material.g_vMoveSpeed1.y );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_Z, material.g_vMoveSpeed1.z );
	SetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_W, material.g_vMoveSpeed1.w );
	SetWin_Num_float( this, IDC_EDIT_IMAGE_SCALE_0, material.g_fImageScale0 );
	SetWin_Num_float( this, IDC_EDIT_IMAGE_SCALE_1, material.g_fImageScale1 );
	SetWin_Num_float( this, IDC_EDIT_COLOR_POWER_0, material.g_fColorPower0 );
	SetWin_Num_float( this, IDC_EDIT_COLOR_POWER_1, material.g_fColorPower1 );

	SetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN, material.g_fTexColorUpDown );
	SetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN_MIN, material.g_fTexColorUpDownMin );
	SetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN_ADD, material.g_fTexColorUpDownAdd );
	SetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN_SPEED, material.g_fTexColorUpDownSpeed );



	SetWin_Enable( this, IDC_EDIT_MATERIAL_NAME, bENABLE && bADD );
	SetWin_Enable( this, IDC_EDIT_SHADER_FILE, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TECHNIQUE_LOW, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TECHNIQUE_HIGH, bENABLE );

	SetWin_Enable( this, IDC_EDIT_BASE_TEXTURE, bENABLE );
	SetWin_Enable( this, IDC_EDIT_BASE_TEXTURE2, bENABLE );
	SetWin_Enable( this, IDC_EDIT_NORMAL_TEXTURE, bENABLE );
	SetWin_Enable( this, IDC_EDIT_CUBE_TEXTURE, bENABLE );
	SetWin_Enable( this, IDC_EDIT_REFLECT_TEXTURE, bENABLE );

	SetWin_Enable( this, IDC_EDIT_ROTATE_UV, bENABLE );
	SetWin_Enable( this, IDC_EDIT_SCALE_FACTOR, bENABLE );
	SetWin_Enable( this, IDC_EDIT_SPECULAR_POWER_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_SPECULAR_INTENSITY_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_SPECULAR_POWER_2, bENABLE );
	SetWin_Enable( this, IDC_EDIT_SPECULAR_INTENSITY_2, bENABLE );

	SetWin_Enable( this, IDC_EDIT_REFLECT_POWER_X, bENABLE );
	SetWin_Enable( this, IDC_EDIT_REFLECT_POWER_Y, bENABLE );
	SetWin_Enable( this, IDC_EDIT_REFLECT_POWER_Z, bENABLE );
	SetWin_Enable( this, IDC_EDIT_REFLECT_POWER_W, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_X, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_Y, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_Z, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_W, bENABLE );
	SetWin_Enable( this, IDC_EDIT_WAVE_DENSITY, bENABLE );
	SetWin_Enable( this, IDC_EDIT_WAVE_SCALE, bENABLE );
	SetWin_Enable( this, IDC_EDIT_WAVE_SPEED, bENABLE );

	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_0_X, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_0_Y, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_0_Z, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_0_W, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_1_X, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_1_Y, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_1_Z, bENABLE );
	SetWin_Enable( this, IDC_EDIT_MOVE_SPEED_1_W, bENABLE );
	SetWin_Enable( this, IDC_EDIT_IMAGE_SCALE_0, bENABLE );
	SetWin_Enable( this, IDC_EDIT_IMAGE_SCALE_1, bENABLE );
	SetWin_Enable( this, IDC_EDIT_COLOR_POWER_0, bENABLE );
	SetWin_Enable( this, IDC_EDIT_COLOR_POWER_1,bENABLE );

	SetWin_Enable( this, IDC_EDIT_TEXCOLOR_UPDOWN, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TEXCOLOR_UPDOWN_MIN, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TEXCOLOR_UPDOWN_ADD, bENABLE );
	SetWin_Enable( this, IDC_EDIT_TEXCOLOR_UPDOWN_SPEED, bENABLE );
}

void CEditorMaterialDlg::OnBnClickedButtonDataSave()
{
	SMATERIAL_STATIC material;
	material.strMaterialName = GetWin_Text( this, IDC_EDIT_MATERIAL_NAME );
	material.strShaderFile = GetWin_Text( this, IDC_EDIT_SHADER_FILE );
	material.strTechniqueLow = GetWin_Text( this, IDC_EDIT_TECHNIQUE_LOW );
	material.strTechniqueHigh = GetWin_Text( this, IDC_EDIT_TECHNIQUE_HIGH );

	material.g_BaseTexture = GetWin_Text( this, IDC_EDIT_BASE_TEXTURE );
	material.g_BaseTexture2 = GetWin_Text( this, IDC_EDIT_BASE_TEXTURE2 );
	material.g_NormalTexture = GetWin_Text( this, IDC_EDIT_NORMAL_TEXTURE );
	material.g_CubeTexture = GetWin_Text( this, IDC_EDIT_CUBE_TEXTURE );
	material.g_ReflectTexRT = GetWin_Text( this, IDC_EDIT_REFLECT_TEXTURE );

	material.g_fRotate_UV = GetWin_Num_float( this, IDC_EDIT_ROTATE_UV );
	material.g_fScaleFactor = GetWin_Num_float( this, IDC_EDIT_SCALE_FACTOR );
	material.g_fSpecularPower1 = GetWin_Num_float( this, IDC_EDIT_SPECULAR_POWER_1 );
	material.g_fSpecularIntensity1 = GetWin_Num_float( this, IDC_EDIT_SPECULAR_INTENSITY_1 );
	material.g_fSpecularPower2 = GetWin_Num_float( this, IDC_EDIT_SPECULAR_POWER_2 );
	material.g_fSpecularIntensity2 = GetWin_Num_float( this, IDC_EDIT_SPECULAR_INTENSITY_2 );

	material.g_vReflectPower.x = GetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_X );
	material.g_vReflectPower.y = GetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_Y );
	material.g_vReflectPower.z = GetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_Z );
	material.g_vReflectPower.w = GetWin_Num_float( this, IDC_EDIT_REFLECT_POWER_W );
	material.g_vMoveSpeed.x = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_X );
	material.g_vMoveSpeed.y = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_Y );
	material.g_vMoveSpeed.z = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_Z );
	material.g_vMoveSpeed.w = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_W );
	material.g_fWaveDensity = GetWin_Num_float( this, IDC_EDIT_WAVE_DENSITY );
	material.g_fWaveScale = GetWin_Num_float( this, IDC_EDIT_WAVE_SCALE );
	material.g_fWaveSpeed = GetWin_Num_float( this, IDC_EDIT_WAVE_SPEED );

	material.g_vMoveSpeed0.x = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_X );
	material.g_vMoveSpeed0.y = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_Y );
	material.g_vMoveSpeed0.z = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_Z );
	material.g_vMoveSpeed0.w = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_0_W );
	material.g_vMoveSpeed1.x = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_X );
	material.g_vMoveSpeed1.y = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_Y );
	material.g_vMoveSpeed1.z = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_Z );
	material.g_vMoveSpeed1.w = GetWin_Num_float( this, IDC_EDIT_MOVE_SPEED_1_W );
	material.g_fImageScale0 = GetWin_Num_float( this, IDC_EDIT_IMAGE_SCALE_0 );
	material.g_fImageScale1 = GetWin_Num_float( this, IDC_EDIT_IMAGE_SCALE_1 );
	material.g_fColorPower0 = GetWin_Num_float( this, IDC_EDIT_COLOR_POWER_0 );
	material.g_fColorPower1 = GetWin_Num_float( this, IDC_EDIT_COLOR_POWER_1 );

	material.g_fTexColorUpDown = GetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN );
	material.g_fTexColorUpDownMin = GetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN_MIN );
	material.g_fTexColorUpDownAdd = GetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN_ADD );
	material.g_fTexColorUpDownSpeed = GetWin_Num_float( this, IDC_EDIT_TEXCOLOR_UPDOWN_SPEED );

	if ( material.strMaterialName.empty() )
	{
		CDebugSet::MsgBox( "Material empty!" );
		return;
	}

	if ( material.strShaderFile.empty() )
	{
		CDebugSet::MsgBox( "Shader empty!" );
		return;
	}

	if ( material.strTechniqueLow.empty() )
	{
		CDebugSet::MsgBox( "TechniqueLow empty!" );
		return;
	}

	if ( material.strTechniqueHigh.empty() )
	{
		CDebugSet::MsgBox( "TechniqueHigh empty!" );
		return;
	}

	DxMaterialManager::SMATERIAL_STATIC_MAP& mapdata = DxMaterialManager::GetInstance().m_mapMaterial;
	if ( m_bAdd )
	{
		DxMaterialManager::SMATERIAL_STATIC_MAP_ITER it = mapdata.find( material.strMaterialName );
		if ( it != mapdata.end() )
		{
			CDebugSet::MsgBox( GetSafeHwnd(), "Material Exist %s", material.strMaterialName.c_str() );
			return;
		}

		mapdata.insert( std::make_pair( material.strMaterialName, material ) );

		ShowData();
		MaterialShow( material, TRUE, FALSE );
	}
	else
	{
		DxMaterialManager::SMATERIAL_STATIC_MAP_ITER it = mapdata.find( material.strMaterialName );
		if ( it != mapdata.end() )
		{
			SMATERIAL_STATIC* pData = &(*it).second;
			*pData = material;

			ShowData();
			MaterialShow( *pData, TRUE, FALSE );
		}else{
			CDebugSet::MsgBox( GetSafeHwnd(), "Failure to save %s", material.strMaterialName.c_str() );

		}
	}

	for( int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		std::string strtext = m_List.GetItemText( nItem, 1 );
		if( strcmp( strtext.c_str(), material.strMaterialName.c_str() ) == 0  )
		{
			m_List.EnsureVisible( nItem, TRUE );
			m_List.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		}
	}
}
