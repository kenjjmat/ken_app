#pragma once

#include <iostream>
#include <string>

#include "state.h"


//ken_app_new_user
class ken_app_new_user :
	public gui {
	state& app_state_;
	const std::string home_page_name;

	// void on_caption() override;
	void on_stop() override;
	void on_shutdown() override;

	void on_save();
	void on_password();

public:
	ken_app_new_user(const std::string& guid, state& app_state_);

	// destructor 
	~ken_app_new_user();

	bool layout(gui::page& persistent_page, gui::page& home_page,
		std::string& error) override;
};
