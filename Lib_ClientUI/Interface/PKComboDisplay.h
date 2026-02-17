#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class	CPKComboImageType;

class CPKComboDisplay : public CUIGroup
{
public:
	CPKComboDisplay ();
	virtual	~CPKComboDisplay ();
public:
	enum
	{
		PK_COMBO_IMAGE_00 = NO_ID + 1,
		PK_COMBO_IMAGE_01,
		PK_COMBO_IMAGE_02,
		PK_COMBO_IMAGE_03,
		PK_COMBO_IMAGE_04,
		PK_COMBO_IMAGE_05,
		PK_COMBO_IMAGE_06,
		PK_COMBO_IMAGE_07,
		PK_COMBO_IMAGE_08,
		PK_COMBO_IMAGE_09,
		PK_COMBO_IMAGE_10,
		PK_COMBO_IMAGE_11,
		PK_COMBO_IMAGE_12,
		PK_COMBO_IMAGE_13,
		PK_COMBO_IMAGE_14,
		PK_COMBO_IMAGE_15,
		PK_COMBO_IMAGE_16,
		PK_COMBO_IMAGE_17,
		PK_COMBO_IMAGE_18,
		PK_COMBO_IMAGE_19,

		PK_COMBO_IMAGE_MAX = 20
	};
public:
	void	CreateSubControl ();

public:
	virtual void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
public:
	bool	START ( UIGUID cID );
	bool	RESET ( UIGUID cID );
	bool	STOP ( UIGUID cID );

	void	ALLSTOP ();

private:
	CPKComboImageType*	m_pTYPE[PK_COMBO_IMAGE_MAX];

public:	
	CUIControl*		m_pPositionControl;	
};