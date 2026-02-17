/*!
 * \file HackShieldGlobal.h
 *
 * \author Juver
 * \date 2020/06/05
 *
 * 
 */

#ifndef HACKSHIELDGLOBAL_H_INCLUDED__
#define HACKSHIELDGLOBAL_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#if defined( BUILD_CH )
	//use hackshield
	//#define USE_HACK_SHIELD
#endif

#ifdef USE_HACK_SHIELD

/*hackshield implementation, Juver, 2018/06/21 */
#define HS_DUMMY_HEARTBEAT 25.0f

namespace HackShieldGlobal
{
	extern int hs_start_ret;
	extern int hs_service_ret;
	extern int hs_monitor_ret;
	extern int hs_detect_ret;
	extern DWORD hs_sdk_version;
	extern std::string hs_detect_string;
};

#endif // USE_HACK_SHIELD

#endif // HACKSHIELDGLOBAL_H_INCLUDED__

