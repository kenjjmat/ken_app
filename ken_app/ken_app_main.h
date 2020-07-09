#pragma once	

#include "state.h"

//STL libraries 
#include <sstream>



// gui main class 

class ken_app_main : public gui {
	state& app_state_;
	const std::string home_page_name;
	const int margin = 10;
	const std::string page_name = "Appointment";
	

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

	// on_clicking function for redirecting to users 
	void on_users();

	// on clicking function for redirecting to share files 
	void on_share();

	// selecting the folder where the files are
	void on_upload();

	// hiding and showing widgets
	void hide_info(std::vector<std::string> aliases);
	void show_info(std::vector<std::string> aliases);

	// function for on_selection of the listviews
	void on_stock_list();
	void on_appointment_list();
	void on_sales_list();
	void on_users_list();
	void on_appointment_completed();

public :
	ken_app_main(const std::string& guid, state& app_state );
	~ken_app_main();

	bool layout(gui::page& persistent_page,
		gui::page& home_page,
		std::string& error) override;
	
};


