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
	set_height(400);
	set_min_height(400);


	// add name caption
	widgets::text name_caption;
	name_caption.alias = "name_caption";
	name_caption.text_value = "Name";
	name_caption.color = color{ 180 , 180 , 180 };
	name_caption.rect.left = 10;
	name_caption.rect.top = 10;
	name_caption.rect.set_height(20);
	name_caption.rect.set_width(100);

	home_page.add_text(name_caption);


	// add a name textbox
	widgets::editbox name_edit;
	name_edit.alias = "name_edit";
	name_edit.rect.left = name_caption.rect.left;
	name_edit.rect.top = name_caption.rect.bottom + 3;
	name_edit.rect.set_height(20);
	name_edit.rect.set_width(300);

	home_page.add_editbox(name_edit);

	//add description caption
	widgets::text description_caption;
	description_caption.alias = "description_caption";
	description_caption.text_value = "Description";
	description_caption.color = color{ 180 , 180 , 180 };
	description_caption.rect.left = name_edit.rect.left;
	description_caption.rect.top = name_edit.rect.bottom + 25;
	description_caption.rect.set_height(20);
	description_caption.rect.set_width(100);

	home_page.add_text(description_caption);

	// add a description textbox 
	widgets::editbox description;
	description.rect.left = description_caption.rect.left;
	description.rect.top = description_caption.rect.bottom + 3;
	description.rect.set_height(50);
	description.rect.set_width(180);
	description.scrollbar = true;


	home_page.add_editbox(description);

	// add caption for quantity
	widgets::text quantity_caption;
	quantity_caption.alias = "quantity_caption";
	quantity_caption.text_value = "Quantity";
	quantity_caption.color = color{ 180 , 180 , 180 };
	quantity_caption.rect.left = description.rect.right + 40;
	quantity_caption.rect.top = description_caption.rect.top;
	quantity_caption.rect.set_height(20);
	quantity_caption.rect.set_width(100);

	home_page.add_text(quantity_caption);


	// add a quantity textbox 
	widgets::editbox quantity;
	quantity.rect.left = description.rect.right + 40;
	quantity.rect.top = description.rect.top;
	quantity.rect.set_height(20);
	quantity.rect.set_width(80);
	quantity.allowed_set = "0123456789";

	home_page.add_editbox(quantity);

	// add a button to add the added items into a listview
	widgets::button add;
	add.tooltip = "Add items";
	add.rect.left = description.rect.right + 45;
	add.rect.top = quantity.rect.bottom + 6;
	add.rect.set_height(25);
	add.rect.set_width(60);
	add.caption = "Add";

	home_page.add_button(add);

	// putting a border between the essential editbox
	widgets::groupbox box;
	box.rects = {
		description_caption.rect ,
		description.rect,
		quantity.rect,
		quantity_caption.rect,
		add.rect
	};

	home_page.add_groupbox(box);

	// add a listview 

	widgets::listview listview;
	listview.border = false;
	listview.gridlines = true;
	listview.rect.left = 10;
	listview.rect.top = description.rect.bottom + 10;
	listview.rect.set_height(150);
	listview.rect.set_width(330);

	//listview.columns = {
	//	{ "Name" , 180  , widgets::listview_column_type::string_},
	//	{"Description" , 200 , widgets::listview_column_type::string_},
	//	{"Quantity" , 60 , widgets::listview_column_type::int_ }
	//};

	home_page.add_listview(listview);

	// add a sava button 
	widgets::button save;
	save.caption = "Save";
	save.rect.left = 130;
	save.rect.top = listview.rect.bottom + 20;
	save.rect.set_height(25);
	save.rect.set_width(80);

	home_page.add_button(save);


	return true;
}

bool stock::saved()
{
	return false;
}
