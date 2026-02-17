#include "stdafx.h"
#include "./RPCIDSMBIOS.h"

#include "../SMBiosParser/SMBiosParser.h"

#include "../Lib_Engine/Hash/CHash.h"

#include <vector>
#include <boost/format.hpp>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RPCID
{
	bool getSMBIOS( SPCID_SMBIOS& sPCID )
	{
		std::vector<uint8_t> buffer;

		const BYTE byteSignature[] = { 'B', 'M', 'S', 'R' };
		const DWORD signature = *((DWORD*)byteSignature);

		// get the size of SMBIOS table
		DWORD size = GetSystemFirmwareTable(signature, 0, NULL, 0);
		if (size == 0)		return false;

		buffer.resize(size, 0);
		// retrieve the SMBIOS table

		if (size != GetSystemFirmwareTable(signature, 0, buffer.data(), size))
		{
			buffer.clear();
			return false ;
		}

		smbios::Parser parser(&buffer.front(), buffer.size());

		int version = parser.version();
		const smbios::Entry *entry = NULL;
		while (true)
		{
			entry = parser.next();
			if (entry == NULL) break;

			if (entry->type == DMI_TYPE_SYSINFO)
			{


				boost::format fmt = boost::format("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X")
					% (unsigned int)entry->data.sysinfo.UUID[0] 
					% (unsigned int)entry->data.sysinfo.UUID[1] 
					% (unsigned int)entry->data.sysinfo.UUID[2] 
					% (unsigned int)entry->data.sysinfo.UUID[3]
					% (unsigned int)entry->data.sysinfo.UUID[4] 
					% (unsigned int)entry->data.sysinfo.UUID[5] 
					% (unsigned int)entry->data.sysinfo.UUID[6] 
					% (unsigned int)entry->data.sysinfo.UUID[7]
					% (unsigned int)entry->data.sysinfo.UUID[8] 
					% (unsigned int)entry->data.sysinfo.UUID[9] 
					% (unsigned int)entry->data.sysinfo.UUID[10] 
					% (unsigned int)entry->data.sysinfo.UUID[11]
					% (unsigned int)entry->data.sysinfo.UUID[12] 
					% (unsigned int)entry->data.sysinfo.UUID[13] 
					% (unsigned int)entry->data.sysinfo.UUID[14]
					% (unsigned int)entry->data.sysinfo.UUID[15]
					;

				sPCID.strSystemUUID = fmt.str().c_str();


			}

			if (entry->type == DMI_TYPE_BASEBOARD)
			{
				if (version >= smbios::SMBIOS_2_0)
				{
	

					boost::format fmt = boost::format("%s") % entry->data.baseboard.SerialNumber;
					sPCID.strBoardSerialNumber = fmt.str().c_str();

					fmt = boost::format("%s") % entry->data.baseboard.Product;
					sPCID.strBoardModel = fmt.str().c_str();


				}
			}

			if (entry->type == DMI_TYPE_PROCESSOR)
			{
				if (version >= smbios::SMBIOS_2_0)
				{


					boost::format fmt = boost::format("%s") % entry->data.processor.ProcessorVersion;
					sPCID.strProcessorVersion = fmt.str().c_str();

					fmt = boost::format("%02X%02X%02X%02X%02X%02X%02X%02X") 
						% (int) entry->data.processor.ProcessorID[0] 
						% (int) entry->data.processor.ProcessorID[1]
						% (int) entry->data.processor.ProcessorID[2] 
						% (int) entry->data.processor.ProcessorID[3]
						% (int) entry->data.processor.ProcessorID[4]
						% (int) entry->data.processor.ProcessorID[5]
						% (int) entry->data.processor.ProcessorID[6] 
						% (int) entry->data.processor.ProcessorID[7] 
						;

					sPCID.strProcessorID = fmt.str().c_str();


				}
			}

		}

		return true;
	}


	std::string getHashSMBiosSystemUUID( const SPCID_SMBIOS& sPCID )
	{
		std::string strHash = "";



		boost::format fmt = boost::format("%s") % sPCID.strSystemUUID;

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm(HASH::MD5);
		g_hashObj.SetHashOperation(HASH::STRING_HASH);
		g_hashObj.SetHashFormat(HASH::UPPERCASE_DASHES);
		g_hashObj.SetHashString(fmt.str().c_str());
		strHash = g_hashObj.DoHash().GetString();



		return strHash;
	}

	std::string getHashSMBiosBoardSerial( const SPCID_SMBIOS& sPCID )
	{
		std::string strHash = "";



		boost::format fmt = boost::format("%s") % sPCID.strBoardSerialNumber;

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm(HASH::MD5);
		g_hashObj.SetHashOperation(HASH::STRING_HASH);
		g_hashObj.SetHashFormat(HASH::UPPERCASE_DASHES);
		g_hashObj.SetHashString(fmt.str().c_str());
		strHash = g_hashObj.DoHash().GetString();



		return strHash;
	}

	std::string getHashSMBiosProcessorVersion( const SPCID_SMBIOS& sPCID )
	{
		std::string strHash = "";



		boost::format fmt = boost::format("%s") % sPCID.strProcessorVersion;

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm(HASH::MD5);
		g_hashObj.SetHashOperation(HASH::STRING_HASH);
		g_hashObj.SetHashFormat(HASH::UPPERCASE_DASHES);
		g_hashObj.SetHashString(fmt.str().c_str());
		strHash = g_hashObj.DoHash().GetString();



		return strHash;
	}

	std::string getHashSMBiosProcessorID( const SPCID_SMBIOS& sPCID )
	{
		std::string strHash = "";



		boost::format fmt = boost::format("%s") % sPCID.strProcessorID;

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm(HASH::MD5);
		g_hashObj.SetHashOperation(HASH::STRING_HASH);
		g_hashObj.SetHashFormat(HASH::UPPERCASE_DASHES);
		g_hashObj.SetHashString(fmt.str().c_str());
		strHash = g_hashObj.DoHash().GetString();



		return strHash;
	}



	std::string getHashSMBIOS( const SPCID_SMBIOS& sPCID )
	{
		std::string strHash = "";

	

		boost::format fmt = boost::format("%s-%s-%s-%s") 
			% sPCID.strSystemUUID
			% sPCID.strBoardSerialNumber
			% sPCID.strProcessorVersion
			% sPCID.strProcessorID
			;

		HASH::CHash g_hashObj;
		g_hashObj.SetHashAlgorithm(HASH::MD5);
		g_hashObj.SetHashOperation(HASH::STRING_HASH);
		g_hashObj.SetHashFormat(HASH::UPPERCASE_DASHES);
		g_hashObj.SetHashString(fmt.str().c_str());
		strHash = g_hashObj.DoHash().GetString();



		return strHash;
	}

};