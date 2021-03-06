#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>

class  ken_app_db {
public :

	// username and password
	struct user_credentials {
		std::string username;
		std::string id;
	};

	// stock details
	struct stock_details {
		std::string id;
		std::string name;
		std::string description;
		 std::string quantity;
		std::vector<stock_details>items;
	};

	// sales details 
	struct sales_details
	{
		std::string id;
		std::string item_name;
		std::string quantity;
		std::string Unit_price;
		std::string Cost;
		std::vector<sales_details> items;
	};

	// appointments details 
	struct appointments_details
	{
		std::string id;
		std::string time;
		std::string date;
		std::string name;
		std::string surname;
		std::string description;
		std::vector<appointments_details>items;
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

	// sales 
	bool new_sales(const sales_details& sales_info, std::string& error);
	bool get_sales(const std::string& sales_id, sales_details& sales, std::string& error);
	bool get_sales_all(std::vector<sales_details>& sales, std::string& error);

	//appointment
	bool new_appointment(const appointments_details& appointment_info, std::string& error);
	bool get_appointment(const std::string& appointment_id, appointments_details& appointment, std::string& error);
	bool get_appointments(std::vector<appointments_details>& appointment, std::string& error);

	// delete operation 
	bool delete_item(const std::string& id,std::string& database_name,  std::string& error);

private :
	//hiding the implementation of the database by only showing the essentials
	class ken_app_db_impl;
	ken_app_db_impl& d_;
};