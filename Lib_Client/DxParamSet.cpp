#include "StdAfx.h"

#include "../Lib_Engine/Core/NSREngineGlobal.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Engine/Core/NSROption.h"
#include "../Lib_Engine/Core/NSRProfile.h"

#include "../Lib_Engine/DxCommon/DxShadowMap.h"
#include "../Lib_Engine/DxCommon/DxSoftShadowMap.h"
#include "../Lib_Engine/DxEffect/DxEffectMan.h"
#include "../Lib_Engine/DxSound/DxSoundMan.h"
#include "../Lib_Engine/DxSound/BgmSound.h"
#include "../Lib_Engine/DxCommon/DxSurfaceTex.h"
#include "../Lib_Engine/DxCommon/DxGlowMan.h"
#include "../Lib_Engine/DxCommon/DxFogMan.h"
#include "../Lib_Engine/DxCommon/DxPostProcess.h"

#include "./DxParamSet.h"

#include "./G-Logic/GLogicData.h"
#include "../Lib_Engine/DxEffect/Single/DxEffExtra.h"
#include "../Lib_Engine/DxResponseMan.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL				g_bFRAME_LIMIT;

namespace DXPARAMSET
{
	STESTSET& GetInstance()
	{
		static STESTSET Instance;
		return Instance;
	}

	void INIT ()
	{
		//	그래픽 옵션 조정.
		//
		DxShadowMap::GetInstance().SetShadowDetail ( (EMSHADOWDETAIL) ROPTION::dwShadowChar );

		if ( DxShadowMap::GetInstance().GetShadowDetail() == SHADOW_DISABLE )
			DxSoftShadowMap::GetInstance().SetShadowDetail( SOFT_SHADOW_DISABLE );
		else
			DxSoftShadowMap::GetInstance().SetShadowDetail( SOFT_SHADOW_ENABLE );

		if (ROPTION::bShadowLand )	DxEffectMan::GetInstance().SetSSDetail ( SS_ONETEX );
		else							DxEffectMan::GetInstance().SetSSDetail ( SS_DISABLE );

		DxFogMan::GetInstance().SetFogRange ( (FOGRANGE)ROPTION::dwFogRange );

		DxEffectMan::GetInstance().SetRealReflect (ROPTION::bRealReflect );
		DxEffectMan::GetInstance().SetSkinDetail ( (EMSKINDETAIL)ROPTION::dwSkinDetail );
		DxSurfaceTex::GetInstance().SetOptionEnable (ROPTION::bRefract );
		DxGlowMan::GetInstance().SetOptionEnable (ROPTION::bGlow );
		DxPostProcess::GetInstance().SetOptionEnable(ROPTION::bPost );
		
		g_bFRAME_LIMIT = ROPTION::bFrameLimit;
		

		//	사운드 설정.
		//		
		DxSoundMan::GetInstance().SetStereo ( !ROPTION::b3D_SOUND );
		DxSoundMan::GetInstance().SetMapVolume (ROPTION::nSndMap );
		DxSoundMan::GetInstance().SetSfxVolume (ROPTION::nSndSfx );
		DxBgmSound::GetInstance().SetVolume (ROPTION::nSndMusic );
		DxSoundMan::GetInstance().Set3DAlgorithm (ROPTION::dw3DAlgorithm );
		DxSoundMan::GetInstance().SetBufferSize (ROPTION::dwSndBufferSize );

		DxSoundMan::GetInstance().SetMapMute (ROPTION::bSndMuteMap );
		DxSoundMan::GetInstance().SetSfxMute (ROPTION::bSndMuteSfx );
		DxBgmSound::GetInstance().SetMute (ROPTION::bSndMuteMusic );

		LOADDATA();
	}

	void LOADDATA ()
	{
		GetInstance().m_bDIS_CONFT = RPROFILE::bDIS_CONFT;
		GetInstance().m_bDIS_TRADE = RPROFILE::bDIS_TRADE;
		GetInstance().m_bDIS_PARTY = RPROFILE::bDIS_PARTY;
//		GetInstance().m_bDIS_FRIEND = RPROFILE::bDIS_FRIEND;		

		GetInstance().m_bSHOW_SIMPLEHP = RPROFILE::bSHOW_SIMPLEHP;

		GetInstance().m_dwScrWidth = ROPTION::dwScrWidth;
		GetInstance().m_dwScrHeight = ROPTION::dwScrHeight;
		GetInstance().m_emScrFormat = ROPTION::emScrFormat;

		GetInstance().m_uScrRefreshHz = ROPTION::uScrRefreshHz;
		GetInstance().m_bScrWindowed = ROPTION::bScrWindowed;

		GetInstance().m_dwVideoLevel = ROPTION::dwVideoLevel;
		GetInstance().m_dwShadowChar = ROPTION::dwShadowChar;
		GetInstance().m_bBuff = ROPTION::bBuff;
		GetInstance().m_bShadowLand = ROPTION::bShadowLand;
		GetInstance().m_bRealReflect = ROPTION::bRealReflect;
		GetInstance().m_dwSkinDetail = ROPTION::dwSkinDetail;
		GetInstance().m_bGlow = ROPTION::bGlow;
		GetInstance().m_bPost = ROPTION::bPost;
		GetInstance().m_bRefract = ROPTION::bRefract;
		GetInstance().m_dwFogRange = ROPTION::dwFogRange;
		GetInstance().m_bFrameLimit = ROPTION::bFrameLimit;

		GetInstance().m_bClickEffect = ROPTION::bClickEffect;
		GetInstance().m_bTargetEffect = ROPTION::bTargetEffect;
		GetInstance().m_bMineEffect = ROPTION::bMineEffect;
		GetInstance().m_dwTextureFilter = ROPTION::dwTextureFilter;
	}

	void STESTSET::APPLY ( CD3DApplication *pD3dApp )
	{
		//	스크린 설정 적용.		
		DWORD dwNewMode = pD3dApp->FindDeviceMode ( m_dwScrWidth, m_dwScrHeight, m_emScrFormat );
		DWORD dwOldMode = pD3dApp->GetCurrentMode();
		if ( dwNewMode != dwOldMode )
		{
			pD3dApp->ChangeDeviceMode ( dwNewMode, m_uScrRefreshHz, m_bScrWindowed );
		}

		//	그래픽 설정.
		//
		DxShadowMap::GetInstance().SetShadowDetail ( (EMSHADOWDETAIL) m_dwShadowChar );

		if ( DxShadowMap::GetInstance().GetShadowDetail() == SHADOW_DISABLE )
			DxSoftShadowMap::GetInstance().SetShadowDetail( SOFT_SHADOW_DISABLE );
		else
			DxSoftShadowMap::GetInstance().SetShadowDetail( SOFT_SHADOW_ENABLE );

		if ( m_bShadowLand )	DxEffectMan::GetInstance().SetSSDetail ( SS_ONETEX );
		else					DxEffectMan::GetInstance().SetSSDetail ( SS_DISABLE );

		DxEffectMan::GetInstance().SetRealReflect ( m_bRealReflect );
		DxSurfaceTex::GetInstance().SetOptionEnable ( m_bRefract );
		DxGlowMan::GetInstance().SetOptionEnable ( m_bGlow );
		DxPostProcess::GetInstance().SetOptionEnable( m_bPost );
		DxEffectMan::GetInstance().SetSkinDetail ( (EMSKINDETAIL) m_dwSkinDetail );

		DxFogMan::GetInstance().SetFogRange ( (FOGRANGE) m_dwFogRange );
		
		g_bFRAME_LIMIT = m_bFrameLimit;

		if (!m_bClickEffect)
			DxEffExtra::GetInstance().RemoveClickEffect( GLCONST_CHAR::strCLICK_EFFECT.c_str() );

		if (!m_bTargetEffect)
			DxEffExtra::GetInstance().RemoveTargetEffect( GLCONST_CHAR::strTARGET_EFFECT.c_str() );

		if (!m_bMineEffect)
			DxEffExtra::GetInstance().RemoveMineEffect( GLCONST_CHAR::strMINE_EFFECT.c_str() );

		RENGINE_GLOBAL::SetTextureFilters();
		DxResponseMan::GetInstance().SetTextureFilters();
		
	}

	void STESTSET::VideoOK( CD3DApplication *pD3dApp )
	{
		ROPTION::dwScrWidth = m_dwScrWidth;
		ROPTION::dwScrHeight = m_dwScrHeight;
		ROPTION::emScrFormat = m_emScrFormat;
		ROPTION::uScrRefreshHz = m_uScrRefreshHz;
		ROPTION::bScrWindowed = m_bScrWindowed;

		ROPTION::dwVideoLevel = m_dwVideoLevel;
		ROPTION::dwShadowChar = m_dwShadowChar;
		ROPTION::bBuff = m_bBuff;
		ROPTION::bShadowLand = m_bShadowLand;
		ROPTION::bRealReflect = m_bRealReflect;
		ROPTION::dwSkinDetail = m_dwSkinDetail;
		ROPTION::bGlow = m_bGlow;
		ROPTION::bPost = m_bPost;
		ROPTION::bRefract = m_bRefract;
		ROPTION::bFrameLimit = m_bFrameLimit;

		ROPTION::bClickEffect = m_bClickEffect;
		ROPTION::bTargetEffect = m_bTargetEffect;
		ROPTION::bMineEffect = m_bMineEffect;
		ROPTION::dwTextureFilter = m_dwTextureFilter;

		ROPTION::dwFogRange = m_dwFogRange;
		

		ROPTION::Save ();
		RPROFILE::Save ();

		APPLY ( pD3dApp );
	}

	void STESTSET::GameOK()
	{
		RPROFILE::bDIS_CONFT = m_bDIS_CONFT;
		RPROFILE::bDIS_TRADE = m_bDIS_TRADE;
		RPROFILE::bDIS_PARTY = m_bDIS_PARTY;
		//		RANPARAM::bDIS_FRIEND = m_bDIS_FRIEND;		

		RPROFILE::bSHOW_SIMPLEHP = m_bSHOW_SIMPLEHP;

		ROPTION::Save ();
		RPROFILE::Save ();
	}

	void STESTSET::AudioOK()
	{
		ROPTION::Save ();
		RPROFILE::Save ();
	}

	void STESTSET::CANCEL ( CD3DApplication *pD3dApp )
	{
		m_bDIS_CONFT = RPROFILE::bDIS_CONFT;
		m_bDIS_TRADE = RPROFILE::bDIS_TRADE;
		m_bDIS_PARTY = RPROFILE::bDIS_PARTY;
//		m_bDIS_FRIEND = RPROFILE::bDIS_FRIEND;


		m_bSHOW_SIMPLEHP = RPROFILE::bSHOW_SIMPLEHP;

		m_dwScrWidth = ROPTION::dwScrWidth;
		m_dwScrHeight = ROPTION::dwScrHeight;
		m_emScrFormat = ROPTION::emScrFormat;
		m_uScrRefreshHz = ROPTION::uScrRefreshHz;
		m_bScrWindowed = ROPTION::bScrWindowed;

		m_dwVideoLevel = ROPTION::dwVideoLevel;				
		m_dwShadowChar = ROPTION::dwShadowChar;
		m_bBuff = ROPTION::bBuff;
		m_bShadowLand = ROPTION::bShadowLand;
		m_bRealReflect = ROPTION::bRealReflect;
		m_dwSkinDetail = ROPTION::dwSkinDetail;
		m_bGlow = ROPTION::bGlow;
		m_bPost = ROPTION::bPost;
		m_bRefract = ROPTION::bRefract;
		m_bFrameLimit = ROPTION::bFrameLimit;

		m_bClickEffect = ROPTION::bClickEffect;
		m_bTargetEffect = ROPTION::bTargetEffect;
		m_bMineEffect = ROPTION::bMineEffect;
		m_dwTextureFilter = ROPTION::dwTextureFilter;

		m_dwFogRange = ROPTION::dwFogRange;

		APPLY ( pD3dApp );
	}
};

