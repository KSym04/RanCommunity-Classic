/*!
 * \file SkillTransformData.h
 *
 * \author Juver
 * \date September 2018
 *
 * 
 */

#pragma once

#define SKILL_TRANSFORM_ID_NULL 0xff

struct SKILL_TRANSFORM_DATA
{  
	bool use_skin_change;
	bool use_force_anim;
	byte skin_change_id;
	byte force_anim_id;

	SKILL_TRANSFORM_DATA()
		: use_skin_change(false)
		, use_force_anim(false)
		, skin_change_id(SKILL_TRANSFORM_ID_NULL)
		, force_anim_id(SKILL_TRANSFORM_ID_NULL)
	{
	}

	bool is_skin_change_valid()
	{
		if ( !use_skin_change )	return false;
		if ( skin_change_id == SKILL_TRANSFORM_ID_NULL )	return false;

		return true;
	};

	bool is_force_anim_valid()
	{
		if ( !use_force_anim )	return false;
		if ( force_anim_id == SKILL_TRANSFORM_ID_NULL )		return false;
		return true;
	};

	void Reset()
	{
		use_skin_change = false;
		use_force_anim = false;
		skin_change_id = SKILL_TRANSFORM_ID_NULL;
		force_anim_id = SKILL_TRANSFORM_ID_NULL;
	};


};
