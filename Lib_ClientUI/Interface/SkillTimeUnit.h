#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

struct	SNATIVEID;
class	CBasicProgressBar;
class	CSkillImage;

class	CSkillTimeUnit : public CUIGroup
{
private:
	enum
	{
		TIMEUNIT_IMAGE = NO_ID + 1,
		TIMEUNIT_PROGRESS,
	};

public:
	CSkillTimeUnit ();
	virtual	~CSkillTimeUnit ();

public:
	void	CreateSubControl ();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetLeftTime ( const float& fLeftTime );

	/*extended buff info, EJCode, 2018/10/06 */
	void	SetSkill ( const DWORD& dwID, const WORD wLevel, const float& fLifeTime, const CString& strSkillName );

	/*extended buff info, EJCode, 2018/10/06 */
	void	show_skill_info();

private:
	float	m_fLEFT_TIME;
	float	m_fLIFE_TIME;
	DWORD	m_dwSkillID;	/*buffs manual remove, Juver, 2018/08/13 */
	WORD	m_wLevel;		/*extended buff info, EJCode, 2018/10/06 */

private:
	CString				m_strSkillName;
	CBasicProgressBar*	m_pProgressBar;
	CSkillImage*		m_pSkillImage;
};