/*!
 * \file s_CIPFilter.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */


#pragma once

#include "WinSock2.h"
#include <set>

#define IP_FILTER_MAX_IP_LENGTH  20  //MAX_IP_LENGTH
	
class CServer;

class CIPFilter
{
public:
	static CIPFilter* GetInstance();

private:
	static CIPFilter* SelfInstance;

private:
	typedef std::set<std::string>	SET_IP;
	typedef SET_IP::iterator		SET_IP_ITER;

private:
	CRITICAL_SECTION m_CSPCLock;
	ULONGLONG	m_llBlockRef;
	ULONGLONG	m_llKnownRef;
	SET_IP		m_setIPBlock;
	SET_IP		m_setIPKnown;

	/*dmk14 whitelist*/
	SET_IP		m_setWhiteListIP;
	ULONGLONG	m_llWhiteListRef;

public:
	CServer*			m_pServer;

public:
	void		SetServer( CServer* _pServer );

	BOOL		IsIPBlocked( SOCKET _sSocket );
	BOOL		IsIPBlocked( std::string _strIP );
	BOOL		IsIPKnown( std::string _strIP );
	BOOL		IsBlocked( std::string _strIP );

	ULONGLONG	GetRefBlocked()	{	return m_llBlockRef;	}
	ULONGLONG	GetRefKnown()	{	return m_llKnownRef;	}

	BOOL		AddIPBlock( std::string _strIP );
	BOOL		AddIPKnown( std::string _strIP );
	BOOL		RemoveIPBlock( std::string _strIP );
	BOOL		RemoveIPKnown( std::string _strIP );
	DWORD		GetIPBlockNum()	{	return (DWORD) m_setIPBlock.size();	}
	DWORD		GetIPKnownNum()	{	return (DWORD) m_setIPKnown.size();	}

	/*dmk14 whitelist*/
	BOOL		IsWhiteList( SOCKET _sSocket );
	BOOL		IsWhiteList( std::string _strIP );
	ULONGLONG	GetWhiteList()	{	return m_llWhiteListRef;	}
	void		AddWhiteList( std::string _strIP, bool _bBroadcast = false );
	void		RemoveWhiteList( std::string _strIP, bool _bBroadcast = false );

	DWORD		GetIPWhiteListNum()	{	return (DWORD) m_setWhiteListIP.size();	}
	SET_IP		GetWhiteListIP()		{ return m_setWhiteListIP; }

	void		ListLoad();
	void		ListSave();

	bool		IsIpAddress( const TCHAR* ipAddress );
public:
	CIPFilter();
	~CIPFilter();
};
