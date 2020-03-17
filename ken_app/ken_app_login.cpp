#include "ken_app_login.h"
#include "resource.h"
#include "ken_app_db.h"
#include <map>


void ken_app_login::on_caption(){
}

void ken_app_login::on_stop() {
	stop();
}


void ken_app_login::on_shutdown(){
}

// on logging into 
void ken_app_login::on_login(){

	// get the username and password
	std::string username;
	std::string password;
	std::string error;

if (get_editbox_text(home_page_name + "/username", username, error) &&
	get_editbox_text(home_page_name + "/password", password, error)) {

	if (username.empty() || password.empty()) {
		set_focus(home_page_name + "/username");
		set_focus(home_page_name + "/password");
		return;
	}

	std::map<std::string, std::string> users;
	users["kennedy"] = "kenny2811";

	bool found = false;
	for (auto& it : users) {
		if (it.first == username && it.second == password) {
			found = true;
			app_state_.log();
		}
	}

	if (!found)
	{
		gui::prompt_params params_;
		params_.type = gui::prompt_type::ok;
		params_.png_icon_resource = dispaly_error;
		prompt(params_, "Error, Wrong Credentials", error);
		set_editbox_text(home_page_name + "/username", "", error);
		set_focus(home_page_name + "/username");
		set_editbox_text(home_page_name + "/password", "", error);
		set_focus(home_page_name + "/password");
		return;
	}
	}
	else

		return;


	
	stop();
}

ken_app_login::ken_app_login(const std::string& guid, state& app_state):
	gui::gui(guid),
	app_state_(app_state),
	home_page_name("ken_app"){

}



ken_app_login::~ken_app_login(){

}

bool ken_app_login::layout(gui::page& persistent_page,
	gui::page& home_page,
	std::string& error)
{
	home_page.set_name(home_page_name);

	// set form parameters
	prevent_resizing();

	// set form dimensions

	set_width(350);
	set_min_width(350);
	set_height(400);
	set_min_height(400);

	// adding a image 
	widgets::image image_login;
	image_login.rect = {40 , (long)width()-50 , 30, 160 };
	//image_login.filename = "login.png";
	image_login.filename = "LOGIN.jpg";
	home_page.add_image(image_login);

	//add caption
	widgets::text caption_login;

    
	caption_login.rect = { 50 , (long)width() - 50  , 205 , 225 };
	caption_login.text_value = "Enter Credentials";
	caption_login.alignment = widgets::text_alignment::center;
	caption_login.color = { 180 , 180 ,180 };
	home_page.add_text(caption_login);

	// add editbox
	
	widgets::editbox username_login;
	username_login.alias = "username";
	username_login.cue_banner = "username";
	username_login.rect = { 50 ,(long)width() - 50 , 230 , 250 };
	home_page.add_editbox(username_login);


	//add passowrd editbox
	widgets::editbox password_login;
	password_login.alias = "password";
	password_login.cue_banner = "Password";
	password_login.password = true;
	password_login.rect = { 50 , (long)width() - 50  , 260 ,280 };
	home_page.add_editbox(password_login);

	//add login button
	widgets::button button_login;
	button_login.alias = "login_button";
	button_login.caption = "LOG IN";
	button_login.rect = { 120 , (long)width() - 120 , 295 , 320 };
	button_login.on_click = [&]() { on_login(); };
	home_page.add_button(button_login);

	return true;
}
