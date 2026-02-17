#include "stdafx.h"
#include "./GLPVPCaptureTheFlagDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NSCTFTeam
{
	DWORD CTFEnumToFlag ( const EMCAPTURE_THE_FLAG_TEAM emTeam )
	{
		switch ( emTeam )
		{
		case EMCAPTURE_THE_FLAG_TEAM_A:	return CAPTURE_THE_FLAG_TEAM_A;
		case EMCAPTURE_THE_FLAG_TEAM_B:	return CAPTURE_THE_FLAG_TEAM_B;
		};

		return CAPTURE_THE_FLAG_TEAM_SIZE;
	}

	EMCAPTURE_THE_FLAG_TEAM CTFFlagToEnum ( const DWORD dwTeam )
	{
		switch ( dwTeam )
		{
		case CAPTURE_THE_FLAG_TEAM_A:	return EMCAPTURE_THE_FLAG_TEAM_A;
		case CAPTURE_THE_FLAG_TEAM_B:	return EMCAPTURE_THE_FLAG_TEAM_B;
		};

		return EMCAPTURE_THE_FLAG_TEAM_ALL;
	}
};
