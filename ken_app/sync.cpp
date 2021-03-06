#include "sync.h"
#include "sync.h"
#include <Windows.h>
#include <ShlObj.h>

#include <filesystem>

#pragma comment(lib , "shell32.lib")

std::string sync_folder()
{
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	if (result != S_OK)
		return "C:\\Ken_App_Sync";

	else
	{
		std::string path(my_documents);
		return path + "\\Ken_App_sync";
	}

}

void get_sync_folder()
{

    // creating sync folder if exists 
	const std::string sync_folder_ = sync_folder();
    // creating object of filesystem path
	std::filesystem::path path(sync_folder_);

	if (!std::filesystem::exists(path));
	std::filesystem::create_directory(path);
	

}


