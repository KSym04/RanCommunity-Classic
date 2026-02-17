/*!
 * \file RPatcherData.h
 *
 * \author Juver
 * \date September 2018
 *
 * 
 */


#pragma once

struct PATCH_BUILDER_FILE_DATA
{
	std::string file_name;
	std::string file_path_sub;
	std::string file_path_full;
	std::string file_hash;

	PATCH_BUILDER_FILE_DATA()
		: file_name("")
		, file_path_sub("")
		, file_path_full("")
		, file_hash("")
	{

	}
};


typedef std::map<std::string,PATCH_BUILDER_FILE_DATA >	PATCH_BUILDER_FILE_DATA_MAP;
typedef PATCH_BUILDER_FILE_DATA_MAP::iterator			PATCH_BUILDER_FILE_DATA_MAP_ITER;

