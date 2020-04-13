#pragma once
#include "Model2D.h"
#include "Options\\OptionExternDecl.h"

#define autoDirsFName "autoDirs.dat"

class Options{
public:
	Options();
	int loadDirs();
	int saveAuto() const;
	int loadAuto();
private:
	static const int szDirs = 16 * 4 * 4;
	static const int szArr = szDirs; // + ...
	int iArr[szArr];
	double dArr[szArr];

	int* dirs;

	int savei(const int* ar, const int sz, const char* fname) const;
	bool loadi(int* ar, const int sz, const char* fname) const;
	int saved(const double* ar, const int sz, const char* fname) const;
	int loadd(double* ar, const int sz, const char* fname) const;
}; // ***************************************************************************************

