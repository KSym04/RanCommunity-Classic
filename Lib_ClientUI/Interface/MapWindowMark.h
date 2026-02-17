#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class	CMapWindowMark : public CUIGroup
{
protected:


public:
	CMapWindowMark ();
	virtual	~CMapWindowMark ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	std::string strTEXT;

};