#include "StdAfx.h"
#include "./DailyLoginList.h"
#include "./DailyLoginListSlot.h"

#include "./BasicLineBox.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicTextBox.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLDailyLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-16-24, daily login - CNDev*/

CDailyLoginList::CDailyLoginList ()	
	: m_fUpdateTime(0.0f)
{
	for( int i=0; i<DAILYLOGIN_LIST_SLOT_MAX; ++i )
		m_pSlot[i] = NULL;

	m_vecData.clear();
}

CDailyLoginList::~CDailyLoginList ()
{
}

void CDailyLoginList::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	std::string strslot[DAILYLOGIN_LIST_SLOT_MAX] = 
	{
		"RAN_DAILYLOGIN_LIST_SLOT0",
		"RAN_DAILYLOGIN_LIST_SLOT1",
		"RAN_DAILYLOGIN_LIST_SLOT2",
		"RAN_DAILYLOGIN_LIST_SLOT3",
		"RAN_DAILYLOGIN_LIST_SLOT4",
		"RAN_DAILYLOGIN_LIST_SLOT5",
		"RAN_DAILYLOGIN_LIST_SLOT6",
		"RAN_DAILYLOGIN_LIST_SLOT7",
		"RAN_DAILYLOGIN_LIST_SLOT8",
		"RAN_DAILYLOGIN_LIST_SLOT9",
		"RAN_DAILYLOGIN_LIST_SLOT10",
		"RAN_DAILYLOGIN_LIST_SLOT11",
		"RAN_DAILYLOGIN_LIST_SLOT12",
		"RAN_DAILYLOGIN_LIST_SLOT13",
		"RAN_DAILYLOGIN_LIST_SLOT14",
		"RAN_DAILYLOGIN_LIST_SLOT15",
		"RAN_DAILYLOGIN_LIST_SLOT16",
		"RAN_DAILYLOGIN_LIST_SLOT17",
		"RAN_DAILYLOGIN_LIST_SLOT18",
		"RAN_DAILYLOGIN_LIST_SLOT19",
		"RAN_DAILYLOGIN_LIST_SLOT20",
		"RAN_DAILYLOGIN_LIST_SLOT21",
		"RAN_DAILYLOGIN_LIST_SLOT22",
		"RAN_DAILYLOGIN_LIST_SLOT23",
		"RAN_DAILYLOGIN_LIST_SLOT24",
		"RAN_DAILYLOGIN_LIST_SLOT25",
		"RAN_DAILYLOGIN_LIST_SLOT26",
		"RAN_DAILYLOGIN_LIST_SLOT27",
	};

	for( int i=0; i<DAILYLOGIN_LIST_SLOT_MAX; ++i )
	{
		m_pSlot[i] = new CDailyLoginListSlot;
		m_pSlot[i]->CreateSub( this, strslot[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE, DAILYLOGIN_LIST_SLOT_0 + i );
		m_pSlot[i]->CreateSubControl ();
		m_pSlot[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pSlot[i] );
	}
}

void CDailyLoginList::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CDailyLoginList::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 1.0f )
	{
		m_fUpdateTime = 0.0f;
		LoadData();
	}
}

void CDailyLoginList::ResetData()
{
	m_vecData.clear();

	for( int i=0; i<DAILYLOGIN_LIST_SLOT_MAX; ++i )
	{
		if ( m_pSlot[i] )
		{
			m_pSlot[i]->ResetData();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}
}

void CDailyLoginList::LoadData()
{
	ResetData();

	DAILYLOGIN_REWARD_MAP map_dailylogin = GLDailyLogin::GetInstance().m_mapDailyLoginReward;
	for( DAILYLOGIN_REWARD_MAP_ITER iter = map_dailylogin.begin(); 
		iter != map_dailylogin.end(); ++ iter )
	{
		SDAILYLOGIN_REWARD &sdailylogin_data = (*iter).second;

		m_vecData.push_back( sdailylogin_data );
		continue;
	}

	std::sort( m_vecData.begin(), m_vecData.end() );

	ShowData();
}

void CDailyLoginList::ShowData()
{
	for ( int i=0; i < DAILYLOGIN_LIST_SLOT_MAX; ++ i )
	{
		if ( m_pSlot[i] )
		{
			m_pSlot[i]->ResetData();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}

	for ( int i=0; i < DAILYLOGIN_LIST_SLOT_MAX; ++ i )
	{
		if ( m_pSlot[i] )
		{
		if ( i >= (int)m_vecData.size() )	continue;

			m_pSlot[i]->SetVisibleSingle( TRUE );
			m_pSlot[i]->SetData( m_vecData[i].dwDay, m_vecData[i].sRewardID );
			//i ++;
		}
	}
}