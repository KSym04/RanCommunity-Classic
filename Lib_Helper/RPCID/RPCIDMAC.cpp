#include "stdafx.h"
#include "./RPCIDMAC.h"

#include "../Lib_Engine/Hash/CHash.h"



#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RPCID
{
	std::string getWindowsMAC()
	{
		std::string strMAC;
		
		const char* format = "%02X-%02X-%02X-%02X-%02X-%02X";
		char buffer[100] = { 0 };
		UUID uuid;

		UuidCreateSequential(&uuid);
		sprintf(buffer, format, uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);
		strMAC = buffer;

		return strMAC;
	}

	std::string getHashWindowsMAC()
	{
		std::string strHash = "";

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm(HASH::MD5);
		g_hashObj.SetHashOperation(HASH::STRING_HASH);
		g_hashObj.SetHashFormat(HASH::UPPERCASE_DASHES);
		g_hashObj.SetHashString(getWindowsMAC().c_str());
		strHash = g_hashObj.DoHash().GetString();

		return strHash;
	}
};
