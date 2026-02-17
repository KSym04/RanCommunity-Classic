#include "stdafx.h"
#include "./RPatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace rpatcher_global
{
	std::string	app_path		= "app_path";
	std::string	source_path		= "profile_path";
	std::string	output_path		= "temp_path";
};

namespace rpatcher_settings
{
	std::string	source_folder	= _T("\\client\\");
	std::string	output_folder	= _T("\\server\\");
	std::string patch_file_list_server = "file_list_server.bin";
	std::string patch_file_list_client = "file_list_client.bin";
	std::string patch_file_upload_list = "file_list_upload.bin";
};



