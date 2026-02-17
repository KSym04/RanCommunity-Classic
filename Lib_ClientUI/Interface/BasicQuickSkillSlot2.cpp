#include "StdAfx.h"
#include "./BasicQuickSkillSlot2.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./SkillImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicQuickSkillSlot2::CBasicQuickSkillSlot2 () :
	m_pSkillImage ( NULL ),
	m_pSkillMouseOver ( NULL ),
	m_pSkillBlock ( NULL ), /*prohibit skill logic, Juver, 2017/06/06 */
	m_pd3dDevice ( NULL )
{
}

CBasicQuickSkillSlot2::~CBasicQuickSkillSlot2 ()
{
}

void  CBasicQuickSkillSlot2::CreateSubControl ()
{
	CreateSkillImage ();
	CreateMouseOver ();
	CreateSkillBlock (); /*prohibit skill logic, Juver, 2017/06/06 */
}

void CBasicQuickSkillSlot2::CreateSkillImage ()
{
	CSkillImage* pSkillImage = new CSkillImage;
	pSkillImage->CreateSub ( this, "QUICK_SKILL_IMAGE_H" );
	pSkillImage->CreateSubControl ();
	pSkillImage->SetUseRender ( TRUE );
	pSkillImage->SetVisibleSingle ( FALSE );
    RegisterControl ( pSkillImage );
	m_pSkillImage = pSkillImage;
}

void CBasicQuickSkillSlot2::SetUseSkillImageTwinkle( bool bUse )
{
	if( !m_pSkillImage)
	{
		GASSERT( 0 && "CBasicQuickSkillSlot2::SetUseSkillImageTwinkle(), m_pSkillImage is NULL." );
		return ;
	}

	m_pSkillImage->SetUseTwinkle( bUse );
}

void	CBasicQuickSkillSlot2::UpdateSlot ( SNATIVEID sNativeID )
{	
	if ( !m_pSkillImage )
	{
		GASSERT ( 0 && "스킬 이미지가 만들어지지 않았습니다."  );
		return ;
	}

	if ( sNativeID != NATIVEID_NULL() )
	{
		m_pSkillImage->SetSkill ( sNativeID );
		m_pSkillImage->SetVisibleSingle ( TRUE );
		m_pSkillImage->SetSkillProcess ( sNativeID );
	}
	else
	{
		m_pSkillImage->SetVisibleSingle ( FALSE );
		m_pSkillImage->ResetSkill ();
	}	
}

void CBasicQuickSkillSlot2::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GetskillRunSlot ();
    UpdateSlot ( sNativeID );
}

void CBasicQuickSkillSlot2::CreateMouseOver ()
{
	CUIControl* pSkillMouseOver = new CUIControl;
	pSkillMouseOver->CreateSub ( this, "QUICK_SKILL_MOUSE_OVER_H", UI_FLAG_DEFAULT, QUICK_SKILL_MOUSE_OVER );
	pSkillMouseOver->SetVisibleSingle ( TRUE );
    RegisterControl ( pSkillMouseOver );
	m_pSkillMouseOver = pSkillMouseOver;
}

/*prohibit skill logic, Juver, 2017/06/06 */
void CBasicQuickSkillSlot2::CreateSkillBlock ()
{
	CUIControl* pSkillBlock = new CUIControl;
	pSkillBlock->CreateSub ( this, "QUICK_SKILL_BLOCK", UI_FLAG_DEFAULT, QUICK_SKILL_BLOCK );
	pSkillBlock->SetVisibleSingle ( FALSE );
	RegisterControl ( pSkillBlock );
	m_pSkillBlock = pSkillBlock;
}

UIRECT  CBasicQuickSkillSlot2::GetAbsPosSkillImage ()
{	
	if ( !m_pSkillImage )
	{
		GASSERT ( 0 && "널입니다." );
		return UIRECT ();
	}
	
	return m_pSkillImage->GetGlobalPos ();	
}