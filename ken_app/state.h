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
	ken_app_db db_;
	ken_app_db::user_credentials credentials;


public :

	// function for log on
	bool loggedin() {return logged_in;}
	void log() {logged_in = true;}
	ken_app_db& get_db() {return db_;}
	ken_app_db::user_credentials& get_user_() { return credentials; }

};