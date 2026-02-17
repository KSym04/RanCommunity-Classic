#include "stdafx.h"
#include "GLPKCombo.h"

#include "./GLogicData.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#include "../../Lib_Engine/DxSound/DxSoundLib.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLPKCombo::GLPKCombo()
{
	CleanUp();
};

GLPKCombo::~GLPKCombo()
{
	CleanUp();
};

GLPKCombo& GLPKCombo::GetInstance ()
{
	static GLPKCombo Instance;
	return Instance;
}

void GLPKCombo::CleanUp()
{
	bPKComboUse = FALSE;

	for ( int i=0; i<EMPK_COMBO_TYPE_SIZE; ++i )
	{
		strPKComboSoundText[i] = "";
		strPKComboSoundFile[i] = "";
		fPKComboDelayTime[i] = 10.0f;
	}

	//pk_combo_state = 0;
	//pk_combo_timer = 0.0f;
}

void GLPKCombo::LoadFile( std::string file_name )
{
	CleanUp();

	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += file_name;


	CIniLoader file;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		file.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	std::string separator( ",[]\t" );
	file.reg_sep( separator );

	if( !file.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ErrorFile( "GLPKCombo::load_file", file_name.c_str() );
		return;
	}

	file.getflag( "PK_COMBO_SETTING", "PK_COMBO_ENABLE", 0, 1, bPKComboUse );

	for ( int i=0; i<EMPK_COMBO_TYPE_SIZE; ++i )
	{
		file.getflag( "PK_COMBO_SOUND_TEXT", i, 0, 1, strPKComboSoundText[i] );
		file.getflag( "PK_COMBO_SOUND_FILE", i, 0, 1, strPKComboSoundFile[i] );
		file.getflag( "PK_COMBO_TIME", i, 0, 1, fPKComboDelayTime[i] );
	}
};

void GLPKCombo::LoadSound()
{
	if( !bPKComboUse )	return;

	for( int i=0; i<EMPK_COMBO_TYPE_SIZE; ++i )
		DxSoundLib::GetInstance()->CreateSound ( strPKComboSoundText[i].c_str(), strPKComboSoundFile[i].c_str(), SFX_SOUND );
}

//void GLPKCombo::framemove( float elapsed )
//{
//	if( !bPKComboUse )	return;
//
//
//	if ( pk_combo_timer > 0.0f )
//	{
//		pk_combo_timer -= elapsed;
//
//		if ( pk_combo_timer <= 0.0f )
//		{
//			state_reset();
//		}
//
//		CDebugSet::ToView( 16, "%g", pk_combo_timer );
//	}
//}
//
//void GLPKCombo::state_increase()
//{ 
//	if( !bPKComboUse )	return;
//
//	pk_combo_state ++;
//
//	WORD pk_combo_current_state = pk_combo_state - 1;
//
//	if ( pk_combo_current_state >= EMPK_COMBO_TYPE_SIZE )
//		pk_combo_current_state = EMPK_COMBO_TYPE_SIZE - 1;
//
//	pk_combo_timer = fPKComboDelayTime[pk_combo_current_state];
//	
//	CInnerInterface::GetInstance().pk_combo_set ( (int)pk_combo_current_state );
//	DxSoundLib::GetInstance()->PlaySound ( strPKComboSoundText[pk_combo_current_state].c_str() ); 
//}
//
//void GLPKCombo::state_reset()
//{ 
//	if( !bPKComboUse )	return;
//
//	pk_combo_state = 0;
//	pk_combo_timer = 0.0f;
//}