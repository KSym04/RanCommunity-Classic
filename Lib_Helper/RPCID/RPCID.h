#pragma once

#include <IPTypes.h>
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")

namespace RPCID
{
	std::string IDStringGet();

	std::string getStringCombined();
	std::string getStringCPU();
	std::string getStringLAN();
	std::string getStringDISK();
	std::string getStringPCNAME();
};