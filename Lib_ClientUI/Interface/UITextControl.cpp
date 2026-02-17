#include "StdAfx.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace	NS_UITEXTCONTROL
{
	DWORD	GetEvaluateColor ( BOOL bValue )
	{
		if ( bValue )	return ENABLE;
		else			return DISABLE;
	}

	DWORD	GetEvaluateColorEx ( BOOL bValue )
	{
		if ( bValue )	return WHITE;
		else			return DISABLE;
	}

	DWORD GetTextColor_Compare( int nVal )
	{
		if ( nVal == 0 )	return NS_UITEXTCOLOR::WHITE;
		if ( nVal > 0 )		return NS_UITEXTCOLOR::BRIGHTGREEN;
		if ( nVal < 0 )		return NS_UITEXTCOLOR::RED;

		return NS_UITEXTCOLOR::WHITE;
	}

	DWORD GetTextColor_Compare( float fOrig, float fToCheck )
	{
		if ( fOrig == fToCheck )	return NS_UITEXTCOLOR::WHITE;
		if ( fToCheck > fOrig )		return NS_UITEXTCOLOR::BRIGHTGREEN;
		if ( fToCheck < fOrig )		return NS_UITEXTCOLOR::RED;

		return NS_UITEXTCOLOR::WHITE;
	}

	DWORD GetTextColor_Compare( int nOrig, int nToCheck )
	{
		if ( nOrig == nToCheck )	return NS_UITEXTCOLOR::WHITE;
		if ( nToCheck > nOrig )		return NS_UITEXTCOLOR::BRIGHTGREEN;
		if ( nToCheck < nOrig )		return NS_UITEXTCOLOR::RED;

		return NS_UITEXTCOLOR::WHITE;
	}
};