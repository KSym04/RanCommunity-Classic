#pragma once

#include "./GLContrlBaseMsg.h"
#include "./GLDailyLogin.h"

/*12-16-24, daily login - CNDev*/

namespace GLMSG
{
#pragma pack(1)

	struct SNETPC_DAILYLOGIN_COMPLETE_BRD : public SNETPC_BROAD
	{
		SNETPC_DAILYLOGIN_COMPLETE_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_DAILYLOGIN_COMPLETE_BRD;
		}
	};

	struct SNETPC_DAILYLOGIN_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwStreak;

		SNETPC_DAILYLOGIN_UPDATE ()
			: dwStreak(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_DAILYLOGIN_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_DAILYLOGIN_COMPLETE
	{
		NET_MSG_GENERIC			nmg;
		SDAILYLOGIN_CHAR_DATA	sData;

		SNETPC_DAILYLOGIN_COMPLETE () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_DAILYLOGIN_COMPLETE;
		}
	};

	struct SNETPC_DAILYLOGIN_NOTIFY_CLIENT
	{
		NET_MSG_GENERIC		nmg;
		float				fRemain;

		SNETPC_DAILYLOGIN_NOTIFY_CLIENT ()
			: fRemain(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_DAILYLOGIN_NOTIFY_CLIENT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

#pragma pack()
};

