#pragma once	

#include "state.h"



// gui main class 

class ken_app_main : public gui {
	state& app_state_;
	const std::string home_page_name;
	bool update_ = false;

	void on_caption() override;
	void on_stop() override;
	void on_shutdown() override;

	// on_click function for redirecting to the main windows
	void on_stock();
	void on_appointment();
	void on_sales();

public :
	ken_app_main(const std::string& guid, state& app_state);
	~ken_app_main();

	bool layout(gui::page& persistent_page,
		gui::page& home_page,
		std::string& error) override;
};


