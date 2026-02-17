/*!
 * \file ClubWarRankingDisplaySlot.h
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

class CClubWarRankingDisplaySlot : public CUIGroup
{
	enum
	{
		//DUMMY = NO_ID + 1,
		GUILD_DUMMY,
	};


public:
	CClubWarRankingDisplaySlot ();
	virtual	~CClubWarRankingDisplaySlot ();
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
	CBasicTextBox*		m_pClubRankTextSlot;
	CBasicTextBox*		m_pClubNameTextSlot;
	CBasicTextBox*		m_pClubScoreTextSlotKill;
	CBasicTextBox*		m_pClubScoreTextSlotDeath;

	CUIControl*			m_pGuildImage;
	CUIControl*			m_pKillIcon;
	CUIControl*			m_pDeathIcon;
	CBasicLineBox*		m_pLineBox;
	CBasicLineBox*		m_pLineBoxWhite;
	CBasicLineBox*		m_pLineBoxRankWhite;
	CBasicLineBox*		m_pLineBoxCharacterDetailsWhite;
	CBasicLineBox*		m_pLineBoxCharacterNameWhite;


public:
	void	ResetData();
	void	SetData( int nRank, std::string strClub, WORD wKill, WORD wDeath, WORD wGuNum, WORD wGuMarkVer );

};