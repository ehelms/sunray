#ifndef _STRING_TOOLS_H_
#define _STRING_TOOLS_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cctype>    

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;
namespace fs = boost::filesystem;  

class StringTools
{
	public:

		static inline double pround(double x,int p){
			return std::floor(x*pow(10.0,p)+0.5)/pow(10.0,p);
		}

		static const std::string changeFileName(const char* filename,const char* suffex){
			char* file=new char[strlen(filename)+1];
			strcpy(file,filename);
			for(int i=0;file[i]!='\0';i++){
				if(file[i]=='\\')file[i]='/';
			}
			fs::path p(file,fs::native);
			std::string ext=suffex;
			boost::filesystem::path p2=change_extension(p,ext);
			delete[] file;
			return p2.native_file_string();	
		}

		static string stripString(string &tmp,const char* tag){
			int pos=tmp.find(tag,0);
			string tmp2;
			if(pos>=0){
				tmp2=tmp.substr(0,pos);
				tmp2+=tmp.substr(pos+strlen(tag),tmp.length());
				tmp=tmp2;
			}   		
			return tmp;
		}

		static string stripString(string &tmp,char tag){
			int pos=tmp.find(tag,0);
			string tmp2;
			if(pos>=0){
				tmp2=tmp.substr(0,pos);
				tmp2+=tmp.substr(pos+1,tmp.length());
				tmp=tmp2;
			}   		
			return tmp;
		}

		static string trimString(string &tmp){
			while((int)tmp.find(' ',0)>=0)stripString(tmp,' ');
			while((int)tmp.find('\n',0)>=0)stripString(tmp,'\n');
			while((int)tmp.find('\t',0)>=0)stripString(tmp,'\t');
			while((int)tmp.find('\f',0)>=0)stripString(tmp,'\f');
			while((int)tmp.find('\r',0)>=0)stripString(tmp,'\r');

			return tmp;
		}

		static double stof(string &tmp){
			return atof(trimString(tmp).c_str());
		}

		static double stoi(string &tmp){
			return atoi(trimString(tmp).c_str());
		}

		/*
		   static char* ftoa( double f, double sigfigs )
		   {
		   char a[81];
		   int prec, width, front, fit;
		   front = (f==0)? 1 : (int)log10(fabs(f))+1;
		   if ( sigfigs < 1.0 && sigfigs >= 0.0 )  // fit number to tolerance
		   {
		   double rem = fabs(f) - int(f);
		   prec=0;
		   int num = (int)rem;
		   while ( rem*pow(10,prec) - num > sigfigs )
		   num = int(rem*pow(10,++prec));
		   width = front;
		   sprintf(a, "%#*.*f", width, prec, f );
		   }
		   else
		   {
		   if ( sigfigs < 2.0 ) sigfigs = 2.0;

		   if ( front > (int)sigfigs )
		   {
		   sprintf( a, "%#.*e", (int)sigfigs-1, f );
		   }
		   else
		   {
		   prec = (int)sigfigs - front;
		   if ( f==0.0 ) width = 2;
		   else width = front + prec + 1;
		   sprintf( a, "%#*.*f", width, prec, f );
		   }
		   }
		   return strdup(a);
		   }
		   */

		static char* ftoa( double f=0, int width=8,int prec=4){
			char a[81];
			sprintf(a, "%#*.*f", width,prec, f );
			return strdup(a);
		}

		static string itos(const int value,const int base=10){
			enum { kMaxDigits = 35 };
			std::string buf;
			buf.reserve( kMaxDigits ); // Pre-allocate enough space.
			// check that the base if valid
			if (base < 2 || base > 16) return buf;
			int quotient = value;
			// Translating number to string with base:
			do {
				buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
				quotient /= base;
			} while ( quotient );
			// Append the negative sign for base 10
			if ( value < 0 && base == 10) buf += '-';
			std::reverse( buf.begin(), buf.end() );
			return buf;
		}

		static string ftos(const double value,const int width=8,const int prec=4){
			string str(ftoa(value,width,prec));
			return str;
		}
};

#endif
