#pragma once
#include "Options\\OptionExternDecl.h"
#include "Masks.h"

#define autoDirsFName "autoArr.dat"

class Options{
public:
	static const int szDirs = Blocks2D2::szFLT;
	Options();
	bool saveAuto() const;
	bool loadAuto();
	FLT2* getFLT2(){return blocks2D2.vin;}
	int showDlg();
	bool isRestart(){return retDlg & ReturnOptions::Restart;}

	Blocks2D2 blocks2D2;
	static const int szArr = szDirs; // + ...
	int iArr[szArr];
	double dArr[szArr];

	int* dirs;
	int retDlg = 0;

	bool setDefault();
	bool save(const char* fname) const;
	bool load(const char* fname);
private:
	void loadAll();
}; // ***************************************************************************************

