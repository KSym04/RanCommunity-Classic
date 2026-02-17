#pragma once

#include "./GLContrlBaseMsg.h"

#include <boost/static_assert.hpp>

namespace GLMSG
{
	#pragma pack(1)

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_LURE
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwGaeaID;
		SPARTY_OPT			sOption;

		SNET_PARTY_LURE () 
			: dwGaeaID(0)
		{
			nmg.dwSize = sizeof(SNET_PARTY_LURE);
			nmg.nType = NET_MSG_GCTRL_PARTY_LURE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* party charid/charname, Juver, 2020/03/02 */
	struct SNET_PARTY_LURE_CHAR_ID
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwCharID;
		SPARTY_OPT			sOption;

		SNET_PARTY_LURE_CHAR_ID () 
			: dwCharID(0)
		{
			nmg.dwSize = sizeof(SNET_PARTY_LURE_CHAR_ID);
			nmg.nType = NET_MSG_GCTRL_PARTY_LURE_CHAR_ID;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* party charid/charname, Juver, 2020/03/02 */
	struct SNET_PARTY_LURE_CHAR_NAME
	{
		NET_MSG_GENERIC		nmg;

		TCHAR				szCharName[CHAR_SZNAME];
		SPARTY_OPT			sOption;

		SNET_PARTY_LURE_CHAR_NAME () 
		{
			memset ( szCharName, 0, sizeof(char)*CHAR_SZNAME );

			nmg.dwSize = sizeof(SNET_PARTY_LURE_CHAR_NAME);
			nmg.nType = NET_MSG_GCTRL_PARTY_LURE_CHAR_NAME;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_LURE_FB
	{
		NET_MSG_GENERIC		nmg;

		//info of requested character
		DWORD				dwGaeaID;
		DWORD				dwCharID;
		char				szCharName[CHAR_SZNAME];

		EMPARTY_LURE_FB		emAnswer;

		SNET_PARTY_LURE_FB () 
			: dwGaeaID(0)
			, dwCharID(GAEAID_NULL)
			, emAnswer(EMPARTY_LURE_OK)
		{
			memset ( szCharName, 0, sizeof(char)*CHAR_SZNAME );

			nmg.dwSize = sizeof(SNET_PARTY_LURE_FB);
			nmg.nType = NET_MSG_GCTRL_PARTY_LURE_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_LURE_TAR
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		DWORD				dwMasterID;
		SPARTY_OPT			sOption;

		char				szCharName[CHAR_SZNAME];

		SNET_PARTY_LURE_TAR () 
			: dwPartyID(0)
			, dwMasterID(0)
		{
			memset ( szCharName, 0, sizeof(char)*CHAR_SZNAME );

			nmg.dwSize = sizeof(SNET_PARTY_LURE_TAR);
			nmg.nType = NET_MSG_GCTRL_PARTY_LURE_TAR;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_LURE_TAR_ANS
	{
		NET_MSG_GENERIC		nmg;
		EMPARTY_LURE_FB		emAnswer;

		DWORD				dwPartyID;
		DWORD				dwMasterID;
		SPARTY_OPT			sOption;

		SNET_PARTY_LURE_TAR_ANS () 
			: emAnswer(EMPARTY_LURE_OK)
			, dwPartyID(0)
			, dwMasterID(0)
		{
			nmg.dwSize = sizeof(SNET_PARTY_LURE_TAR_ANS);
			nmg.nType = NET_MSG_GCTRL_PARTY_LURE_TAR_ANS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_SECEDE
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		DWORD				dwGaeaID;

		SNET_PARTY_SECEDE () 
			: dwPartyID(0)
			, dwGaeaID(0)
		{
			nmg.dwSize = sizeof(SNET_PARTY_SECEDE);
			nmg.nType = NET_MSG_GCTRL_PARTY_SECEDE;
		}
	};
	
	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_AUTHORITY
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		DWORD				dwGaeaID;

		SNET_PARTY_AUTHORITY () 
			: dwPartyID(0)
			, dwGaeaID(0)
		{
			nmg.dwSize = sizeof( SNET_PARTY_AUTHORITY );
			nmg.nType = NET_MSG_GCTRL_PARTY_AUTHORITY;
		}
	};


	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_FNEW
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		DWORD				dwMaster;
		SPARTY_OPT			sOption;

		DWORD				dwPartyNum;
		GLPARTY_FNET		sPartyInfo[MAXPARTY];

		void SETINFO ( DWORD _dwPartyID, DWORD _dwMaster )
		{
			dwPartyID = _dwPartyID;
			dwMaster = _dwMaster;
		}

		void ADDMEMBER ( GLPARTY_FNET &sParty )
		{
			if (dwPartyNum < (MAXPARTY))
			{
				sPartyInfo[dwPartyNum] = sParty;
				++dwPartyNum;
				nmg.dwSize = GETHEADSIZE() + sizeof(GLPARTY_FNET)*dwPartyNum;
			}
		}

		DWORD GETHEADSIZE ()
		{
			return sizeof(nmg)
					+ sizeof(dwPartyID)
					+ sizeof(dwMaster)
					+ sizeof(sOption)
					+ sizeof(dwPartyNum);
		}

		SNET_PARTY_FNEW () :
			dwPartyID(0),
			dwMaster(0),

			dwPartyNum(0)
		{
			nmg.dwSize = GETHEADSIZE();
			nmg.nType = NET_MSG_GCTRL_PARTY_FNEW;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_MBR_DETAIL
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwGaeaID;
		D3DXVECTOR3			vPos;
		GLPADATA			sHP;
		GLPADATA			sMP;
		WORD				wLevel;

		SNET_PARTY_MBR_DETAIL () :
			dwGaeaID(0),
			vPos(0,0,0),
			wLevel(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_MBR_DETAIL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_DISSOLVE
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;

		SNET_PARTY_DISSOLVE () :
			dwPartyID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_DISSOLVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_ADD
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		GLPARTY_FNET		sPartyInfo;

		SNET_PARTY_ADD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_ADD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_DEL
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		DWORD				dwDelMember;

		SNET_PARTY_DEL () :
			dwDelMember(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_DEL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PARTY_MASTER_RENEW
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		DWORD				dwNewMaster;
		DWORD				dwDelMaster;

		GLPARTY_FNET		sNewMaster;

		SNET_PARTY_MASTER_RENEW ()
			: dwPartyID(0)
			, dwNewMaster(0)
			, dwDelMaster(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_MASTER_RENEW;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_MBR_MOVEMAP
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwPartyID;
		DWORD				dwGaeaID;
		SNATIVEID			sMapID;

		SNET_PARTY_MBR_MOVEMAP () :
			dwPartyID(PARTY_NULL),
			dwGaeaID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_MBR_MOVEMAP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_MBR_POINT
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwGaeaID;
		GLPADATA			sHP;
		GLPADATA			sMP;

		SNET_PARTY_MBR_POINT () :
			dwGaeaID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_MBR_POINT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_MBR_POS
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwGaeaID;
		D3DXVECTOR3			vPos;
		GLPADATA			sHP;
		GLPADATA			sMP;
		WORD				wLevel;

		SNET_PARTY_MBR_POS () :
			dwGaeaID(0),
			vPos(0,0,0),
			wLevel(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_MBR_POS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_PARTY_MBR_PICKUP_BRD : public SNETPC_BROAD
	{
		/*link drop item, EJCode, 2018/10/20 */
		SITEMLINK			sitem_link;

		SNETPC_PARTY_MBR_PICKUP_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_MBR_PICKUP_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_MBR_PICKUP_BRD)<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PARTY_BRD : public SNETPC_BROAD
	{
		DWORD dwPartyID;
		DWORD dwPMasterID;
		
		SNET_PARTY_BRD () :
			dwPartyID(GAEAID_NULL),
			dwPMasterID(GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_CONFT_SPTY_EXP_FLD
	{
		NET_MSG_GENERIC		nmg;

		WORD				wSCHOOL;
		float				fEXP_RATE;

		SNET_CONFT_SPTY_EXP_FLD () :
			wSCHOOL(0),
			fEXP_RATE(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CONFRONTSPTY_EXP_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PARTY_MEMBER_RENAME_CLT
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwGaeaID;
		DWORD				dwPartyID;
		char				szName[CHAR_SZNAME];

		SNET_PARTY_MEMBER_RENAME_CLT ():
			dwGaeaID(0),
			dwPartyID(0)
		{
			memset ( szName, 0, CHAR_SZNAME );
			nmg.dwSize = sizeof(SNET_PARTY_MEMBER_RENAME_CLT);
			nmg.nType = NET_MSG_GCTRL_PARTY_MBR_RENAME_CLT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PARTY_MEMBER_RENAME_FLD
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwGaeaID;
		DWORD				dwPartyID;
		char				szName[CHAR_SZNAME];

		SNET_PARTY_MEMBER_RENAME_FLD ():
			dwGaeaID(0),
			dwPartyID(0)
		{
			memset ( szName, 0, CHAR_SZNAME );
			nmg.dwSize = sizeof(SNET_PARTY_MEMBER_RENAME_FLD);
			nmg.nType = NET_MSG_GCTRL_PARTY_MBR_RENAME_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PARTY_CHANGEOPTION
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwGaeaID;
		DWORD				dwPartyID;
		SPARTY_OPT			sOption;

		SNET_PARTY_CHANGEOPTION () 
			: dwGaeaID(0)
			, dwPartyID(0)
		{
			nmg.dwSize = sizeof(SNET_PARTY_CHANGEOPTION);
			nmg.nType = NET_MSG_GCTRL_PARTY_CHANGEOPTION;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PARTY_OPTIONTOPARTY
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwPartyID;
		SPARTY_OPT			sOption;

		SNET_PARTY_OPTIONTOPARTY() /*:*/
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_OPTIONTOPARTY;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	// Revert to default structure packing
	#pragma pack()
};