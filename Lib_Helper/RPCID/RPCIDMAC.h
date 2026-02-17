/*!
 * \file RPCIDMAC.h
 *
 * \author Juver
 * \date 2020/03/27
 *
 * 
 */

#ifndef RPCIDMAC_H_INCLUDED__
#define RPCIDMAC_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RPCID
{
	std::string getWindowsMAC();
	std::string getHashWindowsMAC();
};

#endif // RPCIDMAC_H_INCLUDED__
