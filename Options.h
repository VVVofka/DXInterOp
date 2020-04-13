#pragma once
#include "Options\\OptionExternDecl.h"
#include "Masks.h"

#define autoDirsFName "autoDirs.dat"

class Options{
public:
	Options();
	int loadDirs();
	bool saveAuto() const;
	bool loadAuto();

private:
	static const int szDirs = 16 * 4 * 4;
	static const int szArr = szDirs; // + ...
	int iArr[szArr];
	double dArr[szArr];

	int* dirs;

	void setDefault();
	bool save(const char* fname) const;
	bool load(const char* fname);
}; // ***************************************************************************************

