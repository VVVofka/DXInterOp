#pragma once
#include <cassert>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

ref class Cell{
	int dir;
public:
	Cell();
	void setDir(int Dir);
	void nextDir(int shiftDir);
	void drawArrow(Graphics^ g, int x0, int y0, int width, int heigh) ;
}; // *************************************************************************************

