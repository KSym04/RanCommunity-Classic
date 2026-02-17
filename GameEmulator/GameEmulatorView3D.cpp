
// GameEmulatorView.cpp : implementation of the CGameEmulatorView class
//

#include "stdafx.h"
#include "./GameEmulator.h"

#include "./GameEmulatorDoc.h"
#include "./GameEmulatorView.h"

#include "./DlgCharset.h"

#include "../Lib_Client/DxServerInstance.h"
#include "../Lib_Client/DxGlobalStage.h"

#include "../Lib_Engine/DxResponseMan.h"
#include "../Lib_Engine/G-Logic/GLOGIC.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "../Lib_Client/dxparamset.h"
#include "../Lib_Engine/Common/CommonWeb.h"
#include "../Lib_Engine/GUInterface/Cursor.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/DxCommon/D3DFont.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"

#include "../Lib_Engine/DxEffect/DxEffectMan.h"
#include "../Lib_Engine/DxCommon/DxGlowMan.h"
#include "../Lib_Engine/DxCommon/DxPostProcess.h"
#include "../Lib_Engine/DxCommon/DxLightMan.h"
#include "../Lib_Engine/DxCommon/DxInputDevice.h"
#include "../Lib_Client/dxincommand.h"

#include "../Lib_Engine/Core/NSRPath.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"
#include "../Lib_Engine/Core/NSRGameGlobal.h"

#include "../Lib_Engine/TextTexture/TextUtil.h"

#include "../Lib_Client/G-Logic/UserTypeDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class DxDirectMsgServer : public DxMsgServer
{
public:
	virtual int SendClient ( DWORD dwClient, LPVOID pBuffer )
	{
		if ( GETMYCLIENTID()==dwClient )
			DxGlobalStage::GetInstance().MsgProcess ( reinterpret_cast<NET_MSG_GENERIC*> ( pBuffer ) );

		return 0;
	}

	virtual int SendField ( DWORD dwClient, LPVOID pBuffer )
	{
		GASSERT ( 0 && "SendField()" );
		return 0;
	}

	virtual int SendFieldSvr ( int nSvrNum, LPVOID pBuffer, int nServerChannel=0 )
	{
		GASSERT ( 0 && "SendFieldSvr()" );
		return 0;
	}

	virtual int SendAgent ( DWORD dwClient, LPVOID pBuffer )
	{
		NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC *)pBuffer;

		switch ( nmg->nType )
		{
		default:
			if ( GETMYCLIENTID()==dwClient )
			{
				DxGlobalStage::GetInstance().MsgProcess ( reinterpret_cast<NET_MSG_GENERIC*> ( pBuffer ) );
			}
			break;
		};

		return 0;
	}

	virtual int SendAgent ( LPVOID pBuffer )
	{
		NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC *)pBuffer;

		switch ( nmg->nType )
		{
		case NET_MSG_GCTRL_REQ_GATEOUT:
			GLGaeaServer::GetInstance().MsgProcess ( nmg, 0, GAEAID_NULL );
			break;

		default:
			break;
		};

		return 0;
	}

	virtual void SendAllField ( LPVOID pBuffer )
	{
		GASSERT(0&&"SendAllField()");
	}

	virtual void SendAllField	( LPVOID pBuffer, int nServerChannel=0 )
	{
		GASSERT(0&&"SendAllField()");
	}

	virtual void SendChannel    ( LPVOID pBuffer, int nServerChannel=0 )
	{
		GASSERT(0&&"SendAllField()");
	}

	virtual void SendAllChannel ( LPVOID pBuffer )
	{
		GASSERT(0&&"SendAllField()");
	}

public:
	static DxDirectMsgServer& GetInstance();
};

DxDirectMsgServer& DxDirectMsgServer::GetInstance()
{
	static DxDirectMsgServer Instance;
	return Instance;
}

class DxSetConsoleMsg : public DxConsoleMsg
{
public:
	virtual void Write ( int nType, const char* szFormat, ... )
	{
		va_list argList;
		char szBuffer[1024];

		va_start(argList, szFormat);
		StringCbVPrintf(szBuffer, 1024, szFormat, argList);
		va_end(argList);

		CDebugSet::ToListView ( szBuffer );
	}

	virtual void Write( const TCHAR* msg, ... )
	{
		if (msg == NULL) return;

		TCHAR szBuf[C_BUFFER_SIZE+1] = {0};	

		va_list ap;
		va_start(ap, msg);
		StringCchVPrintf(szBuf, C_BUFFER_SIZE+1, msg, ap);
		va_end(ap);	

		CDebugSet::ToListView ( szBuf );
	}

	static DxSetConsoleMsg& GetInstance();
};

DxSetConsoleMsg& DxSetConsoleMsg::GetInstance()
{
	static DxSetConsoleMsg Instance;
	return Instance;
}

void CGameEmulatorView::OnInitialUpdate()
{
	__super::OnInitialUpdate();

}

HRESULT CGameEmulatorView::FrameMove3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::FrameMove3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CGameEmulatorView::Render3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::Render3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CGameEmulatorView::ConfirmDevice ( D3DCAPSQ* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
	if ( dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		return E_FAIL;
	return S_OK;
}

HRESULT CGameEmulatorView::OneTimeSceneInit()
{
	HRESULT hr;
	DxResponseMan::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), this, ROPTION::strFontType, RPARAM::emLangSet, RPARAM::strGDIFont );
	RGAME_GLOBAL::SETUSERID ( "userid" );

	hr = DxFieldInstance::Create ( RPATH::getAppPath(), &DxDirectMsgServer::GetInstance(), &DxSetConsoleMsg::GetInstance(), NULL, SP_MINCOMS, "mapslist.mst", FIELDSERVER_MAX , 1000, true, 0, FALSE, 3);	
	if ( FAILED(hr) )	return E_FAIL;

	ROPTION::bScrWindowed = TRUE;
	DxGlobalStage::GetInstance().SetD3DApp(this);
	DxGlobalStage::GetInstance().OneTimeSceneInit ( RPATH::getAppPath(), m_hWndApp, TRUE, static_cast<WORD>(ROPTION::dwScrWidth), static_cast<WORD>(ROPTION::dwScrHeight), "mapslist.mst" );

	//if( m_bEnableGUI )	CCommonWeb::Get()->Create( (CWnd*)this, &m_bVisibleGUI, &m_rtBoundGUI );

	return S_OK;
}

HRESULT CGameEmulatorView::CreateObjects()
{
	{ 
		DxFontMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
		CD3DFontPar* pD3dFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
		CD3DFontPar* pD3dFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW|D3DFONT_ASCII );
		CDebugSet::InitDeviceObjects( pD3dFont9 );
		TCHAR szFullPath[MAX_PATH] = {0};
		StringCchCopy( szFullPath, MAX_PATH, RPATH::getAppPath() );
		StringCchCat( szFullPath, MAX_PATH, SUBPATH::GUI_FILE_ROOT);
		CGameTextMan::GetInstance().SetPath(szFullPath);
		CGameTextMan::GetInstance().LoadText(RPARAM::strGameWord.GetString(), CGameTextMan::EM_GAME_WORD, RPARAM::bXML_USE );
		CGameTextMan::GetInstance().LoadText(RPARAM::strGameInText.GetString(), CGameTextMan::EM_GAME_IN_TEXT, RPARAM::bXML_USE );
		CGameTextMan::GetInstance().LoadText(RPARAM::strGameExText.GetString(), CGameTextMan::EM_GAME_EX_TEXT, RPARAM::bXML_USE );
	}

	HRESULT hr = S_OK;

	hr = InitDeviceObjects();
	if( FAILED(hr) )	return hr;

	hr = RestoreDeviceObjects();	
	if( FAILED(hr) )	return hr;

	return hr;
}

HRESULT CGameEmulatorView::InitDeviceObjects()
{
	SCHARDATA2 sCharData2;
	BOOL bLOAD = sCharData2.LOADFILE( m_sCharacterSetting.strCharSet.c_str() );
	if ( !bLOAD )
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "CGameEmulatorView::InitDeviceObjects sCharData2.LOADFILE failed :%s", m_sCharacterSetting.strCharSet.c_str() );
	}

	if ( m_sCharacterSetting.strCharName.size() )
		StringCchCopy( sCharData2.m_szName, CHAR_SZNAME, m_sCharacterSetting.strCharName.c_str() );

	if ( m_sCharacterSetting.wLevel != 0 )
		sCharData2.m_wLevel = m_sCharacterSetting.wLevel;

	if ( m_sCharacterSetting.wStats != 0 )
	{
		sCharData2.m_sStats.wPow = m_sCharacterSetting.wStats;
		sCharData2.m_sStats.wStr = m_sCharacterSetting.wStats;
		sCharData2.m_sStats.wSpi = m_sCharacterSetting.wStats;
		sCharData2.m_sStats.wDex = m_sCharacterSetting.wStats;
		sCharData2.m_sStats.wInt = m_sCharacterSetting.wStats;
		sCharData2.m_sStats.wSta = m_sCharacterSetting.wStats;
	}

	if ( m_sCharacterSetting.llMoney != 0 )
		sCharData2.m_lnMoney = m_sCharacterSetting.llMoney;

	if ( m_sCharacterSetting.bOpenSkills )
	{
		for( int MID = 0; MID < EMSKILLCLASS_NSIZE; ++ MID )
		{
			for( int SID = 0; SID < GLSkillMan::MAX_CLASSSKILL; ++ SID )
			{
				PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( MID, SID );
				if ( pSKILL )
				{
					if ( (pSKILL->m_sLEARN.dwCLASS&sCharData2.m_emClass) )
					{
						WORD wSkillLevel = SKILL::MAX_SKILL_LEVEL-1;

						if ( wSkillLevel >= pSKILL->m_sBASIC.dwMAXLEVEL ){
							wSkillLevel = (WORD)pSKILL->m_sBASIC.dwMAXLEVEL-1;
						}

						sCharData2.m_ExpSkills.insert(  std::make_pair( pSKILL->m_sBASIC.sNATIVEID.dwID, SCHARSKILL( pSKILL->m_sBASIC.sNATIVEID, wSkillLevel,NATIVEID_NULL(),0)) );
					}	
				}
			}
		}
	}

	sCharData2.m_cInventory.SetItemGenTime();
	sCharData2.m_dwUserLvl = (DWORD)NSUSER_TYPE::UserTypeIndexToUserType( (NSUSER_TYPE::EMUSER_TYPE_INDEX) m_sCharacterSetting.dwUserLevelIndex );
	sCharData2.m_wHairColor = HAIRCOLOR::GetHairColor( CharClassToIndex(sCharData2.m_emClass), sCharData2.m_wHair );

	if ( m_sCharacterSetting.bOpenInventory )
	{
		sCharData2.m_wINVENLINE = (EM_INVENSIZE_Y - EM_INVEN_DEF_SIZE_Y - EM_INVEN_PREMIUM_SIZE);
		sCharData2.m_cInventory.SetAddLine ( sCharData2.GetOnINVENLINE(), true );
	}

	sCharData2.m_wSex = CharClassToGender( sCharData2.m_emClass );

	if ( m_sCharacterSetting.dwActivityPoints != 0 )
		sCharData2.m_dwActivityPoint = m_sCharacterSetting.dwActivityPoints;

	if ( m_sCharacterSetting.dwContributionPoints != 0 )
		sCharData2.m_dwContributionPoint = m_sCharacterSetting.dwContributionPoints;

	if ( m_sCharacterSetting.dwWarChips != 0 )
		sCharData2.m_dwWarChips = m_sCharacterSetting.dwWarChips;

	if ( m_sCharacterSetting.dwGamePoints != 0 )
		sCharData2.m_dwGamePoints = m_sCharacterSetting.dwGamePoints;

	/* user flag verified, Juver, 2020/02/25 */
	sCharData2.m_bUserFlagVerified = m_sCharacterSetting.bUserVerified;

	/* user flag restricted, Juver, 2020/04/20 */
	sCharData2.m_bUserFlagRestricted = m_sCharacterSetting.bUserRestricted;


	/* play time system, Juver, 2021/01/26 */
	if ( m_sCharacterSetting.llChaPlayTime != 0 )
		sCharData2.m_llPlayTime = m_sCharacterSetting.llChaPlayTime;

	/* play time system, Juver, 2021/01/26 */
	if ( m_sCharacterSetting.llChaPlayPoint != 0 )
		sCharData2.m_llPlayPoint = m_sCharacterSetting.llChaPlayPoint;

	/* reborn system, Juver, 2021/09/17 */
	if ( m_sCharacterSetting.dwReborn != 0 )
		sCharData2.m_dwReborn = m_sCharacterSetting.dwReborn;

	if ( m_sCharacterSetting.wSkillPoints != 0 )
		sCharData2.m_dwSkillPoint = m_sCharacterSetting.wSkillPoints;

	if ( m_sCharacterSetting.wStatPoints != 0 )
		sCharData2.m_wStatsPoint = m_sCharacterSetting.wStatPoints;

	if ( m_sCharacterSetting.emVIP != EMVIP_LEVEL_NONE )
		sCharData2.m_emVIPLevel = m_sCharacterSetting.emVIP;

	if ( m_sCharacterSetting.wExchangeItemPoints )
		sCharData2.m_wExchangeItemPoints = m_sCharacterSetting.wExchangeItemPoints;

	PGLCHAR pGLChar = GLGaeaServer::GetInstance().CreatePC ( &sCharData2, 0, 0, FALSE );
	if ( pGLChar )
	{
		pGLChar->MsgGameJoin ();
		DxGlobalStage::GetInstance().MsgProcessFrame ();
	}
	else
	{
		CDebugSet::MsgBox( GetSafeHwnd(), "GLGaeaServer::GetInstance().CreatePC Failed." );
	}

	DxViewPort::GetInstance().InitDeviceObjects ( m_pd3dDevice, m_hWnd );
	DxResponseMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	DXLIGHT sDirectional;
	sDirectional.SetDefault();
	sDirectional.m_Light.Diffuse = D3DXCOLOR(0,0,0,0);
	sDirectional.m_Light.Ambient = D3DXCOLOR(0,0,0,0);
	DxLightMan::SetDefDirect ( sDirectional );
	
	DxGlobalStage::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	CCursor::GetInstance().InitDeviceObjects ();

	CString strTitle;
	strTitle.Format( "%s(%d) %s", m_sCharacterSetting.strCharName.c_str(), sCharData2.m_dwUserLvl, m_sCharacterSetting.strCharSet.c_str() );
	AfxGetMainWnd()->SetWindowText( strTitle.GetString() );

	return S_OK;
}

HRESULT CGameEmulatorView::RestoreDeviceObjects()
{
	HRESULT hr=S_OK;
	DxResponseMan::GetInstance().RestoreDeviceObjects ();
	DxGlobalStage::GetInstance().RestoreDeviceObjects ();
	DXPARAMSET::INIT ();
	DxGlowMan::GetInstance().SetProjectActiveON();
	DxPostProcess::GetInstance().SetProjectActiveON();
	PROFILE_INIT();

	return S_OK;
}

HRESULT CGameEmulatorView::FrameMove()
{
	PROFILE_BEGIN("FrameMove");

	CheckControlfp();
	GLPeriod::GetInstance().UpdateWeather ( m_fElapsedTime );
	GLGaeaServer::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );
	DxResponseMan::GetInstance().FrameMove( m_fTime, m_fElapsedTime, m_bDefWin );
	DxGlobalStage::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );
	DxViewPort::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );

	PROFILE_END("FrameMove");

	return S_OK;
}

HRESULT CGameEmulatorView::Render()
{
	PROFILE_BEGIN("Render");

	D3DCOLOR colorClear = D3DCOLOR_XRGB(89,135,179);
	DxFogMan::GetInstance().RenderFogSB ( m_pd3dDevice );
	colorClear = DxFogMan::GetInstance().GetFogColor();

	HRESULT hr = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,colorClear, 1.0f, 0L );

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		DxResponseMan::GetInstance().Render( m_pd3dDevice );
		DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );
		DxLightMan::GetInstance()->Render ( m_pd3dDevice );
		DXLIGHT &Light = *DxLightMan::GetInstance()->GetDirectLight ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT, (float*)&Light.m_Light.Direction, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIFFUSE, (float*)&Light.m_Light.Diffuse, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTAMBIENT, (float*)&Light.m_Light.Ambient, 1 );
		D3DXVECTOR3 &vFromPt = DxViewPort::GetInstance().GetFromPt ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_CAMERAPOSITION, (float*)&vFromPt, 1 );

		D3DLIGHTQ	pLight;
		D3DXVECTOR4	vPointPos;
		D3DXVECTOR3	vPointDiff;
		for ( int i=0; i<7; i++ )
		{
			if ( DxLightMan::GetInstance()->GetClosedLight(i+1) )
			{
				pLight = DxLightMan::GetInstance()->GetClosedLight(i+1)->m_Light;
				vPointDiff = D3DXVECTOR3 ( pLight.Diffuse.r, pLight.Diffuse.g, pLight.Diffuse.b );
				vPointPos.x = pLight.Position.x;
				vPointPos.y = pLight.Position.y;
				vPointPos.z = pLight.Position.z;
				vPointPos.w = pLight.Range;
			}else{
				vPointPos = D3DXVECTOR4 ( 0.f, 0.f, 0.f, 0.1f );
				vPointDiff = D3DXVECTOR3 ( 0.f, 0.f, 0.f );
			}
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTPOS01, (float*)&vPointPos, 1);
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTDIFF01, (float*)&vPointDiff, 1);
		}

		D3DXMATRIX matView = DxViewPort::GetInstance().GetMatView();
		D3DXMATRIX matProj = DxViewPort::GetInstance().GetMatProj();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_SKIN_DEFAULT, (float*)&D3DXVECTOR4 (1.f, 0.5f, 0.f, 765.01f), 1 );
		D3DXVECTOR3	vLightVector = DxLightMan::GetInstance()->GetDirectLight()->m_Light.Direction;
		D3DXVec3TransformNormal ( &vLightVector, &vLightVector, &matView );
		D3DXVec3Normalize ( &vLightVector, &vLightVector);
		vLightVector = -vLightVector;
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT_VIEW, (float*)&vLightVector, 1 );
		D3DXMatrixTranspose( &matView, &matView );
		D3DXMatrixTranspose( &matProj, &matProj );
		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity( &matIdentity );
		m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );	
		DxGlobalStage::GetInstance().Render ();
		RenderText();
		CCursor::GetInstance().Render ( m_pd3dDevice, DxInputDevice::GetInstance().GetMouseLocateX(), DxInputDevice::GetInstance().GetMouseLocateY() );
		m_pd3dDevice->EndScene();
	}

	PROFILE_END("Render");
	return S_OK;
}


HRESULT CGameEmulatorView::RenderText()
{
	if ( !dxincommand::bDISP_CONSOLE && !dxincommand::bDISP_FPS )	return S_OK;

	D3DCOLOR fontColor  = D3DCOLOR_ARGB(255,255,255,255);

	TCHAR szMsg[MAX_PATH] = TEXT("");

	CD3DFontPar* pD3dFont = DxFontMan::GetInstance().FindFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	if ( !pD3dFont )	return S_OK;

	CTextUtil::Get()->Render( TRUE );

	if ( dxincommand::bDISP_FPS )
	{
		FLOAT fNextLine = 40.0f; 

		lstrcpy( szMsg, m_strDeviceStats );
		fNextLine -= 20.0f;
		pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );

		lstrcpy( szMsg, m_strFrameStats );
		fNextLine -= 20.0f;
		pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );
	}

	if ( dxincommand::bDISP_CONSOLE )
	{
		CDebugSet::Render ();
	}

	CTextUtil::Get()->Render( FALSE );

	return S_OK;
}

HRESULT CGameEmulatorView::InvalidateDeviceObjects()
{
	DxResponseMan::GetInstance().InvalidateDeviceObjects ();
	DxGlobalStage::GetInstance().InvalidateDeviceObjects ();
	return S_OK;
}

HRESULT CGameEmulatorView::DeleteDeviceObjects()
{
#ifdef _DEBUG
	_CrtCheckMemory();
#endif //_DEBUG

	DxResponseMan::GetInstance().DeleteDeviceObjects ();
	DxGlobalStage::GetInstance().DeleteDeviceObjects ();
	CCursor::GetInstance().DeleteDeviceObjects ();

	return S_OK;
}

HRESULT CGameEmulatorView::FinalCleanup()
{
	DxFieldInstance::CleanUp ();
	DxGlobalStage::GetInstance().FinalCleanup ();
	DxResponseMan::GetInstance().FinalCleanup ();

	return S_OK;
}

void CGameEmulatorView::SetActive ( BOOL bActive )
{
	DxInputDevice::GetInstance().OnActivate ( bActive );
}

void CGameEmulatorView::PostNcDestroy()
{
	CD3DApplication::Cleanup3DEnvironment();
	::SetCursor ( NULL );
	::DestroyCursor ( m_hCursorDefault );
	__super::PostNcDestroy();
}

void CGameEmulatorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGameEmulatorApp *pApp = (CGameEmulatorApp*) AfxGetApp();
	pApp->SetActive ( bActivate );
	CGameEmulatorView::SetActive ( bActivate );

	__super::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CGameEmulatorView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_bCreated )
	{
		ReSizeWindow(cx,cy);
		if ( m_pd3dDevice )
		{
			DxGlobalStage::GetInstance().ReSizeWindow ( cx, cy );
		}
	}
}

BOOL CGameEmulatorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR hCursl = CCursor::GetInstance().GetCurCursor();
	if ( !hCursl )
	{
		if ( !m_hCursorDefault )		m_hCursorDefault = LoadCursor ( NULL, IDC_ARROW );
		hCursl = m_hCursorDefault;
	}

	if ( hCursl )	::SetCursor ( hCursl );
	return TRUE;
}