#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_Client/G-Logic/GLPVPWoeData.h"

class	CBasicProgressBar;

class CPVPWoeTowerProgress : public CUIGroup
{
private:
	enum
	{
	};

public:
	CPVPWoeTowerProgress ();
	virtual	~CPVPWoeTowerProgress ();

public:
	void	CreateSubControl ();

public:
	void	UpdateInfo( WORD wOwner, float* fDamage );

}; 