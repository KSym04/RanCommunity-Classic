/*!
 * \file RPCIDSMBIOS.h
 *
 * \author Juver
 * \date 2020/03/27
 *
 * 
 */

#ifndef RPCIDSMBIOS_H_INCLUDED__
#define RPCIDSMBIOS_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SPCID_SMBIOS
{
	std::string strSystemUUID;
	std::string strBoardModel;
	std::string strBoardSerialNumber;
	std::string strProcessorVersion;
	std::string strProcessorID;

	SPCID_SMBIOS()
		: strSystemUUID("None")
		, strBoardModel("None")
		, strBoardSerialNumber("None")
		, strProcessorVersion("None")
		, strProcessorID("None")
	{

	};
};

namespace RPCID
{
	bool getSMBIOS( SPCID_SMBIOS& sPCID );
	std::string getHashSMBiosSystemUUID( const SPCID_SMBIOS& sPCID );
	std::string getHashSMBiosBoardSerial( const SPCID_SMBIOS& sPCID );
	std::string getHashSMBiosProcessorVersion( const SPCID_SMBIOS& sPCID );
	std::string getHashSMBiosProcessorID( const SPCID_SMBIOS& sPCID );
	std::string getHashSMBIOS( const SPCID_SMBIOS& sPCID );
};

#endif // RPCIDSMBIOS_H_INCLUDED__
