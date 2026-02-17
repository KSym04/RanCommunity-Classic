#pragma	once

#include "./UIWindowEx.h"

class	CBasicTextButton;
class	CBasicVideoOption;
class	CBasicAudioOption;
class	CBasicGamePlayOption;

//#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
//class	CBasicCharViewOptionEP6;
//#else
//class	CBasicCharViewOptionEP9;
//#endif 


class CBasicHWOptionWindow : public CUIWindowEx
{
protected:
	enum
	{
		HWOPTION_VIDEO_BUTTON = ET_CONTROL_NEXT,
		HWOPTION_AUDIO_BUTTON,
		HWOPTION_GAMEPLAY_BUTTON,
		//HWOPTION_CHARVIEW_BUTTON,

		HWOPTION_VIDEO_PAGE,
		HWOPTION_AUDIO_PAGE,
        HWOPTION_GAMEPLAY_PAGE,
		//HWOPTION_CHARVIEW_PAGE,

		HWOPTION_APPLY,
		HWOPTION_OK
	};

public:
	CBasicHWOptionWindow ();
	virtual	~CBasicHWOptionWindow ();

public:
	void	CreateSubControl ();

protected:
	void	SetVisiblePage ( int nPage );
	void	GamePlayOption_OK();
	void	AudioOption_OK();
	void	VideoOption_OK();
	//void	CharViewOption_OK();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	CBasicTextButton*		m_pVideoButton;
	CBasicTextButton*		m_pAudioButton;
	CBasicTextButton*		m_pGamePlayButton;
	//CBasicTextButton*		m_pCharViewButton;

private:
	CBasicVideoOption*		m_pVideoOption;
	CBasicAudioOption*		m_pAudioOption;
	CBasicGamePlayOption*	m_pGamePlayOption;

//#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
//	CBasicCharViewOptionEP6*		m_pCharViewOption;
//#else
//	CBasicCharViewOptionEP9*		m_pCharViewOption;
//#endif


	bool					m_bFirstSetting;
};