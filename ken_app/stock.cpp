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


void stock::on_add(){
	std::string error;
	std::string stock_id;
	ken_app_db::stock_details details;

	// getting all the information in the textboxes
	get_editbox_text(home_page_name + "/name_edit", details.name, error);
	get_editbox_text(home_page_name + "/description_edit", details.description, error);
	get_editbox_text(home_page_name + "/quantity_edit", details.quantity, error);

	details.id = unique_string_short();
	stock_id = details.id;

	std::vector<widgets::listview_column> columns;
	std::vector<widgets::listview_row> data;

	get_listview(home_page_name + "/stock_list", columns, data, error);

	widgets::listview_row row = {
		{{{"ID"} , {stock_id}},
		{{"Name"} , {details.name}},
		{{ "Description"}, {details.description}},
		{{"Quantity"} , {details.quantity}}}
	};
		
	//add the row to the listview
	add_listview_row(home_page_name + "/stock_list", row, true , error);
}

void stock::on_save(){
	
	// get the listview data and save it to the page listview 
	std::vector<widgets::listview_column> columns;
	std::vector<widgets::listview_row> data;
	std::string error;
	ken_app_db::stock_details stock;
	get_listview(home_page_name + "/stock_list", columns, data, error);

	for (auto& row : data) {
		ken_app_db::stock_details details;
		for (auto& item : row.items) {
			
			if (item.column_name == "Name")
				details.name = item.item_data;
			else
				if (item.column_name == "ID")
					details.id = item.item_data;
				else
					if (item.column_name == "Description")
						details.description = item.item_data;
					else
						if (item.column_name == "Quantity")
							details.quantity = item.item_data;
		}
		stock.items.push_back(details);
		stock = details;
	}

	//saving the information from the listview into the database
	if (!app_state_.get_db().new_stock(stock, error)) {
		prompt_params params;
		params.type = prompt_type::ok;
		prompt(params, "Error", error);
	}
	saved_ = true;
	details_ = stock;
	close();
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
	description.alias = "description_edit";
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
	quantity.alias = "quantity_edit";
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
	add.on_click = [&]() { on_add(); };

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
	listview.alias = "stock_list";
	listview.border = false;
	listview.gridlines = true;
	listview.rect.left = 10;
	listview.rect.top = description.rect.bottom + 10;
	listview.rect.set_height(150);
	listview.rect.set_width(330);

	listview.columns = {
	app_state_.column_details("ID", 100 , widgets::listview_column_type::string_, color { 0, 0, 0}),
	app_state_.column_details("Name", 100 , widgets::listview_column_type::string_, color { 0, 0, 0}),
	app_state_.column_details("Description", 100 , widgets::listview_column_type::string_, color { 0, 0, 0}),
	app_state_.column_details("Quantity", 80 , widgets::listview_column_type::int_,color { 0, 0, 0})
	};

	home_page.add_listview(listview);

	// add a sava button 
	widgets::button save;
	save.caption = "Save";
	save.rect.left = 130;
	save.rect.top = listview.rect.bottom + 20;
	save.rect.set_height(25);
	save.rect.set_width(80);
	save.on_click = [&]() {on_save(); };

	home_page.add_button(save);


	return true;
}

bool stock::saved()
{
	return saved_;
}

const ken_app_db::stock_details& stock::get_details()
{
	return details_;
}
