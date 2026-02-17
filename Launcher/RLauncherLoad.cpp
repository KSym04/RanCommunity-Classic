#include "stdafx.h"
#include "./RLauncher.h"

#include "../Lib_Engine/Common/IniLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace rlauncher_setting
{
	void load_file( const TCHAR *root_path )
	{
		std::string	file_path(root_path);
		file_path += "\\rlauncher.ini";

		CIniLoader cFILE;

		if( !cFILE.open( file_path.c_str(), true) )	return;
		
		cFILE.getflag ( "SETTING", "url_news", 0, 1, url_news );
		cFILE.getflag ( "SETTING", "launcher_text_file", 0, 1, launcher_text_file );

		cFILE.getflag ( "SETTING", "game_filename", 0, 1, game_filename );
		cFILE.getflag ( "SETTING", "game_parameter", 0, 1, game_parameter );
		cFILE.getflag ( "SETTING", "game_minimize_other", 0, 1, game_minimize_other );

		cFILE.getflag ( "SETTING", "patch_function", 0, 1, patch_function );
		cFILE.getflag ( "SETTING", "patch_server_url_0", 0, 1, patch_server_url[0] );
		cFILE.getflag ( "SETTING", "patch_server_url_1", 0, 1, patch_server_url[1] );
		cFILE.getflag ( "SETTING", "patch_server_url_2", 0, 1, patch_server_url[2] );
		cFILE.getflag ( "SETTING", "patch_server_url_3", 0, 1, patch_server_url[3] );
		cFILE.getflag ( "SETTING", "patch_server_url_4", 0, 1, patch_server_url[4] );
		cFILE.getflag ( "SETTING", "patch_temp_folder", 0, 1, patch_temp_folder );
		cFILE.getflag ( "SETTING", "patch_file_list", 0, 1, patch_file_list );
	}
};

