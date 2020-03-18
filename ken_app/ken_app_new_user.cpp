#include "ken_app_new_user.h"

void ken_app_new_user::on_stop(){
	stop();
}

void ken_app_new_user::on_shutdown(){


}

void ken_app_new_user::on_save() {
    
}

ken_app_new_user::ken_app_new_user(const std::string& guid, state& app_state_): 
	gui::gui(guid), 
	app_state_(app_state_),
	home_page_name("Create New User") {

}

ken_app_new_user::~ken_app_new_user()
{
}

bool ken_app_new_user::layout(gui::page& persistent_page, gui::page& home_page, std::string& error)
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
	widgets::image image_new_user;
	image_new_user.rect = { 40 , (long)width() - 50 , 30, 160 };
	//image_new_user.filename = "login.png";
	image_new_user.filename = "new_user.png";
	home_page.add_image(image_new_user);

	//add caption
	widgets::text caption_new_user;
	caption_new_user.rect = { 50 , (long)width() - 50  , 200 , 220 };
	caption_new_user.text_value = "Enter Credentials";
	caption_new_user.alignment = widgets::text_alignment::center;
	caption_new_user.color = { 180 , 180 ,180 };
	home_page.add_text(caption_new_user);

	// add editbox

	widgets::editbox username_new_user;
	username_new_user.alias = "username_new_user";
	username_new_user.cue_banner = "username";
	username_new_user.rect = { 50 ,(long)width() - 50 , 210 , 230 };
	home_page.add_editbox(username_new_user);

	// add editbox

	widgets::editbox password_confirm;
	password_confirm.alias = "password_confirm";
	password_confirm.cue_banner = "confirm password";
	password_confirm.rect = { 50 ,(long)width() - 50 , 230 , 250 };
	home_page.add_editbox(password_confirm);

	//add passowrd editbox
	widgets::editbox password_new_user;
	password_new_user.alias = "password_new_user";
	password_new_user.cue_banner = "Password";
	password_new_user.password = true;
	password_new_user.rect = { 50 , (long)width() - 50  , 260 ,280 };
	home_page.add_editbox(password_new_user);

	//add login button
	widgets::button button_new_user;
	button_new_user.alias = "button_new_user";
	button_new_user.caption = "Save";
	button_new_user.rect = { 120 , (long)width() - 120 , 295 , 320 };
	button_new_user.on_click = [&]() { on_save(); };
	home_page.add_button(button_new_user);

	return true;
	return true;
}
