/*
File     : regtoy.h
Code-Name: RegToy
Version  : 1.0s (support tHemT string class) 19.03m.2006
Desc     : Modifiy Windows(R) registry

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

// Note: in this code(s) used some code(s) from Jorgen Bosman's PowerOff project

#ifndef head_of_regtoy_class
#define head_of_regtoy_class



//
//
// Includes
//

#include <windows.h>
#include "string.h"



//
//
// Interface start
//

class regtoy {

public:

	// Root key handles
	enum root_keys {
		classes_root  = 0,
		current_user,
		local_machine,
		users
	};



public:

	// Constructors
	inline regtoy() : _root_key(root_keys::current_user) {}
	inline regtoy(root_keys root_key) : _root_key(root_key) {}



	// Initialize
	inline void      set_root_key(root_keys root_key) { this->_root_key = root_key; }
	inline root_keys get_root_key() { return this->_root_key; }



	// Converts...
	HKEY root_keys_to_hkey();



	// Registry writting operations...
	// tHemT string class support
	bool write(const string *key_tree, const string *key, string *value);
	bool write(const string *key_tree, const string *key, long value);
	bool write(const string *key_tree, const string *key, bool value);

	// c-style string support
	bool write(const char *key_tree, const char *key, const char *value, unsigned long val_len = 0);
	bool write(const char *key_tree, const char *key, long value);
	bool write(const char *key_tree, const char *key, bool value);



	// Registry reading operations...
	// tHemT string class support
	bool read(const string *key_tree, const string *key, string *value);
	bool read(const string *key_tree, const string *key, long *value);
	bool read(const string *key_tree, const string *key, bool *value);

	// c-style string support
	bool read(char *key_tree, char *key, char *value, unsigned long val_len = 0);
	bool read(char *key_tree, char *key, long *value);
	bool read(char *key_tree, char *key, bool *value);

	// Registry deleting operations
	// tHemT string class support
	bool del(const string *key_tree, const string *key);
	bool del(const string *key_tree);

	// c-style string support
	bool del(const char *key_tree, const char *key);
	bool del(const char *key_tree);



protected:

	root_keys _root_key;
};

//
//
// Interface ended
//



#endif // head_of_regtoy_class
