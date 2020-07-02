#include "sales.h"

// when you click the home page name
void sales::on_caption(){
	gui::prompt_params params;
	params.type = gui::prompt_type::ok;
	prompt(params, "", app_state_.version_info());
}

void sales::on_stop(){
	close();
}

void sales::on_shutdown()
{
}

// adding the information on the listview 
void sales::on_add()
{
	std::string error;
	ken_app_db::sales_details sales;
	// getting the edit box values
	get_editbox_text(home_page_name + "/Item_name_edit", sales.item_name, error);
	get_editbox_text(home_page_name + "/unit_price_edit", sales.Unit_price, error);
	get_editbox_text(home_page_name + "/cost_edit", sales.Cost, error);
	get_editbox_text(home_page_name + "/quantity_edit", sales.quantity, error);
}

// on clicking the button save
void sales::on_save()
{
}

// initializing the the constructor 
sales::sales(state& app_state) : 
home_page_name("Add a new Sales Item"),
app_state_(app_state){
}

sales::~sales()
{
}

bool sales::layout(gui::page& persistent_page, gui::page& home_page, std::string& error){
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
	name_caption.alias = "item_name_caption";
	name_caption.text_value = "Item Name";
	name_caption.color = color{ 180 , 180 , 180 };
	name_caption.rect.left = 10;
	name_caption.rect.top = 10;
	name_caption.rect.set_height(20);
	name_caption.rect.set_width(100);

	home_page.add_text(name_caption);


	// add a name textbox
	widgets::editbox item_name_edit;
	item_name_edit.alias = "item_name_edit";
	item_name_edit.rect.left = name_caption.rect.left;
	item_name_edit.rect.top = name_caption.rect.bottom + 3;
	item_name_edit.rect.set_height(20);
	item_name_edit.rect.set_width(300);

	home_page.add_editbox(item_name_edit);

	//add Unit price caption
	widgets::text unit_price_caption;
	unit_price_caption.alias = "unit_price_caption";
	unit_price_caption.text_value = "Unit Price";
	unit_price_caption.color = color{ 180 , 180 , 180 };
	unit_price_caption.rect.left = item_name_edit.rect.left;
	unit_price_caption.rect.top = item_name_edit.rect.bottom + 25;
	unit_price_caption.rect.set_height(20);
	unit_price_caption.rect.set_width(100);

	home_page.add_text(unit_price_caption);

	// add a unit_price textbox 
	widgets::editbox unit_price;
	unit_price.alias = "unit_price_edit";
	unit_price.rect.left = unit_price_caption.rect.left;
	unit_price.rect.top = unit_price_caption.rect.bottom + 3;
	unit_price.rect.set_height(20);
	unit_price.rect.set_width(100);
	unit_price.allowed_set = "0123456789";

	home_page.add_editbox(unit_price);

	// add caption for cost 
	widgets::text cost_caption;
	cost_caption.alias = "cost_caption";
	cost_caption.text_value = "Cost";
	cost_caption.color = color{ 180 , 180 , 180 };
	cost_caption.rect.left = unit_price_caption.rect.right + 20;
	cost_caption.rect.top = unit_price_caption.rect.top;
	cost_caption.rect.set_height(20);
	cost_caption.rect.set_width(100);

	home_page.add_text(cost_caption);

	// add cost textbox 
	widgets::editbox cost;
	cost.alias = "cost_edit";
	cost.read_only = true;
	cost.rect.left = cost_caption.rect.left;
	cost.rect.top = cost_caption.rect.bottom + 3;
	cost.rect.set_height(20);
	cost.rect.set_width(100);
	cost.allowed_set = "0123456789";

	home_page.add_editbox(cost);


	// add caption for quantity
	widgets::text quantity_caption;
	quantity_caption.alias = "quantity_caption";
	quantity_caption.text_value = "Quantity";
	quantity_caption.color = color{ 180 , 180 , 180 };
	quantity_caption.rect.left = cost_caption.rect.right + 20;
	quantity_caption.rect.top = cost_caption.rect.top;
	quantity_caption.rect.set_height(20);
	quantity_caption.rect.set_width(80);

	home_page.add_text(quantity_caption);


	// add a quantity textbox 
	widgets::editbox quantity;
	quantity.alias = "quantity_edit";
	quantity.rect.left = cost.rect.right + 20;
	quantity.rect.top = cost.rect.top;
	quantity.rect.set_height(20);
	quantity.rect.set_width(90);
	quantity.allowed_set = "0123456789";

	home_page.add_editbox(quantity);

	// add a button to add the added items into a listview
	widgets::button add;
	add.tooltip = "Add items";
	add.rect.left = cost.rect.left;
	add.rect.top = quantity.rect.bottom + 20;
	add.rect.set_height(25);
	add.rect.set_width(60);
	add.caption = "Add";

	home_page.add_button(add);

	// putting a border between the essential editbox
	widgets::groupbox box;
	box.rects = {
		unit_price.rect , 
		unit_price_caption.rect , 
		cost.rect , 
		cost_caption.rect , 
		quantity.rect , 
		quantity_caption.rect , 
		add.rect
	};

	home_page.add_groupbox(box);

	// add a listview 

	widgets::listview listview;
	listview.border = false;
	listview.gridlines = true;
	listview.rect.left = 10;
	listview.rect.top = add.rect.bottom + 10;
	listview.rect.set_height(150);
	listview.rect.set_width(330);
	listview.columns = {
		app_state_.column_details("ID", 50 , widgets::listview_column_type::int_),
		app_state_.column_details("Item Name", 100 , widgets::listview_column_type::string_),
		app_state_.column_details("Unit Price", 50 , widgets::listview_column_type::int_),
		app_state_.column_details("Cost", 50 , widgets::listview_column_type::int_),
		app_state_.column_details("Quantity", 50 , widgets::listview_column_type::int_)
	};
	home_page.add_listview(listview);

	// add a sava button 
	widgets::button save;
	save.caption = "Save";
	save.rect.left = 130;
	save.rect.top = listview.rect.bottom + 15;
	save.rect.set_height(25);
	save.rect.set_width(80);

	home_page.add_button(save);
	return true;
}

bool sales::saved()
{
	return false;
}
