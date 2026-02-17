// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef WINVER		
#define WINVER 0x0501
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif
#ifndef _WIN32_WINNT	
#define _WIN32_WINNT 0x0501
#endif	
#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components

#include "../Lib_Engine/DxCommon9/dxstdafx.h"



#if defined ( PH_PARAM ) || defined ( VN_PARAM )
#import "EGameEncrypt.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#else
#import "MyRossoEncrypt.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#endif


#import "DaumCrypt.DLL" rename_namespace("DAUM"), named_guids