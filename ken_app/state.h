// To prevent multiple inclusions of the types defined in a header file
#pragma once 

//STL libraries
#include <iostream>
#include "resource.h"
#include "ken_app_db.h"

// including the liblec library
#include <liblec/cui/gui.h>

using namespace liblec::cui;

class state{

	bool logged_in = false;
	bool create_user = false;


public :

	// function for log on
	bool loggedin() {
		return logged_in;
	}
	
	void log() {
		logged_in = true;
	}

	// function for creating new user
	bool new_user() {
		return create_user;
	}

	void new_user_() {
		create_user = true;
	}


	ken_app_db db;
	ken_app_db& get_db() {
		return db;
	}

};