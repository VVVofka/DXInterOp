#pragma once
#include <iostream>
struct FLT2{
	float y;
	float x;
	FLT2() : y(0), x(0){}
	FLT2(float Y, float X) : x(X), y(Y){}
	FLT2(float YX) : x(YX), y(YX){}
	void operator +=(FLT2& other){ x += other.x; y += other.y; }
	bool not0(){ return x != 0 || y != 0; }
	void dump(){
		std::cout << "y:" << y << " x:" << x;
	}
}; // ********************************************************************************************

float normal(int pos, int width) restrict(amp, cpu);

