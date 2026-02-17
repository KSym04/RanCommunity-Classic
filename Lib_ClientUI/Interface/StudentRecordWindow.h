/*!
 * \file StudentRecordWindow.h
 *
 * \author Juver
 * \date October 2017
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"

class CMultiModeButton;

/* play stats window, Juver, 2021/01/26 */
class CPlayStatsPage;

/*activity system, Juver, 2017/10/30 */
class CActivityPage; 
class CCodexPage;


/*12-16-24, daily login - CNDev*/
class CDailyLoginPage;
/*12-9-14, Battle Pass - CNDev*/
class CBattlePassPage;
class CStudentRecordWindow: public CUIWindowEx
{
public:
	enum
	{
		STUDENT_RECORD_WINDOW_BUTTON_PAGE_PLAY_STATS = ET_CONTROL_NEXT,
		STUDENT_RECORD_WINDOW_BUTTON_PAGE_ACTIVITY,
		STUDENT_RECORD_WINDOW_BUTTON_PAGE_DAILYLOGIN,
		STUDENT_RECORD_WINDOW_BUTTON_PAGE_CODEX,
		STUDENT_RECORD_WINDOW_BUTTON_PAGE_BATTLEPASS,
		
		STUDENT_RECORD_WINDOW_PAGE_PLAY_STATS,
		STUDENT_RECORD_WINDOW_PAGE_ACTIVITY,
		STUDENT_RECORD_WINDOW_PAGE_DAILYLOGIN,
		STUDENT_RECORD_WINDOW_PAGE_CODEX,
		STUDENT_RECORD_WINDOW_PAGE_BATTLEPASS,
	};


	enum
	{
		PAGE_PLAY_STATS		= 0,		/* play stats window, Juver, 2021/01/26 */
		PAGE_ACTIVITY		= 1,		/*activity system, Juver, 2017/10/30 */
		PAGE_DAILYLOGIN		= 2,		/*activity system, Juver, 2017/10/30 */
		PAGE_CODEX			= 3,
		PAGE_BATTLEPASS		= 4,		/*activity system, Juver, 2017/10/30 */
		
	};

public:
	CStudentRecordWindow ();
	virtual	~CStudentRecordWindow ();

public:
	CMultiModeButton*		m_pButtonPagePlayStats;	/* play stats window, Juver, 2021/01/26 */
	CMultiModeButton*		m_pButtonPageActivity;	/*activity system, Juver, 2017/10/30 */
	CMultiModeButton*		m_pButtonPageDailyLogin;	/*activity system, Juver, 2017/10/30 */
	CMultiModeButton*		m_pButtonPageCodex;
	CMultiModeButton*		m_pButtonPageBattlepass;	/*activity system, Juver, 2017/10/30 */

	CPlayStatsPage*			m_pPagePlayStats;		/* play stats window, Juver, 2021/01/26 */
	CActivityPage*			m_pPageActivity;		/*activity system, Juver, 2017/10/30 */
	CDailyLoginPage*		m_pPageDailyLogin;		/*12-16-24, daily login - CNDev*/
	CCodexPage*				m_pPageCodex;
	CBattlePassPage*		m_pPageBattlePass;		/*12-9-14, Battle Pass - CNDev*/

public:
	void	CreateSubControl ();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	int		m_nPage;

public:
	void	OpenPage( int nPage );

	CPlayStatsPage* getPlayStatsPage() { return m_pPagePlayStats; }
};