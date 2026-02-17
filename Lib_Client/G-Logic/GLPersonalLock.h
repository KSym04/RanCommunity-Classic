/*!
 * \file GLPersonalLock.h
 *
 * \author Juver
 * \date 2019/11/27
 *
 * 
 */

#ifndef GLPERSONALLOCK_H_INCLUDED__
#define GLPERSONALLOCK_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define PERSONAL_LOCK_PIN_SIZE 4

#define PERSONAL_LOCK_RETRY_MAXED_DISCONNECT_TIME 5.0f

enum EMPERSONAL_LOCK
{
	EMPERSONAL_LOCK_EQUIP	= 0,
	EMPERSONAL_LOCK_INVEN	= 1,
	EMPERSONAL_LOCK_LOCKER	= 2,
	EMPERSONAL_LOCK_SIZE	= 3,
};


struct SPERSONAL_LOCK
{
	bool bLock;
	char szPin[PERSONAL_LOCK_PIN_SIZE+1];
	WORD wErrorCount;

	SPERSONAL_LOCK()
		: bLock(false)
		, wErrorCount(0)
	{
		memset (szPin, 0, sizeof(szPin));
	}

	SPERSONAL_LOCK ( const SPERSONAL_LOCK &value )
	{
		operator=(value);
	}

	SPERSONAL_LOCK& operator = ( const SPERSONAL_LOCK& rvalue )
	{
		bLock		= rvalue.bLock;
		wErrorCount = rvalue.wErrorCount;

		StringCchCopy( szPin, PERSONAL_LOCK_PIN_SIZE+1, rvalue.szPin );

		return *this;
	}

	void DeletePinInFo();
};

struct SPERSONAL_LOCK_PIN
{
	char szPin[PERSONAL_LOCK_PIN_SIZE+1];

	SPERSONAL_LOCK_PIN()
	{
		memset (szPin, 0, sizeof(szPin));
	}
};

#endif // GLPERSONALLOCK_H_INCLUDED__
