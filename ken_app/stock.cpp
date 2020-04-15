#include "stock.h"	
#include "resource.h"

void stock::on_caption(){
	gui::prompt_params param;
	param.type = gui::prompt_type::ok;
	prompt(param, "", app_state_.version_info());
}

void stock::on_stop(){
	close();
}

void stock::on_shutdown()
{
}

void stock::on_new_stock()
{
}

void stock::on_new_item()
{
}

void stock::on_save(){

}

stock::stock(state& app_state): 
	home_page_name("Add new Stock Item"),
	app_state_(app_state)
{
}


stock::~stock(){
}

bool stock::layout(gui::page& persistent_page, gui::page& home_page, std::string& error)
{
	// set caption 
	home_page.set_name(home_page_name);

	// do not allow the window to be resized
	prevent_resizing();

	//set form parameters
	set_width(350);
	set_min_width(350);
	set_height(500);
	set_min_height(500);


	// add name 
	widgets::text name_caption;
	name_caption.alias = "name_caption";
	name_caption.text_value = "Name";
	name_caption.color = color{ 180 , 180 , 180 };
	name_caption.rect.left = 10;
	name_caption.rect.top = 10;
	name_caption.rect.set_height(20);
	name_caption.rect.set_width(50);

	home_page.add_text(name_caption);


	return true;
}

bool stock::saved()
{
	return false;
}
