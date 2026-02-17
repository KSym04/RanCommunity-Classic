#include "stdafx.h"
#include "PVPWoePageHistory.h"

#include "../Lib_Engine/DxCommon/D3DFont.h"
#include "GameTextControl.h"
#include "UITextControl.h"

#include "BasicTextBox.h"
#include "BasicLineBox.h"
#include "BasicButton.h"

#include "../Lib_Client/G-Logic/GLPVPWoeClient.h"
#include "InnerInterface.h"
#include "../Lib_Client/DxGlobalStage.h"

#include "../Lib_Engine/DxCommon/DxClubMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPWoePageHistory::CPVPWoePageHistory ()
	: m_pTextHistoryTitleStatic(NULL)
	, m_pTextHistoryDurationStatic(NULL)
	, m_pTextHistoryNameStatic(NULL)
	, m_pTextHistoryBadgeStatic(NULL)
	, m_pTextHistoryLeaderStatic(NULL)
	, m_pTextHistoryTimeText(NULL)
	, m_pTextHistoryNameText(0)
	, m_pTextHistoryLeaderText(0)
	, m_fUpdateTime(0.0f)
	, m_pClubMark(0)
{
}

CPVPWoePageHistory::~CPVPWoePageHistory ()
{
}

void CPVPWoePageHistory::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "COMPETITION_WOE_PAGE_HISTORY_LINBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "COMPETITION_WOE_PAGE_HISTORY_LINBOX" );
	RegisterControl ( pLineBox );
	
	m_pTextHistoryTitleStatic = CreateUITextBox( "COMPETITION_WOE_PAGE_HISTORY_TITLE_STATICTEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextHistoryDurationStatic = CreateUITextBox( "COMPETITION_WOE_PAGE_HISTORY_DURATION_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHistoryNameStatic = CreateUITextBox( "COMPETITION_WOE_PAGE_HISTORY_NAME_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHistoryBadgeStatic = CreateUITextBox( "COMPETITION_WOE_PAGE_HISTORY_BADGE_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHistoryLeaderStatic = CreateUITextBox( "COMPETITION_WOE_PAGE_HISTORY_LEADER_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pTextHistoryTitleStatic->AddText( ID2GAMEWORD( "COMPETITION_WOE_HISTORY", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextHistoryDurationStatic->AddText( ID2GAMEWORD( "COMPETITION_WOE_HISTORY", 1 ), NS_UITEXTCOLOR::WHITE );
	m_pTextHistoryNameStatic->AddText( ID2GAMEWORD( "COMPETITION_WOE_HISTORY", 2 ), NS_UITEXTCOLOR::WHITE );
	m_pTextHistoryBadgeStatic->AddText( ID2GAMEWORD( "COMPETITION_WOE_HISTORY", 3 ), NS_UITEXTCOLOR::WHITE );
	m_pTextHistoryLeaderStatic->AddText( ID2GAMEWORD( "COMPETITION_WOE_HISTORY", 4 ), NS_UITEXTCOLOR::WHITE );

	m_pTextHistoryTimeText = CreateUITextBox( "COMPETITION_WOE_PAGE_HISTORY_TIME", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHistoryNameText = CreateUITextBox("COMPETITION_WOE_PAGE_HISTORY_NAME", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	m_pTextHistoryLeaderText = CreateUITextBox("COMPETITION_WOE_PAGE_HISTORY_LEADER", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);

	m_pClubMark = new CUIControl;
	m_pClubMark->CreateSub(this, "COMPETITION_WOE_PAGE_CLUB_MARK");
	RegisterControl(m_pClubMark);
}

CBasicTextBox* CPVPWoePageHistory::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

CUIControl* CPVPWoePageHistory::CreateUIControl( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	pControl->SetVisibleSingle( TRUE );
	RegisterControl ( pControl );
	return pControl;
}

void CPVPWoePageHistory::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CPVPWoePageHistory::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f )
	{
		ShowInfo();
		m_fUpdateTime = 0.0f;
	}
}

void CPVPWoePageHistory::ShowInfo()
{
	WOE_SCHED_NEXT& sNextSched = GLPVPWoeClient::GetInstance().m_sScheduleNext;

	m_strName = GLPVPWoeClient::GetInstance().strName;
	m_strLeader = GLPVPWoeClient::GetInstance().strLeader;

	m_dwClubID = GLPVPWoeClient::GetInstance().m_dwEmperiumGuildPrev;
	m_dwMarkVer = GLPVPWoeClient::GetInstance().m_dwEmperiumMarkVerPrev;

	if ( m_pTextHistoryTimeText )
	{
		m_pTextHistoryTimeText->ClearText();
		if ( sNextSched.dwIndex != UINT_MAX )
		{
			CString strCombine;
			strCombine.Format ( "%02d:%02d~%02d:%02d", sNextSched.wPrevStartHour, sNextSched.wPrevStartMinute, sNextSched.wPrevEndHour, sNextSched.wPrevEndMinute );
			m_pTextHistoryTimeText->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE  );
		}
		else
		{
			m_pTextHistoryTimeText->AddText( "00:00~00:00", NS_UITEXTCOLOR::WHITE  );
		}
	}

	if (m_pTextHistoryNameText)
	{
		m_pTextHistoryNameText->ClearText();
		m_pTextHistoryNameText->AddText(m_strName.c_str(), NS_UITEXTCOLOR::WHITE);
	}

	if (m_pTextHistoryLeaderText)
	{
		m_pTextHistoryLeaderText->ClearText();
		m_pTextHistoryLeaderText->AddText(m_strLeader.c_str(), NS_UITEXTCOLOR::WHITE);
	}
}

HRESULT CPVPWoePageHistory::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects(pd3dDevice);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CPVPWoePageHistory::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	HRESULT hr = S_OK;

	m_pClubMark->SetVisibleSingle(FALSE);

	{
		hr = CUIGroup::Render(pd3dDevice);
		if (FAILED(hr)) return hr;
	}

	BOOL bGuild = m_dwClubID ? TRUE : FALSE;
	if (bGuild && m_dwClubID > 0)
	{
		m_pClubMark->SetVisibleSingle(TRUE);
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_dwClubID, m_dwMarkVer);

		m_pClubMark->SetTexturePos(0, sMarkData.vTex_1_LU);
		m_pClubMark->SetTexturePos(1, sMarkData.vTex_2_RU);
		m_pClubMark->SetTexturePos(3, sMarkData.vTex_3_LD);
		m_pClubMark->SetTexturePos(2, sMarkData.vTex_4_RD);
		m_pClubMark->SetTexture(sMarkData.pddsTexture);
		m_pClubMark->Render(pd3dDevice);
	}
	return hr;
}
