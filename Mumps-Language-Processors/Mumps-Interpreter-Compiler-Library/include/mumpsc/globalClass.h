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

#ifndef GLOBAL_CLASS_H
#define GLOBAL_CLASS_H

class global {
      public:
            global(const char *);
            global MakeGlobal(int, char * gblnme, char * key,...);

	mstring Translate(mstring, mstring);
	mstring Translate(mstring);
	mstring Soundex();

	mstring Concat(string);
	mstring Concat(global);
	mstring Concat(char *);
	mstring Concat(mstring);
	mstring Concat(int);
	mstring Concat(long);
	mstring Concat(double);

	mstring Extract(int=1, int=-1);
        int Find(mstring, int=1);
        int Find(global, int=1);
        int Find(char *, int=1);
        int Ascii(int start);
        int Ascii();
	mstring zBlanks();
	mstring Query();
	int Qlength();
	mstring Reverse();
	mstring Qsubscript(int);
	int Pattern(mstring &);
	int Pattern(string);
	int Pattern(global);
        int Pattern(const char *);
        mstring Justify(int,int=-1);
	int Length();
	int Length(char *);
	int Length(mstring);
	int Length(global);
	int Length(string);
        mstring Piece(const char *, int, int=-1); 
        mstring Piece(mstring &, int, int=-1); 
        mstring Piece(string, int, int=-1); 
        mstring Piece(global, int, int=-1); 
	mstring Fnumber(mstring, mstring);
	mstring Fnumber(mstring);
        bool ReadLine();
        bool ReadLine(FILE *);
        bool ReadLine(istream &);
        friend class mstring;

            void Flush();
            void Transpose(global);
            void Multiply(global, global);
            void Centroid(global);
            double Sum();
            void TermCorrelate(global);
            void DocCorrelate(global, string="Cosine", double=0.8);
            void DocCorrelate(global, char *, double=0.8);
            void DocCorrelate(global, mstring, double=0.8);
            double Avg();
            void IDF(double);
            long Count();
            double Max();
            double Min();

            void TreePrint(int=1, const char=' ');
            void TablePrint(int=1, const char=' ');


	mstring operator+() ; // unary plus
	mstring operator-() ; // unary minus

	mstring operator&(mstring);
	mstring operator&(char *);
	mstring operator&(global);
	mstring operator&(string);
	mstring operator&(int);
	mstring operator&(long);
	mstring operator&(double);

            global & operator=(const char *);
            global & operator=(int);
            global & operator=(double);
            global & operator=(string);
            global & operator=(global);
            global & operator=(unsigned int);
            global & operator=(float);
            global & operator=(short);
            global & operator=(unsigned short);
            global & operator=(long);
            global & operator=(unsigned long);
            global & operator=(mstring);

            global & operator<<(char *);
            global & operator<<(int);
            global & operator<<(unsigned int);
            global & operator<<(short);
            global & operator<<(unsigned short);
            global & operator<<(long);
            global & operator<<(unsigned long);
            global & operator<<(float);
            global & operator<<(double);
            global & operator<<(string);
            global & operator<<(global);
            global & operator<<(mstring);

            global & operator()(indirect);

            global & operator()();
            global & operator()(string);
            global & operator()(string,string);
            global & operator()(string,string,string);
            global & operator()(string,string,string,string);
            global & operator()(string,string,string,string,string);
            global & operator()(string,string,string,string,string,string);
            global & operator()(string,string,string,string,string,string,string);
            global & operator()(string,string,string,string,string,string,string,string);
            global & operator()(string,string,string,string,string,string,string,string,string);
            global & operator()(string,string,string,string,string,string,string,string,string,string);

            global & operator()(mstring);
            global & operator()(mstring,mstring);
            global & operator()(mstring,mstring,mstring);
            global & operator()(mstring,mstring,mstring,mstring);
            global & operator()(mstring,mstring,mstring,mstring,mstring);
            global & operator()(mstring,mstring,mstring,mstring,mstring,mstring);
            global & operator()(mstring,mstring,mstring,mstring,mstring,mstring,mstring);
            global & operator()(mstring,mstring,mstring,mstring,mstring,mstring,mstring,mstring);
            global & operator()(mstring,mstring,mstring,mstring,mstring,mstring,mstring,mstring,mstring);
            global & operator()(mstring,mstring,mstring,mstring,mstring,mstring,mstring,mstring,mstring,mstring);

global & operator() (const long int a);
            global & operator()(const char *);
            global & operator()(const char *,const char *);
            global & operator()(const char *,const char *,const char *);
            global & operator()(const char *,const char *,const char *,const char *);
            global & operator()(const char *,const char *,const char *,const char *,const char *);
            global & operator()(const char *,const char *,const char *,const char *,const char *,
		const char *);
            global & operator()(const char *,const char *,const char *,const char *,const char *,
		const char *,const char *);
            global & operator()(const char *,const char *,const char *,const char *,const char *,
		const char *,const char *,const char *);
            global & operator()(const char *,const char *,const char *,const char *,const char *,
		const char *,const char *,const char *,const char *);
            global & operator()(const char *,const char *,const char *,const char *,const char *,
		const char *,const char *,const char *,const char *,const char *);

            char * operator>>(char *);
            int operator>>(int &);
            unsigned int operator>>(unsigned int &);
            long operator>>(long &);
            unsigned long operator>>(unsigned long &);
            short operator>>(short &);
            unsigned short operator>>(unsigned short &);
            float operator>>(float &);
            double operator>>(double &);
            string operator>>(string &);
            mstring operator>>(mstring &);
            global & operator>>(global);
            friend char * __Name(global,struct MSV *);

            int Int();
            double Double();
            char * Char(char *, int);
            mstring Mstring();

            int operator+=(int);
            unsigned int operator+=(unsigned int);
            short operator+=(short);
            unsigned short operator+=(unsigned short);
            long operator+=(long);
            unsigned long operator+=(unsigned long);
            float operator+=(float);
            double operator+=(double);

            friend int operator+=(int &,global);
            friend unsigned int operator+=(unsigned int,global);
            friend short operator+=(short,global);
            friend unsigned short operator+=(unsigned short,global);
            friend long operator+=(long,global);
            friend unsigned long operator+=(unsigned long,global);
            friend float operator+=(float,global);
            friend double operator+=(double,global);

            friend int operator-=(int &,global);
            friend unsigned int operator-=(unsigned int,global);
            friend short operator-=(short,global);
            friend unsigned short operator-=(unsigned short,global);
            friend long operator-=(long,global);
            friend unsigned long operator-=(unsigned long,global);
            friend float operator-=(float,global);
            friend double operator-=(double,global);

            friend int operator*=(int &,global);
            friend unsigned int operator*=(unsigned int,global);
            friend short operator*=(short,global);
            friend unsigned short operator*=(unsigned short,global);
            friend long operator*=(long,global);
            friend unsigned long operator*=(unsigned long,global);
            friend float operator*=(float,global);
            friend double operator*=(double,global);

            friend int operator/=(int &,global);
            friend unsigned int operator/=(unsigned int,global);
            friend short operator/=(short,global);
            friend unsigned short operator/=(unsigned short,global);
            friend long operator/=(long,global);
            friend unsigned long operator/=(unsigned long,global);
            friend float operator/=(float,global);
            friend double operator/=(double,global);

            int operator-=(int);
            unsigned int operator-=(unsigned int);
            short operator-=(short);
            unsigned short operator-=(unsigned short);
            long operator-=(long);
            unsigned long operator-=(unsigned long);
            float operator-=(float);
            double operator-=(double);

            int operator*=(int);
            unsigned int operator*=(unsigned int);
            short operator*=(short);
            unsigned short operator*=(unsigned short);
            long operator*=(long);
            unsigned long operator*=(unsigned long);
            float operator*=(float);
            double operator*=(double);

            int operator/=(int);
            unsigned int operator/=(unsigned int);
            short operator/=(short);
            unsigned short operator/=(unsigned short);
            long operator/=(long);
            unsigned long operator/=(unsigned long);
            float operator/=(float);
            double operator/=(double);

            int operator+(int);
            unsigned int operator+(unsigned int);
            long operator+(long);
            unsigned long operator+(unsigned long);
            short operator+(short);
            float operator+(float);
            unsigned short operator+(unsigned short);
            double operator+(double);
            double operator+(global);

            friend int operator+(int,global);
            friend unsigned int operator+(unsigned int,global);
            friend unsigned long operator+(unsigned long,global);
            friend long operator+(long,global);
            friend short operator+(short,global);
            friend unsigned short operator+(unsigned short,global);
            friend float operator+(float,global);
            friend double operator+(double,global);

            int operator-(int);
            unsigned int operator-(unsigned int);
            long operator-(long);
            unsigned long operator-(unsigned long);
            short operator-(short);
            float operator-(float);
            double operator-(double);
            double operator-(global);
            unsigned short operator-(unsigned short);

            friend int operator-(int,global);
            friend unsigned int operator-(unsigned int,global);
            friend unsigned long operator-(unsigned long,global);
            friend short operator-(short,global);
            friend long operator-(long,global);
            friend float operator-(float,global);
            friend double operator-(double,global);
            friend unsigned short operator-(unsigned short,global);

            int operator*(int);
            unsigned int operator*(unsigned int);
            long operator*(long);
            unsigned long operator*(unsigned long);
            short operator*(short);
            float operator*(float);
            double operator*(double);
            double operator*(global);
            unsigned short operator*(unsigned short);

            friend int operator*(int,global);
            friend unsigned int operator*(unsigned int,global);
            friend long operator*(long,global);
            friend unsigned long operator*(unsigned long,global);
            friend short operator*(short,global);
            friend float operator*(float,global);
            friend double operator*(double,global);
            friend unsigned short operator*(unsigned short,global);

            int operator/(int);
            unsigned int operator/(unsigned int);
            long operator/(long);
            unsigned long operator/(unsigned long);
            short operator/(short);
            unsigned short operator/(unsigned short);
            float operator/(float);
            double operator/(double);
            double operator/(global);

            friend int operator/(int,global);
            friend unsigned int operator/(unsigned int,global);
            friend long operator/(long,global);
            friend unsigned long operator/(unsigned long,global);
            friend short operator/(short,global);
            friend unsigned short operator/(unsigned short,global);
            friend float operator/(float,global);
            friend double operator/(double,global);

            double operator++();
            double operator--();
            double operator++(int);
            double operator--(int);

            int operator>(global);
            int operator>(int);
            int operator>(unsigned int);
            int operator>(long);
            int operator>(unsigned long);
            int operator>(short);
            int operator>(unsigned short);
            int operator>(float);
            int operator>(double);
            int operator>(char *);
            int operator>(string);

            int operator<(global);
            int operator<(int);
            int operator<(unsigned int);
            int operator<(long);
            int operator<(unsigned long);
            int operator<(short);
            int operator<(unsigned short);
            int operator<(float);
            int operator<(double);
            int operator<(char *);
            int operator<(string);
            int operator<(mstring);

            int operator<=(global);
            int operator<=(int);
            int operator<=(unsigned int);
            int operator<=(long);
            int operator<=(unsigned long);
            int operator<=(short);
            int operator<=(unsigned short);
            int operator<=(float);
            int operator<=(double);
            int operator<=(char *);
            int operator<=(string);

            int operator>=(global);
            int operator>=(int);
            int operator>=(unsigned int);
            int operator>=(long);
            int operator>=(unsigned long);
            int operator>=(short);
            int operator>=(unsigned short);
            int operator>=(float);
            int operator>=(double);
            int operator>=(char *);
            int operator>=(string);

            int operator==(global);
            int operator==(int);
            int operator==(unsigned int);
            int operator==(long);
            int operator==(unsigned long);
            int operator==(short);
            int operator==(unsigned short);
            int operator==(float);
            int operator==(double);
            int operator==(char *);
            int operator==(string);

            int operator!=(global);
            int operator!=(int);
            int operator!=(unsigned int);
            int operator!=(long);
            int operator!=(unsigned long);
            int operator!=(short);
            int operator!=(unsigned short);
            int operator!=(float);
            int operator!=(double);
            int operator!=(char *);
            int operator!=(string);

            // casts

            operator char*() ;
            operator int();
            operator unsigned int();
            operator short();
            operator unsigned short();
            operator long();
            operator unsigned long();
            operator float();
            operator double();
            operator mstring();

            friend double Cosine(global,global);
            friend double Sim1(global, global);
            friend double Jaccard(global, global);
            friend double Dice(global, global);

            friend int operator>(int,global);
            friend int operator>(unsigned int,global);
            friend int operator>(long,global);
            friend int operator>(unsigned long,global);
            friend int operator>(short,global);
            friend int operator>(unsigned short,global);
            friend int operator>(float,global);
            friend int operator>(double,global);
            friend int operator>(char *,global);
            friend int operator>(string,global);

            friend int operator<(int,global);
            friend int operator<(unsigned int,global);
            friend int operator<(long,global);
            friend int operator<(unsigned long,global);
            friend int operator<(short,global);
            friend int operator<(unsigned short,global);
            friend int operator<(float,global);
            friend int operator<(double,global);
            friend int operator<(char *,global);
            friend int operator<(string,global);
            friend int operator<(mstring,global);

            friend int operator<=(int,global);
            friend int operator<=(unsigned int,global);
            friend int operator<=(long,global);
            friend int operator<=(unsigned long,global);
            friend int operator<=(short,global);
            friend int operator<=(unsigned short,global);
            friend int operator<=(float,global);
            friend int operator<=(double,global);
            friend int operator<=(char *,global);
            friend int operator<=(string,global);

            friend int operator>=(int,global);
            friend int operator>=(unsigned int,global);
            friend int operator>=(long,global);
            friend int operator>=(unsigned long,global);
            friend int operator>=(short,global);
            friend int operator>=(unsigned short,global);
            friend int operator>=(float,global);
            friend int operator>=(double,global);
            friend int operator>=(char *,global);
            friend int operator>=(string,global);

            friend int operator==(int,global);
            friend int operator==(unsigned int,global);
            friend int operator==(long,global);
            friend int operator==(unsigned long,global);
            friend int operator==(short,global);
            friend int operator==(unsigned short,global);
            friend int operator==(float,global);
            friend int operator==(double,global);
            friend int operator==(char *,global);
            friend int operator==(string,global);

            friend int operator!=(int,global);
            friend int operator!=(unsigned int,global);
            friend int operator!=(long,global);
            friend int operator!=(unsigned long,global);
            friend int operator!=(short,global);
            friend int operator!=(unsigned short,global);
            friend int operator!=(float,global);
            friend int operator!=(double,global);
            friend int operator!=(char *,global);
            friend int operator!=(string,global);
            
            friend ostream & operator<<(ostream&,global);
            char * get();
            char * Set (char *);
            long   Set (long);
            int    Set (int);
            double Set (double);
            string Set (string);
            char * Get ();
            char * Get (char *);
            long   Get (long *);
            double Get (double *);
            string Get (string &);
            mstring Order(int=1);
            char * C_Order(int=1);
            int    Order_Next(char *);
            int    Order_Prior(char *);
            int    Data();
            mstring Name();
            int Merge(global);
            int Lock();
            int UnLock();
            void Kill ();
            char Gtmp[STR_MAX];
            char tmp[STR_MAX];
      private:
            static int Eflg;
            int ArgCount;
            char myname[256];
            char ref[STR_MAX];
            // char Sref[STR_MAX];
            string Sref;
            //char Sname[STR_MAX];
            string Sname;
      };

#endif
