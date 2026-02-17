#include "stdafx.h"
#include "./NSROption.h"
#include "./NSRParamDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ROPTION
{
	UINT				nChinaRegion		= RPARAM::MAX_CHINA_REGION;
	
	CString				strFontType			= "D3DXFONT";
	BOOL				bD3DXShadowFont		= TRUE;

	BOOL				bCHECKWHQL			= TRUE;
	DWORD				dwScrWidth			= 1024;
	DWORD				dwScrHeight			= 768;
	EMSCREEN_FORMAT		emScrFormat			= EMSCREEN_F16;
	UINT				uScrRefreshHz		= 60;
	BOOL				bScrAllowToggle		= FALSE;
	BOOL				bScrWindowed		= TRUE;
	BOOL				bScrWndHalfSize		= TRUE;
	BOOL				bScrWndFullSize		= FALSE;
	BOOL				bGameCursor			= TRUE;

	BOOL				b3D_SOUND			= FALSE;
	LONG				nSndSfx				= 0;
	LONG				nSndMap				= 0;
	LONG				nSndMusic			= 0;
	DWORD				dw3DAlgorithm		= 0;
	DWORD				dwSndBufferSize		= 10;
	BOOL				bSndMuteSfx			= FALSE;
	BOOL				bSndMuteMap			= FALSE;
	BOOL				bSndMuteMusic		= FALSE;

	DWORD				dwVideoLevel		= 0;
	DWORD				dwShadowChar		= 4;
	BOOL				bBuff				= TRUE;
	BOOL				bShadowLand			= TRUE;
	BOOL				bRealReflect		= TRUE;
	BOOL				bRefract			= TRUE;
	BOOL				bGlow				= TRUE;
	BOOL				bPost				= TRUE;
	BOOL				bFrameLimit			= TRUE;
	DWORD				dwSkinDetail		= 2;
	DWORD				dwTextureFilter		= 0;
	DWORD				dwFogRange			= 0;

	BOOL				bBUFF_SFX			= TRUE;
	BOOL				bEMOTICON			= TRUE;
	BOOL				bClickEffect		= TRUE;
	BOOL				bTargetEffect		= TRUE;
	BOOL				bMineEffect			= TRUE;

	BOOL				bHideSkillEffect	= FALSE;
	BOOL				bHideAnnouncements	= FALSE;
	BOOL				bShowRankBadge		= FALSE;
	EMHIDE_DEAD			emHideDead			= EMHIDE_DEAD_NONE;

	BOOL				bShowNameBackground = TRUE;
	BOOL				bShowPKDisplay	= TRUE;
	BOOL				bShowRankMark = TRUE;
	BOOL				bShowRankName = FALSE;
};