#pragma once	

#include "state.h"



// gui main class 

class ken_app_main : public gui {
	state& app_state_;
	const std::string home_page_name;
	bool update_ = false;

	// overriding virtual functions
	void on_caption() override;
	void on_stop() override;
	void on_shutdown() override;

	// on_click function for redirecting to the stock windows
	void on_stock();
	void on_add_stock();

	// on_click function for redirecting to the sales window 
	void on_sales();
	void on_add_sales();

	// on_clicking function for redirecting to appontments window 
	void on_appoinment();
	void on_add_appointment();

public :
	ken_app_main(const std::string& guid, state& app_state);
	~ken_app_main();

	bool layout(gui::page& persistent_page,
		gui::page& home_page,
		std::string& error) override;
};


