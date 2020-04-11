#pragma once
#include <cassert>
namespace CppCLRWinformsProjekt{
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace CppCLRWinformsProjekt;

private ref class Cell{
	int dir;
public:
	Cell();
	void setDir(int Dir);
	int getDir(){return dir;}
	void nextDir(int shiftDir);
	void setRotate(Cell^ src);
	void drawArrow(Graphics^ g, int x0, int y0, int width, int heigh) ;
}; // *************************************************************************************
}
