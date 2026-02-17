#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class	CPVPClubDeathMatchDoublePointImage;
class	CPVPClubDeathMatchDoublePointDisplay : public CUIGroup
{
public:
	enum
	{
		PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_IMAGE_00 = NO_ID + 1,

		PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_MAX_TYPE = 1
	};

public:
	CPVPClubDeathMatchDoublePointDisplay ();
	virtual	~CPVPClubDeathMatchDoublePointDisplay ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	bool	START ( UIGUID cID );
	bool	RESET ( UIGUID cID );
	bool	STOP ( UIGUID cID );

	void	ALLSTOP ();

public:
	bool	KEEP_START ( UIGUID cID );
	void	KEEP_STOP ();

private:
	CPVPClubDeathMatchDoublePointImage*	m_pQI_TYPE[PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_MAX_TYPE];
	CUIControl*							m_QI_TYPE_KEEP[PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_MAX_TYPE];
	CUIControl*							m_pPositionControl;	
};