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
	FLT2* getFLT2(){ return blocks2D2.vin; }
	int showDlg();
	bool isRestart(){ return retDlg & InpOptions::Restart; }

	Blocks2D2 blocks2D2;
	static const int sziArr = InpOptions::AMasks + 16; // + ...
	static const int szdArr = InpOptions::kLays + InpOptions::LaysCntReserv; // + ...
	int iArr[sziArr];
	double dArr[szdArr];

	int* dirs;
	int retDlg = 0;

	int normDir = 0; // if DirX < 0.5 * DirY then DirX = 0
	INT2 LaysSzUp{1, 1};
	INT2 LaysSzDn{1024, 1024};

	bool setDefault();
	bool save(const char* fname) const;
	bool load(const char* fname);
						// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
						// 00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
						// 00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
	const int AMask[16] = {0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};  // line=1 diag=1 ++ (large cave)
  //const int AMask[16] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // point=1
  //const int AMask[16] = {0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1};  // line=1 diag=0 +
  //const int AMask[16] = {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};  // line=0 diag=1 quadro
  //const int AMask[16] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1};  // line=0 diag=0 quadro
	int seedRnd = 0;
	int LaysCnt = 0;

	double kBorder = 1;
	double kLays[InpOptions::LaysCntReserv];
	DBL2 kSigma{0.35, 0.35};
	double kInertion = 0;

private:
	void loadAll();
}; // ***************************************************************************************

