/*
File     : string.cpp
Code-Name: tHemT string
Version  : 3.8s (convertable to c style) 14.05m.2006
Desc     : string-type

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

#ifdef __cplusplus


#include "string.h"



//
// Interface start
//

// Local globals

static const unsigned int temp_cstr_len = 25;
static char temp_char                   = '\0';
static char temp_cstr[temp_cstr_len]    = {'\0'};
static char bool_type_true[]            = "true";
static char bool_type_false[]           = "false";
static unsigned int bool_type_true_len  = (unsigned int)strlen(bool_type_true);
static unsigned int bool_type_false_len = (unsigned int)strlen(bool_type_false);
string::states string::_state           = string::states::none;
char *string::_spcl_chrs                = 0;



//
// Assing
//

void string::buy_string(unsigned int capacity) {
	// Erase old string
	this->sell_string();

	// Get new string
	this->_address = new char[capacity];

	if(this->_address) {
		this->_address[0] = '\0';
		this->_capacity   = capacity;
		this->_lenght     = 0;
	}
}
void string::assign(char chr, unsigned int capacity) {
	// if left capacity is not enought for str or it is uncapacity
	if(capacity>this->capacity()) {
		// Get alloc
		this->buy_string( capacity );
	}

	// copy/fill char to got alloc
	this->copy(chr, capacity);
}
void string::assign(const char *str, unsigned int capacity) {
	// if left capacity is not enought for str or it is uncapacity
	if(capacity>this->capacity()) {
		// Get alloc
		this->buy_string( capacity );
	}

	// copy str to got alloc
	this->copy(str, capacity);
}
void string::assign(const string *str, unsigned int capacity) {
	// if left capacity is not enought for str or it is uncapacity
	if(capacity>this->capacity()) {
		// Get alloc
		this->buy_string( capacity );
	}

	// copy str to got alloc
	this->copy(str, capacity);
}

//
// Adding
//

void string::add(char chr, bool resize) {
	// if not enought...
	if(resize && !this->left_capacity()) {
		string newstr(this);

		// If temp string is ready
		if(newstr.capacity()) {
			// erase this class and later get new size to this class
			this->sell_string();
			this->buy_string(newstr.lenght()+1);
			// copy from temp class to this class (new size)
			this->copy(&newstr, newstr.lenght());
			// Delete temp string
			newstr.sell_string();
		}
	}

	// If enought for an character
	if(this->left_capacity()) {
		this->_address[this->_lenght++] = chr;
		this->_address[this->_lenght]   = '\0';
	}
}
void string::add(const char *str, bool resize) {
	// Control
	if(!str) { return; }

	unsigned int len = (unsigned int)strlen(str);

	// if left capacity is not enought for copying
	if(resize && (len>this->left_capacity())) {
		// this class copying to temp class
		string newstr(this);

		// If temp string is ready
		if(newstr.capacity()) {
			// erase this class and later get new size to this class
			this->sell_string();
			this->buy_string(this->lenght()+len+1);
			// copy from temp class to this class (new size)
			this->copy(&newstr, newstr.lenght());
			// Delete temp string
			newstr.sell_string();
		}
	}

	// If enought for an character
	if(len<=this->left_capacity()) {
		// and... copy/adding show starting
		for(unsigned int j=0; j<len; j++) {
			this->_address[this->_lenght++] = str[j];
		}
		this->_address[this->_lenght] = '\0';
	}
}
void string::add(const string *str, bool resize) {
	string *ffstring = const_cast<string*>(str);
	// Constrol
	if(!ffstring) { return; }

	// if left capacity is not enought for copying
	if(resize && (ffstring->lenght()>this->left_capacity())) {
		// this class copying to temp class
		string newstr(this);

		// If temp string is ready
		if(newstr.capacity()) {
			// erase this class and later get new size to this class
			this->sell_string();
			this->buy_string(this->lenght()+ffstring->lenght()+1);
			// copy from temp class to this class (new size)
			this->copy(&newstr, newstr.lenght());
			// Delete temp string
			newstr.sell_string();
		}
	}

	// If enought for an character
	if(ffstring->lenght()<=this->left_capacity()) {
		// and... copy/adding show starting
		for(unsigned int j=0; j<ffstring->lenght(); j++) {
			this->_address[this->_lenght++] = ffstring->_address[j];
		}
		this->_address[this->_lenght] = '\0';
	}
}

//
// Search, Search & Replace
//

unsigned int string::search(const char *search_str, unsigned int contiue) {
	// Control
	if(!search_str || !(contiue>=1&&contiue<=this->lenght()) ) return 0;

	// Initiliazing
	unsigned int search_str_len = (unsigned int)strlen(search_str);
	unsigned int found_point    =0, i=contiue-1, j=0;
	        bool found = false;

	// loop inside the string
	while(i<this->lenght()) {
		// if found first character of search string
		if(this->_address[i]==search_str[0]) {
			found_point = ++i;
			for(j=1; j<search_str_len && this->_address[i]==search_str[j]; j++, i++) {}
			// if match is true or it is not true
			if(j == search_str_len) {
				found = true;
				//contiue += search_str_len;
				break;
			} else { i = found_point; /*contiue++;*/ }
		} else { i++; /*contiue++;*/ }
	}

	// if not found then return fail
	return (found ? found_point : 0);
}
unsigned int string::replace(const char *search_str, const char *replace_str, unsigned int replace_num) {
	// Control
	if(!replace_num || !search_str || !replace_str) return 0;

	// 'found_pos_lst' will hold all found positions
	unsigned int  found_num = 0, found_pos = 0, search_str_len = (unsigned int)strlen(search_str);
	unsigned int  found_pos_lst_len = 32, *found_pos_lst = new unsigned int[found_pos_lst_len];
	// if 'found position list' can't builded then return with 0
	if(!found_pos_lst) return 0;

	// start from begin to search
	unsigned int cursor = 1;

	// ALL SEARCH LINES
	// loop for search
	do {
		// search and continue
		found_pos = this->search(search_str, cursor);
		// if found it
		if(found_pos) {
			cursor = found_pos+search_str_len;
			found_num++;
			// if 'found positions list' is smaller than found num then resize 'found positions list' [:1]
			// and carry/copy to new 'found positions list'
			if(found_num > found_pos_lst_len) {
				// [:1]
				unsigned int *new_found_pos_lst = new unsigned int[found_pos_lst_len+32];
				if(!new_found_pos_lst) {
					delete [] found_pos_lst;
					found_num = 0;
					break;
				} else {
					memcpy(new_found_pos_lst, found_pos_lst, sizeof(unsigned int)*found_pos_lst_len);
					delete [] found_pos_lst;
					found_pos_lst = new_found_pos_lst;
					found_pos_lst_len += 32;
				}
			}
			// found new position add to 'found positions list'
			found_pos_lst[found_num-1] = found_pos;
			// if programmer want searching by limit then break
			if(found_num == replace_num) {
				break;
			}
		} else {
			// if can't found then break
			break;
		}
	} while(true);

	// ALL REPLACE LINES
	// if found
	if(found_num) {
		// Calculate lengths
		unsigned int search_str_len  = (unsigned int)strlen(search_str);
		unsigned int replace_str_len = (unsigned int)strlen(replace_str);

		// Calculate capacities
		long     int extr_capacity = found_num * (replace_str_len - search_str_len);
		unsigned int new_capacity  = this->lenght() + extr_capacity;

		// 'from_i' is old string, 'to_i' is new string
		unsigned int from_i=0, found_pos_lst_i=0, to_i=0, replace_i=0;

		// if capacitiy is leak then build new capacity
		if(extr_capacity > (long)this->left_capacity()) {
			char *new_address = new char[new_capacity+1];	// +1 for NULL
			if(!new_address) {
				return 0;
			}
			while(from_i<this->lenght()) {
				if((from_i+1)==found_pos_lst[found_pos_lst_i]) {
					found_pos_lst_i++;
					from_i += search_str_len;
					for(replace_i=0; replace_i<replace_str_len; replace_i++, to_i++) {
						new_address[to_i] = replace_str[replace_i];
					}
				} else {
					new_address[to_i++] = this->_address[from_i++];
				}
			}

			this->sell_string();
			this->_address = new_address;
			this->_capacity = new_capacity+1;					// +1 for NULL
		}
		// else capacitiy isn't leak then...
		else {
			// if extra capacity is smaller then this capacity
			// example: search this 'abcd', replace to this 'abc'
			if(extr_capacity < 0) {
				while(from_i<this->lenght()) {
					if((from_i+1)==found_pos_lst[found_pos_lst_i]) {
						found_pos_lst_i++;
						from_i += search_str_len;
						for(replace_i=0; replace_i<replace_str_len; replace_i++, to_i++) {
							this->_address[to_i] = replace_str[replace_i];
						}
					} else {
						this->_address[to_i++] = this->_address[from_i++];
					}
				}
			}
			// if extra capacity is bigger then this capacity
			// example: search this 'abc', replace to this 'abcd'
			else if(extr_capacity > 0) {
				found_pos_lst_i = found_num-1;
				from_i          = this->lenght()-1;
				to_i            = from_i+extr_capacity;

				while(from_i!=-1) {
					if((found_pos_lst[found_pos_lst_i]+search_str_len)==(from_i+2)) {
						found_pos_lst_i--;
						from_i -= search_str_len;
						for(replace_i=replace_str_len-1; replace_i!=-1; replace_i--, to_i--) {
							this->_address[to_i] = replace_str[replace_i];
						}
					} else {
						this->_address[to_i--] = this->_address[from_i--];
					}
				}
			}
			// if extra capacity is match to this capacity
			// example: search this 'abcd', replace to this 'abcd'
			else {
				for(; found_pos_lst_i<found_num; found_pos_lst_i++) {
					to_i=(found_pos_lst[found_pos_lst_i]-1);
					for(replace_i=0; replace_i<replace_str_len; replace_i++, to_i++) {
						this->_address[to_i] = replace_str[replace_i];
					}
				}
			}
		}

		// delete 'found positions list'
		delete [] found_pos_lst;

		// Recalculate lenght
		this->_address[this->_lenght = new_capacity] = '\0';
	}

	return found_num;
}

//
// Copying
//

void string::copy(char chr, unsigned int num) {
	// Control
	if(!(*this)) {
		return;
	}

	unsigned int i=0;

	// Show time
	for(; i<this->capacity() && i<num; i++) {
		this->_address[i] = chr;
	}

	// get new lenght after copying
	this->_address[this->_lenght = i] = '\0';
}
void string::copy(const char *str, unsigned int num) {
	unsigned int i=0;

	// Control
	if(!str || !(*this)) {
		return;
	}

	unsigned int len = (unsigned int)strlen(str);

	// Show time
	for(i=0; i<this->capacity() && i<num && i<len; i++) {
		this->_address[i] = str[i];
	}

	// get new lenght after copying
	this->_address[this->_lenght = i] = '\0';
}
void string::copy(const string *str, unsigned int num) {
	unsigned int i=0;
	string *ffstring = const_cast<string*>(str);

	// Control
	if(!ffstring || !(*this)) {
		return;
	}

	// Show time
	for(i=0; i<this->capacity() && i<num && i<ffstring->lenght(); i++) {
		this->_address[i] = ffstring->_address[i];
	}

	// get new lenght after copying
	this->_address[this->_lenght = i] = '\0';
}

//
// Erasing & Clearing
//

void string::sell_string() {
	if((*this)) delete [] this->_address;

	// Reset members
	this->_address  = 0;
	this->_capacity = this->_lenght = 0;
}
void string::sell_string(unsigned int from, unsigned int to) {
	this->clear(from, to);
	this->decrease_capacity();
}

void string::clear(unsigned int from, unsigned int to) {
	// Control
	// if deleting from 0 to lenght then del everything
	if(from==1 && to==this->lenght()) {
		this->clear();
	}
	// if everything is ok then so show time
	else if(from>=1 && to<=this->lenght()) {
		// Calculate carry start point and its carry lenght
		// 'from-1' is for delete it begin from 'from-1'
		unsigned int i=0, start = from-1, end = to;
		unsigned int del_len    = end-start, crr_len = this->lenght()-end;
		// Calculate again lenght
		this->_lenght -= del_len;

		// Carry
		for( ; i<crr_len; i++) {
			this->_address[start+i] = this->_address[end+i];
		}

		this->_address[this->_lenght] = '\0';
	}
}

//
// Others
//

unsigned int string::word_count() {
	unsigned int found_word=0;
	if(this->lenght()) {
		for(unsigned int i=1; i<=this->lenght(); i++) {
			// if space then skip ws
			if(this->is_space(i,i)) {
				i++;
				while(i<this->lenght()&&(this->is_space(i,i))) { i++; }
			}
			// else not space then skip it and increase word count
			else {
				i++;
				found_word++;
				while(i<this->lenght()&&(!this->is_space(i,i))) { i++; }
			}
		}
	}

	return found_word;
}

// Trims/Crops
void string::trim_left() {
	if(this->lenght()) {
		unsigned int i=2;
		// First space
		bool space_found=this->is_space(1,1);
		while(this->is_space(i,i)) { i++; }

		// if first space is founded
		if(space_found) this->clear(1,i-1);
	}
}
void string::trim_right() {
	if(this->lenght()) {
		unsigned int i=this->lenght()-1;
		// Last space
		bool space_found=this->is_space(this->lenght(),this->lenght());
		while(this->is_space(i,i)) { i--; }

		// if last space is founded
		if(space_found) this->clear(i+1,this->lenght());
	}
}
void string::trim_lefrig() {
	this->trim_left();
	this->trim_right();
}
void string::decrease_capacity()  {
	// Temp string
	string ffstring(this);
	// If temp string is ready
	if(ffstring) {
		this->sell_string();
		this->assign(&ffstring);
		// Delete temp string
		ffstring.sell_string();
	}
}

//
// Converts...
//

// to string from ...
void string::importing(int number) {
	itoa(number,temp_cstr,10);
	this->assign(temp_cstr);
}
void string::importing(float number) {
	gcvt((double)number,temp_cstr_len,temp_cstr);
	this->assign(temp_cstr);
}
void string::importing(bool value) {
	char        *type_cstr = (value ? bool_type_true : bool_type_false);
	this->assign(type_cstr);
}

// to ... from string
void string::exporting(int *number) {
	if(this->lenght()&&number) {
		*number=atoi(this->to_c_style());
	}
}
void string::exporting(float *number) {
	if(this->lenght()&&number) {
		*number=(float)atof(this->to_c_style());
	}
}
void string::exporting(bool *value) {
	if(this->lenght()&&value) {
		if(this->is_equal(bool_type_true))
			*value=true;
		else
			*value=false;
	}
}
/*
void string::do_case(cases case_state) {
	unsigned int i=0, sp_chr_found=0;
	// Control
	if(this->lenght()) {
		switch (case_state) {
		case cases::all_lower:
			for(; i<this->lenght(); i++) {
				if(this->is_upper(i+1))
					this->_address[i]=tolower(this->_address[i]);
				else if(this->_spcl_chrs&&(sp_chr_found=this->is_upper(i+1))) {
					if(sp_chr_found%2==0) {
						this->_address[i]=this->_spcl_chrs[sp_chr_found-2];
					}
				}
			}
			break;
		case cases::all_upper:
			for(; i<this->lenght(); i++) {
				if(this->is_lower(i+1))
					this->_address[i]=toupper(this->_address[i]);
				else if(this->_spcl_chrs&&(sp_chr_found=this->is_case_lower(i+1))) {
					if(sp_chr_found%2==1) {
						this->_address[i]=this->_spcl_chrs[sp_chr_found];
					}
				}
			}
			break;
		case cases::first_chr_lower:
			for(; i<this->lenght(); i++) {
				if(this->is_alpha(i+1)&&(i==0||this->is_space(i))) {
					if(this->is_case_upper(i+1))
						this->_address[i]=tolower(this->_address[i]);
					else if(this->_spcl_chrs&&this->is_case_upper(i+1)) {
						if(sp_chr_found%2==0) {
							this->_address[i]=this->_spcl_chrs[sp_chr_found];
						}
					}
				}
			}
		break;
		case cases::first_chr_upper:
			for(; i<this->lenght(); i++) {
				if(this->is_alpha(i+1)&&(i==0||this->is_space(i))) {
					if(this->is_case_lower(i+1))
						this->_address[i]=toupper(this->_address[i]);
					else if(this->_spcl_chrs&&(sp_chr_found=this->is_case_lower(i+1))) {
						if(sp_chr_found%2==1) {
							this->_address[i]=this->_spcl_chrs[sp_chr_found];
						}
					}
				}
			}
		break;
		}
	}
}*/

//
// Funs...
//

void string::fun_bigy_smally(const char special_characters[]) {
	// Control
	if(this->lenght()) {
		// Special characters for other languages
		unsigned int sp_chrs_len = 0, j=0;
		if(special_characters) { sp_chrs_len = (unsigned int)strlen(special_characters); }
		for(unsigned int i=0; i < this->lenght(); i++) {
			// if known character
			if(strchr("aouei", this->_address[i])) {
				this->_address[i] = tolower(this->_address[i]);
			}
			// if special character
			else if(special_characters) {
				unsigned int sp_chrs_len = (unsigned int)strlen(special_characters);
				for(j=0; j < sp_chrs_len; j++) {
					if(this->_address[i] == special_characters[j]) {
						break;
					}
				}
				if(j<sp_chrs_len) {
					this->_address[j] = special_characters[j+1];
				}
			}
			// unknown then do BIG
			else {
				this->_address[i] = toupper(this->_address[i]);
			}
		}
	}
}

//
// Comparise
//

bool string::is_equal(char chr, unsigned int index) {
	// Control
	if(this->lenght()) {
		if(index>=0&&index<=this->lenght()) {
			return (this->_address[index-1]==chr);
		}
	}

	return false;
}
bool string::is_equal(const char *str) {
	if(str && this->lenght()) {
		if( !strncmp(this->_address,str,this->lenght()) )
			return true;
	}

	return false;
}
bool string::is_equal(const string *str) {
	string *ffstring = const_cast<string*>(str);
	if(ffstring && this->lenght()) {
		if( !strncmp(this->_address,ffstring->_address,this->lenght()) )
			return true;
	}

	return false;
}

bool string::is_digit(unsigned int from, unsigned int to) {
	unsigned int i=0, len=to-from+1;
	char       chr=0;
	if(this->lenght()) {
		for(; i<len; i++) {
			chr=this->_address[from-1+i];
			if((chr>='0'&&chr<='9')||(chr=='.')) continue;
			break;
		}

		if(i>=len) return true;
	}

	return false;
}
bool string::is_alpha(unsigned int from, unsigned int to) {
	unsigned int i=0, len=to-from+1;
	char       chr=0;
	if(this->lenght()) {
		for(; i<len; i++) {
			chr=this->_address[from-1+i];
			if((chr>='a'&&chr<='z')||(chr>='A'&&chr<='Z')) continue;
			else if(this->_spcl_chrs)
				if(strchr(this->_spcl_chrs,chr))
					continue;
			break;
		}

		if(i>=len) return true;
	}

	return false;
}
bool string::is_space(unsigned int from, unsigned int to) {
	unsigned int i=0, len=to-from+1;
	char       chr=0;
	if(this->lenght()) {
		for(; i<len; i++) {
			chr=this->_address[from-1+i];
			if(chr==' '||chr=='\t') continue;
			break;
		}

		if(i>=len) return true;
	}

	return false;
}

bool string::is_upper(unsigned int from, unsigned int to) {
	unsigned int i=0, len=to-from+1;
	char       chr=0;
	if(this->lenght()) {
		for(; i<len; i++) {
			chr=this->_address[from-1+i];
			if(chr>='A'&&chr<='Z') continue;
			else if(this->_spcl_chrs)
				if(strchr(this->_spcl_chrs,chr))
					continue;
			break;
		}

		if(i>=len) return true;
	}

	return false;
}/*
short string::is_lower(unsigned int from, unsigned int to) {
	char chr=this->_address[index-1];
	if(chr>='a'&&chr<='z')
		return toupper(chr);
	else if(this->_spcl_chrs) {
		unsigned int i=0,len=(unsigned int)strlen(this->_spcl_chrs);
		for( ; i<len; i++) { if(chr==this->_spcl_chrs[i]) break; }
		if(i<=len) return j;
	}

	return 0;
}*/

//
// C style
//

char *const string::to_c_style() {
	if(this->capacity()) { return this->_address; }

	return 0;
}

//
// C++ style...
//

// Console
std::ostream &operator<<(string &str, std::ostream &o) {
	if(str.lenght()) {
		o.write(str._address, str.lenght());
	}

	return o;
}
std::ostream &operator<<(std::ostream &o, string &str) {
	if(str.lenght()) {
		o.write(str._address, str.lenght());
	}

	return o;
}
std::istream &operator>>(string &str, std::istream &i) {
	// Get byte count
	i.seekg(0, std::ios::end);
	unsigned int lenght = (unsigned int)i.tellg();
	i.seekg(0, std::ios::beg);

	if(lenght==-1) {
		if(!str)
			str.buy_string(1024);
		if(str) {
			i.getline(str._address,str.capacity());
		}
	} else {
		// if not capacity then get new capacity
		if(!str) {
			str.buy_string(lenght+1);
		}

		// if it has capacitiy
		if(str) {
			i.read(str._address, str.capacity());
			str._address[str._lenght = lenght] = '\0';
		}
	}

	return i;
}
std::istream &operator>>(std::istream &i, string &str) {
	// Get byte count
	i.seekg(0, std::ios::end);
	unsigned int lenght = (unsigned int)i.tellg();
	i.seekg(0, std::ios::beg);

	if(lenght==-1) {
		if(!str)
			str.buy_string(1024);
		if(str) {
			i.getline(str._address,str.capacity());
		}
	} else {
		// if not capacity then get new capacity
		if(!str) {
			str.buy_string(lenght+1);
		}

		// if it has capacitiy
		if(str) {
			i.read(str._address, str.capacity());
			str._address[str._lenght = lenght] = '\0';
		}
	}

	return i;
}

//
// Operators...
//

// Index
char &string::operator[](unsigned int i) {
	char *ffpchr = &temp_char;
	if(i>=0 && i<this->capacity()) {
		// if i bigger than lenght or equal then increase lenght (for adding)
		if(i > this->lenght()) { this->_lenght = i+1; }

		// return the will write address
		ffpchr = this->_address + i;
	}

	return *ffpchr;
}

// Assigments
string &string::operator=(const string &str) {
	string *ffstring = const_cast<string*>(&str);
	if(ffstring->lenght()>this->capacity()-1) {
		// [TOP]
		if(this->_state==states::drag) {
			this->_state=states::none;
			this->sell_string();
			memcpy(this,ffstring,sizeof(string));
			return *this;
		} else {
			this->assign(ffstring);
		}
	} else {
		this->copy(ffstring,ffstring->lenght());
	}

	return *this;
}

// + (Adding)

// string + character
string operator+(const string &left, char right) {
	string ffnewstr, *ffleft = const_cast<string*>(&left);

	ffnewstr.assign(ffleft, ffleft->lenght()+2);

	if(ffnewstr.capacity()) {
		ffnewstr.add(right);
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// character + string
string operator+(char left, const string &right) {
	string ffnewstr, *ffright = const_cast<string*>(&right);

	ffnewstr.assign(left, ffright->lenght()+2);

	if(ffnewstr.capacity()) {
		ffnewstr.add(ffright);
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// string + string (C style)
string operator+(const string &left, const char *right) {
	string ffnewstr, *ffstring = const_cast<string*>(&left);

	ffnewstr.assign(&left, ffstring->lenght()+(unsigned int)strlen(right)+1);

	if(ffnewstr.capacity()) {
		ffnewstr.add(right);
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// string (C style) + string
string operator+(const char *left, const string &right) {
	string ffnewstr, *ffstring=const_cast<string*>(&right);

	ffnewstr.assign(left, (unsigned int)strlen(left)+ffstring->lenght()+1);

	if(ffnewstr.capacity()) {
		ffnewstr.add(ffstring);
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// string + string
string operator+(const string &left, const string &right) {
	string *ffleft  = const_cast<string*>(&left);
	string *ffright = const_cast<string*>(&right);
	string  ffnewstr;

	ffnewstr.assign(ffleft, ffleft->lenght()+ffright->lenght()+1);

	if(ffnewstr) {
		ffnewstr.add(ffright);
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// Extreme adding
// string + bool
string operator+(const string &left, bool right) {
	string      *ffleft  = const_cast<string*>(&left);
	string converted(temp_cstr_len);

	converted.importing(right);

	string ffnewstr(ffleft, ffleft->lenght()+converted.lenght()+1);

	if(ffnewstr) {
		ffnewstr.add(&converted);
		converted.sell_string();
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// bool + string
string operator+(bool left, const string &right) {
	string      *ffright  = const_cast<string*>(&right);
	string converted(temp_cstr_len);

	converted.importing(left);

	string ffnewstr(&converted, converted.lenght()+ffright->lenght()+1);

	if(ffnewstr) {
		ffnewstr.add(ffright);
		converted.sell_string();
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// string + number
string operator+(const string &left, int right) {
	string *ffleft = const_cast<string*>(&left);
	string converted(temp_cstr_len);

	converted.importing(right);

	string ffnewstr(ffleft, ffleft->lenght()+converted.lenght()+1);

	if(ffnewstr) {
		ffnewstr.add(&converted);
		converted.sell_string();
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// number + string
string operator+(int left, const string &right) {
	string *ffright = const_cast<string*>(&right);
	string converted(temp_cstr_len);

	converted.importing(left);

	string ffnewstr(&converted, converted.lenght()+ffright->lenght()+1);

	if(ffnewstr) {
		ffnewstr.add(ffright);
		converted.sell_string();
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// string + real
string operator+(const string &left, float right) {
	string *ffleft = const_cast<string*>(&left);
	string converted(temp_cstr_len);

	converted.importing(right);

	string ffnewstr(ffleft, ffleft->lenght()+converted.lenght()+1);

	if(ffnewstr) {
		ffnewstr.add(&converted);
		converted.sell_string();
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

// real + string
string operator+(float left, const string &right) {
	string *ffright = const_cast<string*>(&right);
	string converted(temp_cstr_len);

	converted.importing(left);

	string ffnewstr(&converted, converted.lenght()+ffright->lenght()+1);

	if(ffnewstr) {
		ffnewstr.add(ffright);
		converted.sell_string();
		ffnewstr._state=string::states::drag;
	}

	return ffnewstr;
}

//
// Interface ended
//



#endif // __cplusplus
