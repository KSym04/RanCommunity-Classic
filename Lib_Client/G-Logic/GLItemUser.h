/*!
 * \file GLItemUser.h
 *
 * \author Juver
 * \date May 2018
 *
 * 
 */

#ifndef GLITEM_USER_H_INCLUDED_
#define GLITEM_USER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif


#if _MSC_VER >= 1600
#include <unordered_set>
#include <unordered_map>
#else
#include <set>
#include <map>
#endif

class GLItemUser
{
private:

#if _MSC_VER >= 1600
	typedef std::unordered_set<DWORD>		SET_ITEM_USER;
	typedef SET_ITEM_USER::iterator			SET_ITEM_USER_ITER;
	typedef std::unordered_map<DWORD, SET_ITEM_USER > MAP_ITEM_USER;
	typedef MAP_ITEM_USER::iterator			MAP_ITEM_USER_ITER;
#else
	typedef std::set<DWORD>					SET_ITEM_USER;
	typedef SET_ITEM_USER::iterator			SET_ITEM_USER_ITER;
	typedef std::map<DWORD, SET_ITEM_USER > MAP_ITEM_USER;
	typedef MAP_ITEM_USER::iterator			MAP_ITEM_USER_ITER;
#endif


private:
	std::string			m_strPath;
	MAP_ITEM_USER		m_mapItemUser;
	MAP_ITEM_USER		m_mapItemChar;
	MAP_ITEM_USER		m_mapItemClub;

public:
	void	LoadFile();
	void	CleanUp();

	BOOL	CharTest( DWORD item_id, DWORD user_id, DWORD char_id, DWORD club_id );

private:
	void	LoadUser();
	void	LoadChar();
	void	LoadClub();

	BOOL	CharTestUserID( DWORD item_id, DWORD user_id );
	BOOL	CharTestCharID( DWORD item_id, DWORD char_id );
	BOOL	CharTestClubID( DWORD item_id, DWORD club_id );

public:
	GLItemUser ();
	~GLItemUser ();

public:
	static GLItemUser& GetInstance();
};



#endif // GLITEM_USER_H_INCLUDED_