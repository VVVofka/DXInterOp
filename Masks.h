#pragma once
#include <cassert>
//#include <vector>
struct CellItem{
	int diry = -1;
	int dirx = -1;
	int eny = -1;
	int enx = -1;
	CellItem(int dirX = -1, int dirY = -1){
		set(dirX, dirY);
	} // /////////////////////////////////////////////////////////////////
	void set(int dirX, int dirY){
		if(dirX < 0){
			dirx = 0, enx = 1;
		} else if(dirx > 0){
			dirx = 1, enx = 1;
		} else{
			dirx = 0, enx = 0;
		}

		if(dirY < 0){
			diry = 0, enx = 1;
		} else if(diry > 0){
			diry = 1, eny = 1;
		} else{
			diry = 0, eny = 0;
		}
	} // /////////////////////////////////////////////////////////
	CellItem reflectHor(){
		return CellItem(-dirx, diry);
	} // //////////////////////////////////////////////////////////////////////////////
	CellItem reflectVer(){
		return CellItem(dirx, -diry);
	} // //////////////////////////////////////////////////////////////////////////////
	CellItem reflectDiag1(){ // axe Top,Left -> Bottom,Right
		return CellItem(diry, dirx);
	} // //////////////////////////////////////////////////////////////////////////////
	CellItem reflectDiag2(){ // axe Top,Right -> Bottom,Left
		return CellItem(-diry, -dirx);
	} // //////////////////////////////////////////////////////////////////////////////
}; // *************************************************************
struct Cell2D{
	int val = -1;
	CellItem dirs[4] = {};
	void setVal(int Val){
		val = Val;
		//for(int j = 0; j < 4; j++)
		//	dirs[j].set(0, 0);
	} // //////////////////////////////////////////////////////////////
	void setDir(int nItem, int dirX, int dirY){
		assert(nItem < 4);
		dirs[nItem].set(dirX, dirY);
	} // /////////////////////////////////////////////////////////////
	Cell2D reflectHor(){
		Cell2D ret;
		ret.setVal(val);
		assert(ret.val >= 0);
		for(int j = 0; j < 4; j++){
			ret.dirs[j] = dirs[j].reflectHor();
			assert(ret.dirs[j].enx == 0 || ret.dirs[j].enx == 1);
			assert(ret.dirs[j].eny == 0 || ret.dirs[j].eny == 1);
			assert(ret.dirs[j].dirx == 0 || ret.dirs[j].dirx == 1);
			assert(ret.dirs[j].diry == 0 || ret.dirs[j].diry == 1);
		}
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Cell2D reflectVer(){
		Cell2D ret;
		ret.setVal(val);
		assert(ret.val >= 0);
		for(int j = 0; j < 4; j++){
			ret.dirs[j] = dirs[j].reflectVer();
			assert(ret.dirs[j].enx == 0 || ret.dirs[j].enx == 1);
			assert(ret.dirs[j].eny == 0 || ret.dirs[j].eny == 1);
			assert(ret.dirs[j].dirx == 0 || ret.dirs[j].dirx == 1);
			assert(ret.dirs[j].diry == 0 || ret.dirs[j].diry == 1);
		}
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Cell2D reflectDiag1(){ // axe Top,Left -> Bottom,Right
		Cell2D ret;
		ret.setVal(val);
		assert(ret.val >= 0);
		for(int j = 0; j < 4; j++){
			ret.dirs[j] = dirs[j].reflectDiag1();
			assert(ret.dirs[j].enx == 0 || ret.dirs[j].enx == 1);
			assert(ret.dirs[j].eny == 0 || ret.dirs[j].eny == 1);
			assert(ret.dirs[j].dirx == 0 || ret.dirs[j].dirx == 1);
			assert(ret.dirs[j].diry == 0 || ret.dirs[j].diry == 1);
		}
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Cell2D reflectDiag2(){ // axe Top,Right -> Bottom,Left
		Cell2D ret;
		ret.setVal(val);
		assert(ret.val >= 0);
		for(int j = 0; j < 4; j++){
			ret.dirs[j] = dirs[j].reflectDiag2();
			assert(ret.dirs[j].enx == 0 || ret.dirs[j].enx == 1);
			assert(ret.dirs[j].eny == 0 || ret.dirs[j].eny == 1);
			assert(ret.dirs[j].dirx == 0 || ret.dirs[j].dirx == 1);
			assert(ret.dirs[j].diry == 0 || ret.dirs[j].diry == 1);
		}
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
}; // ***********************************************************************
// 01
// 23
// 3210
// dirY dirX enY enX
struct Block2D2{
	Cell2D v[4] = {};
	Block2D2(int val0 = -1, int val1 = -1, int val2 = -1, int val3 = -1){
		v[0].setVal(val0);
		v[1].setVal(val1);
		v[2].setVal(val2);
		v[3].setVal(val3);
	} // /////////////////////////////////////////////////////////////////////
	int A(){
		int ret = 0;
		for(int j = 0; j < 4; j++){
			if(v[3 - j].val >= 0) ret |= 1;
			ret <<= 1;
		}
		assert(ret < 16);
		return ret;
	} // /////////////////////////////////////////////////////////////////////////////
	bool isAEqual(const Block2D2* other){
		for(int j = 0; j < 4; j++){
			if(v[j].val != other->v[j].val)
				return false;
		}
		return true;
	} // ///////////////////////////////////////////////////////////////////
	void setVal(int nCell, int Val){
		assert(nCell < 4);
		v[nCell].setVal(Val);
	} // /////////////////////////////////////////////////////////////////////////
	void setDir(int nCell, int nItem, int dirX, int dirY){
		assert(nCell < 4);
		assert(nItem < 4);
		v[nCell].setDir(nItem, dirX, dirY);
	} // /////////////////////////////////////////////////////////////////////////
	Block2D2 reflectHor(){
		Block2D2 ret;
		ret.v[0] = v[1].reflectHor();
		ret.v[1] = v[0].reflectHor();
		ret.v[2] = v[3].reflectHor();
		ret.v[3] = v[2].reflectHor();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Block2D2 reflectVer(){
		Block2D2 ret;
		ret.v[0] = v[2].reflectVer();
		ret.v[1] = v[3].reflectVer();
		ret.v[2] = v[0].reflectVer();
		ret.v[3] = v[1].reflectVer();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Block2D2 reflectDiag1(){ // axe Top,Left -> Bottom,Right
		Block2D2 ret;
		ret.v[0] = v[0].reflectDiag1();
		ret.v[1] = v[2].reflectDiag1();
		ret.v[2] = v[1].reflectDiag1();
		ret.v[3] = v[3].reflectDiag1();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
	Block2D2 reflectDiag2(){ // axe Top,Right -> Bottom,Left
		Block2D2 ret;
		ret.v[0] = v[3].reflectDiag1();
		ret.v[1] = v[1].reflectDiag1();
		ret.v[2] = v[2].reflectDiag1();
		ret.v[3] = v[0].reflectDiag1();
		return ret;
	} // //////////////////////////////////////////////////////////////////////////////
}; // **************************************************************************
struct Blocks2D2{
	Block2D2 v[16] = {};
	void Add(int dirx00, int diry00, int dirx01, int diry01, int dirx10, int diry10, int dirx11, int diry11,
			 int dirx02, int diry02, int dirx03, int diry03, int dirx12, int diry12, int dirx13, int diry13,
			 int dirx20, int diry20, int dirx21, int diry21, int dirx30, int diry30, int dirx31, int diry31,
			 int dirx22, int diry22, int dirx23, int diry23, int dirx32, int diry32, int dirx33, int diry33,
			 int index
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
		Block2D2 vmodifs[7] = {v[index].reflectHor(), v[index].reflectVer(), v[index].reflectDiag1(), v[index].reflectDiag2(),
		v[index].reflectHor().reflectDiag2(), v[index].reflectVer().reflectDiag2(), v[index].reflectDiag1().reflectDiag2()};
		for(int j = 0; j < 16; j++){
			if(j == index) continue;
			for(int m = 0; m < 7; m++){
				int a = vmodifs[m].A();
				if(a == j)
					v[a] = vmodifs[m];
			}
		}
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	Blocks2D2(){
		Add( 0b0000,
			-1,-1,  0,-1,   0,-1,  1,-1,  // 0 0
			-1, 0, -1,-1,   1,-1,  1, 0,  // 0 0

			-1, 0, -1, 1,   1, 1,  1, 0,
			-1, 1,  0, 1,   0, 1,  1, 1
			);
		Add( 0b0001,
			 0, 0,  0, 0,   0, 1, -1, 1,  // 1 0
			 0, 0,  0, 0,   0, 1, -1, 1,  // 0 0

			 1, 0,  1, 0,   0, 0, -1, 0,
			 1,-1,  1,-1,   0,-1, -1,-1
			);
		Add( 0b0011,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1
			 0, 0,  0, 0,   0, 0,  0, 0,   // 0 0

			-1,-1, -1, 0,   1, 0,  1,-1,
			-1,-1, -1,-1,   1,-1,  1,-1
			);
		Add( 0b0110,
			 1, 1,  1,-1,   0, 0,  0, 0,   // 0 1
			-1, 1,  1, 1,   0, 0,  0, 0,   // 1 0

			 0, 0,  0, 0,  -1,-1,  1,-1,
			 0, 0,  0, 0,  -1, 1, -1,-1
			);
		Add( 0b0111,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 0

			 0, 0,  0, 0,   1, 1,  1,-1,
			 0, 0,  0, 0,  -1, 1,  1, 1
			);
		Add( 0b1111,
			-1,-1,  0,-1,   0,-1,  1,-1,  // 1 1
			-1, 0, -1,-1,   1,-1,  1, 0,  // 1 1

			-1, 0, -1, 1,   1, 1,  1, 0,
			-1, 1,  0, 1,   0, 1,  1, 1
			);
			/*
		Add( 0b0000, 
		    -1,-1,  0,-1,   0,-1,  1,-1,  // 0 0
			-1, 0, -1,-1,   1,-1,  1, 0,  // 0 0
			
			-1, 0, -1, 1,   1, 1,  1, 0,
			-1, 1,  0, 1,   0, 1,  1, 1,
			);
		Add( 0b0001,
			 0, 0,  0, 0,   0, 1, -1, 1,  // 1 0
			 0, 0,  0, 0,   0, 1, -1, 1,  // 0 0
			 
			 1, 0,  1, 0,   0, 0, -1, 0,
			 1,-1,  1,-1,   0,-1, -1,-1,
			);
		Add( 0b0011,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1
			 0, 0,  0, 0,   0, 0,  0, 0,   // 0 0

			-1,-1, -1, 0,   1, 0,  1,-1,
			-1,-1, -1,-1,   1,-1,  1,-1,
			);
		Add( 0b0110,
			 1, 1,  1,-1,   0, 0,  0, 0,   // 0 1
			-1, 1,  1, 1,   0, 0,  0, 0,   // 1 0

			 0, 0,  0, 0,  -1,-1,  1,-1,
			 0, 0,  0, 0,  -1, 1, -1,-1,
			);
		Add( 0b0111,
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 1
			 0, 0,  0, 0,   0, 0,  0, 0,   // 1 0

			 0, 0,  0, 0,   1, 1,  1,-1,
			 0, 0,  0, 0,  -1, 1,  1, 1,
			);
		Add( 0b1111,
			-1,-1,  0,-1,   0,-1,  1,-1,  // 1 1
			-1, 0, -1,-1,   1,-1,  1, 0,  // 1 1

			-1, 0, -1, 1,   1, 1,  1, 0,
			-1, 1,  0, 1,   0, 1,  1, 1,
			);

			*/
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
}; // ***************************************************************************
