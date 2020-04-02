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
	void setVal(int Val){
		assert(Val == 0 || Val == 1 || Val == -1);
		val = Val;
	} // //////////////////////////////////////////////////////////////
	std::string dump(int startpos){
		std::string ret;
		ret = dirs[startpos + 0].dump() + " " + dirs[startpos + 1].dump();
		return ret;
	} // /////////////////////////////////////////////////////////////////
	std::string dumpx(std::string delimet = ", "){
		std::string ret = dirs[0].dumpx() + delimet;
		ret += dirs[1].dumpx() + delimet;
		ret += dirs[2].dumpx() + delimet;
		return ret + dirs[3].dumpx();
	} // /////////////////////////////////////////////////////////////////
	std::string dumpy(std::string delimet = ", "){
		std::string ret = dirs[0].dumpy() + delimet;
		ret += dirs[1].dumpy() + delimet;
		ret += dirs[2].dumpy() + delimet;
		return ret + dirs[3].dumpy();
	} // /////////////////////////////////////////////////////////////////
	std::string Dump(std::string inp){
		std::string ret = inp;
		ret += dirs[0].dump() + " " + dirs[1].dump() + "\n";
		ret += dirs[2].dump() + " " + dirs[3].dump() + "\n";
		_RPT0(0, ret.c_str());
		return ret;
	} // /////////////////////////////////////////////////////////////////
	void setDir(int nItem, int dirX, int dirY){
		assert(nItem < 4);
		if(nItem < 4)
			dirs[nItem].set(dirX, dirY);
	} // /////////////////////////////////////////////////////////////
	Cell2D reflectHor(){
		Cell2D ret;
		ret.setVal(val);
		ret.dirs[1] = dirs[0].reflectHor();
		ret.dirs[0] = dirs[1].reflectHor();
		ret.dirs[3] = dirs[2].reflectHor();
		ret.dirs[2] = dirs[3].reflectHor();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Cell2D reflectVer(){
		Cell2D ret;
		ret.setVal(val);
		ret.dirs[2] = dirs[0].reflectVer();
		ret.dirs[3] = dirs[1].reflectVer();
		ret.dirs[0] = dirs[2].reflectVer();
		ret.dirs[1] = dirs[3].reflectVer();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Cell2D reflectDiag1(){ // axe Top,Left -> Bottom,Right
		Cell2D ret;
		ret.setVal(val);
		ret.dirs[0] = dirs[0].reflectDiag1();
		ret.dirs[2] = dirs[1].reflectDiag1();
		ret.dirs[1] = dirs[2].reflectDiag1();
		ret.dirs[3] = dirs[3].reflectDiag1();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Cell2D reflectDiag2(){ // axe Top,Right -> Bottom,Left
		Cell2D ret;
		ret.setVal(val);
		assert(ret.val >= 0);
		ret.dirs[3] = dirs[0].reflectDiag2();
		ret.dirs[1] = dirs[1].reflectDiag2();
		ret.dirs[2] = dirs[2].reflectDiag2();
		ret.dirs[0] = dirs[3].reflectDiag2();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
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
	std::string dump(std::string ret, std::string delim = " "){
		ret += v[0].dump(0) + delim + v[1].dump(0) + "\n";
		ret += v[0].dump(2) + delim + v[1].dump(2) + "\n\n";
		ret += v[2].dump(0) + delim + v[3].dump(0) + "\n";
		ret += v[2].dump(2) + delim + v[3].dump(2) + "\n";
		return ret;
	} // ////////////////////////////////////////////////////////////////////////////////
	std::string dumpx(std::string ret, std::string delim = ", "){
		ret += v[0].dumpx(delim) + delim;
		ret += v[1].dumpx(delim) + delim;
		ret += v[2].dumpx(delim) + delim;
		ret += v[3].dumpx(delim);
		return ret;
	} // ////////////////////////////////////////////////////////////////////////////////
	std::string dumpy(std::string ret, std::string delim = ", "){
		ret += v[0].dumpy(delim) + delim;
		ret += v[1].dumpy(delim) + delim;
		ret += v[2].dumpy(delim) + delim;
		ret += v[3].dumpy(delim);
		return ret;
	} // ////////////////////////////////////////////////////////////////////////////////
	void Dump(std::string ret){
		ret += std::to_string(A()) + "\n";
		_RPT0(0, dump(ret).c_str());
	} // ////////////////////////////////////////////////////////////////////////////////
	int A(){
		int ret = 0;
		for(int j = 0; j < 4; j++){
			if(v[j].val != 0)
				ret += (1 << j);
		}
		assert(ret < 16);
		return ret;
	} // /////////////////////////////////////////////////////////////////////////////
	void setDir(int nCell, int nItem, int dirX, int dirY){
		assert(nCell < 4);
		if(nCell < 4)
			v[nCell].setDir(nItem, dirX, dirY);
	} // /////////////////////////////////////////////////////////////////////////
	Block2D2 reflectHor(){
		Block2D2 ret;
		ret.v[0] = v[1].reflectHor(); ret.v[0].setVal(v[1].val);
		ret.v[1] = v[0].reflectHor(); ret.v[1].setVal(v[0].val);
		ret.v[2] = v[3].reflectHor(); ret.v[2].setVal(v[3].val);
		ret.v[3] = v[2].reflectHor(); ret.v[3].setVal(v[2].val);
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Block2D2 reflectVer(){
		Block2D2 ret;
		ret.v[0] = v[2].reflectVer();  ret.v[0].setVal(v[2].val);
		ret.v[1] = v[3].reflectVer();  ret.v[1].setVal(v[3].val);
		ret.v[2] = v[0].reflectVer();  ret.v[2].setVal(v[0].val);
		ret.v[3] = v[1].reflectVer();  ret.v[3].setVal(v[1].val);
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Block2D2 reflectDiag1(){ // axe Top,Left -> Bottom,Right
		Block2D2 ret;
		ret.v[0] = v[0].reflectDiag1();  ret.v[0].setVal(v[0].val);
		ret.v[1] = v[2].reflectDiag1();  ret.v[1].setVal(v[2].val);
		ret.v[2] = v[1].reflectDiag1();  ret.v[2].setVal(v[1].val);
		ret.v[3] = v[3].reflectDiag1();  ret.v[3].setVal(v[3].val);
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Block2D2 reflectDiag2(){ // axe Top,Right -> Bottom,Left
		Block2D2 ret;
		ret.v[0] = v[3].reflectDiag2();  ret.v[0].setVal(v[3].val);
		ret.v[1] = v[1].reflectDiag2();  ret.v[1].setVal(v[1].val);
		ret.v[2] = v[2].reflectDiag2();  ret.v[2].setVal(v[2].val);
		ret.v[3] = v[0].reflectDiag2();  ret.v[3].setVal(v[0].val);
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
}; // **************************************************************************

struct Blocks2D2{
	FLT2 vin[16 * 16];
	Block2D2 v[16] = {};
	Blocks2D2(){  // x, y !!!
		Add(0b0000,
			-1, -1, 0, -1, 0, -1, 1, -1,  // 0 0
			-1, 0, -1, -1, 1, -1, 1, 0,

			-1, 0, -1, 1, 1, 1, 1, 0,  // 0 0
			-1, 1, 0, 1, 0, 1, 1, 1
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
			 1, 1, 1, -1, 0, 0, 0, 0,   // 0 1
			-1, 1, 1, 1, 0, 0, 0, 0,

			 0, 0, 0, 0, -1, -1, 1, -1,   // 1 0
			 0, 0, 0, 0, -1, 1, -1, -1
		);
		Add(0b0111,
			 0, 0, 0, 0, 0, 0, 0, 0,   // 1 1
			 0, 0, 0, 0, 0, 0, 0, 0,

			 0, 0, 0, 0, 1, 1, 1, -1,   // 1 0
			 0, 0, 0, 0, -1, 1, 1, 1
		);
		Add(0b1111,
			-1, -1, 0, -1, 0, -1, 1, -1,  // 1 1
			-1, 0, -1, -1, 1, -1, 1, 0,

			-1, 0, -1, 1, 1, 1, 1, 0,  // 1 1
			-1, 1, 0, 1, 0, 1, 1, 1
		);

		/* Formatted
	Add( 0b0000,
		-1,-1,  0,-1,   0,-1,  1,-1,  // 0 0
		-1, 0, -1,-1,   1,-1,  1, 0,

		-1, 0, -1, 1,   1, 1,  1, 0,  // 0 0
		-1, 1,  0, 1,   0, 1,  1, 1
		);
	Add( 0b0001,
		 0, 0,  0, 0,   0, 1, -1, 1,  // 1 0
		 0, 0,  0, 0,   0, 1, -1, 1,

		 1, 0,  1, 0,   0, 0, -1, 0,  // 0 0
		 1,-1,  1,-1,   0,-1, -1,-1
		);
	Add( 0b0011,
		 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1
		 0, 0,  0, 0,   0, 0,  0, 0,

		-1,-1, -1, 0,   1, 0,  1,-1,   // 0 0
		-1,-1, -1,-1,   1,-1,  1,-1
		);
	Add( 0b0110,
		 1, 1,  1,-1,   0, 0,  0, 0,   // 0 1
		-1, 1,  1, 1,   0, 0,  0, 0,

		 0, 0,  0, 0,  -1,-1,  1,-1,   // 1 0
		 0, 0,  0, 0,  -1, 1, -1,-1
		);
	Add( 0b0111,
		 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1
		 0, 0,  0, 0,   0, 0,  0, 0,

		 0, 0,  0, 0,   1, 1,  1,-1,   // 1 0
		 0, 0,  0, 0,  -1, 1,  1, 1
		);
	Add( 0b1111,
		-1,-1,  0,-1,   0,-1,  1,-1,  // 1 1
		-1, 0, -1,-1,   1,-1,  1, 0,

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
	){
		v[index] = Block2D2((index >> 3) & 1, (index >> 2) & 1, (index >> 1) & 1, index & 1);
		int nItem = 0, nCell = 0;
		v[index].setDir(nCell, nItem++, dirx00, diry00);
		v[index].setDir(nCell, nItem++, dirx01, diry01);
		v[index].setDir(nCell, nItem++, dirx02, diry02);
		v[index].setDir(nCell++, nItem, dirx03, diry03);
		nItem = 0;
		v[index].setDir(nCell, nItem++, dirx10, diry10);
		v[index].setDir(nCell, nItem++, dirx11, diry11);
		v[index].setDir(nCell, nItem++, dirx12, diry12);
		v[index].setDir(nCell++, nItem, dirx13, diry13);
		nItem = 0;
		v[index].setDir(nCell, nItem++, dirx20, diry20);
		v[index].setDir(nCell, nItem++, dirx21, diry21);
		v[index].setDir(nCell, nItem++, dirx22, diry22);
		v[index].setDir(nCell++, nItem, dirx23, diry23);
		nItem = 0;
		v[index].setDir(nCell, nItem++, dirx30, diry30);
		v[index].setDir(nCell, nItem++, dirx31, diry31);
		v[index].setDir(nCell, nItem++, dirx32, diry32);
		v[index].setDir(nCell++, nItem, dirx33, diry33);
		//v[index].Dump("Main:");

		Block2D2 hor = v[index].reflectHor();
		Block2D2 ver = v[index].reflectVer();
		Block2D2 dgl1 = v[index].reflectDiag1();
		Block2D2 dgl2 = v[index].reflectDiag2();

		Block2D2 vmodifs[7] = {hor, ver, dgl1, dgl2,
				   hor.reflectDiag2(), ver.reflectDiag2(), dgl1.reflectDiag2()};

		for(int m = 0; m < 7; m++){
			int a = vmodifs[m].A();
			v[a] = vmodifs[m];
			//v[a].Dump(" add \tm:" + std::to_string(m) + " \ta:");
		}
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	std::string dump(){
		std::string ret;
		for(int j = 0; j < 16; j++){
			ret += "\n" + v[j].dump(std::to_string(j) + ":\n");
		}
		return ret;
	} // ////////////////////////////////////////////////////////////////////////////////
	void dumpx(std::string delim = ","){
		for(int j = 0; j < 16; j++){
			std::string s = v[j].dumpx("", delim) + (j == 15 ? std::string("\n") : delim);
			_RPT2(0, "\n%s", s.c_str());
		}
	} // ////////////////////////////////////////////////////////////////////////////////
	void dumpy(std::string delim = ","){
		for(int j = 0; j < 16; j++){
			std::string s = v[j].dumpy("", delim) + (j == 15 ? std::string("\n") : delim);
			_RPT2(0, "\n%s", s.c_str());
		}
	} // ////////////////////////////////////////////////////////////////////////////////
}; // ***************************************************************************
