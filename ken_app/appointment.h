#pragma once
#include <iostream>
#include <string>
#include "state.h"


class appointment : public gui {
	state& app_state_;
	const std::string home_page_name;
	const int margin = 10;
	bool saved_ = false;
	const std::string page_name = "Appointment";

	// overiding all the virtual functions
	void on_caption() override;
	void on_stop() override;
	void on_shutdown() override;


	// functions for adding new appointment/ item to the database
	void on_add();
	void on_save();

public:
	// constructor 
	appointment(state& app_state);

	//destructor
	~appointment();

	bool layout(gui::page& persistent_page,
		gui::page& home_page,
		std::string& error)override;
	bool saved();
	const ken_app_db::appointments_details& get_details();

private:
	 ken_app_db::appointments_details details_;
}; 
