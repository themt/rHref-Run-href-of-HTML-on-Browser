/*
File     : regtoy.cpp
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

#include "regtoy.h"



//
// Interface start
//

//
// Converts...
//

HKEY regtoy::root_keys_to_hkey() {
	switch (this->_root_key) {
	case root_keys::classes_root:
		return HKEY_CLASSES_ROOT;
	case root_keys::current_user:
		return HKEY_CURRENT_USER;
	case root_keys::local_machine:
		return HKEY_LOCAL_MACHINE;
	case root_keys::users:
		return HKEY_USERS;
	}

	return HKEY_CURRENT_USER;
}
	

//
// Registry writting operations...
// string class support
//

bool regtoy::write(const string *key_tree, const string *key, string *value) {
	char *cstr_key_tree, *cstr_key;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();
		cstr_key      = const_cast<string*>(key)->to_c_style();

		if(!value->capacity()) value->buy_string(1024);

		if(!cstr_key_tree || !cstr_key || !value->capacity()) throw 0;
		
		if(!this->read(cstr_key_tree, cstr_key, &(*value)[0], value->capacity())) { throw 0; }

		return true;
	}

	catch(...) {
		value->sell_string();

		return false;
	}
}
bool regtoy::write(const string *key_tree, const string *key, long value) {
	char *cstr_key_tree, *cstr_key;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();
		cstr_key      = const_cast<string*>(key)->to_c_style();

		if(!cstr_key_tree || !cstr_key || !value) throw 0;
		
		if(!this->write(cstr_key_tree, cstr_key, value)) { throw 0; }

		return true;
	}

	catch(...) {
		return false;
	}
}
bool regtoy::write(const string *key_tree, const string *key, bool value) {
	char *cstr_key_tree, *cstr_key;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();
		cstr_key      = const_cast<string*>(key)->to_c_style();

		if(!cstr_key_tree || !cstr_key || !value) throw 0;
		
		if(!this->write(cstr_key_tree, cstr_key, value)) { throw 0; }

		return true;
	}

	catch(...) {
		return false;
	}
}

//
// c-style string support
//

bool regtoy::write(const char *key_tree, const char *key, const char *value, unsigned long value_len) {
	HKEY ffroot_key;
	DWORD dp, ret_val;

	if(!value_len) { value_len=(unsigned long)strlen(value)+1; }
	else { value_len+=1; }

	if(RegCreateKeyEx(this->root_keys_to_hkey(),key_tree,0,"REG_SZ",REG_OPTION_NON_VOLATILE,\
	  KEY_WRITE,NULL,&ffroot_key,&dp) != ERROR_SUCCESS) {
		return false;
	}
  
	if((ret_val=RegSetValueEx(ffroot_key,key,0,REG_SZ,(BYTE*)value,(DWORD)value_len)) != ERROR_SUCCESS) {
		RegCloseKey(ffroot_key);
		SetLastError(ret_val);
		return false;
	}

	RegCloseKey(ffroot_key);

	return true;
}
bool regtoy::write(const char *key_tree, const char *key, long value) {
	HKEY  ffroot_key;
	DWORD dp, ret_val, value_size;

	value_size=(DWORD)sizeof(long);

	if(RegCreateKeyEx(this->root_keys_to_hkey(),key_tree,0,"REG_DWORD",REG_OPTION_NON_VOLATILE, \
		KEY_WRITE,NULL,&ffroot_key,&dp) != ERROR_SUCCESS) {
		return false;
	}

	if ((ret_val=RegSetValueEx(ffroot_key,key,0,REG_DWORD,(BYTE*)&value,value_size)) != ERROR_SUCCESS){
		RegCloseKey(ffroot_key);
		SetLastError(ret_val);
		return false;
	}

	RegCloseKey(ffroot_key);

	return true;
}
bool regtoy::write(const char *key_tree, const char *key, bool value) {
	HKEY  ffroot_key;
	DWORD dp, ret_val, value_size;
	char  ul_value;

	value_size=(DWORD)sizeof(char);

	if(value) { ul_value=1; }
	else { ul_value=0; }

	if(RegCreateKeyEx(this->root_keys_to_hkey(),key_tree,0,"REG_BINARY",REG_OPTION_NON_VOLATILE, \
		KEY_WRITE,NULL,&ffroot_key,&dp) != ERROR_SUCCESS) {
		return false;
	}

	if ((ret_val=RegSetValueEx(ffroot_key,key,0,REG_BINARY,(BYTE*)&value,value_size)) != ERROR_SUCCESS){
		RegCloseKey(ffroot_key);
		SetLastError(ret_val);
		return false;
	}

	RegCloseKey(ffroot_key);

	return true;
}

//
// string class support
//

bool regtoy::read(const string *key_tree, const string *key, string *value) {
	char *cstr_key_tree, *cstr_key;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();
		cstr_key      = const_cast<string*>(key)->to_c_style();

		if(!value->capacity()) value->buy_string(1024);

		if(!cstr_key_tree || !cstr_key || !value->capacity()) throw 0;
		
		if(!this->read(cstr_key_tree, cstr_key, &(*value)[0], value->capacity())) { throw 0; }

		return true;
	}

	catch(...) {
		value->sell_string();

		return false;
	}
}
bool regtoy::read(const string *key_tree, const string *key, long *value) {
	char *cstr_key_tree, *cstr_key;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();
		cstr_key      = const_cast<string*>(key)->to_c_style();

		if(!cstr_key_tree || !cstr_key || !value) throw 0;
		
		if(!this->read(cstr_key_tree, cstr_key, value)) { throw 0; }

		return true;
	}

	catch(...) {
		return false;
	}
}
bool regtoy::read(const string *key_tree, const string *key, bool *value) {
	char *cstr_key_tree, *cstr_key;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();
		cstr_key      = const_cast<string*>(key)->to_c_style();

		if(!cstr_key_tree || !cstr_key || !value) throw 0;
		
		if(!this->read(cstr_key_tree, cstr_key, value)) { throw 0; }

		return true;
	}

	catch(...) {
		return false;
	}
}

//
// c-style string support
//

bool regtoy::read(char *key_tree, char *key, char *value, unsigned long value_len) {
	HKEY  ffroot_key;
	DWORD val_type, ret_val;

	if(!value_len) { value_len=(unsigned long)strlen(value); }

	if(RegOpenKeyEx(this->root_keys_to_hkey(),key_tree,0,KEY_QUERY_VALUE,&ffroot_key) != ERROR_SUCCESS) {
		return FALSE;
	}

	if ((ret_val=RegQueryValueEx(ffroot_key,key,0,&val_type,(LPBYTE)value,(LPDWORD)&value_len)) != ERROR_SUCCESS) {
		RegCloseKey(ffroot_key);
		SetLastError(ret_val);
		return FALSE;
	}

	RegCloseKey(ffroot_key);

	return true;
}

bool regtoy::read(char *key_tree, char *key, long *value) {
	HKEY  ffroot_key;
	DWORD val_type, ret_val, val_size=(DWORD)sizeof(long);

	if(RegOpenKeyEx(this->root_keys_to_hkey(),key_tree,0,KEY_QUERY_VALUE,&ffroot_key) != ERROR_SUCCESS) {
		return false;
	}
	if ((ret_val=RegQueryValueEx(ffroot_key,key,0,&val_type,(LPBYTE)value,&val_size)) != ERROR_SUCCESS) {
		RegCloseKey(ffroot_key);
		SetLastError(ret_val);
		return false;
	}

	RegCloseKey(ffroot_key);

	return true;
}

bool regtoy::read(char *key_tree,char *key, bool *value) {
	HKEY  ffroot_key;
	DWORD ret_val, val_type, tmpvalue, tmpvalue_size=sizeof(char);

	if(RegOpenKeyEx(this->root_keys_to_hkey(),key_tree,0,KEY_QUERY_VALUE,&ffroot_key) != ERROR_SUCCESS) {
		return false;
	}

	if((ret_val=RegQueryValueEx(ffroot_key,key,0,&val_type,(LPBYTE)&tmpvalue,&tmpvalue_size)) != ERROR_SUCCESS) {
		RegCloseKey(ffroot_key);
		SetLastError(ret_val);
		return false;
	}

	RegCloseKey(ffroot_key);

	if(tmpvalue) { *value = true; }
	else { *value = false; }

	return true;
}

//
// Registry deleting operations
//
// tHemT string class support
//

bool regtoy::del(const string *key_tree, const string *key) {
	char *cstr_key_tree, *cstr_key;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();
		cstr_key      = const_cast<string*>(key)->to_c_style();

		if(!cstr_key_tree || !cstr_key) throw 0;
		
		if(!this->del(cstr_key_tree, cstr_key)) { throw 0; }

		return true;
	}

	catch(...) {
		return false;
	}
}
bool regtoy::del(const string *key_tree) {
	char *cstr_key_tree;

	try {
		cstr_key_tree = const_cast<string*>(key_tree)->to_c_style();

		if(!cstr_key_tree) throw 0;
		
		if(!this->del(cstr_key_tree)) { throw 0; }

		return true;
	}

	catch(...) {
		return false;
	}
}

//
// c-style string support
//

bool regtoy::del(const char *key_tree, const char *key) {
	HKEY  ffroot_key;
	unsigned long ret_val;

	if(RegOpenKeyEx(this->root_keys_to_hkey(),key_tree,0,KEY_SET_VALUE,&ffroot_key) != ERROR_SUCCESS) {
		return false;
	}
	if((ret_val=RegDeleteValue(ffroot_key,key)) != ERROR_SUCCESS) {
		RegCloseKey(ffroot_key);
		SetLastError(ret_val);
		return false;
	}

	RegCloseKey(ffroot_key);

	return true;
}

bool regtoy::del(const char *key_tree) {
	unsigned long ret_val;

	if((ret_val=RegDeleteKey(this->root_keys_to_hkey(),key_tree)) != ERROR_SUCCESS) {
		SetLastError(ret_val);
		return false;
	}

	return true;
}

//
// Iterface ended
//
