/*!
 * \file LoginDefine.h
 *
 * \author Juver
 * \date 2022/05/03
 *
 * 
 */

#ifndef LOGINDEFINE_H_INCLUDED__
#define LOGINDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_LOGIN_IP 5

struct SLOGIN_ADDRESS
{
	std::string strLoginIP;
	UINT		nLoginPort;

	SLOGIN_ADDRESS()
		: strLoginIP("")
		, nLoginPort(0)
	{
	}

	SLOGIN_ADDRESS( std::string strIP, UINT nPort )
	{
		strLoginIP = strIP;
		nPort = nPort;
	}
};


#endif // LOGINDEFINE_H_INCLUDED__
