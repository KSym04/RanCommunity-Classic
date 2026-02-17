#include "stdafx.h"
#include "./NSRGameGlobal.h"

#include "../Lib_Engine/Common/compbyte.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RGAME_GLOBAL
{
	std::string	strENC_USERID		= "";
	std::string	strENC_CHARID		= "";
	const std::string strIDNULL		= "(null)";

	std::string	strIDPARAM1;
	std::string	strIDPARAM2;
	std::string	strIDPARAM3;

	std::string strJPUUID;

	void SETIDPARAM1( std::string strPARAM )	{ strIDPARAM1 = strPARAM; }
	void SETIDPARAM2( std::string strPARAM )	{ strIDPARAM2 = strPARAM; }
	void SETIDPARAM3( std::string strPARAM )	{ strIDPARAM3 = strPARAM; }
	void SETJAPANUUID( std::string strUUID )	{ strJPUUID = strUUID; }

	const TCHAR* GETIDPARAM1()					{ return strIDPARAM1.c_str(); }
	const TCHAR* GETIDPARAM2()					{ return strIDPARAM2.c_str(); }
	const TCHAR* GETIDPARAM3()					{ return strIDPARAM3.c_str(); }
	const TCHAR* GETJAPANUUID()					{ return strJPUUID.c_str(); }

	bool VALIDIDPARAM ()
	{
		return !strIDPARAM1.empty();
	}

	enum { emENCKEYLENG = MAX_COMPUTERNAME_LENGTH+32 };
	char szEncryptKEY[emENCKEYLENG] = "unnamed";

	void SETENCRYPT ()
	{
		StringCchCopy ( szEncryptKEY, emENCKEYLENG, "XWeRtSOn32QwRtQs" );
	}

	BOOL SETCHARID( const TCHAR* szCHARID )
	{
		strENC_CHARID = szCHARID;

		return TRUE;
	}

	std::string GETCHARID()
	{
		return strENC_CHARID;
	}

	BOOL SETUSERID ( const TCHAR* szUSERID )
	{
		SETENCRYPT ();

		BYTE szENC_USERID[MAX_PATH] = "";

		size_t nKEYLENG = strlen ( szEncryptKEY );
		size_t nIDLENG = strlen ( szUSERID );
		compbyte::encrypt ( (UCHAR*) szEncryptKEY, (DWORD)nKEYLENG, (const UCHAR*) szUSERID, szENC_USERID, (DWORD)nIDLENG );
		compbyte::hex2string( szENC_USERID, (DWORD)nIDLENG, strENC_USERID );

		return TRUE;
	}

	std::string GETUSERID_ENC ()
	{
		return strENC_USERID;
	}

	std::string GETUSERID_DEC ()
	{
		SETENCRYPT ();

		const char* szENC_USERID = strENC_USERID.c_str();

		size_t nKEYLENG = strlen ( szEncryptKEY );
		size_t nIDLENG = strlen ( szENC_USERID );
		if ( nIDLENG == 0 )		return "";

		BYTE szHEX_USERID[MAX_PATH] = "";
		char szDEC_USERID[MAX_PATH] = "";

		SecureZeroMemory ( szHEX_USERID, MAX_PATH );
		SecureZeroMemory ( szDEC_USERID, MAX_PATH );

		BOOL bOK = compbyte::string2hex ( (UCHAR*)szENC_USERID, (UCHAR*) szHEX_USERID, (DWORD) nIDLENG );
		size_t nHEXLENG = strlen ( (char*) szHEX_USERID );

		compbyte::decrypt ( (UCHAR*) szEncryptKEY, (DWORD) nKEYLENG, (const UCHAR*) szHEX_USERID, (UCHAR*) szDEC_USERID, (DWORD) nHEXLENG );

		return szDEC_USERID;
	}

	std::string GETUSERID_NULL ()
	{
		return strIDNULL;
	}

	void SETUSERID_ENC( std::string strUser )
	{
		strENC_USERID = strUser;
	}

	BOOL	bSAVE_USERID = FALSE;

	BOOL	bNPCShopConfirm		= TRUE;
	BOOL	bItemPreviewStance	= TRUE;
	BOOL	bVehicleWindowRun	= FALSE;

	BOOL	bNPCContributionShopConfirm = TRUE;

	BOOL	bAutoDraw = FALSE;
	BOOL	bConfirmDraw = FALSE;
};