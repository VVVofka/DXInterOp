#pragma once
#include <Windows.h>
#include <cstdio>
#include <iostream>

void setConsole();
float normal(int pos, int width) restrict(amp, cpu);

class Utils{
}; // *******************************************************************************************
struct FLT2{
	float y;
	float x;
	FLT2() : y(0), x(0){}
	FLT2(float Y, float X) : x(X), y(Y){}
	bool not0(){ return x != 0 || y != 0; }
}; // ********************************************************************************************

struct INT2{
	int y;
	int x;
	INT2(){}
	INT2(int Y, int X) restrict (amp, cpu){y = Y, x = X;}
	INT2 operator * (int mult){ return INT2(y * mult, x * mult); }
	void operator *=(int mult){ y *= mult; x *= mult; }
	int Max(){ return __max(y, x); }
	//bool not0(){ return x != 0 || y != 0; }
}; // ********************************************************************************************

struct DrQuadro{
	FLT2 items[4];
	bool not0(){ return items[0].not0() || items[1].not0() || items[2].not0() || items[3].not0(); }
	void dump(){
		for(int i = 0; i < 4; i++){
			printf("\ti:%d y:%+.1f x:%+.1f", i, items[i].y, items[i].x);
		}
	} // ///////////////////////////////////////////////////////////////////////////////////////
}; // ********************************************************************************************

struct aType{
	int adress = -1;
	int talehead = 0; // 0-tale; 1-head
}; // *******************************************************************************************