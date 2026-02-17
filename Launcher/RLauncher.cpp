#include "stdafx.h"
#include "./RLauncher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace rlauncher_global
{
	std::string	app_path		= "app_path";
	std::string	profile_path	= "profile_path";
	std::string	temp_path		= "temp_path";
};

namespace rlauncher_log_control
{
	ULONGLONG	cur_pos = 0;
	ULONGLONG	cur_end = 0;
	ULONGLONG	all_pos = 0;
	ULONGLONG	all_end = 0;

	void	set_process_cur_position ( ULONGLONG  _cur_pos,  ULONGLONG  _cur_end )
	{
		cur_pos = _cur_pos;
		cur_end = _cur_end;
	}

	void	get_process_cur_position ( ULONGLONG* _pcur_pos, ULONGLONG* _pcur_end )
	{
		*_pcur_pos = cur_pos;
		*_pcur_end = cur_end;
	}

	void	get_process_cur_position_addr ( ULONGLONG** _ppcur_pos, ULONGLONG** ppcur_end )
	{
		*_ppcur_pos = &cur_pos;
		*ppcur_end = &cur_end;
	}

	void	set_process_all_position ( ULONGLONG  _all_pos,  ULONGLONG  _all_end )
	{
		all_pos = _all_pos;
		all_end = _all_end;
	}

	void	get_process_all_position ( ULONGLONG* _pall_pos, ULONGLONG* _pall_end )
	{
		*_pall_pos = all_pos;
		*_pall_end = all_end;
	}
};


namespace rlauncher_setting
{
	std::string	url_news				= "google.com";
	std::string	launcher_text_file		= "Launcher.XML";

	std::string	game_filename			= "Game.exe";
	std::string	game_parameter			= "app_run";
	BOOL game_minimize_other = TRUE;

	BOOL patch_function	= FALSE;

	std::string	patch_server_url[LAUNCHER_TOTAL_SERVER]	 = 
	{ 
		"patch.ran.com", 
		"patch.ran.com", 
		"patch.ran.com", 
		"patch.ran.com", 
		"patch.ran.com" 
	};

	std::string	patch_temp_folder		= _T("\\$temp$\\");
	std::string	patch_file_list			= "file_list_client.bin";
};

