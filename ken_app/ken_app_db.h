#pragma once
#include "sqlite3.h"
#include <string>

class  ken_app_db {
public :

	struct users {
		std::string username;
	};
	ken_app_db();
	~ken_app_db();

	bool connect(const std::string& file_name, const std::string& password, std::string& error);

	// users
	bool new_user(const users& user, const std::string& password, std::string& error);
	bool get_users(std::vector<users>& users, std::string& error);
	bool get_user(const std::string& username, const std::string& password,
		users& user, std::string& error);
	bool get_user(const std::string& username, users& user, std::string& error);


private :
	class ken_app_db_impl;
	ken_app_db_impl& d_;
};