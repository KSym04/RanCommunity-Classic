#include "stdafx.h"
#include "./MShieldGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef USE_MSHIELD

namespace MShieldGlobal
{
	UINT nKey					= 0;
	UINT nResult				= 0;
	TCHAR szLastUser[MSHIELD_LAST_USER_SIZE] = "";

	UINT nHandShakeRequestKey	= 0;
	UINT nHandShakeResultKey	= 0;
};

#endif // USE_MSHIELD