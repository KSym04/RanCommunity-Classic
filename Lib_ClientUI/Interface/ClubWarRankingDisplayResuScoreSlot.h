/*!
 * \file ClubWarRankingDisplayResuScoreSlot.h
 *
 * \author RSiLENT
 * \date January 22/1/2023 11:15AM
 *
 * 
 */

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_Client/G-Logic/GLCharClient.h"

#include "../Lib_Client/G-Logic/GLCWDefine.h"
struct	GLCHARLOGIC;
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLCharData.h"
class GLCLUB;
class CBasicLineBox;

class CBasicTextBox;

class CClubWarRankingDisplayResuScoreSlot : public CUIGroup
{
	enum
	{
		DUMMY = NO_ID + 1,
		GUILD_DUMMY,
		SCHOOL_DUMMY,
		CLASS_DUMMY
	};

	enum
	{
		RANK_INFO_ICON_SCHOOL = 3,
	};

public:
	CClubWarRankingDisplayResuScoreSlot ();
	virtual	~CClubWarRankingDisplayResuScoreSlot ();

	CUIControl*	CreateControl ( const char* szControl );

public:
	void	CreateSubControl ();

	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual HRESULT InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

public:
	CBasicTextBox*		m_pResuRankTextSlot;
	CBasicTextBox*		m_pResuNameTextSlot;
	CBasicTextBox*		m_pResuScoreTextSlot;

	CUIControl*			m_pGuildImage;
	CUIControl*			m_pResuIcon;
	CBasicLineBox*		m_pLineBox;
	CBasicLineBox*		m_pLineBoxWhite;
	CBasicLineBox*		m_pLineBoxRankWhite;
	CBasicLineBox*		m_pLineBoxCharacterDetailsWhite;
	CBasicLineBox*		m_pLineBoxCharacterNameWhite;


	CUIControl*			m_pClassImage;
	CUIControl*			m_pSchoolIcon[RANK_INFO_ICON_SCHOOL];

public:
	void	ResetData();
	void	SetData( int nRank, std::string strName, WORD wResuScore, WORD wSchool, int nChaClass, WORD wGuNum, WORD wGuMarkVer );

};