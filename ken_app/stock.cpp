#include "stock.h"	
#include "resource.h"

void stock::on_stop(){
	close();
}

void stock::on_save(){

}


stock::~stock(){
}

bool stock::layout(gui::page& persistent_page, gui::page& home_page, std::string& error)
{
	// set caption 
	home_page.set_name(home_page_name);

	//set form parameters
	set_width(1000);
	set_min_width(1000);
	set_height(500);
	set_min_height(500);


	return true;
}

bool stock::saved()
{
	return false;
}
