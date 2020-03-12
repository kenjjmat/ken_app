
#include "ken_app_login.h"
#include "ken_app_main.h"

// gui app using main
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {

	// lambda function for showing the error
	auto on_error = [&](gui& ui, const std::string& error) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		params.png_icon_resource = dispaly_error;
		ui.prompt(params, "Error", error);
	};
	
	// constants used by the app to only allow one instance
	 const std::string guid_login =  "{F7AA7B63 - 6AAC - 4B7D - 84DC - 107F352217A1}";
	 const std::string guid_main  =  "{D1E8548A - 0749 - 4D2C - 86B8 - 26EB5DB5AF5F}";

	// appication state object
	state state_data;
	
	std::string error;
	// run login form
	ken_app_login login(guid_login, state_data);
	if (!login.run(guid_login, error)) {
		on_error(login, error);
	}

	if (state_data.loggedin()) {
		// run main form
		ken_app_main main_form(guid_main, state_data);

		if (!main_form.run(guid_main, error)) {
			on_error(main_form, error);
		}
	}
	
}

