#include "appointment.h"

void appointment::on_caption()
{
	gui::prompt_params params;
	params.type = gui::prompt_type::ok;
	prompt(params, "", app_state_.version_info());
}

void appointment::on_stop()
{
	prompt_params params;
	params.type = prompt_type::yes_no;
	if (prompt(params, "", "Are you sure you want to close"))
		close();
      
}

void appointment::on_shutdown()
{
}

void appointment::on_add()
{
	// declaring the values that we gonna get form the textbox
	ken_app_db::appointments_details details;
	std::string appointment_id , error;
	
	get_editbox_text(home_page_name + "/name_edit", details.name, error);
	get_editbox_text(home_page_name + "/surname_edit", details.surname, error);
	get_editbox_text(home_page_name + "/description_edit", details.description, error);
	get_editbox_text(home_page_name + "/time_edit",details.time, error);
	get_editbox_text(home_page_name + "/date_edit", details.date, error);

	
	
	// assingning the id to be a unique number and assinging it to appointment id
	details.id = unique_string_short();
	appointment_id = details.id;

	// new variable for date and time 
	
	 //getting the listview and its columns and data 
	std::vector<widgets::listview_column> columns;
	std::vector<widgets::listview_row> data;
	get_listview(home_page_name + "/appointment_list", columns, data, error);


	for (auto& row : data) {
		for (auto& item : row.items) {
			if (item.column_name == "ID")
				if (appointment_id == item.item_data)
					return;
		}
	}

	
	widgets::listview_row row = {
		{{{"Name"} , {details.name}},
		{{ "Description"}, {details.description}},
		{{"Surname"} , {details.surname}},
		{{ "Time" } , {details.time}},
		{{"Date"} , {details.date}},
		{{ "ID" } , {details.id}}}
	};

	add_listview_row(home_page_name + "/appointment_list", row, true, error);
	
}

void appointment::on_save()
{
	ken_app_db::appointments_details details;
	std::string error;
	std::string appointments_id;


	get_editbox_text(home_page_name + "/name_edit", details.name, error);
	get_editbox_text(home_page_name + "/surname_edit", details.surname, error);
	get_editbox_text(home_page_name + "/description_edit", details.description, error);
	get_editbox_text(home_page_name + "/time_edit", details.time, error);
	get_editbox_text(home_page_name + "/date_edit", details.date, error);



	// assingning the id to be a unique number and assinging it to appointment id
	details.id = unique_string_short();
	appointments_id = details.id;

	// inserting appointments details in the database
	if (!app_state_.get_db().new_appointment(details, error)) {
		prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", error);
		return;
     }

	// getting the new appointment in the database and assinging them to details struct
	if (!app_state_.get_db().get_appointment(appointments_id, details, error)) {
		prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", error);
		return;
	}
	
	// getting the listview and its columns and data 
	std::vector<widgets::listview_column> columns;
	std::vector<widgets::listview_row> data;
	get_listview("Appointment/Appointment_page_list", columns, data, error);


	for (auto& row : data) {
		for (auto& item : row.items) {
			if (item.column_name == "ID")
				if (appointments_id == item.item_data)
					return;
		}
	}

	widgets::listview_row row = {
		{{{"Name"} , {details.name}},
		{{ "Description"}, {details.description}},
		{{"Surname"} , {details.surname}},
		{{ "Time" } , {details.time}},
		{{"Date"} , {details.date}},
		{{ "ID" } , {details.id}}}
	};
     
	//adding the row to the appointments listview
	add_listview_row(home_page_name + "/Appointment_page_list", row, true, error);

	saved_ = true;
	details_ = details;
	stop();
}


appointment::appointment(state& app_state):
	home_page_name("Appointment"),
	app_state_(app_state)
{
}

appointment::~appointment()
{
}

bool appointment::layout(gui::page& persistent_page, gui::page& home_page, std::string& error)
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
	name_caption.rect.left = width()- 340;
	name_caption.rect.top = height() - 390;
	name_caption.rect.set_height(20);
	name_caption.rect.set_width(100);

	home_page.add_text(name_caption);


	// add a name textbox
	widgets::editbox name_edit;
	name_edit.alias = "name_edit";
	name_edit.rect.left = name_caption.rect.left;
	name_edit.rect.top = name_caption.rect.bottom + 3;
	name_edit.rect.set_height(20);
	name_edit.rect.set_width(140);

	home_page.add_editbox(name_edit);

	// add a surname description 
	widgets::text surname_caption;
	surname_caption.alias = "surname_caption";
	surname_caption.text_value = "Surname";
	surname_caption.color = color{ 180 , 180 , 180 };
	surname_caption.rect.left = name_caption.rect.right + 80;
	surname_caption.rect.top = name_caption.rect.top;
	surname_caption.rect.set_height(20);
	surname_caption.rect.set_width(100);

	home_page.add_text(surname_caption);

	// add a surname textbox
	widgets::editbox surname;
	surname.alias = "surname_edit";
	surname.rect.left = surname_caption.rect.left;
	surname.rect.top = name_edit.rect.top;
	surname.rect.set_height(20);
	surname.rect.set_width(140);

	home_page.add_editbox(surname);


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

	// add caption for Time
	widgets::text Time_caption;
	Time_caption.alias = "Time_caption";
	Time_caption.text_value = "Date and Time";
	Time_caption.color = color{ 180 , 180 , 180 };
	Time_caption.rect.left = description.rect.right + 40;
	Time_caption.rect.top = description_caption.rect.top;
	Time_caption.rect.set_height(20);
	Time_caption.rect.set_width(100);

	home_page.add_text(Time_caption);


	// add a quantity textbox 
	widgets::editbox date;
	date.alias = "date_edit";
	date.rect.left = description.rect.right + 40;
	date.rect.top = description.rect.top;
	date.rect.set_height(20);
	date.rect.set_width(80);

	home_page.add_editbox(date);

	// add a time textbox for selecting 
	widgets::editbox time;
	time.alias = "time_edit";
	time.rect.left = date.rect.left;
	time.rect.top = date.rect.bottom + 6;
	time.rect.set_height(20);
	time.rect.set_width(80);

	home_page.add_editbox(time);

	// add a button to add the added items into a listview
	widgets::button add;
	add.tooltip = "Add items";
	add.rect.left = description.rect.right + 45;
	add.rect.top = time.rect.bottom + 6;
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
		date.rect,
		Time_caption.rect,
		add.rect,
		time.rect
		
	};

	home_page.add_groupbox(box);

	// add a listview 

	widgets::listview appointment_list;
	appointment_list.border = false;
	appointment_list.gridlines = true;
	appointment_list.rect.left = 10;
	appointment_list.rect.top = description.rect.bottom + 40;
	appointment_list.rect.set_height(140);
	appointment_list.rect.set_width(330);
	appointment_list.alias = "appointment_list";

	appointment_list.columns = {
		app_state_.column_details("ID", 60 , widgets::listview_column_type::int_),
		app_state_.column_details("Time", 80 , widgets::listview_column_type::string_),
		app_state_.column_details("Date", 80 , widgets::listview_column_type::string_),
		app_state_.column_details( "Name" , 180  , widgets::listview_column_type::string_),
		app_state_.column_details("Surname" , 100 , widgets::listview_column_type::string_),
		app_state_.column_details("Description", 200 , widgets::listview_column_type::string_)
	};

	appointment_list.unique_column_name = "ID";

	home_page.add_listview(appointment_list);

	// add a sava button 
	widgets::button save;
	save.caption = "Save";
	save.rect.left = 130;
	save.rect.top = appointment_list.rect.bottom + 10;
	save.rect.set_height(25);
	save.rect.set_width(80);
	save.on_click = [&]() { on_save(); };

	home_page.add_button(save);
	return true;
}

bool appointment::saved()
{
	return saved_;
}

const ken_app_db::appointments_details& appointment::get_details()
{
	return details_;
}
