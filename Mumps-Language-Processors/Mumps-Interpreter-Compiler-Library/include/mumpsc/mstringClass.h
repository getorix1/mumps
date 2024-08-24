/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+   
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2015 by Kevin C. O'Kane  
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     http://threadsafebooks.com
#+
#+ This program is free software; you can redistribute it and/or modify
#+ it under the terms of the GNU General Public License as published by
#+ the Free Software Foundation; either version 2 of the License, or
#+ (at your option) any later version.
#+ 
#+ This program is distributed in the hope that it will be useful,
#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#+ GNU General Public License for more details.
#+ 
#+ You should have received a copy of the GNU General Public License
#+ along with this program; if not, write to the Free Software
#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#+
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//	June 5, 2022

#ifndef MSTRING_H
#define MSTRING_H

class mstring {
  public:
      mstring();
      mstring(const char *);
      mstring(long);
      mstring(int);
      mstring(double);
      mstring(float);
      mstring(char);
      mstring(const mstring &);
      mstring(const string &);

      friend class global;

      const char * c_str();
      const string s_str();

      friend mstring Hash::operator=(mstring);
      friend mstring global::Piece(mstring &, int, int); 
      friend mstring SQL_Table(mstring,int);

      mstring Eval();

      mstring Translate(mstring, mstring);
      mstring Translate(mstring);
      mstring Soundex();
      int replace(mstring,mstring); // pattern, replacement
      int decorate(mstring,mstring,mstring); // pattern, prefix, suffix
      int ends(mstring); // pattern
      int begins(mstring); // pattern

	void Assign(char *);
	void Assign(global);
	void Assign(mstring);
	void Assign(string);
	void Assign(int);
      void Assign(long);
      void Assign(double);

      friend int SQL_Connect(mstring);
      friend int SQL_Format(mstring);
      friend int SQL_Command(mstring);

	mstring Concat(char *);
	mstring Concat(global);
	mstring Concat(mstring);
	mstring Concat(string);
	mstring Concat(int);
        mstring Concat(long);
        mstring Concat(double);

	mstring Reverse();
        bool ReadLine();
        bool ReadLine(FILE *);
        
	bool ReadLine(istream &);
        operator int();
        operator float();
        operator double();
        operator char();
        operator short();
        operator long();
        operator unsigned long();
        operator unsigned int();
        operator unsigned char();
        operator unsigned short();

        int Pattern(mstring &);
        int Pattern(string);
        int Pattern(global);
        int Pattern(const char *);

	mstring Query();
	int Qlength();
	mstring Qsubscript(int);
	mstring FixGblRef();

        mstring Justify(int,int=-1);

	mstring Fnumber(mstring, mstring);
	mstring Fnumber(mstring);

        int Length(mstring);
        int Length(global);
        int Length(string);
        int Length(char *);
        int Length();

        int Ascii(int start);
        int Ascii();

	mstring zBlanks();

        mstring Extract(int=1, int=-1);

        int Find(const char *, int=1);
        int Find(mstring, int=1);
        int Find(global, int=1);

        mstring Piece(const char *, int, int=-1); 
        mstring Piece(mstring &, int, int=-1); 
        mstring Piece(string, int, int=-1); 
        mstring Piece(global, int, int=-1); 

	mstring operator&(mstring);
	mstring operator&(char *);
	mstring operator&(global);
	mstring operator&(string);
	mstring operator&(int);
	mstring operator&(long);
	mstring operator&(double);

      mstring & operator[](mstring);
      mstring & operator[](int);
      mstring & operator[](const char *);
      mstring & operator[](string);

      mstring operator^=(int);
      mstring operator^=(long);
      mstring operator^=(double);
      mstring operator^=(float);
      mstring operator^=(string);
      mstring operator^=(const char *);
      mstring operator^=(global);

      mstring operator+=(mstring);
      mstring operator+=(int);
      mstring operator+=(long);
      mstring operator+=(double);
      mstring operator+=(float);
      mstring operator+=(string);
      mstring operator+=(const char *);
      mstring operator+=(global);

      mstring operator-=(mstring);
      mstring operator-=(int);
      mstring operator-=(long);
      mstring operator-=(double);
      mstring operator-=(float);
      mstring operator-=(string);
      mstring operator-=(const char *);
      mstring operator-=(global);

      mstring operator*=(mstring);
      mstring operator*=(int);
      mstring operator*=(long);
      mstring operator*=(double);
      mstring operator*=(float);
      mstring operator*=(string);
      mstring operator*=(const char *);
      mstring operator*=(global);

      mstring operator/=(mstring);
      mstring operator/=(int);
      mstring operator/=(long);
      mstring operator/=(double);
      mstring operator/=(float);
      mstring operator/=(string);
      mstring operator/=(const char *);
      mstring operator/=(global);

      mstring operator%=(mstring);
      mstring operator%=(int);
      mstring operator%=(long);
      mstring operator%=(string);
      mstring operator%=(const char *);
      mstring operator%=(global);

      mstring& operator=(int);
      mstring& operator=(long);
      mstring& operator=(double);
      mstring& operator=(float);
      mstring& operator=(string);
      mstring& operator=(const char *);
      mstring& operator=(mstring);
      mstring& operator=(global);

      mstring operator+(int);
      mstring operator+(long);
      mstring operator+(double);
      mstring operator+(float);
      mstring operator+(mstring);
      mstring operator+(string);
      mstring operator+(const char *);
      mstring operator+(char *);
      mstring operator+(global);

      mstring operator||(mstring);
      mstring operator||(string);
      mstring operator||(global);
      mstring operator||(const char *);
      mstring operator||(int);
      mstring operator||(long);
      mstring operator||(float);
      mstring operator||(double);

      friend mstring operator||(string,mstring);
      friend mstring operator||(global,mstring);
      friend mstring operator||(const char *,mstring);
      friend mstring operator||(int,mstring);
      friend mstring operator||(long,mstring);
      friend mstring operator||(float,mstring);
      friend mstring operator||(double,mstring);

      mstring operator-(int);
      mstring operator-(long);
      mstring operator-(double);
      mstring operator-(float);
      mstring operator-(mstring);
      mstring operator-(string);
      mstring operator-(const char *);
      mstring operator-(char *);
      mstring operator-(global);

      mstring operator*(int);
      mstring operator*(long);
      mstring operator*(double);
      mstring operator*(float);
      mstring operator*(mstring);
      mstring operator*(string);
      mstring operator*(global);
      mstring operator*(const char *);

      mstring operator/(int);
      mstring operator/(long);
      mstring operator/(double);
      mstring operator/(float);
      mstring operator/(mstring);
      mstring operator/(string);
      mstring operator/(global);
      mstring operator/(const char *);

      mstring operator%(long);
      mstring operator%(int);
      mstring operator%(mstring);
      mstring operator%(string);
      mstring operator%(const char *);
      mstring operator%(global);

      mstring operator++();
      mstring operator--();
      mstring operator++(int);
      mstring operator--(int);

      mstring operator!(); // unary
      mstring operator+(); // unary
      mstring operator-(); // unary

      friend mstring operator%(int,mstring);
      friend mstring operator%(long,mstring);
      friend mstring operator%(string,mstring);
      friend mstring operator%(global,mstring);
      friend mstring operator%(const char *,mstring);

      friend mstring operator+(int,mstring);
      friend mstring operator+(long,mstring);
      friend mstring operator+(double,mstring);
      friend mstring operator+(float,mstring);
      friend mstring operator+(string,mstring);
      friend mstring operator+(global,mstring);
      friend mstring operator+(const char *,mstring);
      friend mstring operator+(char *,mstring);

      friend mstring operator-(int,mstring);
      friend mstring operator-(long,mstring);
      friend mstring operator-(double,mstring);
      friend mstring operator-(float,mstring);
      friend mstring operator-(string,mstring);
      friend mstring operator-(global,mstring);
      friend mstring operator-(const char *,mstring);
      friend mstring operator-(char *,mstring);

      friend mstring operator*(int,mstring);
      friend mstring operator*(long,mstring);
      friend mstring operator*(double,mstring);
      friend mstring operator*(float,mstring);
      friend mstring operator*(string,mstring);
      friend mstring operator*(global,mstring);
      friend mstring operator*(const char *,mstring);

      friend mstring operator/(int,mstring);
      friend mstring operator/(long,mstring);
      friend mstring operator/(double,mstring);
      friend mstring operator/(float,mstring);
      friend mstring operator/(string,mstring);
      friend mstring operator/(global,mstring);
      friend mstring operator/(const char *,mstring);

      friend ostream & operator<<(ostream &, mstring);
      friend istream & operator>>(istream &, mstring &);

      bool operator<(int);
      bool operator<(long);
      bool operator<(double);
      bool operator<(float);
      bool operator<(char *);
      bool operator<(const char *);
      bool operator<(string);
      bool operator<(global);
      bool operator<(mstring);

      bool operator>(int);
      bool operator>(long);
      bool operator>(double);
      bool operator>(float);
      bool operator>(char *);
      bool operator>(const char *);
      bool operator>(string);
      bool operator>(global);
      bool operator>(mstring);

      bool operator>=(int);
      bool operator>=(long);
      bool operator>=(double);
      bool operator>=(float);
      bool operator>=(char *);
      bool operator>=(const char *);
      bool operator>=(string);
      bool operator>=(global);
      bool operator>=(mstring);

      bool operator<=(int);
      bool operator<=(long);
      bool operator<=(double);
      bool operator<=(float);
      bool operator<=(char *);
      bool operator<=(const char *);
      bool operator<=(string);
      bool operator<=(global);
      bool operator<=(mstring);

      bool operator==(int);
      bool operator==(long);
      bool operator==(double);
      bool operator==(float);
      bool operator==(const char *);
      bool operator==(char *);
      bool operator==(string);
      bool operator==(global);
      bool operator==(mstring);

      bool operator!=(int);
      bool operator!=(long);
      bool operator!=(double);
      bool operator!=(float);
      bool operator!=(char *);
      bool operator!=(const char *);
      bool operator!=(string);
      bool operator!=(global);
      bool operator!=(mstring);

      operator const char * ();

  private:
      string data;
      string name;
      };

#endif
