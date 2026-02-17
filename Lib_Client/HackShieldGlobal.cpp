#include "stdafx.h"
#include "./HackShieldGlobal.h"

#include "./HShield.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef USE_HACK_SHIELD

/*hackshield implementation, Juver, 2018/06/18 */
namespace HackShieldGlobal
{
	int hs_start_ret = HS_ERR_UNKNOWN;
	int hs_service_ret = HS_ERR_UNKNOWN;
	int hs_monitor_ret = 0;
	int hs_detect_ret = 0;
	DWORD hs_sdk_version = 0xffff; //wont check if value is default
	std::string hs_detect_string = "";
};

#endif // USE_HACK_SHIELD
