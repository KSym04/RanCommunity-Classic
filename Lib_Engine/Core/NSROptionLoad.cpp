#include "stdafx.h"
#include "./NSROption.h"

#include "./NSRPath.h"
#include "./NSRInfo.h"
#include "./NSRLog.h"
#include "./NSRGameGlobal.h"
#include "../Lib_Engine/Common/IniLoader.h"
#include "../Lib_Engine/Common/Rijndael.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ROPTION
{
	BOOL Load( bool bServer /*= false*/ )
	{
		std::string	strFile = RPATH::getAppPath();
		strFile += RINFO::getc( EMRINFO_FILE_NAME_DBSLASH );
		strFile += RINFO::getc( EMRINFO_FILE_NAME_ROPTION );

		CIniLoader cFILE;

		if( !cFILE.open( strFile, true) )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_ROPTION_LOAD ), strFile.c_str() );
			return FALSE;
		}

		cFILE.SetUseErrorMsgBox( FALSE );

#ifndef KR_PARAM
		{
			if( !bServer )
			{
				std::string strENC_USERID = "";
				cFILE.getflag( "GAME OPTION", "strENC_USERID", 0, 1, strENC_USERID );
				if ( RGAME_GLOBAL::GETUSERID_NULL() == strENC_USERID )	strENC_USERID = "";
				RGAME_GLOBAL::SETUSERID_ENC( strENC_USERID );
			}
		}
#endif

		cFILE.getflag( "GAME OPTION", "nChinaRegion", 0, 1, nChinaRegion );
		cFILE.getflag( "GAME OPTION", "strFontType", 0, 1, strFontType );
		cFILE.getflag ( "GRAPHIC OPTION", "bD3DXShadowFont", 0, 1, bD3DXShadowFont );

		cFILE.getflag ( "SCREEN OPTION", "bCHECKWHQL", 0, 1, bCHECKWHQL );
		cFILE.getflag ( "SCREEN OPTION", "dwScrWidth", 0, 1, dwScrWidth );
		cFILE.getflag ( "SCREEN OPTION", "dwScrHeight", 0, 1, dwScrHeight );

		int nTEMP(0);
		cFILE.getflag ( "SCREEN OPTION", "emScrFormat", 0, 1, nTEMP );
		emScrFormat = (EMSCREEN_FORMAT) nTEMP;
		cFILE.getflag ( "SCREEN OPTION", "uScrRefreshHz", 0, 1, uScrRefreshHz );

		cFILE.getflag ( "SCREEN OPTION", "bScrAllowToggle", 0, 1, bScrAllowToggle );
		cFILE.getflag ( "SCREEN OPTION", "bScrWindowed", 0, 1, bScrWindowed );
		cFILE.getflag ( "SCREEN OPTION", "bScrWndFullSize", 0, 1, bScrWndFullSize );
		cFILE.getflag ( "SCREEN OPTION", "bGameCursor", 0, 1, bGameCursor );



		cFILE.getflag ( "SOUND OPTION", "b3D_SOUND", 0, 1, b3D_SOUND );
		cFILE.getflag ( "SOUND OPTION", "nSndSfx", 0, 1, nSndSfx );
		cFILE.getflag ( "SOUND OPTION", "nSndMap", 0, 1, nSndMap );
		cFILE.getflag ( "SOUND OPTION", "nSndMusic", 0, 1, nSndMusic );
		cFILE.getflag ( "SOUND OPTION", "dw3DAlgorithm", 0, 1, dw3DAlgorithm );
		cFILE.getflag ( "SOUND OPTION", "dwSndBufferSize", 0, 1, dwSndBufferSize );
		cFILE.getflag ( "SOUND OPTION", "bSndMuteSfx", 0, 1, bSndMuteSfx );
		cFILE.getflag ( "SOUND OPTION", "bSndMuteMap", 0, 1, bSndMuteMap );
		cFILE.getflag ( "SOUND OPTION", "bSndMuteMusic", 0, 1, bSndMuteMusic );


		cFILE.getflag ( "GRAPHIC OPTION", "dwVideoLevel", 0, 1, dwVideoLevel );
		cFILE.getflag ( "GRAPHIC OPTION", "dwShadowChar", 0, 1, dwShadowChar );
		cFILE.getflag ( "GRAPHIC OPTION", "bBuff", 0, 1, bBuff );
		cFILE.getflag ( "GRAPHIC OPTION", "bShadowLand", 0, 1, bShadowLand );
		cFILE.getflag ( "GRAPHIC OPTION", "bRealReflect", 0, 1, bRealReflect );
		cFILE.getflag ( "GRAPHIC OPTION", "bRefract", 0, 1, bRefract );
		cFILE.getflag ( "GRAPHIC OPTION", "bGlow", 0, 1, bGlow );
		cFILE.getflag ( "GRAPHIC OPTION", "bPost", 0, 1, bPost );
		cFILE.getflag ( "GRAPHIC OPTION", "bFrameLimit", 0, 1, bFrameLimit );
		cFILE.getflag ( "GRAPHIC OPTION", "dwSkinDetail", 0, 1, dwSkinDetail );
		cFILE.getflag ( "GRAPHIC OPTION", "dwTextureFilter", 0, 1, dwTextureFilter );
		cFILE.getflag ( "GRAPHIC OPTION", "dwFogRange", 0, 1, dwFogRange );
		cFILE.getflag ( "GRAPHIC OPTION", "bBUFF_SFX", 0, 1, bBUFF_SFX );
		cFILE.getflag ( "GRAPHIC OPTION", "bEMOTICON", 0, 1, bEMOTICON );

		cFILE.getflag ( "GRAPHIC OPTION", "bClickEffect", 0, 1, bClickEffect );
		cFILE.getflag ( "GRAPHIC OPTION", "bTargetEffect", 0, 1, bTargetEffect );
		cFILE.getflag ( "GRAPHIC OPTION", "bMineEffect", 0, 1, bMineEffect );

		cFILE.getflag ( "GRAPHIC OPTION", "bHideSkillEffect", 0, 1, bHideSkillEffect );

		cFILE.getflag ( "GRAPHIC OPTION", "bHideAnnouncements", 0, 1, bHideAnnouncements );

		cFILE.getflag ( "GRAPHIC OPTION", "bShowRankBadge", 0, 1, bShowRankBadge ); // Hide Summons - JX	

		cFILE.getflag ( "GRAPHIC OPTION", "emHideDead", 0, 1, emHideDead );

		cFILE.getflag ( "GRAPHIC OPTION", "bShowNameBackground", 0, 1, bShowNameBackground );

		cFILE.getflag ( "GRAPHIC OPTION", "bShowPKDisplay", 0, 1, bShowPKDisplay );

		cFILE.getflag ( "GRAPHIC OPTION", "bShowRankMark", 0, 1, bShowRankMark );
		

		if ( dwScrWidth < 1024 )
			dwScrWidth = 1024;

		if ( dwScrHeight < 768 )
			dwScrHeight = 768;

		if ( !bScrWndHalfSize )		bScrWndFullSize = TRUE;

		if ( COMMENT::FONTSYS[0]!=strFontType.GetString() && COMMENT::FONTSYS[1]!=strFontType.GetString() )
		{
			strFontType = COMMENT::FONTSYS[0].c_str();
		}		

		return TRUE;

	};

	BOOL Save()
	{
		CString strBuffer;

		strBuffer += _T("[GAME OPTION]\n");

#ifndef KR_PARAM
		{
			std::string strENC_USERID = RGAME_GLOBAL::GETUSERID_ENC();
			std::string strIDNULL = RGAME_GLOBAL::GETUSERID_NULL();
			if ( strENC_USERID.empty() )
			{
				strBuffer.AppendFormat("bSAVE_USERID = %d\n", FALSE);
				strBuffer.AppendFormat( "strENC_USERID = %s\n", strIDNULL );
			}
			else
			{
				strBuffer.AppendFormat("bSAVE_USERID = %d\n", TRUE);
				strBuffer.AppendFormat( "strENC_USERID = %s\n", strENC_USERID );
			}
		}
#endif

		strBuffer.AppendFormat( "nChinaRegion = %d\n", nChinaRegion );
		strBuffer.AppendFormat( "strFontType = %s\n", strFontType );
		strBuffer.AppendFormat( "bD3DXShadowFont = %d\n", bD3DXShadowFont );

		strBuffer += _T(";\n");
		strBuffer += _T("[SCREEN OPTION]\n");
		strBuffer.AppendFormat( "bCHECKWHQL	= %d\n", bCHECKWHQL );
		strBuffer.AppendFormat( "dwScrWidth	= %d\n", dwScrWidth );
		strBuffer.AppendFormat( "dwScrHeight = %d\n", dwScrHeight );
		strBuffer.AppendFormat( "emScrFormat = %d\n", emScrFormat );
		strBuffer.AppendFormat( "uScrRefreshHz = %d\n", uScrRefreshHz );
		strBuffer.AppendFormat( "bScrAllowToggle = %d\n", bScrAllowToggle );
		strBuffer.AppendFormat( "bScrWindowed = %d\n", bScrWindowed );
		strBuffer.AppendFormat( "bScrWndFullSize = %d\n", bScrWndFullSize );
		strBuffer.AppendFormat( "bGameCursor = %d\n", bGameCursor );


		strBuffer += _T(";\n");
		strBuffer += _T("[SOUND OPTION]\n");
		strBuffer.AppendFormat( "b3D_SOUND = %d\n", b3D_SOUND );
		strBuffer.AppendFormat( "nSndSfx = %d\n", nSndSfx );
		strBuffer.AppendFormat( "nSndMap = %d\n", nSndMap );
		strBuffer.AppendFormat( "nSndMusic = %d\n", nSndMusic );
		strBuffer.AppendFormat( "dw3DAlgorithm = %d\n", dw3DAlgorithm );
		strBuffer.AppendFormat( "dwSndBufferSize = %d\n", dwSndBufferSize );
		strBuffer.AppendFormat( "bSndMuteSfx = %d\n", bSndMuteSfx );
		strBuffer.AppendFormat( "bSndMuteMap = %d\n", bSndMuteMap );
		strBuffer.AppendFormat( "bSndMuteMusic = %d\n", bSndMuteMusic );


		strBuffer += _T(";\n");
		strBuffer += _T("[GRAPHIC OPTION]\n");
		strBuffer.AppendFormat( "dwVideoLevel = %d\n", dwVideoLevel );
		strBuffer.AppendFormat( "dwShadowChar = %d\n", dwShadowChar );
		strBuffer.AppendFormat( "bBuff = %d\n", bBuff );
		strBuffer.AppendFormat( "bShadowLand = %d\n", bShadowLand );
		strBuffer.AppendFormat( "bRealReflect = %d\n", bRealReflect );
		strBuffer.AppendFormat( "bRefract = %d\n", bRefract );
		strBuffer.AppendFormat( "bGlow = %d\n", bGlow );
		strBuffer.AppendFormat( "bPost = %d\n", bPost );
		strBuffer.AppendFormat( "bFrameLimit = %d\n", bFrameLimit );
		strBuffer.AppendFormat( "dwSkinDetail = %d\n", dwSkinDetail );
		strBuffer.AppendFormat( "dwTextureFilter = %d\n", dwTextureFilter );
		strBuffer.AppendFormat( "dwFogRange = %d\n", dwFogRange );
		strBuffer.AppendFormat( "bBUFF_SFX = %d\n", bBUFF_SFX );
		strBuffer.AppendFormat( "bEMOTICON = %d\n", bEMOTICON );
		
		strBuffer.AppendFormat( "bClickEffect = %d\n", bClickEffect );
		strBuffer.AppendFormat( "bTargetEffect = %d\n", bTargetEffect );
		strBuffer.AppendFormat( "bMineEffect = %d\n", bMineEffect );

		strBuffer.AppendFormat( "bHideSkillEffect = %d\n", bHideSkillEffect );

		strBuffer.AppendFormat( "bHideAnnouncements = %d\n", bHideAnnouncements );

		strBuffer.AppendFormat( "bShowRankBadge = %d\n", bShowRankBadge );


		strBuffer.AppendFormat( "emHideDead = %d\n", emHideDead );

		strBuffer.AppendFormat( "bShowNameBackground = %d\n", bShowNameBackground );

		strBuffer.AppendFormat( "bShowPKDisplay= %d\n", bShowPKDisplay);

		strBuffer.AppendFormat( "bShowRankMark = %d\n", bShowRankMark );

		CRijndael oRijndael;
		oRijndael.Initialize( CRijndael::VERSION, CRijndael::sm_Version[CRijndael::VERSION-1].c_str(), CRijndael::sm_KeyLength[CRijndael::VERSION-1] );

		int nLen = oRijndael.GetEncryptStringLength( strBuffer );
		char * szBuffer = new char[nLen];
		memset( szBuffer, 0, nLen );

		oRijndael.EncryptEx( strBuffer.GetString(), szBuffer, strBuffer.GetLength() );
		int nVersion = CRijndael::VERSION;

		if ( nVersion <= 0 && strlen( szBuffer ) <= 0 )	return FALSE;

		std::string	strFile = RPATH::getAppPath();
		strFile += RINFO::getc( EMRINFO_FILE_NAME_DBSLASH );
		strFile += RINFO::getc( EMRINFO_FILE_NAME_ROPTION );

		CFile file;
		if ( ! file.Open( strFile.c_str(), CFile::modeCreate|CFile::modeWrite ) )	
		{
			delete [] szBuffer;
			return FALSE;
		}

		file.Write( &nVersion, sizeof(int) );
		file.Write( szBuffer, nLen );
		file.Close();

		delete [] szBuffer;

		return TRUE;
	};
};