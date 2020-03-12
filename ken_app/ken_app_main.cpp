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
	set_width(800);
	set_min_width(1000);
	set_height(500);
	set_min_height(500);

	// add stock
	widgets::icon icon_stock;
	icon_stock.rect.left =50;
	icon_stock.rect.top = 20;
	icon_stock.rect.set_width(200);
	icon_stock.rect.set_height(100);
	icon_stock.text = "Stock";
	icon_stock.description = "View and Management of All Inventory";
	icon_stock.text_position = widgets::image_text_placement::right_top;
	icon_stock.filename = "stock.png";
	icon_stock.alias = "icon_stock";
	icon_stock.size = { 100 , 90 };

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

	persistent_page.add_icon(icon_reports);

	//add sales
	widgets::icon icon_sales;
	icon_sales.rect.left = icon_stock.rect.left + 200 + 100;
	icon_sales.rect.top = 20;
	icon_sales.rect.set_width(200);
	icon_sales.rect.set_height(100);
	icon_sales.text = "Sales";
	icon_sales.description = "Daily running of the Business";
	icon_sales.text_position = widgets::image_text_placement::right_top;
	icon_sales.filename = "sales.png";
	icon_sales.alias = "icon_sales";
	icon_sales.size = { 100 , 90 };

	home_page.add_icon(icon_sales);

	//add share information
	widgets::icon icon_credentials;
	icon_credentials.rect.left = 50;
	icon_credentials.rect.top = icon_sales.rect.top + 100 + 100;
	icon_credentials.rect.set_width(200);
	icon_credentials.rect.set_height(100);
	icon_credentials.text = "Users";
	icon_credentials.description = "All the credentials of the users";
	icon_credentials.text_position = widgets::image_text_placement::right_top;
	icon_credentials.filename = "users.png";
	icon_credentials.alias = "icon_credentials";
	icon_credentials.size = { 100 , 90 };

	home_page.add_icon(icon_credentials);

	//add appointments
	widgets::icon icon_appointment;
	icon_appointment.rect.left = icon_stock.rect.left + 200 + 100;;
	icon_appointment.rect.top = icon_sales.rect.top + 100 + 100;
	icon_appointment.rect.set_width(200);
	icon_appointment.rect.set_height(100);
	icon_appointment.text = "Apointment";
	icon_appointment.description = "View and Manage Appointments";
	icon_appointment.text_position = widgets::image_text_placement::right_top;
	icon_appointment.filename = "appointment.jpg";
	icon_appointment.alias = "icon_appointment";
	icon_appointment.size = { 100 , 90 };

	home_page.add_icon(icon_appointment);

	//add setting
	widgets::icon icon_settings;
	icon_settings.rect.left = icon_reports.rect.left + 200 + 10;
	icon_settings.rect.top = 400;
	icon_settings.rect.set_width(160);
	icon_settings.rect.set_height(60);
	icon_settings.text = "Settings";
	icon_settings.description = "Troubleshooting";
	icon_settings.text_position = widgets::image_text_placement::right_top;
	icon_settings.filename = "settings.png";
	icon_settings.alias = "icon_settings";
	icon_settings.size = { 48, 48 };

	persistent_page.add_icon(icon_settings);
	
	//add a list view for the appointments
	widgets::listview list_appointments;
	list_appointments.rect.top = 20;
	list_appointments.rect.left = icon_stock.rect.left + 200 + 100 + 10 ;
	list_appointments.alias = "list_appointments";
	list_appointments.border = false;
	list_appointments.gridlines = false;

	list_appointments.columns = {
		{"# ", 35 ,widgets::listview_column_type::int_},
	{"Name" , 170 , widgets::listview_column_type::string_} , 
	{"Surmane", 170 , widgets::listview_column_type::string_},
	{"Contact Details", 170 , widgets::listview_column_type::string_}
	};

	list_appointments.unique_column_name = "#";


	home_page.add_listview(list_appointments);
	return true;
}
