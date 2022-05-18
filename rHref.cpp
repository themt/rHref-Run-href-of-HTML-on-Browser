/*
File     : rHref.cpp
Code-Name: tHemT rHref
Version  : 1.9.5s (11.06m.2006)
Desc     : Open your files on internet browser

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

//
//
// Includes
//

#include "rHref.h"
#include "../../Common/Include/file_operations.h"
#include <fstream>



//
//
// Global variables
//

char Settings_file_cant_found[] = "Setting file can't found so will load default settings";
char BSlash[]                   = "\\";
char BSlashBSlash[]             = "\\\\";
char WhiteSpace[]               = " ";
char Plus[]                     = "+";
char MemoryNotEnought[]         = "Memory not enought";
char ProtocolCantFound[]        = "Protocol can't found error";
char CranidRegRoot[]            = "SOFTWARE\\tHemT\\rHref";
long Version                    = 1080000;
char VersionExt[]               = "stable";
char EmptyString[]              = "";
char AppDir[]                   = "$appdir$";

// Registry of URL Protocol
char ProtocolRegRoot[]          = "SOFTWARE\\Classes";
char ProtocolRegCommandRoot[]   = "shell\\open\\command";
char ProcotolRegKeyProtocol[]   = "URL: $protocol$Protocol";
char ProcotolRegKeyProtocolT[]  = "URL Protocol";
char ProtocolRegKeyCommand[]    = "\"$appdir$\" \"%1\"";
char ProtocolRegKeyProtocolVer[]= "1.06";

// Registry of tHemT\rHref\...
char RegKeyVersion[]            = "Version";
char RegKeyVersionExt[]         = "Version Ext";
char RegKeyPath[]               = "Path";
char RegKeyFileNotFountMsg[]    = "File Not Fount Msg";
char RegKeyWarningNewVersion[]  = "Warning New Version";
char RegKeyDevelopment[]        = "Development";
char RegKeyProtocol[]           = "Protocol";
char RegKeyOpen[]               = "Open";

char True[]   = "true";
char False[]  = "false";


//
//
// Interface start
//

//
//
// Sector E
//

//
// Fake constructor :)
//

bool rHref::initialize() {
	try {
	this->_app_dir.buy_string(1024);
	this->_setting_file_name.assign("rHref.ini", 128);
	this->_run_file_path.buy_string(1024);
	this->_run_file_param.buy_string(1024);
	this->_command_line.buy_string(1024);
	this->_str0.buy_string(1024);
	this->_str1.buy_string(1024);
	this->_cstr = new char[1024];
	this->_exit = false;

	// Initializing settings
	strncpy(this->settings.open,               "index.htm", 31);
	strncpy(this->settings.development_msg,    "Development mode (Active):", 255);
	strncpy(this->settings.protocol_name,      "themt", 31);
	strncpy(this->settings.file_not_found_msg, "File can't found or opener program is not installed!", 255);
	strncpy(this->settings.app_name,           "rHref.exe", 31);
	this->settings.development = false;
	this->settings.warning_new_version = true;
	this->settings.version = 0;
	strncpy(this->settings.version_ext,        Plus, 31);

	if(!this->_app_dir || !this->_setting_file_name || \
		!this->_run_file_path || !this->_run_file_param || \
	   !this->_str0 || !this->_str1 || !this->_cstr) {
		this->_exit = true;
		throw (errors::memory_not_enought);
	}

	return true;

	}
	catch (errors err) {
		switch (err) {
		case errors::memory_not_enought:
			MessageBox(0, MemoryNotEnought, 0, MB_OK|MB_ICONERROR);
			break;
		}

		return false;
	}
}

//
// Show Time!
//

bool rHref::showTime(const char *commandLine) {
	//if(this->_exit) { return false; }

	this->_command_line.assign(commandLine);

	switch (this->parse_command_line()) {
	case 0:
		this->make_it();
		break;
	case 1:
		this->do_it();
		break;
	default:
		return false;
	}
	return true;
}

//
// Fake destructor
//

void rHref::uninitialize() {
	this->_str0.sell_string(); this->_str1.sell_string();
	this->_command_line.sell_string(); this->_app_dir.sell_string(); this->_setting_file_name.sell_string();
	this->_run_file_path.sell_string();
	this->_run_file_param.sell_string();
	if(this->_cstr) { delete [] this->_cstr; this->_cstr=0; }

	this->_exit = false;
}




//
//
// Sector F
//

//
//
// Load or Save settings from registry
//

bool rHref::regProcess(bool read) {
	regtoy current_user(regtoy::current_user);
	regtoy classes_root(regtoy::classes_root);
	regtoy local_machine(regtoy::local_machine);

	long version = 0;
	char *cstr   = 0, *cstr1=0;
	bool step    = false;
	char value   = 1;

	try {

	this->_str0.assign(CranidRegRoot);
	this->_str0.add('\\', false);
	this->_str0.add(this->settings.protocol_name, false);

	cstr = this->_str0.to_c_style();

	if(read) {
		// Check version
		step = current_user.read(CranidRegRoot,RegKeyVersion,&this->settings.version);
		if(!step) throw 0;
		current_user.read(CranidRegRoot,RegKeyVersionExt,this->settings.version_ext,31);

		step = current_user.read(cstr,RegKeyPath,this->_app_dir.to_c_style(),this->_app_dir.capacity());
		this->_app_dir.relenght();

		if(!step) throw 0;
		current_user.read(cstr,RegKeyFileNotFountMsg,this->settings.file_not_found_msg,31);
		current_user.read(cstr,RegKeyWarningNewVersion,&value);
		if(value) { this->settings.warning_new_version = true; }
		else { this->settings.warning_new_version = false; }
		current_user.read(cstr,RegKeyDevelopment,&value);
		if(value) { this->settings.development = true; }
		current_user.read(cstr,RegKeyOpen,this->settings.open,31);
	} else {
		// Check version
		current_user.read(CranidRegRoot,RegKeyVersion,&this->settings.version);
		current_user.read(CranidRegRoot,RegKeyVersionExt,this->settings.version_ext,31);
		if(this->settings.version>Version||strcmp(this->settings.version_ext,VersionExt)) {
			current_user.write(CranidRegRoot,RegKeyVersion,Version);
			current_user.write(CranidRegRoot,RegKeyVersionExt,VersionExt);
		}

		current_user.write(cstr,RegKeyPath,this->_app_dir.to_c_style(),this->_app_dir.lenght());
		current_user.write(cstr,RegKeyFileNotFountMsg,this->settings.file_not_found_msg);
		current_user.write(cstr,RegKeyWarningNewVersion,this->settings.warning_new_version);
		current_user.write(cstr,RegKeyDevelopment,this->settings.development);
		current_user.write(cstr,RegKeyOpen,this->settings.open);

		// HKEY_LOCAL_MACHINE and HKEY_CLASSES_ROOT\SOFTWARE\Classes\$protocol$
		this->_str0.assign(ProtocolRegRoot);
		this->_str0.add('\\', false);
		this->_str0.add(this->settings.protocol_name, false);

		this->_str1.assign(ProcotolRegKeyProtocol);
		this->_str1.replace("$protocol$",this->settings.protocol_name);

		local_machine.write(this->_str0.to_c_style(),0,this->_str1.to_c_style());
		local_machine.write(this->_str0.to_c_style(),RegKeyVersion,ProtocolRegKeyProtocolVer);
		local_machine.write(this->_str0.to_c_style(),ProcotolRegKeyProtocolT,EmptyString);
		classes_root.write(this->_str0.to_c_style(),0,this->_str1.to_c_style());
		classes_root.write(this->_str0.to_c_style(),RegKeyVersion,ProtocolRegKeyProtocolVer);
		classes_root.write(this->_str0.to_c_style(),ProcotolRegKeyProtocolT,EmptyString);

		this->_str0.assign(ProtocolRegRoot);
		this->_str0.add('\\', false);
		this->_str0.add(this->settings.protocol_name, false);
		this->_str0.add('\\',false);
		this->_str0.add(ProtocolRegCommandRoot,false);

		// HKEY_LOCAL_MACHINE and HKEY_CLASSES_ROOT\SOFTWARE\Classes\$protocol$\shell\open\command
		this->_str1.assign('\"');
		this->_str1.add(&this->_app_dir,false);
		this->_str1.add('\\',false);
		this->_str1.add(this->settings.app_name,false);
		this->_str1.add("\" \"%1\"",false);

		local_machine.write(this->_str0.to_c_style(),0,this->_str1.to_c_style(),this->_str1.lenght());
		classes_root.write(this->_str0.to_c_style(),0,this->_str1.to_c_style(),this->_str1.lenght());
	}

	return true;

	}

	catch(...) {
		return false;
	}
}

//
//
// Load settings from file
//

bool rHref::fileProcess() {
	if(this->_exit) { return false; }

	char *file_path = this->_setting_file_name.to_c_style();
	if(!file_path) { return false; }

	std::ifstream fin(file_path, std::ios::binary);
	if(!fin) {
		return false;
	}

	fin.seekg(0, std::ios::end);
	unsigned int len = (unsigned int)fin.tellg();
	fin.seekg(0, std::ios::beg);

	char *setbuf = new char[len+1];
	if(!setbuf) {
		fin.close();
		return false;
	}
	fin.read((char*)setbuf, len);
	fin.close();
	setbuf[len] = '\0';

	char *ppsetbuf = setbuf;
	while(*ppsetbuf) {
		if(*ppsetbuf == '/' && *(ppsetbuf+1) == '/') {
			while(*ppsetbuf && *ppsetbuf != '\n') { ppsetbuf++; }
		}
		else if(isalpha(*ppsetbuf)) {
			const short command_len = 31;
			char command[command_len+1] = {'\0'};
			short i;
			for(i=0; i < command_len && isalpha(*ppsetbuf); i++, ppsetbuf++) {
				command[i] = *ppsetbuf;
			}
			if(!strcmp(command, "Open")) {
				ppsetbuf = readString(ppsetbuf,this->settings.open,31);
			} else if(!strcmp(command, "DevelopmentMsg")) {
				ppsetbuf = readString(ppsetbuf,this->settings.development_msg,255);
			}
			else if(!strcmp(command, "FileNotFoundMsg")) {
				ppsetbuf = readString(ppsetbuf,this->settings.file_not_found_msg,255);
			}
			else if(!strcmp(command, "ProtocolName")) {
				ppsetbuf = readString(ppsetbuf,this->settings.protocol_name,31);
			}
			else if(!strcmp(command, "AppName")) {
				ppsetbuf = readString(ppsetbuf,this->settings.app_name,31);
			} else if(!strcmp(command, "Development")) {
				ppsetbuf = readString(ppsetbuf,this->_cstr,5);
				if(!strcmp(this->_cstr, True)) {
					this->settings.development = true;
				} else if(!strcmp(this->_cstr, False)) {
					this->settings.development = false;
				}
			} else if(!strcmp(command, "WarningNewVersion")) {
				ppsetbuf = readString(ppsetbuf,this->_cstr,5);
				if(!strcmp(this->_cstr, True)) {
					this->settings.warning_new_version = true;
				} else if(!strcmp(this->_cstr, False)) {
					this->settings.warning_new_version = false;
				}
			}
		} else {
			ppsetbuf++;
		}
	}

	delete [] setbuf;

	return true;
}

//
//
// Apply the rHref
//

void rHref::do_it() {
	try {

	if(this->_exit) throw (errors::no_error);

	if(!this->regProcess(true)) {
		throw errors::protocol_cant_found;
	}

	if(this->settings.version>Version&&this->settings.warning_new_version) {
		MessageBox(0, "tHemT rHref has a new version","Information",MB_OK|MB_ICONINFORMATION);
	}

	this->_run_file_param.replace(AppDir,this->_app_dir.to_c_style());

	this->_str0.assign(&this->_app_dir);
	this->_str0.add('\\', false);
	this->_str0.add(&this->_run_file_path, false);
		
	if(this->settings.development==true) {
		this->_str1.assign("File Path: ");
		this->_str1.add(&this->_run_file_path,false);
		this->_str1.add('\n',false);
		this->_str1.add("File Param: ",false);
		this->_str1.add(&this->_run_file_param,false);

		MessageBox(NULL,this->_str1.to_c_style(),this->settings.development_msg,MB_OK | MB_ICONINFORMATION);
	}

	if(run_path(0,0,this->_str0.to_c_style(),this->_run_file_param.to_c_style())) {
		MessageBox(NULL,this->settings.file_not_found_msg,0,MB_OK | MB_ICONINFORMATION);
	}

	}

	catch (errors err) {
		switch (err) {
		case errors::memory_not_enought:
			MessageBox(0,MemoryNotEnought,0,MB_OK|MB_ICONERROR);
			break;
		case errors::protocol_cant_found:
			MessageBox(0,ProtocolCantFound,0,MB_OK|MB_ICONERROR);
			break;
		}

		return;
	}
}

//
//
// Init the rHref
//

void rHref::make_it() {
	// Get this application path
	getCurrentDirectory(this->_app_dir.to_c_style(), this->_app_dir.capacity());
	this->_app_dir.relenght();

	this->fileProcess();
	this->regProcess(false);

	run(0,0,this->_app_dir.to_c_style(),this->settings.open,0);
}

//
//
// Others
//

char *rHref::readString(const char *from, char *to, short to_len) {
	char *ppfrom = (char*)from;
	char *ppto   = to;
	short i      = 0;

	// skip ws
	while(*ppfrom && (*ppfrom==' ' || *ppfrom=='\t')) { ppfrom++; }

	if(*ppfrom=='\"') {
		ppfrom++; //skip "
		while(i<to_len && *ppfrom && (*ppfrom!='\"' || *(ppfrom-1)=='\\')) {
			ppto[i++] = *ppfrom++;
		}
		ppfrom++; //skip "
	} else {
		while(i<to_len && *ppfrom && *ppfrom!=13 && *ppfrom!=10 && *ppfrom!='\t' && *ppfrom!=' ') {
			ppto[i++] = *ppfrom++;
		}
	}

	ppto[i] = '\0';

	return ppfrom;
}
short rHref::parse_command_line() {
	char *pf;
	unsigned int f = 0, f_b = 0, p = 0, l = 0;
	short ret_val = -1;

	pf = this->_command_line.to_c_style();

	if(!pf) { return ret_val; }

	while(*pf) {
		pf = this->readString(pf,this->_cstr,1023);
		this->_str1.assign(this->_cstr);

		if(!this->_str1.is_equal('-', 1)) {
			
				this->_run_file_path = this->_str1;
				f = this->_run_file_path.search("://");
				if(f) {
					f_b = f+2;
					if(f-1<32) {
						strncpy(this->settings.protocol_name,this->_run_file_path.to_c_style(),f-1);
					}
					this->_run_file_path.clear(1, f_b);
					this->_run_file_path.replace("%2B",Plus);
					this->_run_file_path.replace("%20",WhiteSpace);
					f = this->_run_file_path.search("%22");
					if(f) {
						this->_run_file_path.clear(f,f_b);
					}
					if( this->_run_file_path.is_equal('/', this->_run_file_path.lenght()) ) {
						this->_run_file_path.clear(this->_run_file_path.lenght(), this->_run_file_path.lenght());
					}

					this->_run_file_path.replace("/", BSlash);
					p = this->_run_file_path.search("?",f_b);
					if(p) {
						this->_run_file_param.assign(&this->_run_file_path);
						this->_run_file_param.clear(1,p);
						this->_run_file_path.clear(p,this->_run_file_path.lenght());
					}
				}

			ret_val = 1;
		}
		else if(this->_str1=="-settings") {
			pf = this->readString(pf,this->_cstr,128);
			this->_setting_file_name.assign(this->_cstr);
			ret_val = 0;
		}
		else if(this->_str1=="-dev") {
			this->settings.development = true;
			ret_val = 1;
		}
	}

	return ret_val;
}

//
// Interface ended
//
