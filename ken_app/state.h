// To prevent multiple inclusions of the types defined in a header file
#pragma once 

//STL libraries
#include <iostream>
#include "resource.h"


// including the liblec library
#include <liblec/lecui/gui.h>

using namespace liblec::lecui_discard;

class state{

	bool logged_in = false;


public :
	
	// function for log on
	bool loggedin() {
		return logged_in;
	}
	
	void log() {
		logged_in = true;
	}


	


};