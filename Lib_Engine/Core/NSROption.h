#ifndef NSROPTION_H__INCLUDED
#define NSROPTION_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Lib_Engine/dxframe/d3dapp.h"

#include "./NSROptionDef.h"

namespace ROPTION
{
	BOOL Load( bool bServer = false );	
	BOOL Save();	

	extern UINT				nChinaRegion;
	
	extern CString			strFontType;
	extern BOOL				bD3DXShadowFont;

	extern BOOL				bCHECKWHQL;
	extern DWORD			dwScrWidth;
	extern DWORD			dwScrHeight;
	extern EMSCREEN_FORMAT	emScrFormat;
	extern UINT				uScrRefreshHz;
	extern BOOL				bScrAllowToggle;
	extern BOOL				bScrWindowed;
	extern BOOL				bScrWndHalfSize;
	extern BOOL				bScrWndFullSize;
	extern BOOL				bGameCursor;

	extern BOOL				b3D_SOUND;
	extern LONG				nSndSfx;
	extern LONG				nSndMap;
	extern LONG				nSndMusic;
	extern DWORD			dw3DAlgorithm;
	extern DWORD			dwSndBufferSize;
	extern BOOL				bSndMuteSfx;
	extern BOOL				bSndMuteMap;
	extern BOOL				bSndMuteMusic;	


	extern DWORD			dwVideoLevel;
	extern DWORD			dwShadowChar;	//	0~4
	extern BOOL				bBuff;
	extern BOOL				bShadowLand;
	extern BOOL				bRealReflect;
	extern BOOL				bRefract;
	extern BOOL				bGlow;
	extern BOOL				bPost;
	extern BOOL				bFrameLimit;
	extern DWORD			dwSkinDetail;	// 0~2
	extern DWORD			dwTextureFilter;
	extern DWORD			dwFogRange;
	extern BOOL				bBUFF_SFX;
	extern BOOL				bEMOTICON;
	extern BOOL				bClickEffect;
	extern BOOL				bTargetEffect;
	extern BOOL				bMineEffect;

	extern BOOL				bHideSkillEffect;
	extern BOOL				bHideAnnouncements;
	extern BOOL				bShowRankBadge;
	extern EMHIDE_DEAD		emHideDead;

	extern BOOL				bShowNameBackground;
	extern BOOL				bShowPKDisplay;
	extern BOOL				bShowRankMark;
	extern BOOL				bShowRankName;
};

#endif // NSROPTION_H__INCLUDED
