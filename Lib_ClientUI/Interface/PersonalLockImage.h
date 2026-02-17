#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CPersonalLockImage : public CUIGroup
{
private:
	enum
	{
	};

public:
	CPersonalLockImage ();
	virtual	~CPersonalLockImage ();

public:
	void	CreateSubControl ();

public:
	void	SetImageLock( BOOL bSet );
	void	SetImageHover( BOOL bSet );

private:
	CUIControl* m_pImageLock;
	CUIControl* m_pImageHover;

};