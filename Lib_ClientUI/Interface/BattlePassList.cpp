#include "StdAfx.h"
#include "./BattlePassList.h"
#include "./BattlePassListSlot.h"

#include "./BasicLineBox.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicTextBox.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLBattlePass.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

CBattlePassList::CBattlePassList ()	
	: m_pTextMission (NULL)
{
	for( int i=0; i<BATTLEPASS_LIST_SLOT_MAX; ++i )
		m_pSlot[i] = NULL;

	m_vecData.clear();
}

CBattlePassList::~CBattlePassList ()
{
}

void CBattlePassList::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	std::string strslot[BATTLEPASS_LIST_SLOT_MAX] = 
	{
		"RAN_BATTLEPASS_MISSION_LIST_SLOT0",
		"RAN_BATTLEPASS_MISSION_LIST_SLOT1",
		"RAN_BATTLEPASS_MISSION_LIST_SLOT2",
		"RAN_BATTLEPASS_MISSION_LIST_SLOT3",
		"RAN_BATTLEPASS_MISSION_LIST_SLOT4",
		"RAN_BATTLEPASS_MISSION_LIST_SLOT5",
		"RAN_BATTLEPASS_MISSION_LIST_SLOT6",
		"RAN_BATTLEPASS_MISSION_LIST_SLOT7",
	};

	for( int i=0; i<BATTLEPASS_LIST_SLOT_MAX; ++i )
	{
		m_pSlot[i] = new CBattlePassListSlot;
		m_pSlot[i]->CreateSub( this, strslot[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE, BATTLEPASS_LIST_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		m_pSlot[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pSlot[i] );
	}

	m_pTextMission = new CBasicTextBox;
	m_pTextMission->CreateSub ( this, "RAN_BATTLEPASS_MISSION_STATIC_TEXT", UI_FLAG_DEFAULT );
	m_pTextMission->SetFont ( pFont9 );
	m_pTextMission->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextMission );
}

void CBattlePassList::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CBattlePassList::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
    CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
    
    LoadData();
}

void CBattlePassList::ResetData()
{
	m_pTextMission->ClearText();

	m_vecData.clear();

	for( int i=0; i<BATTLEPASS_LIST_SLOT_MAX; ++i )
	{
		if ( m_pSlot[i] )
		{
			m_pSlot[i]->ResetData();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}
}

void CBattlePassList::LoadData()
{
	ResetData();

	CString strText;
	strText.Format("%s", ID2GAMEINTEXT("BATTLEPASS_MISSION_TEXT"));
	m_pTextMission->SetText(strText.GetString(), NS_UITEXTCOLOR::WHITE);

	SBATTLEPASS_FILE_DATA_MAP map_battlepass = GLBattlePass::GetInstance().m_mapBattlePass;
	for( SBATTLEPASS_FILE_DATA_MAP_ITER iter = map_battlepass.begin(); 
		iter != map_battlepass.end(); ++ iter )
	{
		SBATTLEPASS_FILE_DATA &sbattlepass_data = (*iter).second;

		SBATTLEPASS_CHAR_DATA* pchar_battlepass_data_done = GLGaeaClient::GetInstance().GetCharacter()->GetBattlePassDone(sbattlepass_data.dwBattlePassID);
		SBATTLEPASS_CHAR_DATA* pchar_battlepass_data_prog = GLGaeaClient::GetInstance().GetCharacter()->GetBattlePassProg(sbattlepass_data.dwBattlePassID);

		if (!pchar_battlepass_data_done && !pchar_battlepass_data_prog)
		{
			continue;
		}

		m_vecData.push_back( sbattlepass_data );
		continue;
	}

	std::sort( m_vecData.begin(), m_vecData.end() );

	ShowData();
}

void CBattlePassList::ShowData()
{
    for ( int i=0; i < BATTLEPASS_LIST_SLOT_MAX; ++ i )
    {
        if ( m_pSlot[i] )
        {
            m_pSlot[i]->ResetData();
            m_pSlot[i]->SetVisibleSingle( FALSE );
        }
    }

    for ( int i=0; i < BATTLEPASS_LIST_SLOT_MAX; ++ i )
    {
        if ( m_pSlot[i] )
        {
            if ( i >= (int)m_vecData.size() )    continue; //add this code

            m_pSlot[i]->SetVisibleSingle( TRUE );
            m_pSlot[i]->SetData( m_vecData[i].dwBattlePassID );
        }
    }
}