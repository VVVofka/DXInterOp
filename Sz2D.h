#pragma once
class Sz2D{
public:
	int x=0, y=0;
	void operator *=(int mult){ x *= mult; y *= mult; }
	size_t area(){ return (x + 1) * (y + 1); }
}; // ***************************************************************

