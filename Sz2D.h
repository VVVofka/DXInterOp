#pragma once
class Sz2D{
public:
	int y;
	int x;
	Sz2D(){ y = x = 0; }
	Sz2D(int Y, int X){ y = Y; x = X; }
	void operator *=(int mult){ y *= mult; x *= mult; }
	//Sz2D operator * (int mult){ return Sz2D(y*mult, x*mult); }
	size_t area(){ return (y + 1) * (x + 1); }
	int Max(){return __max(y, x);}
}; // ***************************************************************

