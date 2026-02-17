#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CBasicLineBox;
class CBasicLineBoxEx;
class CBasicTextBox;
class CBasicTextButton;
class CBasicScrollBarEx;

struct MULTI_MAP_MOVE_SELECT_DATA
{
	DWORD dwIndex;
	std::string strText;

	MULTI_MAP_MOVE_SELECT_DATA()
		: dwIndex(-1)
		, strText("")
	{

	}
};

typedef std::vector<MULTI_MAP_MOVE_SELECT_DATA> VEC_MULTI_MAP_MOVE_SELECT_DATA;

class CMultiMapMoveDisplay : public CUIGroup
{
private:
	enum
	{
		MULTIMAPMOVE_WINDOW_OK_BUTTON = NO_ID + 1,
		MULTIMAPMOVE_LIST_SCROLLBAR,
		MULTIMAPMOVE_LIST_SELECT_0,
		MULTIMAPMOVE_LIST_SELECT_1,
		MULTIMAPMOVE_LIST_SELECT_2,
		MULTIMAPMOVE_LIST_SELECT_3,
	};

	enum 
	{
		MAX_SELECTION = 4,
	};

public:
	CMultiMapMoveDisplay ();
	virtual	~CMultiMapMoveDisplay ();

public:
	void	CreateSubControl ();

public:
	void	DoMAPMOVE ();

	void	SetSelectionData();
	void	ResetSelectionData();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	VEC_MULTI_MAP_MOVE_SELECT_DATA m_vecData;

	CBasicLineBoxEx*	m_pLineBoxBody;
	CBasicLineBoxEx*	m_pLineBoxList;
	CBasicTextBox*		m_pTextBoxInfo;
	CBasicTextButton*	m_pButtonMoveOK;

	CBasicLineBoxEx*	m_pLineBoxSelection[MAX_SELECTION];
	CBasicTextBox*		m_pTextBoxSelection[MAX_SELECTION];
	CBasicScrollBarEx*	m_pScrollBarList;
	DWORD				m_dwSelectionIndex[MAX_SELECTION];
	DWORD				m_dwSelected;
};