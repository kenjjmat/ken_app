#include "ken_app_main.h"

void ken_app_main::on_caption()
{
}

void ken_app_main::on_stop(){
	stop();
}

void ken_app_main::on_shutdown()
{
}

ken_app_main::ken_app_main(const std::string& guid, state& main_state) : 
	home_page_name("Main_form"),
	main_state(main_state), 
	gui::gui(guid){
}

ken_app_main::~ken_app_main()
{
}

bool ken_app_main::layout(gui::page& persistent_page, gui::page& home_page, std::string& error) {

	// set parameters
	home_page.set_name(home_page_name);

	//allow the app to exit
	allow_quit();

	return true;
}
