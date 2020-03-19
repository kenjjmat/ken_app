#include "ken_app_new_user.h"

void ken_app_new_user::on_stop(){
	stop();
}

void ken_app_new_user::on_shutdown(){


}

void ken_app_new_user::on_save() {

// get username and password 
	std::string username, password, confirm_password, position, error;
	get_editbox_text(home_page_name + "/username_new_user", username, error);
	get_editbox_text(home_page_name + "/password_new_user", password, error);
	get_editbox_text(home_page_name + "/password_confirm", confirm_password, error);

	if (username.empty() || password.empty()) return;

	if (password != confirm_password) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", "Password mis-match");
		return;
	}

	ken_app_db::user_credentials user;
	user.username = username;

	if (!app_state_.get_db().new_user(user, password, error)) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		prompt(params, "Error", error);
	}


	stop();
    
}

void ken_app_new_user::on_password() {

	//  get username and password form the textboxes
	std::string username, password, error;
	get_editbox_text(home_page_name + "/username_new_user", username, error);
	get_editbox_text(home_page_name + "/password_new_user", password, error);

	// get the password rating ans set password strenght bar accordingly
	const auto rating = password_rating(username, password);
	set_password_strength_bar(home_page_name + "/password_strength", rating.strength, error);
	set_text(home_page_name + "/rating", rating.rating_summary, error);

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
	set_height(480);
	set_min_height(480);

	// add caption 
	widgets::text caption;
	caption.rect = { 50 , (long)width() - 50 , 210 , 230 };
	caption.text_value = "Add a new User to Database";
	caption.alignment = widgets::text_alignment::center;
	caption.color = { 180 , 180 , 180 };

	home_page.add_text(caption);

	// adding a image 
	widgets::image image_new_user;
	image_new_user.rect = { 40 , (long)width() - 50 , 30, 160 };
	//image_new_user.filename = "login.png";
	image_new_user.filename = "new_user.png";
	home_page.add_image(image_new_user);

	// add editbox

	widgets::editbox username_new_user;
	username_new_user.alias = "username_new_user";
	username_new_user.cue_banner = "username";
	username_new_user.rect = { 50 ,(long)width() - 50 , 235 , 255 };
	username_new_user.on_type = [&]() { on_password(); };
	home_page.add_editbox(username_new_user);

	//add passowrd editbox
	widgets::editbox password_new_user;
	password_new_user.alias = "password_new_user";
	password_new_user.cue_banner = "Password";
	password_new_user.password = true;
	password_new_user.rect = { 50 , (long)width() - 50  , 265 ,285 };
	password_new_user.on_type = [&]() { on_password(); };
	home_page.add_editbox(password_new_user);

	// add editbox
	widgets::editbox password_confirm;
	password_confirm.alias = "password_confirm";
	password_confirm.cue_banner = "confirm password";
	password_confirm.password = true;
	password_confirm.rect = { 50 ,(long)width() - 50 , 300 , 320 };
	home_page.add_editbox(password_confirm);

	
	//add password strenght bar
	widgets::password_strength_bar password_strength;
	password_strength.alias = "password_strength";
	password_strength.rect = { 50 , (long)width() - 50 , 355, 360 };
	home_page.add_password_strength_bar(password_strength);

	//add password strength summary caption 
	widgets::text rating;
	rating.alias = "rating";
	rating.rect = { 50 , (long)width() - 50 , 360 , 380 };
	rating.alignment = widgets::text_alignment::center;
	rating.color = { 180 , 180 , 180 };
	home_page.add_text(rating);

	//add a group box control 
	widgets::groupbox group;
	group.rects = {
		username_new_user.rect,
		password_new_user.rect,
		password_confirm.rect ,
		password_strength.rect,
		rating.rect
	};

	home_page.add_groupbox(group);

	//add save button
	widgets::button button_new_user;
	button_new_user.alias = "button_new_user";
	button_new_user.caption = "Save";
	button_new_user.rect = { 120 , (long)width() - 120 , 400 , 425 };
	button_new_user.on_click = [&]() { on_save(); };
	home_page.add_button(button_new_user);

	return true;
	return true;
}
