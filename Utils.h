#pragma once
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <amp.h>

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
	INT2() : x(0), y(0) {}
	INT2(int Y, int X) : x(X), y(Y){}
	//bool not0(){ return x != 0 || y != 0; }
	void operator *=(int mult){ y *= mult; x *= mult; }

	//Sz2D operator * (int mult){ return Sz2D(y*mult, x*mult); }

	//size_t Area(){ return (y + 1) * (x + 1); }
	int Max(){ return __max(y, x); }

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

