#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>

class  ken_app_db {
public :

	struct user_credentials {
		std::string username;
	};
	ken_app_db();
	~ken_app_db();

	bool connect(const std::string& file_name, const std::string& password, std::string& error);

	// users
	bool new_user(const user_credentials& user, const std::string& password, std::string& error);
	bool get_users(std::vector<user_credentials>& users, std::string& error);
	bool get_user(const std::string& username, const std::string& password,
		user_credentials& user, std::string& error);
	bool get_user(const std::string& username, user_credentials& user, std::string& error);


private :
	class ken_app_db_impl;
	ken_app_db_impl& d_;
};