/*!
 * \file RLauncher.h
 *
 * \author Juver
 * \date September 2018
 *
 * 
 */

#pragma once

#define WM_LISTADDSTRING WM_USER+109

#define LAUNCHER_BROWSER_START_TIME				1000
#define LAUNCHER_PATCH_THREAD_START_TIME		30

#define LAUNCHER_TOTAL_SERVER		5

enum 
{
	TIMER_BROWSER				= 1,
	TIMER_PATCH_THREAD			= 2,
};

enum LAUNCHER_THREAD_TYPE
{
	LAUNCHER_THREAD_TYPE_PATCH = 0,
};


namespace rlauncher_global
{
	extern std::string app_path;
	extern std::string profile_path;
	extern std::string temp_path;
};
	
namespace rlauncher_log_control
{
	void	set_process_cur_position ( ULONGLONG  _cur_pos,  ULONGLONG  _cur_end );
	void	get_process_cur_position ( ULONGLONG* _pcur_pos, ULONGLONG* _pcur_end );
	void	get_process_cur_position_addr ( ULONGLONG** _ppcur_pos, ULONGLONG** ppcur_end );

	void	set_process_all_position ( ULONGLONG  _all_pos,  ULONGLONG  _all_end );
	void	get_process_all_position ( ULONGLONG* _pall_pos, ULONGLONG* _pall_end );
};

namespace rlauncher_setting
{
	extern std::string url_news;
	extern std::string launcher_text_file;

	extern std::string game_filename;
	extern std::string game_parameter;
	extern BOOL game_minimize_other;

	extern BOOL	patch_function;

	extern std::string patch_server_url[LAUNCHER_TOTAL_SERVER];

	extern std::string patch_temp_folder;
	extern std::string patch_file_list;

	

	void load_file( const TCHAR *root_path );	
};

