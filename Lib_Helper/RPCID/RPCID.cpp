#include "stdafx.h"
#include "./RPCID.h"
#include "../Lib_Engine/Hash/MD52.h"

#if _MSC_VER >= 1600
#include <intrin.h>
#endif // _MSC_VER > 1600

#include "./c99printf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INFO_UINT_SIZE 10
#define INFO_MACSTRING_SIZE 14
#define INFO_BUFFER_SIZE 32767

namespace RPCID
{
	USHORT  getVolumeHash()
	{
		DWORD serialNum = 0;
		GetVolumeInformation( "c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0 );
		USHORT hash = (USHORT)(( serialNum + ( serialNum >> 16 )) & 0xFFFF );
		return hash;
	};

#if _MSC_VER >= 1600
	USHORT getCpuHash()
	{
		int cpuinfo[4] = { 0, 0, 0, 0 };
		__cpuid( cpuinfo, 0 );
		USHORT hash = 0;
		USHORT* ptr = (USHORT*)(&cpuinfo[0]);
		for ( int i = 0; i < 8; i++ )
			hash += ptr[i];

		return hash;
	};
#else
	USHORT _getCpuHash()
	{
		return 0;
	};
#endif // _MSC_VER > 1600


	std::string IDStringGet()
	{
		std::string hash = md5( getStringCombined() );
		return hash;
	};

	std::string getStringCombined()
	{
		std::string out("");
		out += getStringLAN();
		out += getStringCPU();
		out += getStringDISK();
		out += getStringPCNAME();
		return out;
	}

	std::string getStringCPU()
	{
		char buff[INFO_UINT_SIZE] = {0};
		snprintf(buff, sizeof(buff), "%x", getCpuHash() );
		return std::string(buff);
	}

	std::string getStringLAN()
	{
		std::string out("");

		IP_ADAPTER_INFO AdapterInfo[18];
		DWORD dwBufLen = sizeof(AdapterInfo);
		DWORD dwStatus = GetAdaptersInfo( AdapterInfo, &dwBufLen );
		if(dwStatus == NO_ERROR)
		{
			PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
			while( pAdapterInfo )
			{
				TCHAR szTemp[INFO_MACSTRING_SIZE] = {0};
				snprintf( szTemp, sizeof(szTemp), "%02X%02X%02X%02X%02X%02X", 
					pAdapterInfo->Address[0], 
					pAdapterInfo->Address[1], 
					pAdapterInfo->Address[2], 
					pAdapterInfo->Address[3], 
					pAdapterInfo->Address[4], 
					pAdapterInfo->Address[5]);

				out += szTemp;

				pAdapterInfo = pAdapterInfo->Next;
			}
		}

		return out;
	}

	std::string getStringDISK()
	{
		char buff[INFO_UINT_SIZE] = {0};
		snprintf(buff, sizeof(buff), "%x", getVolumeHash() );
		return std::string(buff);
	}

	std::string getStringPCNAME()
	{
		char buff[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD len = sizeof(buff);
		GetComputerNameEx((COMPUTER_NAME_FORMAT)0, buff, &len);
		return std::string(buff);
	}
};