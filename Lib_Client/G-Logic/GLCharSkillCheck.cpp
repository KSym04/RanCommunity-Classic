#include "stdafx.h"

#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#include "./GLClubDeathMatch.h"
#include "./GLPVPTyrannyField.h"
#include "./GLPVPSchoolWarsField.h"
#include "./GLPVPCaptureTheFlagField.h"
#include "./PVPClubDeathMatchField.h"
#include "./GLSchoolFreePK.h"

#include "./GLGaeaServer.h"
#include "./GLChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL _IsExceedDistance( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const float& fLimit )
{
	D3DXVECTOR3 vPos1 = v1;
	D3DXVECTOR3 vPos2 = v2;

	//skip checking difference in Y axis to avoid skill bug in slope areas
	vPos1.y = 0.0f;
	vPos2.y = 0.0f;

	const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(vPos1 - vPos2));
	if ( fDistance > fLimit )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL GLChar::ValidateSkillHash( PGLSKILL pSkill, const char* szHash )
{
	if ( !pSkill )	return FALSE;
	if ( !szHash )	return FALSE;

	if ( strcmp( szHash, pSkill->m_szHash ) != 0 )
	{
		if ( RPARAM::bCaptchaDcLogs == 1 )
		{
			CDebugSet::ToFileWithTime( "_validate_skill_hash.txt", "[%d]%s [%d~%d]%s hash mismatch server:%s client:%s", m_dwCharID, m_szName, 
				pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), pSkill->m_szHash, szHash );
		}

		return FALSE;
	}

	return TRUE;
}


BOOL GLChar::SkillTargetCheck( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	vecTarID.clear();

	if ( !pSkill )									return FALSE;
	if ( wSkillLevel >= SKILL::MAX_SKILL_LEVEL )	return FALSE;
	if ( wTargetNum >= EMTARGET_NET )				return FALSE;
	if ( wTargetNum == 0 )							return TRUE;		//no skill targets
	
	switch ( pSkill->m_sBASIC.emIMPACT_TAR )
	{
	case TAR_SELF:
		return SkillTargetCheck_TargetSelf( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case TAR_SPEC:
		return SkillTargetCheck_TargetSpec( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case TAR_SELF_TOSPEC:
		return SkillTargetCheck_TargetSelfToSpec( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case TAR_ZONE:
		return SkillTargetCheck_TargetZone( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case TAR_SPECIFIC:
		return SkillTargetCheck_TargetSpecific( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	};

	return FALSE;
}


BOOL GLChar::SkillTargetCheck_TargetSelf( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];

	//target num more than skill maximum target
	if ( wTargetNum > SkillLevelData.wTARNUM && !bNextSkill )
	{
		CDebugSet::ToFileWithTime( "_validate_skill_target.txt", "[%d]%s [%d~%d]%s target num more than skill maximum target %d/%d", 
			m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
			wTargetNum, SkillLevelData.wTARNUM );

		return FALSE;
	}


	switch ( pSkill->m_sBASIC.emIMPACT_REALM )
	{
	case REALM_SELF:
			return SkillTargetCheck_TargetSelf_RealmSelf( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case REALM_ZONE:
			return SkillTargetCheck_TargetSelf_RealmZone( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSelf_RealmSelf( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	//just get the very first target
	STARID sTartID = pTarIDS[0];

	STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

	//check if target exist
	GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
	if ( !pTARGET )	return FALSE;

	//check if target is self
	if ( sTargetID.emCrow != GETCROW() || sTargetID.dwID != m_dwGaeaID )
	{
		CDebugSet::ToFileWithTime( "_validate_skill_target.txt", "[%d]%s [%d~%d]%s target not self", 
			m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName() );

		return FALSE;
	}

	vecTarID.push_back( sTartID );

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSelf_RealmZone( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
	{
	case SIDE_OUR:
		return SkillTargetCheck_TargetSelf_RealmZone_SideOur( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case SIDE_ENEMY:
	case SIDE_ANYBODY:
		return SkillTargetCheck_TargetSelf_RealmZone_SideEnemyAnybody( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	}

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSelf_RealmZone_SideOur( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	//check if target is within casters(self) apply range
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		//check if target is PC
		if ( sTargetID.emCrow != CROW_PC )	continue;

		//check if target is a party member
		if ( sTargetID != STARGETID( GETCROW(), m_dwGaeaID ) )
		{
			GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
			GLPARTY_FIELD* pPartyField = sPartyFieldMan.GetParty ( m_dwPartyID );
			if ( !pPartyField )		continue;
			if ( !pPartyField->ISMEMBER( sTargetID.dwID ) )	continue;
		}

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance ( GetPosition(), pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}
		
		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSelf_RealmZone_SideEnemyAnybody( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within casters(self) apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )	
			if ( _IsExceedDistance ( GetPosition(), pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];

	//target num more than skill maximum target
	if ( wTargetNum > SkillLevelData.wTARNUM && !bNextSkill )
	{
		CDebugSet::ToFileWithTime( "_validate_skill_target.txt", "[%d]%s [%d~%d]%s target num more than skill maximum target %d/%d", 
			m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
			wTargetNum, SkillLevelData.wTARNUM );

		return FALSE;
	}

	switch ( pSkill->m_sBASIC.emIMPACT_REALM )
	{
		case REALM_SELF:
			return SkillTargetCheck_TargetSpec_RealmSelf( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;

		case REALM_ZONE:
			return SkillTargetCheck_TargetSpec_RealmZone( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;

		case REALM_FANWIZE:
			return SkillTargetCheck_TargetSpec_RealmFanWize( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmSelf( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
	{
		case SIDE_OUR:
			return SkillTargetCheck_TargetSpec_RealmSelf_SideOur( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;

		case SIDE_ENEMY:
			return SkillTargetCheck_TargetSpec_RealmSelf_SideEnemy( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;

		case SIDE_ANYBODY:
			return SkillTargetCheck_TargetSpec_RealmSelf_SideAnybody( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmSelf_SideOur( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		//check if target is PC
		if ( sTargetID.emCrow != CROW_PC )	continue;

		//check if target is a party member
		if ( sTargetID != STARGETID( GETCROW(), m_dwGaeaID ) )
		{
			GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
			GLPARTY_FIELD* pPartyField = sPartyFieldMan.GetParty ( m_dwPartyID );
			if ( !pPartyField )		continue;
			if ( !pPartyField->ISMEMBER( sTargetID.dwID ) )	continue;
		}

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within casters(self) target range
			//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - pTARGET->GetPosition()));
			//if ( fDistanceToTarget > fLimitTargetRange )
			if ( _IsExceedDistance ( GetPosition(), pTARGET->GetPosition(), fLimitTargetRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmSelf_SideEnemy( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within casters(self) target range
			//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - pTARGET->GetPosition()));
			//if ( fDistanceToTarget > fLimitTargetRange )
			if ( _IsExceedDistance ( GetPosition(), pTARGET->GetPosition(), fLimitTargetRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmSelf_SideAnybody( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		//check if target is PC
		if ( sTargetID.emCrow != CROW_PC )	continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within casters(self) target range
			//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - pTARGET->GetPosition()));
			//if ( fDistanceToTarget > fLimitTargetRange )
			if ( _IsExceedDistance ( GetPosition(), pTARGET->GetPosition(), fLimitTargetRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmZone( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
	{
	case SIDE_OUR:
		return SkillTargetCheck_TargetSpec_RealmZone_SideOur( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case SIDE_ENEMY:
		return SkillTargetCheck_TargetSpec_RealmZone_SideEnemy( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;

	case SIDE_ANYBODY:
		return SkillTargetCheck_TargetSpec_RealmZone_SideAnyBody( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmZone_SideOur( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];

	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//primary target distance check
	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within casters(self) target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - vTargetPos));
		//if ( fDistanceToTarget > fLimitTargetRange )
		if ( _IsExceedDistance ( GetPosition(), vTargetPos, fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( vTargetPos + _vDeviation, vTargetPos - _vDeviation );
		if ( !_bColl )
		{
			return FALSE;
		}
	}

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		//check if target is PC
		if ( sTargetID.emCrow != CROW_PC )	continue;

		//check if target is a party member
		if ( sTargetID != STARGETID( GETCROW(), m_dwGaeaID ) )
		{
			GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
			GLPARTY_FIELD* pPartyField = sPartyFieldMan.GetParty ( m_dwPartyID );
			if ( !pPartyField )		continue;
			if ( !pPartyField->ISMEMBER( sTargetID.dwID ) )	continue;
		}

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(vTargetPos - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance ( vTargetPos, pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmZone_SideEnemy( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//primary target distance check
	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within casters(self) target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - vTargetPos));
		//if ( fDistanceToTarget > fLimitTargetRange )
		if ( _IsExceedDistance ( GetPosition(), vTargetPos, fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( vTargetPos + _vDeviation, vTargetPos - _vDeviation );
		if ( !_bColl )	
		{
			return FALSE;
		}
	}

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(vTargetPos - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance ( vTargetPos, pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmZone_SideAnyBody( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//primary target distance check
	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within casters(self) target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - vTargetPos));
		//if ( fDistanceToTarget > fLimitTargetRange )
		if ( _IsExceedDistance ( GetPosition(), vTargetPos, fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( vTargetPos + _vDeviation, vTargetPos - _vDeviation );
		if ( !_bColl )
		{
			return FALSE;
		}
	}

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		//check if target is PC
		if ( sTargetID.emCrow != CROW_PC )	continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(vTargetPos - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance (vTargetPos, pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmFanWize( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
	{
	case SIDE_ENEMY:
		return SkillTargetCheck_TargetSpec_RealmFanWize_SideEnemy( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSpec_RealmFanWize_SideEnemy( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	//get the first target
	STARID sTartID0 = pTarIDS[0];
	STARGETID sTargetID0( sTartID0.GETCROW(), sTartID0.GETID() );

	GLACTOR* pTARGET0 = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID0 );
	if ( !pTARGET0 ) return FALSE;

	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitTargetRange = ::pow(float(pTARGET0->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const float fLimitApplyRange = ::pow(float(pTARGET0->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const float fLimitApplyAngle = D3DXToRadian(SkillLevelData.wAPPLYANGLE) * 0.5f;
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//primary target distance check
	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within casters(self) target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(pTARGET0->GetPosition() - GetPosition()));
		//if ( fDistanceToTarget > fLimitTargetRange )
		if ( _IsExceedDistance (pTARGET0->GetPosition(), GetPosition(), fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET0->GetPosition() + _vDeviation, pTARGET0->GetPosition() - _vDeviation );
		if ( !_bColl )
		{
			return FALSE;
		}
	}

	const D3DXVECTOR3& vMyPosition = GetPosition();
	const D3DXVECTOR3& vTargetDirect = pTARGET0->GetPosition() - vMyPosition;

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//this detection is the correct one but official have some bugs with range configuration having lower apply range than attack range
			//causing player to have longer attack reach than the skill and this detection will false trigger
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(pTARGET->GetPosition() - GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance (pTARGET->GetPosition(), GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}

			//we will use the attack range detection instead
			//check if target is within casters(self) target range
			//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(pTARGET->GetPosition() - GetPosition()));
			//if ( fDistanceToTarget > fLimitTargetRange )
			//if ( _IsExceedDistance (pTARGET->GetPosition(), GetPosition(), fLimitTargetRange ) )
			//{
			//	continue;
			//}

			//angle check
			const D3DXVECTOR3 vDirectToTarget = pTARGET->GetPosition() - GetPosition();
			const float fAngle = ::abs(::DXGetThetaYFromDirection(vDirectToTarget, vTargetDirect));
			if ( (SkillLevelData.wAPPLYANGLE != 0) && (fAngle > fLimitApplyAngle) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}


BOOL GLChar::SkillTargetCheck_TargetSelfToSpec( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const DWORD dwTarget = GETSUM_PIERCE() + SkillLevelData.wPIERCENUM;

	//target num more than skill maximum target
	if ( wTargetNum > dwTarget && !bNextSkill )
	{
		CDebugSet::ToFileWithTime( "_validate_skill_target.txt", "[%d]%s [%d~%d]%s target num more than skill maximum target %d/%d", 
			m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
			wTargetNum, dwTarget );

		return FALSE;
	}


	switch ( pSkill->m_sBASIC.emIMPACT_REALM )
	{
	case REALM_SELF:
	case REALM_ZONE:
	case REALM_KEEP_ZONE:
	case REALM_FANWIZE:
		return SkillTargetCheck_TargetSelfToSpec_RealmAll( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSelfToSpec_RealmAll( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
	{
	case SIDE_ENEMY:	
		return SkillTargetCheck_TargetSelfToSpec_RealmAll_SideEnemy( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSelfToSpec_RealmAll_SideEnemy( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	//get the first target
	STARID sTartID0 = pTarIDS[0];
	STARGETID sTargetID0( sTartID0.GETCROW(), sTartID0.GETID() );

	GLACTOR* pTARGET0 = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID0 );
	if ( !pTARGET0 ) return FALSE;

	const D3DXVECTOR3& vTargetPosition = pTARGET0->GetPosition();
	const D3DXVECTOR3& vTargetDirect = vTargetPosition - GetPosition();
	
	const float fLimitTargetRange = ::pow(float(pTARGET0->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	D3DXVECTOR3 vRightNormal;
	::D3DXVec3Normalize(&vRightNormal, &D3DXVECTOR3(vTargetDirect.z, vTargetDirect.y, -vTargetDirect.x));
	const WORD wApplyRange = WORD(GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2);

	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&vTargetDirect);
		//if ( fDistanceToTarget > fLimitTargetRange )
		if ( _IsExceedDistance (vTargetPosition, GetPosition(), fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET0->GetPosition() + _vDeviation, pTARGET0->GetPosition() - _vDeviation );
		if ( !_bColl )
		{
			return FALSE;
		}
	}

	
	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within target range
			const D3DXVECTOR3 vDirectToTarget = vTargetPosition - pTARGET->GetPosition();
			//const float fDistance = ::D3DXVec3LengthSq(&vDirectToTarget);
			//if ( fDistance > fLimitTargetRange )
			if ( _IsExceedDistance (vTargetPosition, pTARGET->GetPosition(), fLimitTargetRange ) )
			{
				continue;
			}

			//check if target is within line range
			const float fDistanceFromLine = ::D3DXVec3Dot(&vDirectToTarget, &vRightNormal);
			if ( fDistanceFromLine > float(pTARGET->GetBodyRadius() + wApplyRange) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetZone( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];

	//target num more than skill maximum target
	if ( wTargetNum > SkillLevelData.wTARNUM && !bNextSkill )
	{
		CDebugSet::ToFileWithTime( "_validate_skill_target.txt", "[%d]%s [%d~%d]%s target num more than skill maximum target %d/%d", 
			m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
			wTargetNum, SkillLevelData.wTARNUM );

		return FALSE;
	}

	switch ( pSkill->m_sBASIC.emIMPACT_REALM )
	{
	case REALM_SELF:
	case REALM_ZONE:
	case REALM_KEEP_ZONE:
	case REALM_FANWIZE:
		return SkillTargetCheck_TargetZone_RealmAll( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetZone_RealmAll( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
	{
		case SIDE_OUR:
			return SkillTargetCheck_TargetZone_RealmAll_SideOur( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;

		case SIDE_ENEMY:
			return SkillTargetCheck_TargetZone_RealmAll_SideEnemy( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;

		case SIDE_ANYBODY:
			return SkillTargetCheck_TargetZone_RealmAll_SideAnyBody( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetZone_RealmAll_SideOur( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//primary target distance check
	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within casters(self) target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - vTargetPos));
		//if ( fDistanceToTarget > fLimitTargetRange )
		if ( _IsExceedDistance (GetPosition(), vTargetPos, fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( vTargetPos + _vDeviation, vTargetPos - _vDeviation );
		if ( !_bColl )	
		{
			return FALSE;
		}
	}

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		//check if target is PC
		if ( sTargetID.emCrow != CROW_PC )	continue;

		//check if target is a party member
		if ( sTargetID != STARGETID( GETCROW(), m_dwGaeaID ) )
		{
			GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
			GLPARTY_FIELD* pPartyField = sPartyFieldMan.GetParty ( m_dwPartyID );
			if ( !pPartyField )		continue;
			if ( !pPartyField->ISMEMBER( sTargetID.dwID ) )	continue;
		}

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(vTargetPos - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance (vTargetPos, pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetZone_RealmAll_SideEnemy( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//primary target distance check
	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within casters(self) target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - vTargetPos));
		//if ( fDistanceToTarget > fLimitTargetRange )	
		if ( _IsExceedDistance (GetPosition(), vTargetPos, fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( vTargetPos + _vDeviation, vTargetPos - _vDeviation );
		if ( !_bColl )	
		{
			return FALSE;
		}
	}

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(vTargetPos - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance (vTargetPos, pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetZone_RealmAll_SideAnyBody( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);
	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//primary target distance check
	if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
	{
		//check if target is within casters(self) target range
		//const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - vTargetPos));
		//if ( fDistanceToTarget > fLimitTargetRange )	
		if ( _IsExceedDistance (GetPosition(), vTargetPos, fLimitTargetRange ) )
		{
			return FALSE;
		}
	}

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( vTargetPos + _vDeviation, vTargetPos - _vDeviation );
		if ( !_bColl )	
		{
			return FALSE;
		}
	}

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		//check if target is PC
		if ( sTargetID.emCrow != CROW_PC )	continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(vTargetPos - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )
			if ( _IsExceedDistance ( vTargetPos, pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

BOOL GLChar::SkillTargetCheck_TargetSpecific( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_REALM )
	{
	case REALM_SELF:
	case REALM_ZONE:
	case REALM_KEEP_ZONE:
	case REALM_FANWIZE:
		return SkillTargetCheck_TargetSpecific_RealmAll( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
		break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSpecific_RealmAll( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
	{
		case SIDE_OUR:
		case SIDE_ENEMY:
		case SIDE_ANYBODY:
			return SkillTargetCheck_TargetSpecific_RealmAll_SideAll( pSkill, wSkillLevel, pTarIDS, wTargetNum, vTargetPos, bNextSkill, vecTarID );
			break;
	};

	return FALSE;
}

BOOL GLChar::SkillTargetCheck_TargetSpecific_RealmAll_SideAll( PGLSKILL pSkill, const WORD& wSkillLevel, STARID* pTarIDS, const WORD& wTargetNum, const D3DXVECTOR3& vTargetPos, const BOOL& bNextSkill, STARID_VEC& vecTarID )
{
	if ( m_pLandMan->GetNavi() == NULL )	return FALSE;

	const D3DXVECTOR3& vTargetPosition = vTargetPos;
	D3DXVECTOR3		vCollision;
	DWORD			dwCollID;
	BOOL			bColl;

	m_pLandMan->GetNavi()->IsCollision(vTargetPosition + D3DXVECTOR3( 0.0f, 10.0f, 0.0f ), vTargetPosition + D3DXVECTOR3( 0.0f, -10.0f, 0.0f ), vCollision, dwCollID, bColl );
	if ( bColl == FALSE )
	{
		return FALSE;	
	}

	const SKILL::SSKILLBASIC& SkillBasic = pSkill->m_sBASIC;
	const D3DXVECTOR3& vMyPosition = GetPosition();
	const float fDistanceToTarget = ::D3DXVec3LengthSq(&D3DXVECTOR3(vMyPosition - vTargetPosition));
	const float fLimitTargetRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + RPARAM::fSRC1 ), 2.0f); 
	if ( fDistanceToTarget > fLimitTargetRange )	
	{
		return FALSE;
	}

	const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);

	//check if primary target is in line of sight
	/* skill collision check, Juver, 2021/07/11 */
	if ( RPARAM::bSCCS )
	{
		BOOL _bColl = m_actorMove.LineOfSightTest ( vTargetPos + _vDeviation, vTargetPos - _vDeviation );
		if ( !_bColl )	return FALSE;
	}

	const SKILL::CDATA_LVL& SkillLevelData = pSkill->m_sAPPLY.sDATA_LVL[wSkillLevel];
	const float fLimitApplyRange = ::pow(float(GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,wSkillLevel) + RPARAM::fSRC2 ), 2.0f);

	for ( WORD i=0; i<wTargetNum; ++i )
	{
		STARID sTartID = pTarIDS[i];

		STARGETID sTargetID( sTartID.GETCROW(), sTartID.GETID() );

		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
		if ( !pTARGET ) continue;

		if ( pSkill->m_sBASIC.IsNoRangeCheck() == FALSE )
		{
			//check if target is within primary target apply range
			//const float fDistance = ::D3DXVec3LengthSq(&D3DXVECTOR3(GetPosition() - pTARGET->GetPosition()));
			//if ( fDistance > fLimitApplyRange )	
			if ( _IsExceedDistance ( GetPosition(), pTARGET->GetPosition(), fLimitApplyRange ) )
			{
				continue;
			}
		}

		//check if target is in line of sight
		/* skill collision check, Juver, 2021/07/11 */
		if ( RPARAM::bSCCS )
		{
			BOOL _bColl = m_actorMove.LineOfSightTest ( pTARGET->GetPosition() + _vDeviation, pTARGET->GetPosition() - _vDeviation );
			if ( !_bColl )	
			{
				continue;
			}
		}

		vecTarID.push_back( sTartID );
	}

	return TRUE;
}

