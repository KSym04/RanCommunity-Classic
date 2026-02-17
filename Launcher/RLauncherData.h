/*!
 * \file RLauncherData.h
 *
 * \author Juver
 * \date September 2018
 *
 * 
 */

#pragma once

class	CHttpPatch;

struct S_PATCH_THREAD_PARAM
{
	CHttpPatch*	pHttpPatch;


	S_PATCH_THREAD_PARAM ()
		: pHttpPatch ( NULL )
	{
	}
};

struct GAME_LAUNCHER_FILE_DATA
{
	std::string file_name;
	std::string file_path_sub;
	std::string file_hash;
	std::string file_path_full;

	GAME_LAUNCHER_FILE_DATA()
		: file_name("")
		, file_path_sub("")
		, file_hash("")
		, file_path_full("")
	{
	}
};

typedef std::map<std::string,GAME_LAUNCHER_FILE_DATA >	GAME_LAUNCHER_FILE_DATA_MAP;
typedef GAME_LAUNCHER_FILE_DATA_MAP::iterator			GAME_LAUNCHER_FILE_DATA_MAP_ITER;


