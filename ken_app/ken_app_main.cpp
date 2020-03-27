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

void ken_app_main::on_stock(){

	if (!page_exists("Stock")) {
		page page("Stock");

		//add back icon 
		widgets::image back;
		back.png_resource = png_back;
		back.rect.left = 900;
		back.rect.top = 400;
		back.rect.set_height(50);
		back.rect.set_width(100);
		back.on_click = [&]() {
			show_previous_page();
			// to-do::
			// this is where you put the code for updating the home page when there are new appointments to be put on the home page
		};

		page.add_image(back);
	}

}


ken_app_main::ken_app_main(const std::string& guid, state& app_state) :
	home_page_name("Main_form"),
	app_state_(app_state),
	gui::gui(guid){
}

ken_app_main::~ken_app_main()
{
}

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

	list_appointments.columns = {
	{"", 80 , widgets::listview_column_type::string_}
	};

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

