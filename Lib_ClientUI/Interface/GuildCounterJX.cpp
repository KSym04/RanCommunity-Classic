#include "StdAfx.h"
#include "GuildCounterJX.h"
#include "../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../Lib_ClientUI/Interface/BasicTextBoxEx.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/DxCommon/DxClubMan.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"

#include "../Lib_Client/G-Logic/GLGaeaClient.h"


CJXGuildCounter::CJXGuildCounter ()
	: m_pTextGuild(NULL)
	, m_pTextOnline(NULL)
	, m_pTextOnMap(NULL)
	, m_pTextClub(NULL)
{
}

CJXGuildCounter::~CJXGuildCounter ()
{
}

void CJXGuildCounter::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_SHADOW_FLAG || _DEFAULT_FONT_FLAG );

	CBasicLineBox* pBasicLineBoxHeaders = new CBasicLineBox;
	pBasicLineBoxHeaders->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxHeaders->CreateBaseBoxQuestList ( "GUILD_COUNTER_HEADER_LINE_BLACK" );
	RegisterControl ( pBasicLineBoxHeaders );

	CBasicLineBox* pBasicLineBoxBody = new CBasicLineBox;
	pBasicLineBoxBody->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxBody->CreateBaseBoxQuestList ( "GUILD_COUNTER_BODY_LINE_BLACK" );
	RegisterControl ( pBasicLineBoxBody );

	m_pTextOnline = new CBasicTextBox;
	m_pTextOnline->CreateSub ( this, "GUILD_COUNTER_ONLINE" );
	m_pTextOnline->SetFont ( pFont10 );
	m_pTextOnline->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextOnline->SetText( "Online" );
	RegisterControl ( m_pTextOnline );

	m_pTextGuild = new CBasicTextBox;
	m_pTextGuild->CreateSub ( this, "GUILD_COUNTER_GUILDNAME" );
	m_pTextGuild->SetFont ( pFont10 );
	m_pTextGuild->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextGuild->SetText( "Guild" );
	RegisterControl ( m_pTextGuild );

	m_pTextOnMap = new CBasicTextBox;
	m_pTextOnMap->CreateSub ( this, "GUILD_COUNTER_ONMAP" );
	m_pTextOnMap->SetFont ( pFont10 );
	m_pTextOnMap->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextOnMap->SetText( "On Map" );
	RegisterControl ( m_pTextOnMap );

	m_pTextClub = new CBasicTextBox;
	m_pTextClub->CreateSub ( this, "GUILD" );
	m_pTextClub->SetFont ( pFont9 );
	m_pTextClub->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextClub->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextClub );
}

