/*
File     : file_operations.h (Only)
Code-Name: File & O.S. Environment Operations
Version  : 1.0s (support tHemT string class) 19.03m.2006
Desc     : Fýle and O.S. Environment functions

Copyright (C) 2005-2006 MuRat 'tHemT' TaMcI (m.tamci@gmail.com)

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

#ifndef head_of_file_operation_fuctions
#define head_of_file_operation_fuctions

#include <windows.h>



//
// Get environment variables
//

enum GET_ENVS {
	GETENV_SYSTEMDRIVE = 1,
	GETENV_USERPATH,
	GETENV_USERPATH_FULL,
	GETENV_OS_NAME,
	GETENV_PC_NAME,
	GETENV_OS_PATH
};

void getEnv(GET_ENVS env_type, char *strbuf, short strlenght) {
	const short env_name_len = 127;
	char env_name[env_name_len+1];
	switch (env_type) {
		case GETENV_SYSTEMDRIVE:
			strncpy(env_name,"systemdrive", env_name_len);
			break;
		case GETENV_USERPATH:
			strncpy(env_name,"homepath", env_name_len);
			break;
		case GETENV_USERPATH_FULL:
			strncpy(env_name,"userprofile", env_name_len);
			break;
		case GETENV_OS_NAME:
			strncpy(env_name,"os", env_name_len);
			break;
		case GETENV_PC_NAME:
			strncpy(env_name,"logonserver", env_name_len);
			break;
		case GETENV_OS_PATH:
			strncpy(env_name,"SystemRoot", env_name_len);
			break;
	}
	GetEnvironmentVariable(env_name, strbuf, strlenght);
}
void getOSDir(char *strbuf, short lenght) {
	GetWindowsDirectory(strbuf, lenght);
}
void getOSSystemDir(char *strbuf, short lenght) {
	GetSystemDirectory(strbuf, lenght);
}

//
// Get application's directory
//

void getCurrentDirectory(char *strbuf, short lenght) {
	GetCurrentDirectory(lenght, strbuf);
}

//
// Run a thing in application's directory
//

void runInCurrentDirectory(const char *filename, const char *parameters, int visible_type = SW_NORMAL) {
	const short path_lenght = 512;
	char path[path_lenght] = {'\0'};
	GetCurrentDirectory(path_lenght-1, path);
	ShellExecute(0, 0, filename, parameters, path, visible_type);
}

//
// Run anything
//

bool run(HWND hwnd, const char *operation, const char *directory, const char *filename, const char *parameters, int visible_type = SW_NORMAL) {
	return (ShellExecute(hwnd, operation, filename, parameters, directory, visible_type)==(HINSTANCE)0x2A ? false : true);
}

//
// Run anything
//

bool run_path(HWND hwnd, const char *operation, const char *path, const char *parameters, int visible_type = SW_NORMAL) {
	return (ShellExecute(hwnd, operation, path, parameters, 0, visible_type)==(HINSTANCE)0x2A ? false : true);
}



#endif // head_of_file_operation_fuctions

