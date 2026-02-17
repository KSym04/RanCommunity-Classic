
// EditorSkinPieceView.cpp : implementation of the CEditorSkinPieceView class
//

#include "stdafx.h"
#include "./WorldTool.h"

#include "./WorldToolDoc.h"
#include "./WorldToolView.h"
#include "./mainfrm.h"
#include "./MainTab.h"

#include "../Lib_Engine/DxCommon/DxShadowMap.h"
#include "../Lib_Engine/DxCommon/DxEnvironment.h"
#include "../Lib_Engine/DxResponseMan.h"

#include "../Lib_Engine/Meshs/DxSkinCharData.h"
#include "../Lib_Engine/Meshs/DxAttBoneData.h"
#include "../Lib_Engine/Meshs/DxAttBoneLink.h"
#include "../Lib_Engine/Meshs/DxVehicleData.h"
#include "../Lib_Engine/Meshs/DxReplaceContainer.h"
#include "../Lib_Engine/Meshs/DxSkinPieceContainer.h"
#include "../Lib_Engine/Meshs/DxSkinAniMan.h"
#include "../Lib_Engine/Meshs/DxBoneCollector.h"
#include "../Lib_Engine/Meshs/DxSkinMeshMan.h"
#include "../Lib_Engine/DxEffect/Single/DxEffSinglePropGMan.h"
#include "../Lib_Engine/DxEffect/Single/DxEffGroupPlayer.h"
#include "../Lib_Engine/DxEffect/DxPieceContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CWorldToolView::DataCleanUp()
{
	DxResponseMan::GetInstance().DoInterimClean ( m_pd3dDevice );

	FOG_PROPERTY prop;
	prop.m_bNewRange = TRUE;
	prop.m_bSkyFogEnable = TRUE;
	prop.m_bFogStaticRange = FALSE;
	prop.m_cFogColor = D3DCOLOR_XRGB(100,100,100);
	prop.m_cFogColor_Day = D3DCOLOR_XRGB(100,100,100);
	prop.m_cFogColor_Night = D3DCOLOR_XRGB(100,100,100);
	DxFogMan::GetInstance().SetProperty( prop );
}

void CWorldToolView::ResetEditor()
{
	DxWeatherMan::GetInstance()->SetInstance( NULL );

	if ( m_pLand )
	{
		m_pLand->InvalidateDeviceObjects ();
		m_pLand->DeleteDeviceObjects ();
		m_pLand->CleanUp();
		SAFE_DELETE( m_pLand );
	}

	if ( m_pPiece )
	{
		m_pPiece->CleanUp();
		SAFE_DELETE( m_pPiece );
	}


	CMainFrame	*pMainFrame = (CMainFrame*) AfxGetMainWnd();
	if ( pMainFrame )
	{
		CMainTab*  pSheetTab = pMainFrame->m_wndEditor.m_pTab;
		if ( pSheetTab )
			pSheetTab->ResetData();

		ResetMouseEdit();
	}

	m_strFileName = "";

	DataCleanUp();
}

BOOL CWorldToolView::WLDLoad( std::string strFile )
{
	if ( strFile.empty() )	return FALSE;

	ResetEditor();

	m_pLand = new DxLandMan;
	m_pLand->CleanUp();

	BOOL bOK = m_pLand->LoadFile( strFile.c_str(), m_pd3dDevice, FALSE );
	if ( !bOK )
	{
		SAFE_DELETE( m_pLand );
		return FALSE;
	}

	m_pLand->ActiveMap();
	m_pLand->LoadBasicPos ( m_pd3dDevice, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ));
	m_pLand->InitDeviceObjects ( m_pd3dDevice );
	m_pLand->RestoreDeviceObjects ( m_pd3dDevice );

	m_strFileName = strFile.c_str();

	return TRUE;
}

BOOL CWorldToolView::WLDSave( std::string strFile )
{
	if ( !m_pLand )		return FALSE;
	if ( strFile.empty() )	return FALSE;

	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxLandMan::GetPath() );
	StringCchCat( szPathName, MAX_PATH, strFile.c_str() );

	BOOL bOk = m_pLand->SaveFile( szPathName );

	m_strFileName = strFile.c_str();

	return bOk;
}

BOOL CWorldToolView::PIELoad( std::string strFile )
{
	if ( strFile.empty() )	return FALSE;

	ResetEditor();

	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxPieceContainer::GetInstance().GetPath() );
	StringCchCat( szPathName, MAX_PATH, strFile.c_str() );

	m_pPiece = new DxPieceEdit;
	HRESULT hr = m_pPiece->LoadPIE( m_pd3dDevice, szPathName );
	if ( FAILED(hr) )
	{
		SAFE_DELETE(m_pPiece);
		return FALSE;
	}

	m_strFileName = strFile.c_str();

	return TRUE;
}

BOOL CWorldToolView::PIESave( std::string strFile )
{
	if ( !m_pPiece )		return FALSE;
	if ( strFile.empty() )	return FALSE;

	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxPieceContainer::GetInstance().GetPath() );
	StringCchCat( szPathName, MAX_PATH, strFile.c_str() );

	m_pPiece->SavePIE( szPathName );

	m_strFileName = strFile.c_str();

	return TRUE;
}