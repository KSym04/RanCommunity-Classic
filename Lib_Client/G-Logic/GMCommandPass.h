/*!
 * \file GMCommandPass.h
 *
 * \author Juver
 * \date 2019/11/24
 *
 * 
 */

#ifndef GMCOMMANDPASS_H_INCLUDED__
#define GMCOMMANDPASS_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGMCommandPass
{
public:
	std::string strPassGetItem;
	std::string strPassGetInven;
	std::string strPassGetSkill;
	std::string strPassGetLevel;
	std::string strPassGetStats;
	std::string strPassGetStatsP;
	std::string strPassGetSkillP;
	std::string strPassSendItem;
	std::string strPassGetCrowTime;
	std::string strPassEventContriTyranny;
	std::string strPassEventContriSchoolWars;
	std::string strPassEventContriCaptureTheFlag;
	std::string strPassKickOut;
	std::string strPassPCID;

	/* user flag restricted, Juver, 2020/04/21 */
	std::string strPassUserRestrict;

	/* gm command inven clear, Juver, 2020/05/09 */
	std::string strPassInvenClear;

	/* game notice, Juver, 2021/06/12 */
	std::string strPassGameNoticeReload;

	std::string strPassGISReload;

public:
	void Reset();
	bool LoadFile();

public:
	CGMCommandPass();
	~CGMCommandPass();

public:
	static CGMCommandPass& GetInstance ();
};


#endif // GMCOMMANDPASS_H_INCLUDED__
