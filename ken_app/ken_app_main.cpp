#include "ken_app_main.h"
#include "stock.h"
#include "sales.h"
#include "appointment.h"

// sync folder location
#include "sync.h"

void ken_app_main::on_caption(){
	gui::prompt_params params;
	params.type = gui::prompt_type::ok;
	prompt(params, "", app_state_.version_info());

	
}

void ken_app_main::on_stop(){
	// checking to see if the user really wants to close 
	gui::prompt_params params;
	params.type = prompt_type::yes_no;
	if(prompt(params, "", "Do you really want to exist"))
	close();
}

void ken_app_main::on_shutdown()
{
}

// when the stock image has been clicked
void ken_app_main::on_stock() {

	if (!page_exists("Stock")) {
		page page("Stock");

		//add back icon 
		widgets::image back;
		back.toggle = "Previous page";
		back.filename = "back.png";
		back.rect = { margin, margin + app_state_.icon_size.width , margin, margin + app_state_.icon_size.height };
		back.on_click = [&]() {
			show_previous_page();
			// to-do::
			// this is where you put the code for updating the home page when there are new appointments to be put on the home page
		};

		page.add_image(back);

		// add tittle 
		widgets::text title;
		title.text_value = "Stock";
		title.font_size = 16;
		title.rect = { back.rect.right + margin , back.rect.right + margin + app_state_.title_width, back.rect.top, back.rect.top + (app_state_.icon_size.height) };
		page.add_text(title);

		// add description 

		widgets::text description;
		description.text_value = "View and Manage Inventory";
		description.color = color{ 180 , 180 , 180 };
		description.rect.left = back.rect.right + 10;
		description.rect.top = title.rect.top + 30;
		description.rect.set_height(20);
		description.rect.set_width(200);

		page.add_text(description);
		// adding a a search space on the form 
		widgets::editbox search;
		search.alias = "search";
		search.cue_banner = "search " + home_page_name;
		search.rect.left = description.rect.right + 525;
		search.rect.top = description.rect.top;
		search.rect.set_height(22);
		search.rect.set_width(200);
		search.rect.top = description.rect.top;
		search.control_to_invoke_alias = "Enter";
		search.on_resize.perc_h = 100;
		search.on_resize.perc_v = 0;

		page.add_editbox(search);

		//add icon for the search bar
		widgets::image icon_search;
		icon_search.filename = "search.png";
		icon_search.tight_fit = true;
		icon_search.rect.left = description.rect.right + 495;
		icon_search.rect.top = search.rect.top;
		icon_search.rect.set_height(22);
		icon_search.rect.set_width(30);
		icon_search.on_resize.perc_h = 100;
		icon_search.on_resize.perc_v = 0;

		page.add_image(icon_search);

		widgets::groupbox box;
		box.rects = {
			icon_search.rect ,
			search.rect
		};
		box.on_resize.perc_h = 100;
		box.on_resize.perc_v = 0;
		page.add_groupbox(box);


		// add image 

		widgets::image image;
		image.bar = false;
		image.filename = "stock.png";
		image.rect.left = back.rect.left;
		image.rect.top = description.rect.bottom + 20;
		image.rect.set_height(100);
		image.rect.set_width(100);

		page.add_image(image);



		// adding a group box 

		widgets::groupbox border;
		border.rects = {
			image.rect
		};

		page.add_groupbox(border);

		// add "add stock item " icon 
		widgets::image image_add;
		image_add.filename = "add.png";
		image_add.tooltip = "Add a new Item";
		image_add.rect.left = back.rect.left;
		image_add.rect.top = image.rect.bottom + 10;
		image_add.rect.set_height(30);
		image_add.rect.set_width(30);
		image_add.change_color = true;
		image_add.color.color = color{ 0, 150, 140 };
		image_add.color.color_hot = color{ 21, 79, 139 };
		image_add.color.color_border_hot = image_add.color.color_border;
		image_add.color_background_hot = image_add.color_background_hot;
		image_add.tight_fit = true;
		image_add.on_click = [&] { on_add_stock(); };

		page.add_image(image_add);

		// add image edit
		widgets::image image_edit;
		image_edit.filename = "edit.png";
		image_edit.tooltip = "Edit Item";
		image_edit.rect.left = image_add.rect.right + 10;
		image_edit.rect.top = image.rect.bottom + 10;
		image_edit.rect.set_height(27);
		image_edit.rect.set_width(27);
		image_edit.change_color = true;
		image_edit.color.color = color{ 0, 150, 140 };
		image_edit.color.color_hot = color{ 21, 79, 139 };
		image_edit.color.color_border_hot = image_edit.color.color_border;
		image_edit.color_background_hot = image_edit.color_background_hot;
		image_edit.tight_fit = true;

		page.add_image(image_edit);

		// add the delete image 
		widgets::image image_delete;
		image_delete.filename = "delete.png";
		image_delete.tooltip = "Delete Item";
		image_delete.rect.left = image_edit.rect.right + 10;
		image_delete.rect.top = image.rect.bottom + 10;
		image_delete.rect.set_height(27);
		image_delete.rect.set_width(27);
		image_delete.change_color = true;
		image_delete.color.color = color{ 255, 0, 0 };
		image_delete.color.color_hot = color{ 21, 79, 139 };
		image_delete.color.color_border_hot = image_delete.color.color_border;
		image_delete.color_background_hot = image_delete.color_background_hot;
		image_delete.tight_fit = true;
		image_delete.on_click = [&]() {on_delete_stock(); };

		page.add_image(image_delete);

		// add stock listview 
		widgets::listview stock_list;
		stock_list.alias = "Stock_list";
		stock_list.rect.left = image.rect.right + 50;
		stock_list.rect.top = image.rect.top;
		stock_list.rect.set_height(400);
		stock_list.rect.set_width(400);
		stock_list.border = true;
		stock_list.sort_by_clicking_column = true;
		stock_list.on_selection = [&]() {on_stock_list(); };
		// on resize 
		stock_list.on_resize.perc_h = 0;
		stock_list.on_resize.perc_v = 0;
		stock_list.on_resize.perc_height = 105;
		stock_list.on_resize.perc_width = 25;


		stock_list.columns = {
			app_state_.column_details("ID", 80 , widgets::listview_column_type::int_,color {0, 0 , 0}),
			app_state_.column_details("Name" , 170 , widgets::listview_column_type::string_, color {0, 0 , 0}),
			app_state_.column_details("Description" , 200 , widgets::listview_column_type::string_, color {0, 0 , 0}),
			app_state_.column_details("Quantity" , 100  , widgets::listview_column_type::int_, color {0, 0 , 0})
		};
		stock_list.unique_column_name = "ID";

		{
			std::vector<ken_app_db::stock_details> stock;
			std::string error;

			if (app_state_.get_db().get_stock_all(stock, error)) {
				int i = 0;

				for (const auto& stock_ : stock) {
					widgets::listview_row row;
					row.items.push_back({ "ID", stock_.id });
					row.items.push_back({ "Name" , stock_.name });
					row.items.push_back({ "Description" , stock_.description });
					row.items.push_back({ "Quantity", stock_.quantity });
					stock_list.data.push_back(row);
					app_state_.count++;
					// typecasting a string into a double
					double answer;
					std::stringstream ss;
					ss << stock_.quantity;
					ss >> answer;
					app_state_.quantity += answer;
				}
				
			}
		}
		page.add_listview(stock_list);

		// name caption
		widgets::text name_caption;
		name_caption.alias = "name_caption";
		name_caption.text_value = "Name";
		name_caption.rect.left = stock_list.rect.right + margin + 40;
		name_caption.rect.top = stock_list.rect.top;
		name_caption.rect.set_height(20);
		name_caption.rect.set_width(300);
		name_caption.color = { 180, 180 , 180 };
		name_caption.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		name_caption.on_resize.perc_v = 0;

		page.add_text(name_caption);

		// name value text 
		widgets::text name;
		name.alias = "Name";
		name.rect.left = name_caption.rect.left;
		name.rect.top = name_caption.rect.bottom + (margin / 2);
		name.rect.set_height(20);
		name.rect.set_width(300);
		name.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		name.on_resize.perc_v = 0;

		page.add_text(name);

		// descritption caption
		widgets::text description_caption;
		description_caption.alias = "description_caption";
		description_caption.text_value = "Description";
		description_caption.rect.left = name.rect.left;
		description_caption.rect.top = name.rect.bottom + margin;
		description_caption.rect.set_height(20);
		description_caption.rect.set_width(300);
		description_caption.color = { 180 , 180 , 180 };
		description_caption.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		description_caption.on_resize.perc_v = 5;
		description_caption.on_resize.perc_height = 5;

		page.add_text(description_caption);

		// description_ text value 
		widgets::text description_;
		description_.alias = "description_";
		description_.rect.left = description_caption.rect.left;
		description_.rect.top = description_caption.rect.bottom + (margin / 2);
		description_.rect.set_height(20);
		description_.rect.set_width(300);
		description_.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		description_.on_resize.perc_v = 5;

		page.add_text(description_);

		// descritption caption
		widgets::text quantity_caption;
		quantity_caption.alias = "caption_quantity";
		quantity_caption.text_value = "Quantity";
		quantity_caption.rect.left = description_.rect.left;
		quantity_caption.rect.top = description_.rect.bottom + margin;
		quantity_caption.rect.set_height(20);
		quantity_caption.rect.set_width(300);
		quantity_caption.color = { 180 , 180 , 180 };
		quantity_caption.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		quantity_caption.on_resize.perc_v = 5;

		page.add_text(quantity_caption);

		// quantity text value 
		widgets::text quantity;
		quantity.alias = "Quantity";
		quantity.rect.left = quantity_caption.rect.left;
		quantity.rect.top = quantity_caption.rect.bottom + (margin / 2);
		quantity.rect.set_height(20);
		quantity.rect.set_width(300);
		quantity.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		quantity.on_resize.perc_v = 5;

		page.add_text(quantity);


		// adding a barchart 
		widgets::barchart bar;
		bar.alias = "barchart";
		bar.data.autocolor = false;
		bar.data.x_label = "Fields";
		bar.data.y_label = "Data";
		bar.data.caption = " Stock BarChart";
		bar.rect.left = stock_list.rect.right + 50;
		bar.rect.top = stock_list.rect.top + 170;
		bar.rect.set_height(230);
		bar.rect.set_width(300);
		bar.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		bar.on_resize.perc_v = stock_list.on_resize.perc_height  - 50;
		bar.on_resize.perc_height = 50;
		bar.on_resize.perc_width = 50;


		// setting out the bars in the barchart
		std::vector<widgets::chart_entry>bar_data;

		// creating object for the chart entry
		widgets::chart_entry data;
		data.color = { 180 , 180, 180 };
		data.label = "Stock";
		data.value = app_state_.count;
		bar_data.push_back(data);
		

		//creating a new object of chart entry 
		widgets::chart_entry data2;
		data2.label = "Quantity";
		data2.value = app_state_.quantity;
		data2.color = { 180, 200, 255 };
		bar_data.push_back(data2);
		bar.data.bars = bar_data;
		page.add_barchart(bar);
		// this is adding a new page to the form 
		add_page(page);

		//resetting the variables to zero 
		app_state_.count = 0;
		app_state_.quantity = 0;
	}
	show_page("Stock");
	// hiding the stock information
	{
		std::vector<std::string> aliases;
		aliases.push_back("Stock/name_caption");
		aliases.push_back("Stock/description_caption");
		aliases.push_back("Stock/caption_quantity");
		aliases.push_back("Stock/Name");
		aliases.push_back("Stock/description_");
		aliases.push_back("Stock/Quantity");

		hide_info(aliases);

	}
}

// when the add stock image has been clicked
void ken_app_main::on_add_stock(){
	//creating a object of type stock and passing in the state object
	stock stock(app_state_);
	stock.modal(*this);

	std::string error;
	if (!stock.run(error)) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", error);
		return;
	}
	
	if (stock.saved()) {

		// declaring variables 
		const auto& stock_info = stock.get_details();

		//getting the data from the database 
		std::vector<widgets::listview_row> data;
		std::vector<ken_app_db::stock_details> stocks;
		widgets::listview_row row;
		if (app_state_.get_db().get_stock_all(stocks, error)) {
			for (const auto& stock_ : stocks) {
				
				row.items.push_back({ "ID", stock_.id });
				row.items.push_back({ "Name", stock_.name });
				row.items.push_back({ "Description", stock_.description });
				row.items.push_back({ "Quantity", stock_.quantity });
				data.push_back(row);

				// setting the variables back to zero 
				
				app_state_.count++;
				double	quantity;
				std::stringstream ss;
				ss << stock_.quantity;
				ss >> quantity;
				app_state_.quantity += quantity;
			}
		}
	    widgets::barchart_data  bar_data;
		bar_data.autocolor = false;
		bar_data.autoscale = true;
		bar_data.caption = "Stock Barchart";
		bar_data.x_label = "Fields";
		bar_data.y_label = "Data";


		// setting out the bars in the barchart
		std::vector<widgets::chart_entry>bar_data_;

		// creating object for the chart entry
		widgets::chart_entry entry;
		entry.color = { 180 , 180, 180 };
		entry.label = "Stock";
		entry.value = app_state_.count;
		bar_data_.push_back(entry);


		//creating a new object of chart entry 
		widgets::chart_entry entry2;
		entry2.label = "Quantity";
		entry2.value = app_state_.quantity;
		entry2.color = { 180, 200, 255 };
		bar_data_.push_back(entry2);
		bar_data.bars = bar_data_;

		repopulate_listview("Stock/Stock_list", data, error);
		barchart_reload("Stock/barchart", bar_data, error);
	
		
	}
	//resetting variables
	app_state_.count = 0;
	app_state_.quantity = 0;
	
}

// when the sales icon is clicked
void ken_app_main::on_sales(){

	if (!page_exists("Sales")) {
		page page("Sales");


		//add back icon 
		widgets::image back;
		back.toggle = "Previous page";
		back.filename = "back.png";
		back.rect.left = 10;
		back.rect.top = 10;
		back.rect.set_height(50);
		back.rect.set_width(40);
		back.on_click = [&]() {
			show_previous_page();
			// to-do::
			// this is where you put the code for updating the home page when there are new appointments to be put on the home page
		};

		page.add_image(back);

		// add tittle 
		widgets::text title;
		title.text_value = "Sales";
		title.font_size = 16;
		title.rect.left = back.rect.right + 10;
		title.rect.top = back.rect.top;
		title.rect.set_height(40);
		title.rect.set_width(100);

		page.add_text(title);

		// add description 
		widgets::text description;
		description.text_value = "The Daily Running of Business";
		description.color = color{ 180 , 180 , 180 };
		description.rect.left = back.rect.right + 10;
		description.rect.top = title.rect.top + 30;
		description.rect.set_height(20);
		description.rect.set_width(200);

		page.add_text(description);

		// adding a a search space on the form 
		widgets::editbox search;
		search.alias = "search";
		search.cue_banner = "search " + home_page_name;
		search.rect.left = description.rect.right + 525;
		search.rect.top = description.rect.top;
		search.rect.set_height(22);
		search.rect.set_width(200);
		search.rect.top = description.rect.top;
		search.control_to_invoke_alias = "Enter";
		search.on_resize.perc_h = 100;
		search.on_resize.perc_v = 0;

		page.add_editbox(search);

		//add icon for the search bar
		widgets::image icon_search;
		icon_search.filename = "search.png";
		icon_search.tight_fit = true;
		icon_search.rect.left = description.rect.right + 495;
		icon_search.rect.top = search.rect.top;
		icon_search.rect.set_height(22);
		icon_search.rect.set_width(30);
		icon_search.on_resize.perc_h = 100;
		icon_search.on_resize.perc_v = 0;

		page.add_image(icon_search);

		widgets::groupbox box;
		box.rects = {
			icon_search.rect ,
			search.rect
		};
		box.on_resize.perc_h = 100;
		box.on_resize.perc_v = 0;
		page.add_groupbox(box);



		// add image 
		widgets::image image;
		image.bar = false;
		image.filename = "sales.png";
		image.rect.left = back.rect.left;
		image.rect.top = description.rect.bottom + 20;
		image.rect.set_height(100);
		image.rect.set_width(100);

		page.add_image(image);

		// adding a group box 
		widgets::groupbox border;
		border.rects = {
			image.rect
		};

		page.add_groupbox(border);

		// add "add stock item " icon 
		widgets::image image_add;
		image_add.filename = "add.png";
		image_add.tooltip = "Add a new Item";
		image_add.rect.left = back.rect.left;
		image_add.rect.top = image.rect.bottom + 10;
		image_add.rect.set_height(30);
		image_add.rect.set_width(30);
		image_add.change_color = true;
		image_add.color.color = color{ 0, 150, 140 };
		image_add.color.color_hot = color{ 21, 79, 139 };
		image_add.color.color_border_hot = image_add.color.color_border;
		image_add.color_background_hot = image_add.color_background_hot;
		image_add.tight_fit = true;
		image_add.on_click = [&] { on_add_sales(); };

		page.add_image(image_add);

		// add image edit
		widgets::image image_edit;
		image_edit.filename = "edit.png";
		image_edit.tooltip = "Edit Item";
		image_edit.rect.left = image_add.rect.right + 10;
		image_edit.rect.top = image.rect.bottom + 10;
		image_edit.rect.set_height(27);
		image_edit.rect.set_width(27);
		image_edit.change_color = true;
		image_edit.color.color = color{ 0, 150, 140 };
		image_edit.color.color_hot = color{ 21, 79, 139 };
		image_edit.color.color_border_hot = image_edit.color.color_border;
		image_edit.color_background_hot = image_edit.color_background_hot;
		image_edit.tight_fit = true;

		page.add_image(image_edit);

		// add the delete image 
		widgets::image image_delete;
		image_delete.filename = "delete.png";
		image_delete.tooltip = "Delete Item";
		image_delete.rect.left = image_edit.rect.right + 10;
		image_delete.rect.top = image.rect.bottom + 10;
		image_delete.rect.set_height(27);
		image_delete.rect.set_width(27);
		image_delete.change_color = true;
		image_delete.color.color = color{ 255, 0, 0 };
		image_delete.color.color_hot = color{ 21, 79, 139 };
		image_delete.color.color_border_hot = image_delete.color.color_border;
		image_delete.color_background_hot = image_delete.color_background_hot;
		image_delete.tight_fit = true;
		image_delete.on_click = [&]() {on_delete_sales(); };
		page.add_image(image_delete);

		// add sales listview 
		widgets::listview sales_list;
		sales_list.alias = "sales_list_main";
		sales_list.rect.left = image.rect.right + 50;
		sales_list.rect.top = image.rect.top;
		sales_list.rect.set_height(400);
		sales_list.rect.set_width(400);
		sales_list.border = true;
		sales_list.on_selection = [&]() {on_sales_list(); };
		// on resize 
		sales_list.on_resize.perc_h = 0;
		sales_list.on_resize.perc_v = 5;
		sales_list.on_resize.perc_height = 90;
		sales_list.on_resize.perc_width = 25;

		sales_list.columns = {
			app_state_.column_details("ID", 100 , widgets::listview_column_type::int_ , color {0, 0 , 0}),
			app_state_.column_details("Item Name" , 110 , widgets::listview_column_type::string_, color {0, 0 , 0}),
			app_state_.column_details("Unit Price" , 100 , widgets::listview_column_type::string_, color {0, 0 , 0}),
			app_state_.column_details("Cost" , 100 , widgets::listview_column_type::string_,color {0, 0 , 0}),
			app_state_.column_details("Quantity" , 100  , widgets::listview_column_type::int_, color {0, 0 , 0})
		};
		sales_list.unique_column_name = "ID";

		{
			std::vector<ken_app_db::sales_details> sales;
			std::string error;

			if (app_state_.get_db().get_sales_all(sales, error)) {
				int i = 0;

				for (const auto& sales_ : sales) {
					widgets::listview_row row;
					row.items.push_back({ "ID", sales_.id , true , color {255, 0, 0} });
					row.items.push_back({ "Item Name" , sales_.item_name });
					row.items.push_back({ "Quantity" , sales_.quantity });
					row.items.push_back({ "Unit Price", sales_.Unit_price });
					row.items.push_back({ "Cost"	, sales_.Cost });
					sales_list.data.push_back(row);

					// counting the sales and cost
					app_state_.count++;
					double cost;
					std::stringstream ss;
					ss << sales_.Cost;
					ss >> cost;
					app_state_.quantity += cost;

				}
			}
		}
		
		page.add_listview(sales_list);
		// adding widgets for displaying the information on the listview
		// name caption
		widgets::text name_caption;
		name_caption.alias = "name_caption";
		name_caption.text_value = "Item Name";
		name_caption.rect.left = sales_list.rect.right + margin + 40;
		name_caption.rect.top = sales_list.rect.top;
		name_caption.rect.set_height(20);
		name_caption.rect.set_width(100);
		name_caption.color = { 180, 180 , 180 };
		name_caption.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		name_caption.on_resize.perc_v = 0;
		page.add_text(name_caption);

		// cost caption 
		widgets::text cost_caption;
		cost_caption.alias = "caption_cost";
		cost_caption.text_value = "Cost";
		cost_caption.rect.left = name_caption.rect.right + margin +40;
		cost_caption.rect.top = name_caption.rect.top;
		cost_caption.rect.set_height(20);
		cost_caption.rect.set_width(100);
		cost_caption.color = { 180, 180 , 180 };
		cost_caption.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		cost_caption.on_resize.perc_v = 0;
		page.add_text(cost_caption);
	

		// name value text 
		widgets::text name;
		name.alias = "Item Name";
		name.rect.left = name_caption.rect.left;
		name.rect.top = name_caption.rect.bottom + (margin / 2);
		name.rect.set_height(20);
		name.rect.set_width(300);
		name.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		name.on_resize.perc_v = 0;

		page.add_text(name);

		// cost value text 
		widgets::text cost;
		cost.alias = "cost";
		cost.rect.left = cost_caption.rect.left;
		cost.rect.top = cost_caption.rect.bottom + (margin / 2);
		cost.rect.set_height(20);
		cost.rect.set_width(300);
		cost.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		cost.on_resize.perc_v = 0;

		page.add_text(cost);

		// descritption caption
		widgets::text unit_price_caption;
		unit_price_caption.alias = "unit_price_caption";
		unit_price_caption.text_value = "Unit Price";
		unit_price_caption.rect.left = name.rect.left;
		unit_price_caption.rect.top = name.rect.bottom + margin;
		unit_price_caption.rect.set_height(20);
		unit_price_caption.rect.set_width(300);
		unit_price_caption.color = { 180 , 180 , 180 };
		unit_price_caption.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		unit_price_caption.on_resize.perc_v = 5;

		page.add_text(unit_price_caption);

		// description_ text value 
		widgets::text unit_price_;
		unit_price_.alias = "unit_price";
		unit_price_.rect.left = unit_price_caption.rect.left;
		unit_price_.rect.top = unit_price_caption.rect.bottom + (margin / 2);
		unit_price_.rect.set_height(20);
		unit_price_.rect.set_width(300);
		unit_price_.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		unit_price_.on_resize.perc_v = 5;

		page.add_text(unit_price_);

		// descritption caption
		widgets::text quantity_caption;
		quantity_caption.alias = "caption_quantity";
		quantity_caption.text_value = "Quantity";
		quantity_caption.rect.left = unit_price_.rect.left;
		quantity_caption.rect.top = unit_price_.rect.bottom + margin;
		quantity_caption.rect.set_height(20);
		quantity_caption.rect.set_width(300);
		quantity_caption.color = { 180 , 180 , 180 };
		quantity_caption.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		quantity_caption.on_resize.perc_v = 5;

		page.add_text(quantity_caption);

		// quantity text value 
		widgets::text quantity;
		quantity.alias = "Quantity";
		quantity.rect.left = quantity_caption.rect.left;
		quantity.rect.top = quantity_caption.rect.bottom + (margin / 2);
		quantity.rect.set_height(20);
		quantity.rect.set_width(300);
		quantity.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		quantity.on_resize.perc_v = 5;

		page.add_text(quantity);

		// adding a bar chart 
		widgets::barchart bar;
		bar.rect.left = sales_list.rect.right + 50 ;
		bar.rect.top = sales_list.rect.top + 170;
		bar.rect.set_height(225);
		bar.rect.set_width(300);
		bar.alias = "barchart";
		bar.data.caption = "Sales statistics";
		bar.data.autocolor = true;
		bar.on_resize.perc_h = sales_list.on_resize.perc_width + 10;
		bar.on_resize.perc_v = sales_list.on_resize.perc_height - 50;
		bar.on_resize.perc_height = 50;
		bar.on_resize.perc_width = 50;
		
		
		// assigning values to the bar chart 
		std::vector< widgets::chart_entry> bar_data;
		
		// creating object for chart_entry 
		widgets::chart_entry details;
		details.color = color { 180 , 200 , 255 }; 
		details.label = "Sales";
		details.value = app_state_.count;

		// creating a second object of chart entry
		widgets::chart_entry details_;
		details_.color = color{ 180 , 180 , 180 };
		details_.label = "Total Cost";
		details_.value = app_state_.quantity;

		bar_data.push_back(details);
		bar_data.push_back(details_);

		bar.data.bars = bar_data;

		page.add_barchart(bar);

		// adding the page to the window
		add_page(page);
	}
	show_page("Sales");

	{
		{
			std::vector<std::string> aliases;
			aliases.push_back("Sales/name_caption");
			aliases.push_back("Sales/unit_price_caption");
			aliases.push_back("Sales/caption_quantity");
			aliases.push_back("Sales/caption_cost");
			aliases.push_back("Sales/cost");
			aliases.push_back("Sales/Quantity");
			aliases.push_back("Sales/Item Name");
			aliases.push_back("Sales/unit_price");
			
			hide_info(aliases);

		}
	}
}

void ken_app_main::on_add_sales(){

	// creating object 

	sales sales(app_state_);
	sales.modal(*this);

	std::string error;
	if (!sales.run(error)) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", error);
		return;
	}

	//displaying information to the listview of the sales window
	if (sales.saved()) {

		// declaring variables 
		const auto& sales_info = sales.get_details();

		//getting the data from the database 
		std::vector<widgets::listview_row> data;
		std::vector<ken_app_db::sales_details> sales_;
		widgets::listview_row row;
		if (app_state_.get_db().get_sales_all(sales_, error)) {
			for (const auto& details : sales_) {

				row.items.push_back({ "ID", details.id, true , color {255, 0, 0} });
				row.items.push_back({ "Item Name", details.item_name });
				row.items.push_back({ "Unit Price", details.Unit_price });
				row.items.push_back({ "Cost", details.Cost });
				row.items.push_back({ "Quantity", details.quantity });
				data.push_back(row);

				// counting the sales and cost
				app_state_.count++;
				double cost;
				std::stringstream ss;
				ss << details.Cost;
				ss >> cost;
				app_state_.quantity += cost;
			}
		}
		repopulate_listview("Sales/appointment_list_main", data, error);

		widgets::barchart_data  bar_data;
		bar_data.autocolor = false;
		bar_data.autoscale = true;
		bar_data.caption = "Sales Barchart";
		bar_data.x_label = "Fields";
		bar_data.y_label = "Data";


		// setting out the bars in the barchart
		std::vector<widgets::chart_entry>bar_data_;

		// creating object for the chart entry
		widgets::chart_entry entry;
		entry.color = { 180 , 180, 180 };
		entry.label = "Sales";
		entry.value = app_state_.count;
		bar_data_.push_back(entry);


		//creating a new object of chart entry 
		widgets::chart_entry entry2;
		entry2.label = "Stats";
		entry2.value = app_state_.quantity;
		entry2.color = { 180, 200, 255 };
		bar_data_.push_back(entry2);
		bar_data.bars = bar_data_;

		
		barchart_reload("Sales/barchart", bar_data, error);


	}
	
	// reseting the variables to zero
	app_state_.count = 0;
	app_state_.quantity = 0;
	
}

void ken_app_main::on_appoinment(){

	if (!page_exists(page_name)) {
		page page(page_name);


		//add back icon 
		widgets::image back;
		back.toggle = "Previous page";
		back.filename = "back.png";
		back.rect.left = 10;
		back.rect.top = 10;
		back.rect.set_height(50);
		back.rect.set_width(40);
		back.on_click = [&]() {
			show_previous_page();
			update_homepage();
		};

		page.add_image(back);

		// add tittle 
		widgets::text title;
		title.text_value = "Appointment";
		title.font_size = 16;
		title.rect.left = back.rect.right + 10;
		title.rect.top = back.rect.top;
		title.rect.set_height(40);
		title.rect.set_width(150);

		page.add_text(title);

		// add description 

		widgets::text description;
		description.text_value = "View and Manage Inventory";
		description.color = color{ 180 , 180 , 180 };
		description.rect.left = back.rect.right + 10;
		description.rect.top = title.rect.top + 30;
		description.rect.set_height(20);
		description.rect.set_width(200);

		page.add_text(description);// adding a a search space on the form 
		widgets::editbox search;
		search.alias = "search";
		search.cue_banner = "search " + home_page_name;
		search.rect.left = description.rect.right + 525;
		search.rect.top = description.rect.top;
		search.rect.set_height(22);
		search.rect.set_width(200);
		search.rect.top = description.rect.top;
		search.control_to_invoke_alias = "Enter";
		search.on_resize.perc_h = 100;
		search.on_resize.perc_v = 0;

		page.add_editbox(search);

		//add icon for the search bar
		widgets::image icon_search;
		icon_search.filename = "search.png";
		icon_search.tight_fit = true;
		icon_search.rect.left = description.rect.right + 495;
		icon_search.rect.top = search.rect.top;
		icon_search.rect.set_height(22);
		icon_search.rect.set_width(30);
		icon_search.on_resize.perc_h = 100;
		icon_search.on_resize.perc_v = 0;

		page.add_image(icon_search);

		widgets::groupbox box;
		box.rects = {
			icon_search.rect ,
			search.rect
		};
		box.on_resize.perc_h = 100;
		box.on_resize.perc_v = 0;
		page.add_groupbox(box);

		// add image 
		widgets::image image;
		image.bar = false;
		image.filename = "appointment.jpg";
		image.rect.left = back.rect.left;
		image.rect.top = description.rect.bottom + 20;
		image.rect.set_height(100);
		image.rect.set_width(100);

		page.add_image(image);



		// adding a group box 
		widgets::groupbox border;
		border.rects = {
			image.rect
		};

		page.add_groupbox(border);

		// add "add appointment item " icon 
		widgets::image image_add;
		image_add.filename = "add.png";
		image_add.tooltip = "Add a new Item";
		image_add.rect.left = back.rect.left;
		image_add.rect.top = image.rect.bottom + 10;
		image_add.rect.set_height(30);
		image_add.rect.set_width(30);
		image_add.change_color = true;
		image_add.color.color = color{ 0, 150, 140 };
		image_add.color.color_hot = color{ 21, 79, 139 };
		image_add.color.color_border_hot = image_add.color.color_border;
		image_add.color_background_hot = image_add.color_background_hot;
		image_add.tight_fit = true;
		image_add.on_click = [&] { on_add_appointment(); };

		page.add_image(image_add);

		// add image edit
		widgets::image image_edit;
		image_edit.filename = "edit.png";
		image_edit.tooltip = "Edit Item";
		image_edit.rect.left = image_add.rect.right + 10;
		image_edit.rect.top = image.rect.bottom + 10;
		image_edit.rect.set_height(27);
		image_edit.rect.set_width(27);
		image_edit.change_color = true;
		image_edit.color.color = color{ 0, 150, 140 };
		image_edit.color.color_hot = color{ 21, 79, 139 };
		image_edit.color.color_border_hot = image_edit.color.color_border;
		image_edit.color_background_hot = image_edit.color_background_hot;
		image_edit.tight_fit = true;

		page.add_image(image_edit);

		// add the delete image 
		widgets::image image_delete;
		image_delete.filename = "delete.png";
		image_delete.tooltip = "Delete Item";
		image_delete.rect.left = image_edit.rect.right + 10;
		image_delete.rect.top = image.rect.bottom + 10;
		image_delete.rect.set_height(27);
		image_delete.rect.set_width(27);
		image_delete.change_color = true;
		image_delete.color.color = color{ 255, 0, 0 };
		image_delete.color.color_hot = color{ 21, 79, 139 };
		image_delete.color.color_border_hot = image_delete.color.color_border;
		image_delete.color_background_hot = image_delete.color_background_hot;
		image_delete.tight_fit = true;
		image_delete.on_click = [&]() {on_delete_appointment(); };

		page.add_image(image_delete);

		// add appointment listview 
		widgets::listview appointment_list;
		appointment_list.alias = "appointment_page_list";
		appointment_list.rect.left = image.rect.right + 50;
		appointment_list.rect.top = image.rect.top;
		appointment_list.rect.set_height(400);
		appointment_list.rect.set_width(400);
		appointment_list.border = true;
		// on resize 
		appointment_list.on_resize.perc_h = 0;
		appointment_list.on_resize.perc_v = 5;
		appointment_list.on_resize.perc_height = 90;
		appointment_list.on_resize.perc_width = 25;
		appointment_list.on_selection = [&]() {on_appointment_list(); };


		appointment_list.columns = {
			app_state_.column_details("ID", 100 , widgets::listview_column_type::int_,color {0, 0 , 0}),
			app_state_.column_details("Name" , 100 , widgets::listview_column_type::string_,color {0, 0 , 0}),
			app_state_.column_details("Surname" , 100  , widgets::listview_column_type::string_,color {0, 0 , 0}),
			app_state_.column_details("Time" , 100 , widgets::listview_column_type::string_,color {0, 0 , 0}),
			app_state_.column_details("Date" , 100 , widgets::listview_column_type::string_,color {0, 0 , 0}),
			app_state_.column_details("Description" , 150 , widgets::listview_column_type::string_,color {0, 0 , 0}),
		};
		appointment_list.unique_column_name = "ID";

		
			std::vector<ken_app_db::appointments_details> appointment;
			std::string error;

			if (app_state_.get_db().get_appointments(appointment, error)) {
	

				for (const auto& appointment_ : appointment) {
					widgets::listview_row row;
					row.items.push_back({ "ID", appointment_.id });
					row.items.push_back({ "Time", appointment_.time });
					row.items.push_back({ "Date", appointment_.date });
					row.items.push_back({ "Name" , appointment_.name });
					row.items.push_back({ "Surname", appointment_.surname });
					row.items.push_back({ "Description" , appointment_.description });
					appointment_list.data.push_back(row);
					app_state_.count++;
				}
				}
				
		
		page.add_listview(appointment_list);

	  

		// adding widgets for displaying the information on the listview
		// name caption
		widgets::text name_caption;
		name_caption.alias = "name_caption";
		name_caption.text_value = " Name";
		name_caption.rect.left = appointment_list.rect.right + margin + 40;
		name_caption.rect.top = appointment_list.rect.top;
		name_caption.rect.set_height(20);
		name_caption.rect.set_width(100);
		name_caption.color = { 180, 180 , 180 };
		name_caption.on_resize.perc_h = appointment_list.on_resize.perc_width + 10;
		name_caption.on_resize.perc_v = 0;
		page.add_text(name_caption);

		// time caption 
		widgets::text time_caption;
		time_caption.alias = "caption_time";
		time_caption.text_value = "Time";
		time_caption.rect.left = name_caption.rect.right + margin + 40;
		time_caption.rect.top = name_caption.rect.top;
		time_caption.rect.set_height(20);
		time_caption.rect.set_width(100);
		time_caption.color = { 180, 180 , 180 };
		time_caption.on_resize.perc_h = appointment_list.on_resize.perc_width + 25;
		time_caption.on_resize.perc_v = 0;
		page.add_text(time_caption);

	
		// name value text 
		widgets::text name;
		name.alias = "Name";
		name.rect.left = name_caption.rect.left;
		name.rect.top = name_caption.rect.bottom + (margin / 2);
		name.rect.set_height(20);
		name.rect.set_width(100);
		name.on_resize.perc_h = appointment_list.on_resize.perc_width + 10;
		name.on_resize.perc_v = 0;

		page.add_text(name);

		// time value text 
		widgets::text time;
		time.alias = "time";
		time.rect.left = time_caption.rect.left;
		time.rect.top = time_caption.rect.bottom + (margin / 2);
		time.rect.set_height(20);
		time.rect.set_width(100);
		time.on_resize.perc_h = appointment_list.on_resize.perc_width + 25;
		time.on_resize.perc_v = 0;
		page.add_text(time);

		// time caption 
		widgets::text date_caption;
		date_caption.alias = "caption_date";
		date_caption.text_value = "Date";
		date_caption.rect.left = time_caption.rect.left;
		date_caption.rect.top = time.rect.bottom + margin;
		date_caption.rect.set_height(20);
		date_caption.rect.set_width(100);
		date_caption.color = { 180, 180 , 180 };
		date_caption.on_resize.perc_h = appointment_list.on_resize.perc_width + 25;
		date_caption.on_resize.perc_v = 0;
		page.add_text(date_caption);

		// date value text 
		widgets::text date;
		date.alias = "date";
		date.rect.left = date_caption.rect.left;
		date.rect.top = date_caption.rect.bottom + (margin / 2);
		date.rect.set_height(20);
		date.rect.set_width(100);
		date.on_resize.perc_h = appointment_list.on_resize.perc_width + 25;
		date.on_resize.perc_v = 0;
		page.add_text(date);

		// descritption caption
		widgets::text surname_caption;
		surname_caption.alias = "surname_caption";
		surname_caption.text_value = "Surname";
		surname_caption.rect.left = name.rect.left;
		surname_caption.rect.top = name.rect.bottom + margin;
		surname_caption.rect.set_height(20);
		surname_caption.rect.set_width(100);
		surname_caption.color = { 180 , 180 , 180 };
		surname_caption.on_resize.perc_h = appointment_list.on_resize.perc_width + 10;
		surname_caption.on_resize.perc_v = 5;

		page.add_text(surname_caption);

		// description_ text value 
		widgets::text surname;
		surname.alias = "surname";
		surname.rect.left = surname_caption.rect.left;
		surname.rect.top = surname_caption.rect.bottom + (margin / 2);
		surname.rect.set_height(20);
		surname.rect.set_width(100);
		surname.on_resize.perc_h = appointment_list.on_resize.perc_width + 10;
		surname.on_resize.perc_v = 5;
		page.add_text(surname);

		// descritption caption
		widgets::text description_caption;
		description_caption.alias = "caption_description";
		description_caption.text_value = "Description";
		description_caption.rect.left = surname.rect.left;
		description_caption.rect.top = surname.rect.bottom + margin;
		description_caption.rect.set_height(20);
		description_caption.rect.set_width(100);
		description_caption.color = { 180 , 180 , 180 };
		description_caption.on_resize.perc_h = appointment_list.on_resize.perc_width + 10;
		description_caption.on_resize.perc_v = 5;
		page.add_text(description_caption);

		// description_ text value 
		widgets::text description_;
		description_.alias = "description_";
		description_.rect.left = description_caption.rect.left;
		description_.rect.top = description_caption.rect.bottom + (margin / 2);
		description_.rect.set_height(20);
		description_.rect.set_width(100);
		description_.on_resize.perc_h = appointment_list.on_resize.perc_width + 10;
		description_.on_resize.perc_v = 5;

		page.add_text(description_);

		// adding a barchart 
		widgets::barchart bar;
		bar.alias = "barchart";
		bar.data.autocolor = false;
		bar.data.x_label = "Fields";
		bar.data.y_label = "Data";
		bar.data.caption = " appointment BarChart";
		bar.rect.left = appointment_list.rect.right + 50;
		bar.rect.top = appointment_list.rect.top + 170;
		bar.rect.set_height(225);
		bar.rect.set_width(300);
		bar.on_resize.perc_h = appointment_list.on_resize.perc_width + 10;
		bar.on_resize.perc_v = appointment_list.on_resize.perc_height - 50;
		bar.on_resize.perc_height = 50;
		bar.on_resize.perc_width = 50;

		// setting out the bars in the barchart
		std::vector<widgets::chart_entry>bar_data;
		// creating object for the chart entry
		widgets::chart_entry details;

		// to-do::
		// the barchart must desplay information from the database that is in the listview

		// assigning the values in bar eat
		details.label = "Appointments";
		details.color = color{ 180 , 180 , 180 };
		details.value = app_state_.count;

		// assigning the values in bar drink
		widgets::chart_entry detail_;
		detail_.label = "stats";
		detail_.value = 100;
		detail_.color = color{ 180 , 200 , 255 };
		bar_data.push_back(detail_);
		bar_data.push_back(details);

		// on resize 


		bar.data.bars = bar_data;

		page.add_barchart(bar);
		// this is adding a new page to the form 
		add_page(page);
	}
	show_page(page_name);

	//hiding the information for the pop up window
	std::vector < std::string> aliases;
	aliases.push_back("Appointment/name_caption");
	aliases.push_back("Appointment/caption_description");
	aliases.push_back("Appointment/caption_time");
	aliases.push_back("Appointment/date");
	aliases.push_back("Appointment/time");
	aliases.push_back("Appointment/caption_date");
	aliases.push_back("Appointment/surname_caption");
	aliases.push_back("Appointment/description_");
	aliases.push_back("Appointment/surname");
	aliases.push_back("Appointment/Name");
	hide_info(aliases);
}

void ken_app_main::on_add_appointment(){
	std::string error;
	// creating object 
    appointment appointment(app_state_);
	appointment.modal(*this);

	
	if (!appointment.run(error)) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", error);
		return;
	}
	
	//adding infomation to the listview
	if (appointment.saved()) {
		const auto& app_info = appointment.get_details();

		std::vector<ken_app_db::appointments_details> appointments;
		std::string error;

		// getting the data from the database
		std::vector<widgets::listview_column> columns;
		std::vector<widgets::listview_row> data;
		if (app_state_.get_db().get_appointments(appointments, error)) {
			for (const auto& app : appointments) {
				widgets::listview_row row;

				row.items.push_back({ "ID", app.id });
				row.items.push_back({ "Name", app.name });
				row.items.push_back({ "Surname", app.surname });
				row.items.push_back({ "Description", app.description });
				row.items.push_back({ "Date", app.date });
				row.items.push_back({ "Time", app.time });

				data.push_back(row);
				app_state_.count++;
			}
		}

		//adding the data to the listview
		repopulate_listview("Appointment/appointment_page_list", data, error);
		widgets::barchart_data  bar_data;
		bar_data.autocolor = false;
		bar_data.autoscale = true;
		bar_data.caption = "Stock Barchart";
		bar_data.x_label = "Fields";
		bar_data.y_label = "Data";


		// setting out the bars in the barchart
		std::vector<widgets::chart_entry>bar_data_;

		// creating object for the chart entry
		widgets::chart_entry entry;
		entry.color = { 180 , 180, 180 };
		entry.label = "Appointments";
		entry.value = app_state_.count;
		bar_data_.push_back(entry);


		//creating a new object of chart entry 
		widgets::chart_entry entry2;
		entry2.label = "Stats";
		entry2.value = 50;
		entry2.color = { 180, 200, 255 };
		bar_data_.push_back(entry2);
		bar_data.bars = bar_data_;

		barchart_reload("Appointment/barchart", bar_data, error);
	}
	//restting the counting values
	app_state_.count = 0;

}


// on_clicking the users icon
void ken_app_main::on_users(){

	if (!page_exists("Users")) {
		page page("Users");


		//add back icon 
		widgets::image back;
		back.toggle = "Previous page";
		back.filename = "back.png";
		back.rect.left = 10;
		back.rect.top = 10;
		back.rect.set_height(50);
		back.rect.set_width(40);
		back.on_click = [&]() {
			show_previous_page();
			// to-do::
			// this is where you put the code for updating the home page when there are new appointments to be put on the home page
		};

		page.add_image(back);

		// add tittle 
		widgets::text title;
		title.text_value = "Users";
		title.font_size = 16;
		title.rect.left = back.rect.right + 10;
		title.rect.top = 15;
		title.rect.set_height(50);
		title.rect.set_width(200);

		page.add_text(title);

		// add description 
		widgets::text description;
		description.text_value = "All the User Credentials";
		description.color = color{ 180 , 180 , 180 };
		description.rect.left = back.rect.right + 10;
		description.rect.top = title.rect.top + 30;;
		description.rect.set_height(20);
		description.rect.set_width(200);

		page.add_text(description);

		// adding a a search space on the form 
		widgets::editbox search;
		search.alias = "search";
		search.cue_banner = "search " + home_page_name;
		search.rect.left = description.rect.right + 525;
		search.rect.top = description.rect.top;
		search.rect.set_height(22);
		search.rect.set_width(200);
		search.rect.top = description.rect.top;
		search.control_to_invoke_alias = "Enter";
		search.on_resize.perc_h = 100;
		search.on_resize.perc_v = 0;

		page.add_editbox(search);

		//add icon for the search bar
		widgets::image icon_search;
		icon_search.filename = "search.png";
		icon_search.tight_fit = true;
		icon_search.rect.left = description.rect.right + 495;
		icon_search.rect.top = search.rect.top;
		icon_search.rect.set_height(22);
		icon_search.rect.set_width(30);
		icon_search.on_resize.perc_h = 100;
		icon_search.on_resize.perc_v = 0;

		page.add_image(icon_search);

		// adding wrappers for the elements
		widgets::groupbox box;
		box.rects = {
			icon_search.rect ,
			search.rect
		};
		box.on_resize.perc_h = 100;
		box.on_resize.perc_v = 0;
		page.add_groupbox(box);

		


		// add image 
		widgets::image image;
		image.bar = false;
		image.filename = "users.png";
		image.rect.left = back.rect.left;
		image.rect.top = description.rect.bottom + 20;
		image.rect.set_height(100);
		image.rect.set_width(100);

		page.add_image(image);

		// adding a group box 
		widgets::groupbox border;
		border.rects = {
			image.rect
		};

		page.add_groupbox(border);

		// add image edit
		widgets::image image_edit;
		image_edit.filename = "edit.png";
		image_edit.tooltip = "Edit Item";
		image_edit.rect.left = back.rect.left;
		image_edit.rect.top = image.rect.bottom + 10;
		image_edit.rect.set_height(27);
		image_edit.rect.set_width(27);
		image_edit.change_color = true;
		image_edit.color.color = color{ 0, 150, 140 };
		image_edit.color.color_hot = color{ 21, 79, 139 };
		image_edit.color.color_border_hot = image_edit.color.color_border;
		image_edit.color_background_hot = image_edit.color_background_hot;
		image_edit.tight_fit = true;

		page.add_image(image_edit);

		// add the delete image 
		widgets::image image_delete;
		image_delete.filename = "delete.png";
		image_delete.tooltip = "Delete Item";
		image_delete.rect.left = image_edit.rect.right + 10;
		image_delete.rect.top = image.rect.bottom + 10;
		image_delete.rect.set_height(27);
		image_delete.rect.set_width(27);
		image_delete.change_color = true;
		image_delete.color.color = color{ 255, 0, 0 };
		image_delete.color.color_hot = color{ 21, 79, 139 };
		image_delete.color.color_border_hot = image_delete.color.color_border;
		image_delete.color_background_hot = image_delete.color_background_hot;
		image_delete.tight_fit = true;

		page.add_image(image_delete);

		// add stock listview 
		widgets::listview users_list;
		users_list.alias = "users_list";
		users_list.rect.left = image.rect.right + 50;
		users_list.rect.top = image.rect.top;
		users_list.rect.set_height(400);
		users_list.rect.set_width(400);
		users_list.border = true;
		// on resize 
		users_list.on_resize.perc_h = 0;
		users_list.on_resize.perc_v = 5;
		users_list.on_resize.perc_height = 90;
		users_list.on_resize.perc_width = 25;
		users_list.on_selection = [&]() {on_users_list(); };
		

		users_list.columns = {
			app_state_.column_details("ID", 100 , widgets::listview_column_type::int_, color {0, 0 , 0}),
			app_state_.column_details("Username" , 300 , widgets::listview_column_type::string_, color {0, 0 , 0})
		};
		users_list.unique_column_name = "ID";

		// adding the values to the listview
		{
			std::vector<ken_app_db::user_credentials> users;
			std::string error;

			if (app_state_.get_db().get_users(users, error)) {
				int i = 0;

				for (const auto& users_ : users) {
					widgets::listview_row row;
					row.items.push_back({ "ID", users_.id });
					row.items.push_back({ "Username", users_.username });
					users_list.data.push_back(row);
					app_state_.count++;
				}
			}
		}
		page.add_listview(users_list);

		// adding widgets for username and password 

		// username caption
		widgets::text username_caption;
		username_caption.alias = "username_caption";
		username_caption.text_value = "Username";
		username_caption.rect.left = users_list.rect.right + margin + 40;
		username_caption.rect.top = users_list.rect.top;
		username_caption.rect.set_height(20);
		username_caption.rect.set_width(300);
		username_caption.color = { 180, 180 , 180 };
		username_caption.on_resize.perc_h = users_list.on_resize.perc_width + 10;
		username_caption.on_resize.perc_v = 0;

		page.add_text(username_caption);

		// username value text 
		widgets::text username;
		username.alias = "username";
		username.rect.left = username_caption.rect.left;
		username.rect.top = username_caption.rect.bottom + (margin / 2);
		username.rect.set_height(20);
		username.rect.set_width(300);
		username.on_resize.perc_h = users_list.on_resize.perc_width + 10;
		username.on_resize.perc_v = 0;

		page.add_text(username);

		// descritption caption
		widgets::text passwordcaption;
		passwordcaption.alias = "password_caption";
		passwordcaption.text_value = "Password";
		passwordcaption.rect.left = username.rect.left;
		passwordcaption.rect.top = username.rect.bottom + margin;
		passwordcaption.rect.set_height(20);
		passwordcaption.rect.set_width(300);
		passwordcaption.color = { 180 , 180 , 180 };
		passwordcaption.on_resize.perc_h = users_list.on_resize.perc_width + 10;
		passwordcaption.on_resize.perc_v = 5;
		passwordcaption.on_resize.perc_height = 5;

		page.add_text(passwordcaption);

		// password text value 
		widgets::text password;
		password.alias = "password";
		password.rect.left = passwordcaption.rect.left;
		password.rect.top = passwordcaption.rect.bottom + (margin / 2);
		password.rect.set_height(20);
		password.rect.set_width(300);
		password.on_resize.perc_h = users_list.on_resize.perc_width + 10;
		password.on_resize.perc_v = 5;

		page.add_text(password);

		// adding a bar chart 
		widgets::barchart bar;
		bar.rect.left = users_list.rect.right + 50;
		bar.rect.top = users_list.rect.top + 170;
		bar.rect.set_height(225);
		bar.rect.set_width(300);
		bar.alias = "barchart";
		bar.data.caption = "users Statistics";
		bar.data.autocolor = true;
		bar.on_resize.perc_h = users_list.on_resize.perc_width + 10;
		bar.on_resize.perc_v = users_list.on_resize.perc_height - 50;
		bar.on_resize.perc_height = 50;
		bar.on_resize.perc_width = 50;

		// assigning values to the bar chart 
		std::vector< widgets::chart_entry> bar_data;

		// creating object for chart_entry 
		widgets::chart_entry details;
		details.color = color{ 180 , 200 , 255 };
		details.label = "Users";
		details.value = app_state_.count;

		// adding another bar 
		widgets::chart_entry details2;
		details2.color = color{ 180 , 200, 255 };
		details2.label = "Stats";
		details2.value = 10;

		bar_data.push_back(details2);
		bar_data.push_back(details);
	

		bar.data.bars = bar_data;

		page.add_barchart(bar);

		// adding the page to the window
		add_page(page);
	}
	show_page("Users");
	// for hiding all the pop up window for showing the username and password
	std::vector<std::string> aliases;
	aliases.push_back("Users/username_caption");
	aliases.push_back("Users/password_caption");
	aliases.push_back("Users/password");
	aliases.push_back("Users/username");
	hide_info(aliases);
	app_state_.count = 0;
}

void ken_app_main::on_share(){

	if (!page_exists("share")) {
		page page("share");

		//add back icon 
		widgets::image back;
		back.toggle = "Previous page";
		back.filename = "back.png";
		back.rect.left = 10;
		back.rect.top = 10;
		back.rect.set_height(50);
		back.rect.set_width(40);
		back.on_click = [&]() {
			show_previous_page();
			// to-do::
			// this is where you put the code for updating the home page when there are new appointments to be put on the home page
		};

		page.add_image(back);

		// add tittle 
		widgets::text title;
		title.text_value = "Share Files";
		title.font_size = 16;
		title.rect.left = back.rect.right + 10;
		title.rect.top = back.rect.top;
		title.rect.set_height(40);
		title.rect.set_width(200);

		page.add_text(title);

		// add description 

		widgets::text description;
		description.text_value = "Share files amoung your piers";
		description.color = color{ 180 , 180 , 180 };
		description.rect.left = back.rect.right + 10;
		description.rect.top = title.rect.top + 30;
		description.rect.set_height(20);
		description.rect.set_width(200);

		page.add_text(description);

		widgets::image image;
		image.bar = false;
		image.filename = "share.png";
		image.rect.left = back.rect.left;
		image.rect.top = description.rect.bottom + 20;
		image.rect.set_height(100);
		image.rect.set_width(100);

		page.add_image(image);

		// adding a group box 
		widgets::groupbox border;
		border.rects = {
			image.rect
		};

		page.add_groupbox(border);

		widgets::richedit rich;
		rich.alias = "Upload_space";
		rich.cue_banner = "Drag and Drop or Upload Files";
		rich.border = true;
		rich.read_only = true;
		rich.rect.left = image.rect.right + 20;
		rich.rect.top = image.rect.top;
		rich.rect.set_height(300);
		rich.rect.set_width(400);
		rich.color_border = color{ 0 , 0 , 0 };

		page.add_richedit(rich);


		// adding the upload button
		widgets::button btn_upload;
		btn_upload.alias = "btn_upload";
		btn_upload.caption = "Upload";
		btn_upload.tooltip = "Click to upload files from the computer";
		btn_upload.rect.left = rich.rect.right + 10;
		btn_upload.rect.top = rich.rect.top;
		btn_upload.rect.set_height(25);
		btn_upload.rect.set_width(60);
		btn_upload.on_click = [&]() {on_upload(); };

		page.add_button(btn_upload);

		// adding the send button
		widgets::button btn_share;
		btn_share.alias = "btn_share";
		btn_share.caption = "Send";
		btn_share.tooltip = "Share files over a network";
		btn_share.rect.left = rich.rect.right + 10;
		btn_share.rect.top = rich.rect.top + 280;
		btn_share.rect.set_height(25);
		btn_share.rect.set_width(60);

		page.add_button(btn_share);
	
		// adding the page to the form
		add_page(page);
	}
	show_page("share");
}
// selecting the folder with the files 
void ken_app_main::on_upload(){
	// function for creating a directory
	auto drop_files = [&]() {
		std::string details = "Please Create a file Directory";

		std::string file = sync_folder();
		// getting the path of sync folder 
		std::filesystem::path path(file);

		if (std::filesystem::exists(path)) {
			gui::notification_params params;
			params.color = color{ 0 ,0 , 0 };
			notification(params, "File Path", details, 1000);
		}
		return file;
	};

	drop_files();
	
}

// function for hiding information of a widgets
void ken_app_main::hide_info(std::vector<std::string> aliases)
{
	for (auto& alias : aliases) {
		hide(alias);
	}
}

// for showing the information of a widgets
void ken_app_main::show_info(std::vector<std::string> aliases)
{
	for (auto& alias : aliases) {
		show(alias);
	}
}

// function for on_selection on the listview stock
void ken_app_main::on_stock_list()
{
	std::vector<widgets::listview_row> rows;
	std::string error;
	get_listview_selected("Stock/Stock_list", rows, error);

	if (rows.size() == 1) {
		std::string stock_id;
		for (auto& item : rows[0].items) {
			if (item.column_name == "ID") {
				stock_id = item.item_data;
				break;
			}
		}

		if (!stock_id.empty()) {
			ken_app_db::stock_details stock;
			if (!app_state_.get_db().get_stock(stock_id, stock, error)) {
				prompt_params params;
				params.type = gui::prompt_type::ok;
				prompt(params, "Error", error);
				return;
			}

			// setting the text of the widgets
			set_text("Stock/Name", stock.name, error);
			set_text("Stock/description_", stock.description, error);
			set_text("Stock/Quantity", stock.quantity, error);

			{
				// showing the stock information
				std::vector<std::string> aliases;
				aliases.push_back("Stock/name_caption");
				aliases.push_back("Stock/description_caption");
				aliases.push_back("Stock/caption_quantity");
				aliases.push_back("Stock/Name");
				aliases.push_back("Stock/description_");
				aliases.push_back("Stock/Quantity");
				show_info(aliases);
			}
		}

	}
	else {
		// hiding the stock information
		std::vector<std::string> aliases;
		aliases.push_back("Stock/name_caption");
		aliases.push_back("Stock/description_caption");
		aliases.push_back("Stock/caption_quantity");
		hide_info(aliases);
	}
		
}

void ken_app_main::on_appointment_list()
{
	std::vector<widgets::listview_row> rows;
	std::string error;
	get_listview_selected("Appointment/appointment_page_list", rows, error);

	if (rows.size() == 1) {
		std::string id;
		for (auto& item : rows[0].items) {
			if (item.column_name == "ID") {
				id = item.item_data;
				break;
			}
		}

		if (!id.empty()) {
			ken_app_db::appointments_details appointment;
			if (!app_state_.get_db().get_appointment(id, appointment, error)) {
				prompt_params params;
				params.type = gui::prompt_type::ok;
				prompt(params, "Error", error);
				return;
			}

			// setting the text of the widgets
			set_text("Appointment/Name", appointment.name, error);
			set_text("Appointment/surname", appointment.surname, error);
			set_text("Appointment/description_", appointment.description, error);
			set_text("Appointment/time", appointment.time, error);
			set_text("Appointment/date", appointment.date, error);

			{
				// showing the stock information
				std::vector<std::string> aliases;
				aliases.push_back("Appointment/name_caption");
				aliases.push_back("Appointment/caption_description");
				aliases.push_back("Appointment/caption_time");
				aliases.push_back("Appointment/date");
				aliases.push_back("Appointment/time");
				aliases.push_back("Appointment/caption_date");
				aliases.push_back("Appointment/surname_caption");
				aliases.push_back("Appointment/description_");
				aliases.push_back("Appointment/surname");
				aliases.push_back("Appointment/Name");
				show_info(aliases);
			}
		}

	}
	else {
		// hiding the stock information
		std::vector<std::string> aliases;
		aliases.push_back("Appointment/name_caption");
		aliases.push_back("Appointment/caption_description");
		aliases.push_back("Appointment/caption_time");
		aliases.push_back("Appointment/date");
		aliases.push_back("Appointment/time");
		aliases.push_back("Appointment/caption_date");
		aliases.push_back("Appointment/surname_caption");
		aliases.push_back("Appointment/description_");
		aliases.push_back("Appointment/surname");
		aliases.push_back("Appointment/Name");
		hide_info(aliases);
	}
}

void ken_app_main::on_sales_list(){
	std::vector<widgets::listview_row> rows;
	std::string error;
	get_listview_selected("Sales/sales_list_main", rows, error);

	if (rows.size() == 1) {
		std::string id;
		for (auto& item : rows[0].items) {
			if (item.column_name == "ID") {
				id = item.item_data;
				break;
			}
		}

		if (!id.empty()) {
			ken_app_db::sales_details sales;
			if (!app_state_.get_db().get_sales(id, sales, error)) {
				prompt_params params;
				params.type = gui::prompt_type::ok;
				prompt(params, "Error", error);
				return;
			}

			// setting the text of the widgets
			set_text("Sales/Item Name", sales.item_name, error);
			set_text("Sales/Item Name", sales.item_name, error);
			set_text("Sales/unit_price", sales.Unit_price, error);
			set_text("Sales/Quantity", sales.quantity, error);
			set_text("Sales/cost", sales.Cost, error);

			{
				// showing the stock information
				std::vector<std::string> aliases;
				aliases.push_back("Sales/name_caption");
				aliases.push_back("Sales/unit_price_caption");
				aliases.push_back("Sales/caption_quantity");
				aliases.push_back("Sales/caption_cost");
				aliases.push_back("Sales/cost");
				aliases.push_back("Sales/Quantity");
				aliases.push_back("Sales/Item Name");
				aliases.push_back("Sales/unit_price");
				show_info(aliases);
			}
		}

	}
	else {
		// hiding the stock information
		std::vector<std::string> aliases;
		aliases.push_back("Sales/name_caption");
		aliases.push_back("Sales/unit_price_caption");
		aliases.push_back("Sales/caption_quantity");
		aliases.push_back("Sales/caption_cost");
		aliases.push_back("Sales/cost");
		aliases.push_back("Sales/Quantity");
		aliases.push_back("Sales/Item Name");
		aliases.push_back("Sales/unit_price");
		hide_info(aliases);
	}
}

void ken_app_main::on_users_list()
{
	std::vector<widgets::listview_row> rows;
	std::string error;
	get_listview_selected("Users/users_list", rows, error);

	if (rows.size() == 1) {
		std::string user_id;
		for (auto& item : rows[0].items) {
			if (item.column_name == "ID") {
				user_id = item.item_data;
				break;
			}
		}

		if (!user_id.empty()) {
			ken_app_db::user_credentials users;
			if (!app_state_.get_db().get_user(user_id, users, error)) {
				prompt_params params;
				params.type = gui::prompt_type::ok;
				prompt(params, "Error", error);
				return;
			}

			// setting the text of the widgets
			set_text("Users/username", users.username, error);
			set_text("Users/password","**********", error);
			

			{
				// showing the stock information
				std::vector<std::string> aliases;
				aliases.push_back("Users/username_caption");
				aliases.push_back("Users/password_caption");
				aliases.push_back("Users/password");
				aliases.push_back("Users/username");
				show_info(aliases);
			}
		}

	}
	else {
// hiding the stock information
std::vector<std::string> aliases;
aliases.push_back("Users/username_caption");
aliases.push_back("Users/password_caption");
aliases.push_back("Users/password");
aliases.push_back("Users/username");
hide_info(aliases);
	}
}

// it changes the color of the appointment item after the appointment is done
void ken_app_main::on_appointment_completed() {
	std::string error, app_id;
	std::vector< widgets::listview_row> rows;
	std::vector<ken_app_db::appointments_details> appointment;
	ken_app_db::appointments_details app;
	get_listview_selected(home_page_name + "/list_appointments", rows, error);

	// getting the information from the database	
	for (const auto& row : rows) {
		ken_app_db::appointments_details details;
		widgets::listview_item items;

		for (const auto& item_ : row.items) {
			if (item_.column_name == "ID")
				app_id = item_.item_data;

			items.column_name = item_.column_name;
			items.item_data = item_.item_data;
			items.custom_text_color = true;
			items.color_text = color{ 0,255 , 0 };
			items.row_number = item_.row_number;
			update_listview_item(home_page_name + "/list_appointments", items, row, error);
		}
		std::string name = "Appointments";
		if (!app_state_.get_db().delete_item(app_id, name, error)) {
			prompt_params params;
			params.type = gui::prompt_type::ok;
			prompt(params, "Error", error);
		}

	}


}
// for updating the home page after adding a new item into the database
void ken_app_main::update_homepage() {
	std::vector<ken_app_db::appointments_details> appointments;
	std::string error;

	// getting the data from the database
	std::vector<widgets::listview_column> columns;
	std::vector<widgets::listview_row> data;
	if (app_state_.get_db().get_appointments(appointments, error)) {
		for (const auto& app : appointments) {
			widgets::listview_row row;

			row.items.push_back({ "ID", app.id , true, color {255, 0, 0 } });
			row.items.push_back({ "Name", app.name, true, color {255, 0, 0 } });
			row.items.push_back({ "Surname", app.surname , true, color {255, 0, 0 } });
			row.items.push_back({ "Description", app.description , true, color {255, 0, 0 } });
			row.items.push_back({ "Date", app.date, true, color {255, 0, 0 } });
			row.items.push_back({ "Time", app.time, true, color {255, 0, 0 } });

			data.push_back(row);
			app_state_.count++;
		}
	}

	//adding the data to the listview
	repopulate_listview(home_page_name + "/list_appointments", data, error);
	app_state_.count = 0;
}

void ken_app_main::on_delete_sales() {
	// function for deleting information from the database
	std::string error, sales_id;
	std::vector< widgets::listview_row> rows;
	get_listview_selected("Sales/sales_list_main", rows, error);

	// resetting the variables 
	app_state_.count = 0;
	app_state_.quantity = 0;

	// checking to see user has selected a row
	if (rows.empty()) {
		prompt_params params;
		params.type = prompt_type::ok;
		prompt(params, "Error", "Please select the item to delete");
	}
	// getting the information from the database	
	for (const auto& row : rows) {
		ken_app_db::sales_details details;
		widgets::listview_item items;

		widgets::listview_row this_row = row;
		for (const auto& item_ : row.items) {
			if (item_.column_name == "ID")
				sales_id = item_.item_data;
		}
		std::string name = "Sales";
		{
			prompt_params params;
			params.type = gui::prompt_type::yes_no;
			if (prompt(params, "DELETE", "Do you want to delete")) {
				if (!app_state_.get_db().delete_item(sales_id, name, error)) {
					prompt_params params;
					params.type = gui::prompt_type::yes_no;
					if ((prompt(params, "Error", error))){
						//getting the data from the database 
						std::vector<widgets::listview_row> data;
						std::vector<ken_app_db::sales_details> sales_;
						widgets::listview_row row_;
						if (app_state_.get_db().get_sales_all(sales_, error)) {
							for (const auto& details : sales_) {

								row_.items.push_back({ "ID", details.id , true , color {255, 0, 0} });
								row_.items.push_back({ "Item Name", details.item_name });
								row_.items.push_back({ "Unit Price", details.Unit_price });
								row_.items.push_back({ "Cost", details.Cost });
								row_.items.push_back({ "Quantity", details.quantity });
								data.push_back(row_);
								// counting the sales and cost
								app_state_.count++;
								double cost;
								std::stringstream ss;
								ss << details.Cost;
								ss >> cost;
								app_state_.quantity += cost;
							}
						}
						repopulate_listview("Sales/appointment_list_main", data, error);
						widgets::barchart_data  bar_data;
						bar_data.autocolor = false;
						bar_data.autoscale = true;
						bar_data.caption = "Sales Barchart";
						bar_data.x_label = "Fields";
						bar_data.y_label = "Data";


						// setting out the bars in the barchart
						std::vector<widgets::chart_entry>bar_data_;

						// creating object for the chart entry
						widgets::chart_entry entry;
						entry.color = { 180 , 180, 180 };
						entry.label = "Sales";
						entry.value = app_state_.count;
						bar_data_.push_back(entry);


						//creating a new object of chart entry 
						widgets::chart_entry entry2;
						entry2.label = "Stats";
						entry2.value = app_state_.quantity;
						entry2.color = { 180, 200, 255 };
						bar_data_.push_back(entry2);
						bar_data.bars = bar_data_;
						barchart_reload("Sales/barchart", bar_data, error);
}
				}

			}
		}
	
	}

	

	
}


void ken_app_main::on_delete_stock(){
	// function for deleting information from the database
	std::string error, stock_id;
	std::vector< widgets::listview_row> rows;
	get_listview_selected("Stock/Stock_list", rows, error);

	// resetting the variables 
	app_state_.count = 0;
	app_state_.quantity = 0;

	if (rows.empty()) {
		prompt_params params;
		params.type = prompt_type::ok;
		prompt(params, "Error", "Please select the item to delete");
	}
	// getting the information from the database	
	for (const auto& row : rows) {
		ken_app_db::sales_details details;
		widgets::listview_item items;

		for (const auto& item_ : row.items) {
			if (item_.column_name == "ID")
				stock_id = item_.item_data;
		}
		std::string name = "Stock";
		{
			prompt_params params;
			params.type = gui::prompt_type::yes_no;
			if (prompt(params, "DELETE", "Do you want to delete")) {
				if (!app_state_.get_db().delete_item(stock_id, name, error)) {
					prompt_params params;
					params.type = gui::prompt_type::yes_no;
					if ((prompt(params, "Error", error))) {
						//getting the data from the database 
						std::vector<widgets::listview_row> data;
						std::vector<ken_app_db::stock_details> stocks;
						widgets::listview_row row;
						if (app_state_.get_db().get_stock_all(stocks, error)) {
							for (const auto& stock_ : stocks) {

								row.items.push_back({ "ID", stock_.id });
								row.items.push_back({ "Name", stock_.name });
								row.items.push_back({ "Description", stock_.description });
								row.items.push_back({ "Quantity", stock_.quantity });
								data.push_back(row);

								// setting the variables back to zero 

								app_state_.count++;
								double	quantity;
								std::stringstream ss;
								ss << stock_.quantity;
								ss >> quantity;
								app_state_.quantity += quantity;
							}
						}
						widgets::barchart_data  bar_data;
						bar_data.autocolor = false;
						bar_data.autoscale = true;
						bar_data.caption = "Stock Barchart";
						bar_data.x_label = "Fields";
						bar_data.y_label = "Data";


						// setting out the bars in the barchart
						std::vector<widgets::chart_entry>bar_data_;

						// creating object for the chart entry
						widgets::chart_entry entry;
						entry.color = { 180 , 180, 180 };
						entry.label = "Stock";
						entry.value = app_state_.count;
						bar_data_.push_back(entry);


						//creating a new object of chart entry 
						widgets::chart_entry entry2;
						entry2.label = "Quantity";
						entry2.value = app_state_.quantity;
						entry2.color = { 180, 200, 255 };
						bar_data_.push_back(entry2);
						bar_data.bars = bar_data_;

						barchart_reload("Stock/barchart", bar_data, error);
					}
				}

			}
		}

	}

	
//resetting variables
app_state_.count = 0;
app_state_.quantity = 0;
}

void ken_app_main::on_delete_appointment() {
	// function for deleting information from the database
	std::string error, app_id;
	std::vector< widgets::listview_row> rows;
	get_listview_selected("Appointment/appointment_page_list", rows, error);

	// resetting the variables 
	app_state_.count = 0;
	app_state_.quantity = 0;

	if (rows.empty()) {
		prompt_params params;
		params.type = prompt_type::ok;
		prompt(params, "Error", "Please select the item to delete");
	}
	// getting the information from the database	
	for (const auto& row : rows) {
		ken_app_db::appointments_details details;
		widgets::listview_item items;

		for (const auto& item_ : row.items) {
			if (item_.column_name == "ID")
				app_id = item_.item_data;
		}
		std::string name = "Appointments";
		{
			prompt_params params;
			params.type = gui::prompt_type::yes_no;
			if (prompt(params, "DELETE", "Do you want to delete")) {
				if (!app_state_.get_db().delete_item(app_id, name, error)) {
					prompt_params params;
					params.type = gui::prompt_type::yes_no;
					if ((prompt(params, "Error", error))) {
						//getting the data from the database 
						std::vector<ken_app_db::appointments_details> appointments;
						std::string error;

						// getting the data from the database
						std::vector<widgets::listview_column> columns;
						std::vector<widgets::listview_row> data;
						if (app_state_.get_db().get_appointments(appointments, error)) {
							for (const auto& app : appointments) {
								widgets::listview_row row;

								row.items.push_back({ "ID", app.id });
								row.items.push_back({ "Name", app.name });
								row.items.push_back({ "Surname", app.surname });
								row.items.push_back({ "Description", app.description });
								row.items.push_back({ "Date", app.date });
								row.items.push_back({ "Time", app.time });

								data.push_back(row);
								app_state_.count++;
							}
						}

						//adding the data to the listview
						repopulate_listview("Appointment/appointment_page_list", data, error);
						widgets::barchart_data  bar_data;
						bar_data.autocolor = false;
						bar_data.autoscale = true;
						bar_data.caption = "Stock Barchart";
						bar_data.x_label = "Fields";
						bar_data.y_label = "Data";


						// setting out the bars in the barchart
						std::vector<widgets::chart_entry>bar_data_;

						// creating object for the chart entry
						widgets::chart_entry entry;
						entry.color = { 180 , 180, 180 };
						entry.label = "Appointments";
						entry.value = app_state_.count;
						bar_data_.push_back(entry);


						//creating a new object of chart entry 
						widgets::chart_entry entry2;
						entry2.label = "Stats";
						entry2.value = 50;
						entry2.color = { 180, 200, 255 };
						bar_data_.push_back(entry2);
						bar_data.bars = bar_data_;

						barchart_reload("Appointment/barchart", bar_data, error);
					}
					//restting the counting values
					app_state_.count = 0;
				}
			}
		}

	}




	//resetting variables
	app_state_.count = 0;
	app_state_.quantity = 0;

}

void ken_app_main::on_delete_user(){
}

ken_app_main::ken_app_main(const std::string& guid, state& app_state) :
	home_page_name("KEN_APP"),
	app_state_(app_state),
	gui::gui(guid){
}

ken_app_main::~ken_app_main(){
	// displaying the message that the system is shutting down whilst the system is shutting down
	std::string message = "The system is shutting down";
	gui::notification_params params;
	notification(params, "Error", message,1000);
}

// the layout of the main window
bool ken_app_main::layout(gui::page& persistent_page,
	gui::page& home_page, 
	std::string& error) {

	// set parameters
	home_page.set_name(home_page_name);

	////set up form icons 
	//set_icons(icon_resource,
	//	gui::caption_icon_png{ icon_16, icon_20 , icon_24, icon_28, icon_32 });

	// setting the parameters for the window
	set_width(app_state_.width_);
	set_min_width(width());
	set_height(app_state_.hieght_);
	set_min_height(height());

	// add stock
	widgets::icon icon_stock;
	icon_stock.rect = { margin , (long)width() - (margin + 780) , (long)height() - 520 , (long)height() - 420};
	icon_stock.text = "Stock";
	icon_stock.description = "View and Management Inventory";
	icon_stock.text_position = widgets::image_text_placement::right_top;
	icon_stock.filename = "stock.png";
	icon_stock.alias = "icon_stock";
	icon_stock.size = { 100 , 90 };
	icon_stock.on_resize.perc_height = 5;
	icon_stock.on_resize.perc_width = 5;
	icon_stock.on_resize.perc_h = 20;
	icon_stock.on_resize.perc_v = 20;
	icon_stock.on_click = [&]() { on_stock(); };



	home_page.add_icon(icon_stock);


	

	//add sales
	widgets::icon icon_sales;
	icon_sales.rect.left = icon_stock.rect.left + 270;
	icon_sales.rect.top = 50;
	icon_sales.rect.set_width(200);
	icon_sales.rect.set_height(100);
	icon_sales.text = "Sales";
	icon_sales.description = "Daily running of the Business";
	icon_sales.text_position = widgets::image_text_placement::right_top;
	icon_sales.filename = "sales.png";
	icon_sales.alias = "icon_sales";
	icon_sales.size = { 100 , 90 };
	icon_sales.on_resize.perc_height = 5;
	icon_sales.on_resize.perc_width = 5;
	icon_sales.on_resize.perc_h = 50;
	icon_sales.on_resize.perc_v = 20;
	icon_sales.on_click = [&](){ on_sales(); };


	home_page.add_icon(icon_sales);

	//add share information
	widgets::icon icon_credentials;
	icon_credentials.rect.left = icon_stock.rect.left;
	icon_credentials.rect.top = icon_sales.rect.top + 200;
	icon_credentials.rect.set_width(200);
	icon_credentials.rect.set_height(100);
	icon_credentials.text = "Users";
	icon_credentials.description = "All the credentials of the users";
	icon_credentials.text_position = widgets::image_text_placement::right_top;
	icon_credentials.filename = "users.png";
	icon_credentials.alias = "icon_credentials";
	icon_credentials.size = { 100 , 90 };
	icon_credentials.on_resize.perc_height = 5;
	icon_credentials.on_resize.perc_width = 5;
	icon_credentials.on_resize.perc_h = 20;
	icon_credentials.on_resize.perc_v = 80;
	icon_credentials.on_click = [&]() { on_users(); };

	home_page.add_icon(icon_credentials);

	//add  reports 
	widgets::icon icon_reports;
	icon_reports.rect.left = margin;
	icon_reports.rect.top = icon_credentials.rect.bottom + 150;
	icon_reports.rect.set_width(160);
	icon_reports.rect.set_height(60);
	icon_reports.text = "Reports";
	icon_reports.description = "Printing pdf";
	icon_reports.text_position = widgets::image_text_placement::right_top;
	icon_reports.filename = "report.png";
	icon_reports.alias = "icon_settings";
	icon_reports.size = { 48, 48 };
	icon_reports.on_resize.perc_height = 1;
	icon_reports.on_resize.perc_width = 1;
	icon_reports.on_resize.perc_h = 0;
	icon_reports.on_resize.perc_v = 100;

	persistent_page.add_icon(icon_reports);

	//add appointments
	widgets::icon icon_appointment;
	icon_appointment.rect.left = icon_stock.rect.left + 270;;
	icon_appointment.rect.top = icon_credentials.rect.top;
	icon_appointment.rect.set_width(200);
	icon_appointment.rect.set_height(100);
	icon_appointment.text = "Apointment";
	icon_appointment.description = " Manage Appointments";
	icon_appointment.text_position = widgets::image_text_placement::right_top;
	icon_appointment.filename = "appointment.jpg";
	icon_appointment.alias = "icon_appointment";
	icon_appointment.size = { 100 , 90 };
	icon_appointment.on_resize.perc_height = 5;
	icon_appointment.on_resize.perc_width = 5;
	icon_appointment.on_resize.perc_h = 50;
	icon_appointment.on_resize.perc_v = 80;
	icon_appointment.on_click = [&]() {on_appoinment(); };


	home_page.add_icon(icon_appointment);

	//add setting
	widgets::icon icon_settings;
	icon_settings.rect.left = icon_reports.rect.right + 20;
	icon_settings.rect.top = icon_reports.rect.top;
	icon_settings.rect.set_width(160);
	icon_settings.rect.set_height(60);
	icon_settings.text = "Settings";
	icon_settings.description = "Troubleshooting";
	icon_settings.text_position = widgets::image_text_placement::right_top;
	icon_settings.filename = "settings.png";
	icon_settings.alias = "icon_settings";
	icon_settings.size = { 48, 48 };
	icon_settings.on_resize.perc_height = 1;
	icon_settings.on_resize.perc_width = 1;
	icon_settings.on_resize.perc_h = 0;
	icon_settings.on_resize.perc_v =100;

	persistent_page.add_icon(icon_settings);

	// add a share icon for sharing information on a network
	widgets::icon icon_share;
	icon_share.rect.left = icon_settings.rect.right +35;
	icon_share.rect.top = icon_reports.rect.top;
	icon_share.rect.set_height(60);
	icon_share.rect.set_width(160);
	icon_share.text = "Sharing";
	icon_share.description = "Share files";
	icon_share.text_position = widgets::image_text_placement::right_top;
	icon_share.filename = "share.png";
	icon_share.alias = "icon_share";
	icon_share.size = { 42 , 42 };
	icon_share.on_resize.perc_height = 1;
	icon_share.on_resize.perc_width = 1;
	icon_share.on_resize.perc_h = 0;
	icon_share.on_resize.perc_v = 100;
	icon_share.on_click = [&]() { on_share(); };

	persistent_page.add_icon(icon_share);


	
	//add a list view for the appointments
	widgets::listview list_appointments;
	list_appointments.rect.top = icon_stock.rect.top;
	list_appointments.rect.left = icon_stock.rect.left + 600;
	list_appointments.rect.set_width(330);
	list_appointments.rect.set_height(450);
	list_appointments.alias = "list_appointments";
	list_appointments.border = true;
	list_appointments.gridlines= true;
	list_appointments.context_menu = {
		{"Mark Completed", false ,[&]() {on_appointment_completed(); }}
	};
 
	list_appointments.columns = {
	   app_state_.column_details("ID", 50 , widgets::listview_column_type::string_, color {236 ,28, 36}),
	   app_state_.column_details("Time", 100 , widgets::listview_column_type::string_, color {236 ,28, 36}),
	   app_state_.column_details("Date", 100 , widgets::listview_column_type::string_, color {236 ,28, 36} ),
	   app_state_.column_details("Name", 100, widgets::listview_column_type::string_,color {236 ,28, 36}),
	   app_state_.column_details("Description", 400, widgets::listview_column_type::string_, color {236 ,28, 36})
	};


	std::vector<ken_app_db::appointments_details> appointment;
	widgets::listview_row row;
	if (app_state_.get_db().get_appointments(appointment, error)) {


		for (const auto& appointment_ : appointment) {
			
			row.items.push_back({ "ID", appointment_.id, true , color {255, 0, 0 } });
			row.items.push_back({ "Time", appointment_.time , true , color {255, 0, 0 } });
			row.items.push_back({ "Date", appointment_.date, true , color {255, 0, 0 } });
			row.items.push_back({ "Name" , appointment_.name , true , color {255, 0, 0 } });
			row.items.push_back({ "Surname", appointment_.surname, true , color {255, 0, 0 } });
			row.items.push_back({ "Description" , appointment_.description, true , color {255, 0, 0 } });
			list_appointments.data.push_back(row);
		
		}
	}

	list_appointments.on_resize.perc_height = 80;
	list_appointments.on_resize.perc_width = 25;
	list_appointments.on_resize.perc_h = 60;
	list_appointments.on_resize.perc_v = 10;

	home_page.add_listview(list_appointments);

	// add caption 
	widgets::text caption_appointments;
	caption_appointments.alias = "caption_appointment";
	caption_appointments.rect.top = 20;
	caption_appointments.rect.left = icon_stock.rect.left + 650;
	caption_appointments.rect.set_height(30);
	caption_appointments.rect.set_width(200);
	caption_appointments.text_value = "Appointments View";
	caption_appointments.alignment = widgets::text_alignment::middle;
	caption_appointments.color = color{ 180 , 180 , 180 };
	
	caption_appointments.on_resize.perc_height = 5;
	caption_appointments.on_resize.perc_width = 5;
	caption_appointments.on_resize.perc_h = 60;
	caption_appointments.on_resize.perc_v = 10;


	// wrapping the appointments in a listview

	widgets::groupbox group;
	group.rects = {
		icon_settings.rect ,
		icon_reports.rect ,
		icon_share.rect
	};
	group.on_resize.perc_h = 0;
	group.on_resize.perc_v = 100;

	persistent_page.add_groupbox(group);

	home_page.add_text(caption_appointments);
	//add icon for the search bar

	// adding a time stamp and date
	const auto today = liblec::cui::date_gen::time_stamp();
	widgets::text time;
	time.alias = "time stamp";
	time.font_size = 10;
	time.text_value = "Date and Time : " + today;
	time.rect.left = icon_share.rect.right + 100;
	time.rect.top = icon_share.rect.top + 20;
	time.rect.set_height(20);
	time.rect.set_width(300);
	time.color = color{ 65,105,225 };
	time.on_resize.perc_h = icon_share.on_resize.perc_h + 120;
	time.on_resize.perc_v = icon_share.on_resize.perc_v;

	persistent_page.add_text(time);
	
	return true;
}



