#pragma	once

#include "./UIWindow.h"

class	CBasicTextBox;
class	CBasicProgressBar;

class CPVPClubDeathMatchRebirthDialogue : public CUIWindow
{
private:
	enum
	{
	};	

public:
	CPVPClubDeathMatchRebirthDialogue ();
	virtual	~CPVPClubDeathMatchRebirthDialogue ();

public:
	void	CreateSubControl ();
	
public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	CBasicTextBox*	m_pTextDesc;
	CBasicProgressBar*	m_pTimer;

public:
	__time64_t	timeStart;
};