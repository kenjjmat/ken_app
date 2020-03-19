
#include "ken_app_login.h"
#include "ken_app_main.h"
#include "ken_app_new_user.h"
#include "ken_app_db.h"

// gui app using main
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {


	// constants used by the app to only allow one instance
	const std::string guid_login = "{F7AA7B63 - 6AAC - 4B7D - 84DC - 107F352217A1}";
	const std::string guid_main = "{D1E8548A - 0749 - 4D2C - 86B8 - 26EB5DB5AF5F}";

	// lambda function for showing the error
	auto on_error = [&](gui& ui, const std::string& error) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		params.png_icon_resource = dispaly_error;
		ui.prompt(params, "Error", error);
	};

	// appication state object
	state state_data;
	std::string error;

	// run login form
	ken_app_login login(guid_login, state_data);

	// connect to the database 
	if (!state_data.get_db().connect("database.dbs", "", error)) {
		on_error(login, error);
		return 1;
	}
	

	// check if there are any users in the database
	std::vector<ken_app_db::user_credentials> users;
	if (!state_data.get_db().get_users(users, error)) {
		on_error(login, error);
		return 1;
	}

	if (users.empty()) {
		// load the new user form 
		ken_app_new_user newuser(guid_login, state_data);

	    // run new user 
		if (!newuser.run(error)) {
			on_error(newuser, error);
			return 1;
		}

		if (!state_data.get_db().get_users(users, error)) {
			on_error(newuser, error);
			return 1;
		}
		if (users.empty()) return 0;
	}
	
	// run login
	if (!login.run(guid_login, error)) {
		on_error(login, error);
	}
	

	if (state_data.loggedin()) {
		 //main form
		ken_app_main main_form(guid_main, state_data);

		// run main form
		if (!main_form.run(guid_main, error)) {
			on_error(main_form, error);
		}
	}

	return 0;
	
}

