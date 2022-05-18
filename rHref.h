/*
File     : rHref.h
Code-Name: tHemT rHref
Version  : 1.9.5s (11.06m.2006)
Desc     : Open your files on internet browser

Copyright (C) 2005-2006 MuRat 'tHemT' TaMcI (theismt@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef head_of_rHref_class
#define head_of_rHref_class

//
//
// Includes
//

#include "string.h"
#include "regtoy.h"



//
//
// Interface start
//

class rHref {

public:

	class Settings {
	public:
		char open[32];
		char protocol_name[32];
		char development_msg[256];
		char file_not_found_msg[256];
		char app_name[32];
		bool development;
		bool warning_new_version;
		long version;
		char version_ext[32];
	};

	enum errors {
		no_error = 0,
		memory_not_enought,
		protocol_cant_found
	};



public:

	// Sector E

	// Destructor
	~rHref() { this->uninitialize(); }

	// Fake constructor :)
	bool initialize();

	// Show Time!
	bool showTime(const char *commandLine);

	// Fake destructor
	void uninitialize();



protected:

	// Sector F

	// Load settings from registry
	bool regProcess(bool read = true);
	// Load settings file
	bool fileProcess();

	// Init the rHref
	void do_it();
	// Apply the rHref
	void make_it();

	// Others
	char *readString(const char *from, char *to, short to_len);
	short parse_command_line();



protected:

	bool   _exit;
	string _str0, _str1;

	int _todos_capacity,_todos_i;
	string *_todos,*_todos_params;

	string _command_line, _app_dir, _setting_file_name, _run_file_path, _run_file_param;
	char  *_cstr;

	Settings settings;
};

//
//
// Interface ended
//

#endif // head_of_rHref_class
