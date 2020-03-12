// To prevent multiple inclusions of the types defined in a header file
#pragma once 

//STL libraries
#include <iostream>
#include "resource.h"


// including the liblec library
#include <liblec/lecui/gui.h>

using namespace liblec::lecui_discard;

class state{

public :
	int loggedin;
	// lambda function for showing the error
	auto on_error(gui& ui, const std::string& error) {
		gui::prompt_params params;
		params.type = gui::prompt_type::ok;
		params.png_icon_resource = dispaly_error;
		ui.prompt(params, "Error", error);
	};


	


};