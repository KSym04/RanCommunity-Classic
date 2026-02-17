// Search2.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Engine/Meshs/DxSkinAniMan.h"
#include "../Lib_Client/G-Logic/GLSkill.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Helper/HLibDataConvert.h"

#include "EditorSkill.h"
#include "Search2.h"



// CSearch2 dialog

IMPLEMENT_DYNAMIC(CSearch2, CDialogEx)

CSearch2::CSearch2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearch2::IDD, pParent)
	, m_sID( NATIVEID_NULL() )
	, m_vec( NULL )
{

}

CSearch2::~CSearch2()
{
	m_vec = NULL;
}

void CSearch2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_List);
}


BEGIN_MESSAGE_MAP(CSearch2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearch2::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_GO, &CSearch2::OnBnClickedButtonGo)
END_MESSAGE_MAP()


// CSearch2 message handlers
BOOL CSearch2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.SetExtendedStyle ( m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List.InsertColumn( 0, "ID", LVCFMT_LEFT, 60 );
	m_List.InsertColumn( 1, "MID", LVCFMT_LEFT, 60 );
	m_List.InsertColumn( 2, "SID", LVCFMT_LEFT, 60 );
	m_List.InsertColumn( 3, "Name", LVCFMT_LEFT, 250 );
	m_List.InsertColumn( 4, "Search", LVCFMT_LEFT,150 );

	SetWin_Combo_Init( this, IDC_COMBO_EMIMPACT_TAR, COMMENT::IMPACT_TAR, TAR_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_EMIMPACT_REALM, COMMENT::IMPACT_REALM, REALM_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_EMIMPACT_SIDE, COMMENT::IMPACT_SIDE, SIDE_SIZE );

	m_sID = NATIVEID_NULL();

	ShowResults();
	SearchInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSearch2::SearchInit()
{
}

void CSearch2::ShowResults()
{
	if ( m_vec )
	{
		m_List.DeleteAllItems();
		m_List.SetRedraw( FALSE );

		for( size_t i=0; i<m_vec->size(); ++i)
		{
			SSEARCH sRESULT = m_vec->at(i);
			m_List.InsertItem( i, _HLIB::cstring_dword( i ).GetString(), NULL );
			m_List.SetItemText( i, 1, _HLIB::cstring_dword( sRESULT.sID.wMainID ).GetString() );
			m_List.SetItemText( i, 2, _HLIB::cstring_dword( sRESULT.sID.wSubID ).GetString() );
			m_List.SetItemText( i, 3, sRESULT.strNAME.c_str() );
			m_List.SetItemText( i, 4, sRESULT.strFIND.c_str() );
		}

		m_List.SetRedraw( TRUE );
	}
}

void CSearch2::OnBnClickedButtonSearch()
{
	m_List.DeleteAllItems();
	m_sID = NATIVEID_NULL();

	std::vector<SSEARCH> vec;

	EMIMPACT_TAR emIMPACT_TAR = static_cast<EMIMPACT_TAR> ( GetWin_Combo_Sel( this, IDC_COMBO_EMIMPACT_TAR ) );
	EMIMPACT_REALM emIMPACT_REALM = static_cast<EMIMPACT_REALM> ( GetWin_Combo_Sel( this, IDC_COMBO_EMIMPACT_REALM ) );
	EMIMPACT_SIDE emIMPACT_SIDE = static_cast<EMIMPACT_SIDE> ( GetWin_Combo_Sel( this, IDC_COMBO_EMIMPACT_SIDE ) );

	for ( int MID = 0; MID < EMSKILLCLASS_NSIZE; MID++ )
	{
		for ( int SID = 0; SID < GLSkillMan::MAX_CLASSSKILL; SID++ )
		{
			PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( MID, SID );
			if ( pSKILL )
			{
				if ( pSKILL->m_sBASIC.emIMPACT_TAR == emIMPACT_TAR && 
					pSKILL->m_sBASIC.emIMPACT_REALM == emIMPACT_REALM && 
					pSKILL->m_sBASIC.emIMPACT_SIDE == emIMPACT_SIDE )
				{
					SSEARCH sResult;
					sResult.sID = pSKILL->m_sBASIC.sNATIVEID;
					sResult.strNAME = pSKILL->GetName();
					sResult.strFIND;
					vec.push_back( sResult );
				}
			}
		}
	}

	if ( m_vec )
	{
		m_vec->clear();
		for( size_t i=0; i<vec.size(); ++i)
		{
			SSEARCH sRESULT = vec[i];
			m_vec->push_back( sRESULT );
		}
	}

	ShowResults();
}


void CSearch2::OnBnClickedButtonGo()
{
	int nSelect = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	m_sID.wMainID = (WORD)atoi( m_List.GetItemText(nSelect,1).GetString() );
	m_sID.wSubID = (WORD)atoi( m_List.GetItemText(nSelect,2).GetString() );
	OnOK();
}
