
#include "ken_app_login.h"
#include "ken_app_main.h"

// gui app using main
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {

	
	// constants used by the app to only allow one instance
	 const std::string guid_login =  "{F7AA7B63 - 6AAC - 4B7D - 84DC - 107F352217A1}";
	 const std::string guid_main  =  "{D1E8548A - 0749 - 4D2C - 86B8 - 26EB5DB5AF5F}";

	// appication state object
	state state_data;
	

	// run login form
	ken_app_login login(guid_login, state_data);
	std::string error;
	if (!login.run(guid_login, error)) {
		state_data.on_error(login, error);
	}

	// run main form
	ken_app_main main_form(guid_main, state_data);
	if (state_data.loggedin == 1) {
		if (!main_form.run(guid_main, error)) {
			state_data.on_error(main_form, error);
		}
	}
	
}

