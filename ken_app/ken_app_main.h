#pragma once	

#include "state.h"



// gui main class 

class ken_app_main : public gui {
	state& main_state;
	const std::string home_page_name;

	void on_caption() override;
	void on_stop() override;
	void on_shutdown() override;

public :
	ken_app_main(const std::string& guid, state& main_state);
	~ken_app_main();

	bool layout(gui::page& persistent_page,
		gui::page& home_page,
		std::string& error) override;
};


