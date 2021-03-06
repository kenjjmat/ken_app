#pragma once
#include <iostream>
#include <string>
#include "state.h"


class stock : public gui {
	state& app_state_;
	const std::string home_page_name;
	const int margin = 10;
	bool saved_ = false;

	// overiding all the virtual functions
	void on_caption() override;
	void on_stop() override;
	void on_shutdown() override;


	// functions for adding new stock/ item to the database
	void on_add();
	void on_save();
public : 
	// constructor 
	stock(state& app_state);

	//destructor
	~stock();

	bool layout(gui::page& persistent_page,
		gui::page& home_page,
		std::string& error)override;
	bool saved();
	const ken_app_db::stock_details& get_details();

private:
	ken_app_db::stock_details details_;
};



