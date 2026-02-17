/*!
 * \file RPatcher.h
 *
 * \author Juver
 * \date September 2018
 *
 * 
 */

#pragma once

#define PATCH_BUILDER_PROGRESS_UPDATE_TIME 20

enum
{
	PATCH_BUILDER_TIMER_PROGRESS	= 1,
	PATCH_BUILDER_TIMER_SCAN		= 2,
	PATCH_BUILDER_TIMER_BUILD		= 3,
	PATCH_BUILDER_TIMER_UPLOAD		= 4,
	
};

enum 
{
	PATCH_FILE_LIST_VERSION = 0x0100,
};

namespace rpatcher_global
{
	extern std::string app_path;
	extern std::string source_path;
	extern std::string output_path;
};

namespace rpatcher_settings
{
	extern std::string source_folder;
	extern std::string output_folder;
	extern std::string patch_file_list_server;
	extern std::string patch_file_list_client;
	extern std::string patch_file_upload_list;
};