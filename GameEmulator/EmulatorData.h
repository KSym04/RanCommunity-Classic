

#pragma once


#include "../Lib_Client/G-Logic/VipDefine.h"

//character Setting
struct SCharacterSetting
{
	std::string strCharSet;
	std::string strCharName;
	DWORD dwUserLevelIndex;

	WORD wLevel;
	WORD wStats;
	bool bOpenSkills;
	bool bOpenInventory;
	LONGLONG llMoney;

	DWORD dwActivityPoints;
	DWORD dwContributionPoints;
	DWORD dwWarChips;
	DWORD dwGamePoints;

	/* user flag verified, Juver, 2020/02/25 */
	bool bUserVerified;

	/* user flag restricted, Juver, 2020/04/20 */
	bool bUserRestricted;

	/* play time system, Juver, 2021/01/26 */
	LONGLONG llChaPlayTime;
	LONGLONG llChaPlayPoint;

	/* reborn system, Juver, 2021/09/17 */
	DWORD	dwReborn;

	WORD	wSkillPoints;

	WORD	wStatPoints;

	EMVIP_LEVEL	emVIP;

	WORD	wExchangeItemPoints;

	SCharacterSetting()
		: strCharSet("class00.charset")
		, strCharName("Marckies")
		, dwUserLevelIndex(0)
		, wLevel(1)
		, wStats(0)
		, bOpenSkills(false)
		, bOpenInventory(false)
		, llMoney(0)
		, dwActivityPoints(0)
		, dwContributionPoints(0)
		, dwWarChips(0)
		, dwGamePoints(0)

		/* user flag verified, Juver, 2020/02/25 */
		, bUserVerified(false)

		/* user flag restricted, Juver, 2020/04/20 */
		, bUserRestricted(false)

		/* play time system, Juver, 2021/01/26 */
		, llChaPlayTime(0)
		, llChaPlayPoint(0)

		/* reborn system, Juver, 2021/09/17 */
		, dwReborn(0)

		, wSkillPoints(0)

		, wStatPoints(0)

		, emVIP(EMVIP_LEVEL_NONE)
		, wExchangeItemPoints(0)
	{

	}

	void Assign( const SCharacterSetting& rValue )
	{
		strCharSet			= rValue.strCharSet;
		strCharName			= rValue.strCharName;
		dwUserLevelIndex	= rValue.dwUserLevelIndex;
		wLevel				= rValue.wLevel;
		wStats				= rValue.wStats;
		bOpenSkills			= rValue.bOpenSkills;
		bOpenInventory		= rValue.bOpenInventory;
		llMoney				= rValue.llMoney;

		dwActivityPoints		= rValue.dwActivityPoints;
		dwContributionPoints	= rValue.dwContributionPoints;
		dwWarChips				= rValue.dwWarChips;
		dwGamePoints			= rValue.dwGamePoints;

		/* user flag verified, Juver, 2020/02/25 */
		bUserVerified			= rValue.bUserVerified;

		/* user flag restricted, Juver, 2020/04/20 */
		bUserRestricted			= rValue.bUserRestricted;

		/* play time system, Juver, 2021/01/26 */
		llChaPlayTime			= rValue.llChaPlayTime;
		llChaPlayPoint			= rValue.llChaPlayPoint;

		/* reborn system, Juver, 2021/09/17 */
		dwReborn					= rValue.dwReborn;

		wSkillPoints			= rValue.wSkillPoints;

		wStatPoints				= rValue.wStatPoints;

		emVIP					= rValue.emVIP;
		wExchangeItemPoints		= rValue.wExchangeItemPoints;
	};

	void Reset()
	{
		*this = SCharacterSetting();
	};
};