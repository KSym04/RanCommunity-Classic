#pragma	once

#include "UIWindowEx.h"

class	CBasicTextBox;

class CJXGuildCounter : public CUIWindowEx
{
public:
	CJXGuildCounter ();
	virtual	~CJXGuildCounter ();

public:
	void	CreateSubControl ();

	CBasicTextBox*		m_pTextOnline;
	CBasicTextBox*		m_pTextGuild;
	CBasicTextBox*		m_pTextOnMap;
	CBasicTextBox*		m_pTextClub;

};