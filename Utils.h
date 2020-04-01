#pragma once
#include <Windows.h>
#include <cstdio>

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

