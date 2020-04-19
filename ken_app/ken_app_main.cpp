#include "ken_app_main.h"
#include "stock.h"
#include "sales.h"

void ken_app_main::on_caption(){
	gui::prompt_params params;
	params.type = gui::prompt_type::ok;
	prompt(params, "", app_state_.version_info());
}

void ken_app_main::on_stop(){
	stop();
}

void ken_app_main::on_shutdown()
{
}

// when the stock image has been clicked
void ken_app_main::on_stock(){

	if (!page_exists("Stock")) {
		page page("Stock");

		//add back icon 
		widgets::image back;
		back.toggle = "Previous page";
		back.filename = "back.png";
		back.rect.left = 10;
		back.rect.top =  10;
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
		title.text_value = "Stock";
		title.font_size = 16;
		title.rect.left = back.rect.right+10;
		title.rect.top =  back.rect.top;
		title.rect.set_height(40);
		title.rect.set_width(100);

		page.add_text(title);

		// add description 

		widgets::text description;
		description.text_value = "View and Manage Inventory";
		description.color = color{ 180 , 180 , 180 };
		description.rect.left =  back.rect.right + 10;
		description.rect.top = title.rect.top + 30;
		description.rect.set_height(20);
		description.rect.set_width(200);

		page.add_text(description);

	


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

		page.add_image(image_delete);
		
		// add stock listview 
		widgets::listview stock_list;
		stock_list.alias = "stock_list";
		stock_list.rect.left = image.rect.right + 50;
		stock_list.rect.top = image.rect.top;
		stock_list.rect.set_height(300);
		stock_list.rect.set_width(400);
		stock_list.border = true;
		// on resize 
		stock_list.on_resize.perc_h = 0;
		stock_list.on_resize.perc_v = 5;
		stock_list.on_resize.perc_height = 90;
		stock_list.on_resize.perc_width = 25;

		//this code is giving a error find a way around it

		//stock_list.columns = {
		//	{"ID", 35 , widgets::listview_column_type::int_ },
		//	{"Name" , 170 , widgets::listview_column_type::string_},
		//	{"Description" , 200 , widgets::listview_column_type::string_},
		//	{"Quantity" , 50  , widgets::listview_column_type::int_	 }
		//};
		//stock_list.unique_column_name = "ID";

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
				}
		   }
		}
		page.add_listview(stock_list);

		// adding a barchart 
		widgets::barchart bar;
		bar.alias = "barchart";
		bar.data.autocolor = false;
		bar.data.x_label = "Fields";
		bar.data.y_label = "Data";
		bar.data.caption = " Stock BarChart";
		bar.rect.left = stock_list.rect.right + 20;
		bar.rect.top = stock_list.rect.top;
		bar.rect.set_height(300);
		bar.rect.set_width(400);
		bar.on_resize.perc_h = stock_list.on_resize.perc_width + 10;
		bar.on_resize.perc_v = stock_list.on_resize.perc_height + 5;

		// setting out the bars in the barchart
			std::vector<widgets::chart_entry>bar_data;
			// creating object for the chart entry
			widgets::chart_entry details;

			// to-do::
			// the barchart must desplay information from the database that is in the listview

			// assigning the values in bar eat
			details.label = "Eat";
			details.color = color{ 180 , 180 , 180 };
			details.value = 50;

			// assigning the values in bar drink
			widgets::chart_entry detail_;
			detail_.label = "Drink";
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
	show_page("Stock");

	// to-do::
	// when on_resize the listview must increase the width and height and the barchar must move a little lower
	//on the bottom so as to increase the space where the information in the listview is going to be desplayed
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
	// to-do:
	// This is where you gonna put the code for collecting information from the database and insert it into the listview
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

		page.add_image(image_delete);

		// add stock listview 
		widgets::listview sales_list;
		sales_list.alias = "sales_list";
		sales_list.rect.left = image.rect.right + 50;
		sales_list.rect.top = image.rect.top;
		sales_list.rect.set_height(300);
		sales_list.rect.set_width(400);
		sales_list.border = true;
		// on resize 
		sales_list.on_resize.perc_h = 0;
		sales_list.on_resize.perc_v = 5;
		sales_list.on_resize.perc_height = 90;
		sales_list.on_resize.perc_width = 25;

		//this code is giving a error find a way around it

		//sales_list.columns = {
		//	{"ID", 35 , widgets::listview_column_type::int_ },
		//	{"Name" , 170 , widgets::listview_column_type::string_},
		//	{"Description" , 200 , widgets::listview_column_type::string_},
		//	{"Quantity" , 50  , widgets::listview_column_type::int_	 }
		//};
		//sales_list.unique_column_name = "ID";

		{
			std::vector<ken_app_db::sales_details> sales;
			std::string error;

			if (app_state_.get_db().get_sales_all(sales, error)) {
				int i = 0;

				for (const auto& sales_ : sales) {
					widgets::listview_row row;
					row.items.push_back({ "ID", sales_.id });
					row.items.push_back({ "Name" , sales_.item_name });
					row.items.push_back({ "Quantity" , sales_.quantity });
					row.items.push_back({ "Unit_price", sales_.Unit_price });
					row.items.push_back({ "Cost"	, sales_.Cost });
				}
			}
		}
		page.add_listview(sales_list);

		// adding a pie chart 
		widgets::piechart pie;
		pie.rect.left = sales_list.rect.right + 20 ;
		pie.rect.top = sales_list.rect.top;
		pie.rect.set_height(300);
		pie.rect.set_width(400);
		pie.alias = "piechart";
		pie.data.caption = "Sales statistics";
		pie.data.autocolor = false;
		pie.data.doughnut = true;
		pie.data.on_hover = widgets::piechart_hover_effect::glow_and_shrink_others;
		
		// assigning values to the pie chart 
		std::vector< widgets::chart_entry> pie_data;

		// creating object for chart_entry 
		widgets::chart_entry details;
		details.color = color { 180 , 200 , 255 }; 
		details.label = "Eat";
		details.value = 50;

		// creating a second object of chart entry
		widgets::chart_entry details_;
		details_.color = color{ 180 , 180 , 180 };
		details_.label = "Drinks";
		details_.value = 50;

		pie_data.push_back(details);
		pie_data.push_back(details_);

		pie.data.slices = pie_data;

		page.add_piechart(pie);

		// wrapping the pie chart in a groupbox 
		widgets::groupbox group;
		group.color.blue;
		group.rects = {
			pie.rect
		};

		page.add_groupbox(group);

		// adding the page to the window
		add_page(page);
	}
	show_page("Sales");
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
	// to-do:
	// This is where you gonna put the code for collecting information from the database and insert it into the listview
}


ken_app_main::ken_app_main(const std::string& guid, state& app_state) :
	home_page_name("KEN_APP"),
	app_state_(app_state),
	gui::gui(guid){
}

ken_app_main::~ken_app_main()
{
}

// the layout of the main window
bool ken_app_main::layout(gui::page& persistent_page,
	gui::page& home_page, 
	std::string& error) {

	// set parameters
	home_page.set_name(home_page_name);

	// setting the parameters for the window
	set_width(1000);
	set_min_width(1000);
	set_height(500);
	set_min_height(500);

	// add stock
	widgets::icon icon_stock;
	icon_stock.rect.left =50;
	icon_stock.rect.top = 50;
	icon_stock.rect.set_width(200);
	icon_stock.rect.set_height(100);
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


	//add  reports 
	widgets::icon icon_reports;
	icon_reports.rect.left = 10;
	icon_reports.rect.top = 400;
	icon_reports.rect.set_width(160);
	icon_reports.rect.set_height(60);
	icon_reports.text = "Reports";
	icon_reports.description = "Printing pdf";
	icon_reports.text_position = widgets::image_text_placement::right_top;
	icon_reports.filename = "report.png";
	icon_reports.alias = "icon_settings";
	icon_reports.size = { 48, 48 };
	icon_reports.on_resize.perc_height = 5;
	icon_reports.on_resize.perc_width = 5;
	icon_reports.on_resize.perc_h = 0;
	icon_reports.on_resize.perc_v = 100;

	persistent_page.add_icon(icon_reports);

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
	icon_sales.on_click = [&] { on_sales(); };


	home_page.add_icon(icon_sales);

	//add share information
	widgets::icon icon_credentials;
	icon_credentials.rect.left = 50;
	icon_credentials.rect.top = icon_sales.rect.top + 170;
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

	home_page.add_icon(icon_credentials);

	//add appointments
	widgets::icon icon_appointment;
	icon_appointment.rect.left = icon_stock.rect.left + 270;;
	icon_appointment.rect.top = icon_sales.rect.top + 170;
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


	home_page.add_icon(icon_appointment);

	//add setting
	widgets::icon icon_settings;
	icon_settings.rect.left = icon_reports.rect.left + 210;
	icon_settings.rect.top = 400;
	icon_settings.rect.set_width(160);
	icon_settings.rect.set_height(60);
	icon_settings.text = "Settings";
	icon_settings.description = "Troubleshooting";
	icon_settings.text_position = widgets::image_text_placement::right_top;
	icon_settings.filename = "settings.png";
	icon_settings.alias = "icon_settings";
	icon_settings.size = { 48, 48 };
	icon_settings.on_resize.perc_height = 5;
	icon_settings.on_resize.perc_width = 5;
	icon_settings.on_resize.perc_h = 0;
	icon_settings.on_resize.perc_v =100;

	persistent_page.add_icon(icon_settings);
	
	//add a list view for the appointments
	widgets::listview list_appointments;
	list_appointments.rect.top = 50;
	list_appointments.rect.left = icon_stock.rect.left + 600;
	list_appointments.rect.set_width(320);
	list_appointments.rect.set_height(350);
	list_appointments.alias = "list_appointments";
	list_appointments.border = true;
	list_appointments.gridlines= true;

	// this code is giving me an error find a way around it 
	/*list_appointments.columns = {
		{"", 80 , widgets::listview_column_type::string_}
	};*/

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


	home_page.add_text(caption_appointments);
	//add icon for the search bar
	
	return true;
}


