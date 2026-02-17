#include "stdafx.h"
#include "./AppString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NSAPP_STRING
{
	const TCHAR* getc( EMAPP_STRING emType )
	{
		if ( emType >= EMAPP_STRING_SIZE )	return NULL;
		return strInfo[emType].c_str();
	};

	std::string gets( EMAPP_STRING emType )
	{
		if ( emType >= EMAPP_STRING_SIZE )	return "";
		return strInfo[emType];
	};

	 std::string strInfo[EMAPP_STRING_SIZE] = 
	 {
		 "Game",			// EMAPP_STRING_BASE
		 "Invalid parameter, Please Use Launcher to Play!",					//EMAPP_STRING_LAUNCH_FAILED
		 "DirectX Version too old. must install DirectX 9.0c",				//EMAPP_STRING_DIRECTX_OLD
		 "Class Registration Failed",										//EMAPP_STRING_CLASS_REG_FAILED
		 "use_rcc",															//EMAPP_STRING_RCC_PARAMETER_1
		 "USE_RCC",															//EMAPP_STRING_RCC_PARAMETER_2
	 };
};
