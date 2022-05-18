/*
File     : string.h
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

/*

TODO:
	do_case();

*/

/*

NOTE [TOP]: This line for performace so if you use operator+ then
can't destroy the string class being inside operator+ and will carry the string class being inside
operator+ to this string class after erase this class

*/

#ifndef head_of_string_class
#define head_of_string_class

//
//
// Includes
//

#include <iostream>				// for c++ i/o stream and c style string functions



//
//
// Interface starting
//

class string {

public:

	enum cases {
		all_lower,
		all_upper,
		first_chr_upper,
		first_chr_lower
	};
	
		// [TOP]
	enum states {
		none = 0,
		drag,
		dont_destroy
	} static _state;



public:

	// Cs&Ds
	inline string()
		: _address(0), _capacity(0), _lenght(0) {}
	inline explicit string(unsigned int size)
		: _address(0), _capacity(0), _lenght(0) { this->buy_string(size); }
	
	inline string(const char   *str)
		: _address(0), _capacity(0), _lenght(0) { this->assign( str); }
	inline string(const char   *str, unsigned int capacity)
		: _address(0), _capacity(0), _lenght(0) { this->assign( str, capacity); }
	
	inline string(const string *str)
		: _address(0), _capacity(0), _lenght(0) { this->assign( str); }
	inline string(const string *str, unsigned int capacity)
		: _address(0), _capacity(0), _lenght(0) { this->assign( str, capacity); }

	inline string(const string &str)
		: _address(0), _capacity(0), _lenght(0) {
		// [TOP]
		if(this->_state==states::drag) {
			this->_state=states::dont_destroy;
			memcpy(this,&str,sizeof(string));
		} else {
			this->assign(&str);
		}
	}
	inline string(const string &str, unsigned int capacity)
		: _address(0), _capacity(0), _lenght(0) { this->assign(&str, capacity); }

	inline ~string() {
		// [TOP]
		if(this->_state==states::dont_destroy) {
			this->_state=states::drag;
		} else {
			if((*this)) {
				this->_state=states::none;
				this->sell_string();
			}
		}
	}

	// Assingment

	void buy_string(unsigned int size);				// Empty string

	inline void assign(char          chr) { this->assign(chr, 1); }
	       void assign(char          chr, unsigned int capacity);
	inline void assign(const char   *str) { this->assign(str, (unsigned int)strlen(str)+1 ); }
	       void assign(const char   *str, unsigned int capacity);
	inline void assign(const string *str) { this->assign(str, const_cast<string*>(str)->lenght()+1 ); }
	       void assign(const string *str, unsigned int capacity);

	// Adding

	void add(char          chr, bool resize = true);
	void add(const char   *str, bool resize = true);
	void add(const string *str, bool resize = true);

	// Search, Search & Replace

	unsigned int search(const char *search_str, unsigned int contiue = 1);
	unsigned int replace(const char *search_str, const char *replace_str, unsigned int replace_num = -1);

	inline unsigned int search(const string *search_str, unsigned int contiue = 1) {
		return this->search(const_cast<string*>(search_str)->to_c_style(),contiue);
	}
	inline unsigned int replace(const string *search_str, const string *replace_str, unsigned int replace_num = -1) {
		return this->replace(const_cast<string*>(search_str)->to_c_style(),const_cast<string*>(replace_str)->to_c_style(),replace_num);
	}

	// Copying

	inline void copy(char          chr) { this->copy(chr, 1); }
	inline void copy(const char   *str) { this->copy(str,(unsigned int)strlen(str)); }
	inline void copy(const string *str) { this->copy(str, const_cast<string*>(str)->lenght()); }
	       void copy(char          chr, unsigned int num);
	       void copy(const char   *str, unsigned int num);
	       void copy(const string *str, unsigned int num);

	// Erasing & Clearing

	void sell_string();
	void sell_string(unsigned int from, unsigned int to);
	
	inline void clear() { this->_lenght=0; if(this->capacity()) this->_address[0] = '\0'; }
	       void clear(unsigned int from, unsigned int to);

	// Others
	// Samples for special characters:
	//    spcl_chrs_set("öÖþÞüÜ")	small -> BIG -> small...

	inline         void spcl_chrs_set(char *spcl_chrs) { this->_spcl_chrs = spcl_chrs; }

	inline         void relenght()      { this->_lenght=(unsigned int)strlen(this->_address); }
	inline unsigned int lenght()        { return this->_lenght;   }
	inline unsigned int capacity()      { return this->_capacity; }
	inline unsigned int left_capacity() { return this->capacity()-this->lenght()-1; }
	       unsigned int word_count();

	// Trims/Crops and capacities

	void trim_left();
	void trim_right();
	void trim_lefrig();

	void decrease_capacity();

	// Coverts...
	// to string from x

	void importing(int    convert_from_this);
	void importing(float  convert_from_this);
	void importing(bool   convert_from_this);

	// to x from string

	void exporting(int   *convert_to_this);
	void exporting(float *convert_to_this);
	void exporting(bool  *convert_to_this);

	void do_case(cases case_state);

	// Funs...
	// fun_bigy_smally() examples:
	//    "Hello World!" -> "HeLLo WoRLD!"

	inline void fun_bigy_smally() { this->fun_bigy_smally(0); }
	       void fun_bigy_smally(const char special_characters[]);

	// Comparise

	bool is_equal(char          chr, unsigned int index=1);
	bool is_equal(const char   *str);
	bool is_equal(const string *str);
	bool is_digit(unsigned int from, unsigned int to);
	bool is_alpha(unsigned int from, unsigned int to);
	bool is_space(unsigned int from, unsigned int to);
	bool is_upper(unsigned int from, unsigned int to);
	bool is_lower(unsigned int from, unsigned int to);

	inline bool is_number() { return this->is_digit(1,this->lenght()); }
	inline bool is_word()   { return this->is_alpha(1,this->lenght()); }

	// C style

	char *const to_c_style();

	// C++ style
	// Console...

	friend std::ostream &operator<<(string &str, std::ostream &o);
	friend std::ostream &operator<<(std::ostream &o, string &str);
	friend std::istream &operator>>(string &str, std::istream &i);
	friend std::istream &operator>>(std::istream &i, string &str);

	// Operators...
	// Index

	char &operator[](unsigned int i);

	// Assignment

	inline string &operator=(char          str) { this->assign((char)str); return *this; }
	inline string &operator=(const char   *str) { this->assign(str); return *this; }
	       string &operator=(const string &str);

	// if/while/for ()

	inline operator bool() { return (this->capacity() ? true : false); }

	// comparise

	inline bool operator==(char          chr) { return this->is_equal( chr,1); }
	inline bool operator==(const char   *str) { return this->is_equal( str);   }
	inline bool operator==(const string &str) { return this->is_equal(&str);   }
	inline bool operator!=(char          chr) { return !this->is_equal( chr,1); }
	inline bool operator!=(const char   *str) { return !this->is_equal( str);   }
	inline bool operator!=(const string &str) { return !this->is_equal(&str);   }

	// Adding.. (For c-style and string)

	friend string operator+(const string &left, char right);
	friend string operator+(char left, const string &right);
	friend string operator+(const string &left, const char *right);
	friend string operator+(const char *left, const string &right);
	friend string operator+(const string &left, const string &right);

	// Extreme adding... (For bool, integer, real)

	friend string operator+(const string &left, bool right);
	friend string operator+(bool left, const string &right);
	friend string operator+(const string &left, int right);
	friend string operator+(int left, const string &right);
	friend string operator+(const string &left, float right);
	friend string operator+(float left, const string &right);

	// Assignment adding... (character, c-style string and string)

	inline string &operator+=(char chr)          { *this = *this + chr; return *this; }
	inline string &operator+=(const char *str)   { *this = *this + str; return *this; }
	inline string &operator+=(const string &str) { *this = *this + str; return *this; }

protected:

	char        *_address;
	unsigned int _capacity;
	unsigned int _lenght;

	static char *_spcl_chrs;
};

//
//
// Interface ended
//



#endif	// head_of_string_class
