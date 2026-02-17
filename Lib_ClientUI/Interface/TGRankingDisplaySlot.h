#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "../../Lib_Client/G-Logic/GLPVPTyrannyDefine.h"

class CBasicTextBox;

class CTGRankingDisplaySlot : public CUIGroup
{
private:
	enum
	{
	};

public:
	CTGRankingDisplaySlot ();
	virtual	~CTGRankingDisplaySlot ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBox*	m_pTextRank;
	CBasicTextBox*	m_pTextClub;
	CBasicTextBox*	m_pTextScore;


public:
	void	ResetData();
	void	SetData( int nRank, std::string strClub, WORD wKill, WORD wDeath );

};