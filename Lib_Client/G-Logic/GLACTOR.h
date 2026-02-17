#pragma once

#include "./GLCharDefine.h"
#include "./GLCharData.h"

class GLARoundSlot;
struct NET_MSG_GENERIC;
struct SSTATEBLOW;

class GLACTOR
{
public:
	virtual EMCROW GetCrow () const = 0;
	virtual DWORD GetCtrlID () const = 0;
	virtual DWORD GetCharID () const { return 0; }
	virtual WORD GetSchool () const { return 0; }

	virtual DWORD GetPartyID () const = 0;

	virtual BOOL IsAction ( EMACTIONTYPE emCur ) const = 0;
	virtual BOOL IsValidBody () const = 0;
	virtual BOOL IsRunning () const = 0;
	virtual BOOL IsSafeTime () const { return FALSE; }
	virtual bool IsSafeZone () const { return false; }

	virtual const SRESIST& GETRESIST () const = 0;
	virtual DWORD GETHOLDBLOW () const = 0;

	virtual const D3DXVECTOR3& GetPosition () const = 0;
	virtual const D3DXVECTOR3& GetDirection () const = 0;
	virtual WORD GetBodyRadius () const = 0;

	virtual WORD	GetLevel () const = 0;

	virtual DWORD	GetNowHP () const = 0;
	virtual DWORD	GetMaxHP () const = 0;

	virtual WORD	GetNowMP () const = 0;
	virtual WORD	GetMaxMP () const = 0;

	virtual WORD	GetNowSP () const = 0;
	virtual WORD	GetMaxSP () const = 0;

	virtual int		GetDefense () const = 0;
	virtual int		GetBodyDefense () const		{ return 0; }
	virtual int		GetItemDefense () const		{ return 0; }

	virtual const DAMAGE_SPEC&	GetDamageSpec()		const = 0;
	virtual DAMAGE_SPEC* GetDamageSpecPtr()			= 0;
	virtual DEFENSE_SKILL GetDefenseSkill()			const = 0;

	virtual DWORD	GetBonusExp () const = 0;

	virtual int GetAvoid () const = 0;
	virtual EMBRIGHT GetBright () const = 0;

	virtual GLARoundSlot* GETSLOT ()			{ return NULL; }
	virtual SCONFTING* GETCONFTING ()			{ return NULL; }
	virtual DWORD GETCLIENTID ()				{ return 0; }

	virtual DWORD ReceiveDamage ( const EMCROW emACrow, const DWORD dwAID, const DWORD dwDamage, const BOOL bShock ) = 0;

	/*push pull skill logic, Juver, 2017/06/04 */
	virtual void ReceivePushPull ( const D3DXVECTOR3 &vMovePos, float fSpeed, bool bSkillMove, bool bReverseDir, bool bSkillDash, WORD wActionAnim ) = 0;

	/* push skill logic, Juver, 2020/12/14 */
	virtual void ReceiveSkillPush ( const D3DXVECTOR3 &vMovePos, const float& fSpeed ) = 0;

	/* pull skill logic, Juver, 2020/12/14 */
	virtual void ReceiveSkillPull ( const D3DXVECTOR3 &vMovePos, const float& fSpeed ) = 0;

	/* dash skill logic, Juver, 2020/12/14 */
	virtual void ReceiveSkillDash ( const D3DXVECTOR3 &vMovePos, const float& fSpeed, const DWORD& dwActionAnim ) = 0;

	/*skill teleport, Juver, 2018/09/06 */
	virtual void ReceiveSkillTeleport( const D3DXVECTOR3& vtarget_pos ) = 0;

	/* skill position shift, Juver, 2020/12/15 */
	virtual void ReceivePositionShift( const D3DXVECTOR3 &vMovePos, const float& fSpeed ) = 0;
	
	virtual void DamageReflectionProc ( int nDAMAGE, STARGETID sACTOR ) = 0;
	virtual void DefenseSkill( SNATIVEID sNativeID, WORD wLevel, STARGETID sACTOR ) = 0;

	virtual void STATEBLOW ( const SSTATEBLOW &sStateBlow ) = 0;
	virtual float GETSTATE_DAMAGE () const = 0;

	virtual void CURE_STATEBLOW ( DWORD dwCUREFLAG ) = 0;
	virtual void BUFF_REMOVE( DWORD dwBuffFlag ) = 0;

	virtual void VAR_BODY_POINT ( const EMCROW emACrow, const DWORD dwAID, const BOOL bPartySkill, int nvar_hp, int nvar_mp, int nvar_sp ) = 0;
	virtual BOOL RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, DWORD &dwSELECT, WORD _wCasterCrow, DWORD _dwCasterID ) = 0;
	virtual HRESULT SNDMSGAROUND ( NET_MSG_GENERIC *pnet ) = 0;

	/*ignore damage, Juver, 2017/12/12 */
	virtual BOOL	IsIgnoreDamage  () const		{ return false; }

	/*block back damage, Juver, 2017/12/12 */
	virtual BOOL	IsBlockDamageBack  () const		{ return false; }
	virtual void	SetBlockDamageBack( bool bSet ) = 0;

	/* skill amplify, Juver, 2020/12/10 */
	virtual SSKILL_AMPLIFY GetSkillAmplifyMelee() const = 0;
	virtual SSKILL_AMPLIFY GetSkillAmplifyMissile() const = 0;
	virtual SSKILL_AMPLIFY GetSkillAmplifyMagic() const = 0;

	/* skill stigma, Juver, 2020/12/12 */
	virtual void LinkSkillStigmaTarget ( const EMCROW _emACrow, const DWORD _dwAID, const SNATIVEID& _sSkillID, const float& fDistance, const float& fDamage, const DWORD& dwAttackFlag, const SNATIVEID& sMapID ) = 0;
	virtual BOOL CheckSkillStigmaExist( const EMCROW _emACrow, const DWORD _dwAID, const SNATIVEID& _sSkillID ) const		{ return false; }

	/* skill buff duration change, Juver, 2020/12/16 */
	virtual void ReceiveSkillBuffDurationChange( const SNATIVEID& _sSkillID, const DWORD& _dwFlags, const float& fSeconds ) = 0;

	/* skill illusion, Juver, 2021/01/17 */
	virtual SKILL_ILLUSION* GetSkillIllusion()			= 0;

	virtual void ReactDamage( GLACTOR* pAttacker, DWORD dwDamageFlags ) = 0;
public:
	GLACTOR ()		{}
	~GLACTOR ()		{}
};