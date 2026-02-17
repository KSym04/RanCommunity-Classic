#include "stdafx.h"
#include "./GLPostData.h"

#include "../../Lib_Engine/Common/GLTexFile.h"
#include "../../Lib_Engine/G-Logic/Glogic.h"

#include "./GLogicData.h"

#include "./GLPost.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace GLCONST_POST
{
	BOOL LOADFILE(const std::string& FileName, BOOL bServer)
	{
		if (FileName.empty())
			return FALSE;

		std::string strPath;
		strPath = GLOGIC::GetPath();
		strPath += FileName;

		gltexfile cFILE;
		cFILE.reg_sep( '\t' );
		cFILE.reg_sep ( ' ' );
		cFILE.reg_sep ( ',' );
		cFILE.reg_sep( '{' );
		cFILE.reg_sep( '}' );
		cFILE.reg_sep( '[' );
		cFILE.reg_sep( ']' );
		cFILE.reg_sep( '(' );
		cFILE.reg_sep( ')' );
		cFILE.reg_sep( '|' );

		if (GLOGIC::bGLOGIC_ZIPFILE)
			cFILE.SetZipFile(GLOGIC::strGLOGIC_ZIPFILE);

		if (!cFILE.open(strPath, true, GLOGIC::bGLOGIC_PACKFILE))
		{
			CDebugSet::ErrorFile( "GLCONST_POST::LOADFILE", strPath.c_str() );
			return FALSE;
		}

		// Note : 우체통 이용가능한 최대 거리
		cFILE.getflag( "fPOSTBOX_DISTANCE",				1, 1, fPOSTBOX_DISTANCE );

		// Note : 첨부할수 있는 금액의 최대치
		cFILE.getflag( "llPOST_MAX_ATTACH_MONEY",		1, 1, llPOST_MAX_ATTACH_MONEY );

		// Note : 첨부 금액에대한 수수료 비율
		cFILE.getflag( "fPOST_COMMISSION",				1, 1, fPOST_COMMISSION );
		// Note : 기본 수수료
		cFILE.getflag( "dwPOST_BASE_FEE",				1, 1, dwPOST_BASE_FEE );
		// Note : 첨부 아이템에대한 수수료
		cFILE.getflag( "dwPOST_COMMISSION_ITEM",		1, 1, dwPOST_COMMISSION_ITEM );
		// Note : 디자인 카드에 대한 수수료
		cFILE.getflag( "dwPOST_COMMISSION_DESIGN",		1, 1, dwPOST_COMMISSION_DESIGN );


		// Note : 한번에 받을 수 있는 최대 우편 갯수
		cFILE.getflag( "dwPOST_HOW_MANY_GET",			1, 1, dwPOST_HOW_MANY_GET );
		// Note : 한번에 받을 수 있는 편지의 최대 현황 갯수
		cFILE.getflag( "dwPOST_STATE_HOW_MANY_GET",     1, 1, dwPOST_STATE_HOW_MANY_GET );
		// Note : 한번에 받을 수 있는 보낸 편지의 최대 현황 갯수
		cFILE.getflag( "dwPOST_SEND_STATE_HOW_MANY_GET",1, 1, dwPOST_SEND_STATE_HOW_MANY_GET );
		// Note : 한번에 받을 수 있는 받은 편지의 최대 현황 갯수
		cFILE.getflag( "dwPOST_RECV_STATE_HOW_MANY_GET",1, 1, dwPOST_RECV_STATE_HOW_MANY_GET );

		// Note : 자동 소멸되는 날짜 (일)
		cFILE.getflag( "dwPOST_EXPIRE_DURATION_DAY",	1, 1, dwPOST_EXPIRE_DURATION_DAY );

		// Note : 우편 보내기 쿨타임 (ms)
		cFILE.getflag( "dwPOST_SEND_COOL_TIME_MS",		1, 1, dwPOST_SEND_COOL_TIME_MS );

		// Note : 우편 받기 쿨타임 (ms)
		cFILE.getflag( "dwPOST_RECV_COOL_TIME_MS",		1, 1, dwPOST_RECV_COOL_TIME_MS );

		// Note : 우편을 보내기위한 최소 레벨
		cFILE.getflag( "dwPOST_SEND_REQUIRE_LEVEL",		1, 1, dwPOST_SEND_REQUIRE_LEVEL );

		//-------------------------------------------------------------------------------------------------//

		// Note : 자동 소멸 날짜를 초로 계산
		GLPost::llPOST_EXPIRE_DURATION_SEC = ((LONGLONG)dwPOST_EXPIRE_DURATION_DAY * 24 * 60 * 60); 

		return TRUE;
	}
};