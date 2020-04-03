#pragma once
#include <WinBase.h>
#include <string>
#define APPNAME "AmpDX"
using namespace std;

class Ini{
public:
	static int getInt(const char* key){
		return GetProfileIntA(APPNAME, key, 0);
	}
	static void setInt(const char* key, int val){
		WriteProfileStringA(APPNAME, key, to_string(val).c_str());
	}
	
	static float getFlt(const char* key){
		char s[255];
		GetProfileStringA(APPNAME, key, "", s, 255);
		return float(atof(s));
	}
	static double getDbl(const char* key){
		char s[255];
		GetProfileStringA(APPNAME, key, "", s, 255);
		return atof(s);
	}
	static void setFlt(const char* key, float val){
		WriteProfileStringA(APPNAME, key, to_string(val).c_str());
	}
	static void setDbl(const char* key, double val){
		WriteProfileStringA(APPNAME, key, to_string(val).c_str());
	}

	static string getStr(const char* key){
		char s[255];
		GetProfileStringA(APPNAME, key, "", s, 255);
		return string(s);
	}
	static void setStr(const char* key, const char* val){
		WriteProfileStringA(APPNAME, key, val);
	}
}; // ***********************************************************************************************

