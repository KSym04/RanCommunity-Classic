#pragma once

#include "../Lib_Engine/Common/RCryptByte.h"

enum EMCRYPT_FILETYPE
{
	EMCRYPT_FILETYPE_TEXTFILE			= 0, //text file 
	EMCRYPT_FILETYPE_STREAMFILE			= 1, //stream file
	EMCRYPT_FILETYPE_STREAMFILE_R2		= 2, //stream file ran2
	EMCRYPT_FILETYPE_STREAMFILE_ORIG	= 3, //stream file orig
	EMCRYPT_FILETYPE_XMLCRYPT			= 4, //xml gui file
	EMCRYPT_FILETYPE_TEXTURECRYPT		= 5, //texture files
	EMCRYPT_FILETYPE_XFILE_CRYPT		= 6, //x files
	EMCRYPT_FILETYPE_ANIMFILE			= 7, //animation bin

	EMCRYPT_FILETYPE_EGP_TO_NEW			= 8, //
	EMCRYPT_FILETYPE_R2_EGP_TO_NEW		= 9, //
	EMCRYPT_FILETYPE_CPS_TO_NEW			= 10, //
	EMCRYPT_FILETYPE_R2_CPS_TO_NEW		= 11, //

	EMCRYPT_FILETYPE_TEXTFILE_GS		= 12, //text file 
	EMCRYPT_FILETYPE_TEXTFILE_GS_V4		= 13, //text file 

	EMCRYPT_FILETYPE_SIZE				= 14,
};

enum EMCRYPT_BYPASSHEADER
{
	EMCRYPT_BYPASSHEADER_0	= 0, //no bypass
	EMCRYPT_BYPASSHEADER_1	= 1, //bypass 128 bytes ( FILETYPE )
	EMCRYPT_BYPASSHEADER_2	= 2, //bypass 132 bytes ( FILETYPE + HEADER )
	EMCRYPT_BYPASSHEADER_3	= 3, //bypass 136 bytes ( FILETYPE + HEADER + VERSION )
	EMCRYPT_BYPASSHEADER_SIZE = 4,
};

namespace FILE_CRYPT_GLOBAL
{
	extern std::string file_crypt_type[EMCRYPT_FILETYPE_SIZE];
	extern std::string file_crypt_bypass_type[EMCRYPT_BYPASSHEADER_SIZE];

	extern std::string byte_crypt_type[EMRCRYPT_BYTE_TYPE_SIZE];
};
