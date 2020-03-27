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
	void on_stop() override;
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

private:
	const ken_app_db::stock_details details_;
};



