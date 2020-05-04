#include "ken_app_login.h"
#include "resource.h"
#include "ken_app_db.h"
#include <map>


void ken_app_login::on_caption(){
	gui::prompt_params params;
	params.type = gui::prompt_type::ok;
	params.png_icon_resource = icon_resource;
	prompt(params, "", app_state_.version_info());
}

void ken_app_login::on_stop() {
	stop();
}


void ken_app_login::on_shutdown(){
}

void ken_app_login::on_create_account() {

	app_state_.create();
	stop();
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
	bool found = false;
	if (!app_state_.get_db().get_user(username, password, app_state_.get_user_(), error)) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", error);
	}
	else
	{
		found = true;
		app_state_.log();
	}


	if (!found)
	{
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

	//set up form icons // but still not working ohh no !!!

	// adding a image 
	widgets::image image_login;
	image_login.rect = {40 , (long)width()-50 , 30, 160 };
	//image_login.filename = "login.png";
	image_login.filename = "LOGIN.jpg";
	image_login.color_background_hot = color{ 255, 255, 255 };
	image_login.tight_fit = true;
	image_login.bar = false;
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

	//add text for creating an account
	widgets::text create_account;
	create_account.alias = "create_account_text";
	create_account.text_value = "Create New Account";
	create_account.rect = { 120 , (long)width() - 100 , 350 , 366 };
	create_account.on_click = [&]() { on_create_account(); };
	create_account.color = { 65,105,225 };
	home_page.add_text(create_account);

	return true;
}
