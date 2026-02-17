#pragma once

#include "./GLContrlBaseMsg.h"
#include "./GLBattlePassData.h"

/*12-9-14, Battle Pass - CNDev*/

namespace GLMSG
{
#pragma pack(1)

	struct SNETPC_BATTLEPASS_COMPLETE_BRD : public SNETPC_BROAD
	{
		SNETPC_BATTLEPASS_COMPLETE_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_BATTLEPASS_COMPLETE_BRD;
		}
	};

	struct SNETPC_BATTLEPASS_UPDATE
	{
		NET_MSG_GENERIC			nmg;
		SBATTLEPASS_CHAR_DATA	sData;

		SNETPC_BATTLEPASS_UPDATE () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_BATTLEPASS_UPDATE;
		}
	};

	struct SNETPC_BATTLEPASS_COMPLETE
	{
		NET_MSG_GENERIC			nmg;
		SBATTLEPASS_CHAR_DATA	sData;

		SNETPC_BATTLEPASS_COMPLETE () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_BATTLEPASS_COMPLETE;
		}
	};

	/*struct SNETPC_BATTLEPASS_NOTIFY_AGENT
	{
	NET_MSG_GENERIC		nmg;
	char	szCharacterName[CHAR_SZNAME];
	char	szBattlePassTitle[BATTLEPASS_TITLE_MSG_SIZE];

	SNETPC_BATTLEPASS_NOTIFY_AGENT ()
	{
	memset (szCharacterName, 0, sizeof(char) * CHAR_SZNAME);
	memset (szBattlePassTitle, 0, sizeof(char) * BATTLEPASS_TITLE_MSG_SIZE );

	nmg.dwSize = sizeof(*this);
	nmg.nType = NET_MSG_GCTRL_BATTLEPASS_NOTIFY_AGENT;
	GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
	}
	};*/

	struct SNETPC_BATTLEPASS_NOTIFY_CLIENT
	{
		NET_MSG_GENERIC		nmg;
		char	szCharacterName[CHAR_SZNAME];
		char	szBattlePassTitle[BATTLEPASS_TITLE_MSG_SIZE];

		SNETPC_BATTLEPASS_NOTIFY_CLIENT ()
		{
			memset (szCharacterName, 0, sizeof(char) * CHAR_SZNAME);
			memset (szBattlePassTitle, 0, sizeof(char) * BATTLEPASS_TITLE_MSG_SIZE);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_BATTLEPASS_NOTIFY_CLIENT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

#pragma pack()
};

