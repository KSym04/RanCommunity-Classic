/*!
 * \file MShieldGlobal.h
 *
 * \author Juver
 * \date 2020/06/01
 *
 * 
 */

#ifndef MSHIELDGLOBAL_H_INCLUDED__
#define MSHIELDGLOBAL_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#if defined( BUILD_X )
//	//use mshield
//	//#define USE_MSHIELD
#endif // BUILD_X





#ifdef USE_MSHIELD

#define MSHIELD_KEY						307592
#define MSHIELD_LOGIN_FEEDBACK			WM_APP + 8000
#define MSHIELD_RESET_INFO_FEEDBACK		WM_APP + 8001
#define MSHIELD_HANDSHAKE_REQUEST		WM_APP + 8002
#define MSHIELD_LAST_USER_SIZE			50

namespace MShieldGlobal
{
	extern UINT nKey;
	extern UINT nResult;
	extern TCHAR szLastUser[MSHIELD_LAST_USER_SIZE];

	extern UINT nHandShakeRequestKey;
	extern UINT nHandShakeResultKey;
};

#endif // USE_MSHIELD

#endif // MSHIELDGLOBAL_H_INCLUDED__
