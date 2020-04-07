#pragma once
#include <cassert>
#include <string>
#include "Utils.h"
struct CellItem{
	int diry;
	int dirx;
	int eny;
	int enx;
	CellItem(int dirX = -2, int dirY = -2){
		if(dirX == -1)
			dirx = 0, enx = 1;
		else if(dirX == 0)
			dirx = 0, enx = 0;
		else if(dirX == 1)
			dirx = 1, enx = 1;
		else
			dirx = -1, enx = -1;

		if(dirY == -1)
			diry = 0, eny = 1;
		else if(dirY == 0)
			diry = 0, eny = 0;
		else if(dirY == 1)
			diry = 1, eny = 1;
		else
			diry = -1, eny = -1;
	} // /////////////////////////////////////////////////////////////////
	void set(int dirX, int dirY){
		assert(dirX >= -1 && dirX <= 1);
		if(dirX < 0){
			dirx = 0, enx = 1;
		} else if(dirX > 0){
			dirx = 1, enx = 1;
		} else{
			dirx = 0, enx = 0;
		}

		assert(dirY >= -1 && dirY <= 1);
		if(dirY < 0){
			diry = 0, eny = 1;
		} else if(dirY > 0){
			diry = 1, eny = 1;
		} else{
			diry = 0, eny = 0;
		}
	} // /////////////////////////////////////////////////////////
	std::string dumpx(){ return dump(enx, dirx); }
	std::string dumpy(){ return dump(eny, diry); }
	std::string dump(){ return dumpx() + dumpy(); }
	FLT2 getFLT2(){ return FLT2(getfloat(eny, diry), getfloat(enx, dirx)); }
	// ///////////////////////////////////////////////////////////
	std::string dump(int en, int dir){
		if(en == 1)
			return dir == 0 ? "-1" : "+1";
		return dir == 0 ? "-0" : "+0";
	} // //////////////////////////////////////////////////////////////////
	float getfloat(int en, int dir){
		if(en == 1)
			return dir == 0 ? -1.f : +1.f;
		return 0.f;
	} // //////////////////////////////////////////////////////////////////
	CellItem reflectHor(){
		int x = b2i(enx, dirx);
		int y = b2i(eny, diry);
		return CellItem(-x, y);
	} // //////////////////////////////////////////////////////////////////////////////
	CellItem reflectVer(){
		int x = b2i(enx, dirx);
		int y = b2i(eny, diry);
		return CellItem(x, -y);
	} // //////////////////////////////////////////////////////////////////////////////
	CellItem reflectDiag1(){ // axe Top,Left -> Bottom,Right
		int x = b2i(enx, dirx);
		int y = b2i(eny, diry);
		return CellItem(y, x);
	} // //////////////////////////////////////////////////////////////////////////////
	CellItem reflectDiag2(){ // axe Top,Right -> Bottom,Left
		int x = b2i(enx, dirx);
		int y = b2i(eny, diry);
		return CellItem(-y, -x);
	} // //////////////////////////////////////////////////////////////////////////////
private:
	static int b2i(int en, int dir){
		assert((en == 0 || en == 1) && (dir == 0 || dir == 1));
		if(en == 0) return 0;
		if(dir == 0) return -1;
		return 1;
	} // ///////////////////////////////////////////////////////////////////////////////

}; // *************************************************************
struct Cell2D{
	int val = -1;
	CellItem dirs[4] = {};
	void setVal(int Val);
	std::string dump(int startpos);
	std::string dumpx(std::string delimet = ", ");
	std::string dumpy(std::string delimet = ", ");
	std::string Dump(std::string inp);
	void setDir(int nItem, int dirX, int dirY);
	Cell2D reflectHor();
	Cell2D reflectVer();
	Cell2D reflectDiag1();
	Cell2D reflectDiag2();
}; // ***********************************************************************

// 01
// 23
// 3210
// dirY dirX enY enX
struct Block2D2{
	Cell2D v[4] = {};
	Block2D2(int val3 = -1, int val2 = -1, int val1 = -1, int val0 = -1){
		v[0].setVal(val0);
		v[1].setVal(val1);
		v[2].setVal(val2);
		v[3].setVal(val3);
	} // /////////////////////////////////////////////////////////////////////
	std::string dump(std::string ret, std::string delim = " ");
	std::string dumpx(std::string ret, std::string delim = ", ");
	std::string dumpy(std::string ret, std::string delim = ", ");
	void Dump(std::string ret);
	void setDir(int nCell, int nItem, int dirX, int dirY);
	int A();
	Block2D2 reflectHor();
	Block2D2 reflectVer();
	Block2D2 reflectDiag1();
	Block2D2 reflectDiag2();
private:
}; // **************************************************************************

struct Blocks2D2{
	FLT2 vin[16 * 16];
	Block2D2 v[16] = {};
	Blocks2D2(){  // x, y !!!
		Add(0b0000,
			-1,-1,  0,-1,   0,-1,  1,-1,
			-1, 0, -1,-1,   1,-1,  1, 0,  // 0 0

			-1, 0, -1, 1,   1, 1,  1, 0,  // 0 0
			-1, 1,  0, 1,   0, 1,  1, 1
		);
		Add(0b0001,
			 0, 0,  0, 0,   0, 1, -1, 1,
			 0, 0,  0, 0,   0, 0, -1, 1,  // 1 0

			 1, 0,  0, 0,   0, 0, -1, 0,  // 0 0
			 1,-1,  1,-1,   0,-1, -1,-1
		);
		Add(0b0011,
			-1, 0, -1, 0,   1, 0,  1, 0,
			-1, 0, -1, 0,   1, 0,  1, 0,   // 1 1

			-1,-1, -1, 0,   1, 0,  1,-1,   // 0 0
			-1,-1, -1,-1,   1,-1,  1,-1
/*
			 0, 0,  0, 0,   0, 0,  0, 0,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1

			-1, 0, -1, 0,   1, 0,  1, 0,   // 0 0
			-1,-1, -1,-1,   1,-1,  1,-1
*/
		);
Add(0b0110,
			 1, 1,  1,-1,   1, 0,  1,-1,
			-1, 1,  1, 1,   1,-1,  0,-1,   // 0 1

			 0, 1, -1, 1,  -1,-1,  1,-1,   // 1 0
			-1, 1, -1, 0,  -1, 1, -1,-1
/*
			 1, 1,  1,-1,   0, 0,  0, 0,
			-1, 1,  1, 1,   0, 0,  0, 0,   // 0 1

			 0, 0,  0, 0,  -1,-1,  1,-1,   // 1 0
			 0, 0,  0, 0,  -1, 1, -1,-1
*/
		);
		Add(0b0111,
			 0, 0,  0, 0,   0, 0,  0, 0,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1

			 0, 0,  0, 0,   1, 1,  1,-1,   // 1 0
			 0, 0,  0, 0,  -1, 1,  1, 1
		);
		Add(0b1111,
			-1,-1,  0,-1,   0,-1,  1,-1,
			-1, 0, -1,-1,   1,-1,  1, 0,  // 1 1

			-1, 0, -1, 1,   1, 1,  1, 0,  // 1 1
			-1, 1,  0, 1,   0, 1,  1, 1
		);
/*
	Blocks2D2(){  // x, y !!!
		Add(0b0000,
			-1,-1,  0,-1,   0,-1,  1,-1,
			-1, 0, -1,-1,   1,-1,  1, 0,  // 0 0

			-1, 0, -1, 1,   1, 1,  1, 0,  // 0 0
			-1, 1,  0, 1,   0, 1,  1, 1
		);
		Add(0b0001,
			 0, 0,  0, 0,   0, 1, -1, 1,
			 0, 0,  0, 0,   0, 0, -1, 1,  // 1 0

			 1, 0,  0, 0,   0, 0, -1, 0,  // 0 0
			 1,-1,  1,-1,   0,-1, -1,-1
		);
		Add(0b0011,
			 0, 0,  0, 0,   0, 0,  0, 0,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1

			-1, 0, -1, 0,   1, 0,  1, 0,   // 0 0
			-1,-1, -1,-1,   1,-1,  1,-1
		);
		Add(0b0110,
			 1, 1,  1,-1,   0, 0,  0, 0,
			-1, 1,  1, 1,   0, 0,  0, 0,   // 0 1

			 0, 0,  0, 0,  -1,-1,  1,-1,   // 1 0
			 0, 0,  0, 0,  -1, 1, -1,-1
		);
		Add(0b0111,
			 0, 0,  0, 0,   0, 0,  0, 0,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1

			 0, 0,  0, 0,   1, 1,  1,-1,   // 1 0
			 0, 0,  0, 0,  -1, 1,  1, 1
		);
		Add(0b1111,
			-1,-1,  0,-1,   0,-1,  1,-1,
			-1, 0, -1,-1,   1,-1,  1, 0,  // 1 1

			-1, 0, -1, 1,   1, 1,  1, 0,  // 1 1
			-1, 1,  0, 1,   0, 1,  1, 1
		);
*/
		for(int n = 0, j = 0; n < 16; n++)
			for(int ncell = 0; ncell < 4; ncell++)
				for(int nitem = 0; nitem < 4; nitem++)
					vin[j++] = v[n].v[ncell].dirs[nitem].getFLT2();
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void Add(int index,
			 int dirx00, int diry00, int dirx01, int diry01, int dirx10, int diry10, int dirx11, int diry11,
			 int dirx02, int diry02, int dirx03, int diry03, int dirx12, int diry12, int dirx13, int diry13,
			 int dirx20, int diry20, int dirx21, int diry21, int dirx30, int diry30, int dirx31, int diry31,
			 int dirx22, int diry22, int dirx23, int diry23, int dirx32, int diry32, int dirx33, int diry33
	); // ///////////////////////////////////////////////////////////////////////////////////////////////
	std::string dump();
	void dumpx(std::string delim = ",");
	void dumpy(std::string delim = ",");
}; // ***************************************************************************
