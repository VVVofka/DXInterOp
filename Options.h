#pragma once
#include "Options\\OptionExternDecl.h"
#include "Masks.h"
#include <cstdio> 

#define autoDirsFName "autoArr.dat"

class Options{
						// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
						// 00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
						// 00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
	int AMask[16] = {0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};  // line=1 diag=1 ++ (large cave)
  //const int AMask[16] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // point=1
  //const int AMask[16] = {0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1};  // line=1 diag=0 +
  //const int AMask[16] = {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};  // line=0 diag=1 quadro
  //const int AMask[16] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1};  // line=0 diag=0 quadro
public:
	static const int szDirs = Blocks2D2::szFLT;
	Options();

	bool saveAuto() const;
	bool loadAuto();
	FLT2* getFLT2(){ return blocks2D2.vin; }
	int showDlg();
	//bool isRestart(){ return iArr[InpOptions::Restart] == 1; }

	Blocks2D2 blocks2D2;
	static const int sziArr = InpOptions::AMasks + _countof(AMask); // + ...
	static const int szdArr = InpOptions::kLays + InpOptions::LaysCntReserv; // + ...
	int iArr[sziArr];
	double dArr[szdArr];

	int* dirs(){ return &iArr[InpOptions::Dirs]; }
	bool normDir(){ return iArr[InpOptions::NormDir] != 0; } // if DirX < 0.5 * DirY then DirX = 0
	INT2 LaysSzUp(){ return INT2(InpOptions::LaysSzUpY, InpOptions::LaysSzUpX); }
	int LaysSzDn(){ InpOptions::LaysSzDn; }
	int seedRnd(){ return iArr[InpOptions::SeedRnd]; }
	int setLaysCnt(int cnt){ iArr[InpOptions::LaysCnt] = cnt; }
	const int* aMask() const { return &iArr[InpOptions::AMasks]; }

	double kFillRnd(){ return dArr[InpOptions::kFillRnd]; }
	double kSigmaY(){ return dArr[InpOptions::kSigmaY]; }
	double kSigmaX(){ return dArr[InpOptions::kSigmaX]; }
	double kInertion(){ return dArr[InpOptions::kInertion]; }
	double kBorder(){ return dArr[InpOptions::kBorder]; }
	//double* kLays(){ return &dArr[InpOptions::kLays]; }
	double kLays(int nlay){ return dArr[InpOptions::kLays + nlay]; }

	bool setDefault();
	bool save(const char* fname) const;
	bool load(const char* fname);

private:
	void loadAll();
}; // ***************************************************************************************

