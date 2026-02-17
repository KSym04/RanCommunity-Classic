#include <Windows.h>
#include <string>

#include "../Lib_Client/MShieldGlobal.h"

#define HANDSHAKE_TIMEOUT_SECONDS 20 //time in seconds

#ifdef USE_MSHIELD

namespace GyxMessages
{
	enum
	{
		BASE = 0xBAD00000,
		SETACCOUNTCALLBACK,
	};
}

typedef struct
{
	UINT hashIN;
	UINT result;
} SHANDSHAKE, *PSHANDSHAKE;

namespace MShield
{
#define NOINLINE __declspec(noinline) 

	typedef void(__stdcall * MTSETCALLBACK)(char* account);
	typedef UINT(__fastcall * HANDSHAKE)(UINT*, UINT*);
	typedef void* (__stdcall * MTP)();
	typedef void(__stdcall * MTU)();

	static void __stdcall HandShake(PSHANDSHAKE param);
	UINT MakeHandShakeHash(UINT handShakeIN);
	void Init();

	extern MTSETCALLBACK pMtSetCallback;
}

#endif