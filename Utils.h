#pragma once
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <amp.h>
#include <amp_math.h>
using namespace Concurrency;
using namespace Concurrency::fast_math;

#define NORMAL_TO_AREA(POS, WIDTH) (float(2 * (POS) + 1) / (WIDTH) - 1.f)

void setConsole();
//class Utils{}; // *******************************************************************************************

struct FLT2{
	float y;
	float x;
	FLT2() : y(0), x(0){}
	FLT2(float Y, float X) restrict(amp, cpu) : x(X), y(Y){}
	void set(float Y, float X) restrict(amp, cpu){ y = Y, x = X; }
	bool not0(){ return x != 0 || y != 0; }
	const FLT2& abs() const restrict(amp, cpu){ return FLT2(fabsf(y), fabsf(x)); }
}; // ********************************************************************************************

struct INT2{
	int y;
	int x;
	INT2() restrict(amp, cpu) : y(0), x(0){}
	INT2(int Y, int X) restrict(amp, cpu) : y(Y), x(X){}
	INT2(concurrency::index<2> idx) restrict(amp, cpu) : y(idx[0]), x(idx[1]){}
	//bool not0(){ return x != 0 || y != 0; }
	void operator *=(int mult){ y *= mult; x *= mult; }

	const INT2& operator * (int mult) const restrict(amp, cpu){ return INT2(y * mult, x * mult); }
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

