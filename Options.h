#pragma once
#include "Model2D.h"
#include "Options\\OptionExternDecl.h"

#define autoDirsFName "autoDirs.dat"

class Options{
public:
	int loadDirs();
private:
	void save(const int* dirs, const int sz, const char* fname) const;
	void load(int* dirs, const int sz, const char* fname) const;
}; // ***************************************************************************************

