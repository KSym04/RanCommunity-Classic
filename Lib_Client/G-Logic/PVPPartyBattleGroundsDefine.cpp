#include "stdafx.h"
#include "./PVPPartyBattleGroundsDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace PVPPBG
{
	std::string STRING_EVENT_STATE[PVPPBG::EVENT_STATE_SIZE] = 
	{
		"Event State Ended",
		"Event State Register",
		"Event State Battle",
		"Event State Reward",
	};
}