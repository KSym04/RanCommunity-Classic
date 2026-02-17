#include "stdafx.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLPKMark.h"

#include "./PKMarkSlot.h"
#include "./PKMarkDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPKMarkDisplay::CPKMarkDisplay()
{
	for( int i=0; i<PKMARK_UI_ARRAY_SIZE; ++i )
	{
		m_pSlot[i] = NULL;
	}
}

CPKMarkDisplay::~CPKMarkDisplay()
{
}

void CPKMarkDisplay::CreateSubControl()
{
	std::string strInfo[PKMARK_UI_ARRAY_SIZE] = 
	{
		"PKMARK_DISPLAY_SLOT_00",
		"PKMARK_DISPLAY_SLOT_01",
		"PKMARK_DISPLAY_SLOT_02",
		"PKMARK_DISPLAY_SLOT_03",
		"PKMARK_DISPLAY_SLOT_04",
		"PKMARK_DISPLAY_SLOT_05",
		"PKMARK_DISPLAY_SLOT_06",
		"PKMARK_DISPLAY_SLOT_07",
		"PKMARK_DISPLAY_SLOT_08",
		"PKMARK_DISPLAY_SLOT_09",
	};

	for( int i=0; i<PKMARK_UI_ARRAY_SIZE; ++i )
	{
		m_pSlot[i] = new CPKMarkSlot;
		m_pSlot[i]->CreateSub ( this, strInfo[i].c_str() );
		m_pSlot[i]->CreateSubControl ();
		m_pSlot[i]->SetVisibleSingle (TRUE);
		RegisterControl ( m_pSlot[i] );
	}
}

void CPKMarkDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	for( int i=0; i<PKMARK_UI_ARRAY_SIZE; ++i )
	{
		if ( m_pSlot[i] )
			m_pSlot[i]->SetVisibleSingle( FALSE );
	}

	const VEC_PKMARK_INFO& vecData = GLPKMark::GetInstance().m_vecMark;

	for( int i = 0; i < (int)vecData.size(); ++ i )
	{
		if ( i >= PKMARK_UI_ARRAY_SIZE )	continue;

		if ( m_pSlot[i] )
		{
			m_pSlot[i]->SetVisibleSingle( TRUE );
			m_pSlot[i]->ResetData();
			m_pSlot[i]->SetData( vecData[i] );
		}
	}
}
