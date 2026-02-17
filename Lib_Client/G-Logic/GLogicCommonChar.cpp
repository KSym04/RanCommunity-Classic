#include "stdafx.h"
#include "./GLogicCommon.h"

#include "./GLGaeaServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace GLOGICEX
{
	int CALCKILLEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan )
	{	
		GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
		if ( !pActor )	return 0;

		WORD wAttackerLev = wMYLEVEL;
		WORD wDefenserLev = pActor->GetLevel();
		DWORD dwBonusExp = pActor->GetBonusExp();

		//	Note : 경험치 산출	
		return GLOGICEX::GLKILLEXP(wAttackerLev,wDefenserLev,dwBonusExp);
	}

	int CALCATTACKEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan, DWORD dwDamage )
	{
		GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
		if ( !pActor )	return 0;

		WORD wTAR_LEVEL = pActor->GetLevel ();
		DWORD dwTAR_MAXHP = pActor->GetMaxHP ();
		DWORD dwBONUSEXP = pActor->GetBonusExp ();

		//	Note : 경험치 산출
		return GLOGICEX::GLATTACKEXP ( wMYLEVEL, wTAR_LEVEL, dwDamage, dwTAR_MAXHP, dwBONUSEXP );
	}
};