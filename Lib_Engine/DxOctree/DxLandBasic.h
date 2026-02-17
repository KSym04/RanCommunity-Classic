/*!
 * \file DxLandBasic.h
 *
 * \author Juver
 * \date May 2018
 *
 * 
 */

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Common/BaseStream.h"
#include "../Common/StringFile.h"
#include "../Common/SerialFile.h"


struct dxland_basic
{
	enum { VERSION = 0x0002 };

	BOOL m_blegacy_shadow;
	float m_fVS_1_1_ColorMulti;

	dxland_basic() 
		: m_blegacy_shadow(FALSE)
		, m_fVS_1_1_ColorMulti(0.75f)
	{
	}

	void load ( CSerialFile &SFile );
	void save ( CSerialFile &SFile );
	void reset();
	void set_shadow_type();
};

