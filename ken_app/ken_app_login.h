
// STL Libraries

#include<iostream>
#include <string>

#include "state.h"



// ken_app_gui login
class ken_app_login : public gui {

	state& app_state;
	const std::string home_page_name;

	// overriding all the virtual functions
	
	void on_caption() override;
	void on_stop() override;
	void on_shutdown() override;

	void on_login();

public :
	// constructor 
	ken_app_login(const std::string& guid, state& app_state);



	// destructor
	~ken_app_login();

	bool layout(gui::page& persistent_page,
		gui::page& home_page,
		std::string& error) override;
};

