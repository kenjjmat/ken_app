#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>

class  ken_app_db {
public :

	struct user_credentials {
		std::string username;
	};

	struct stock_details {
		std::string id;
		std::string name;
		std::string description;
		std::string quantity;
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

	// stock 
	bool new_stock(const stock_details& stock_info, std::string& error);
	bool get_stock(const std::string& stock_id, stock_details& stock, std::string& error);
	bool get_stock_all(std::vector<stock_details>& stock, std::string& error);
private :
	class ken_app_db_impl;
	ken_app_db_impl& d_;
};