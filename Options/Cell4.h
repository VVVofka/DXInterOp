#pragma once
#include <cassert>
#include "Cell.h"
#include "Types.h"
#include <string>
namespace CppCLRWinformsProjekt{
using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Configuration;

private ref class Cell4{
	array<Cell^>^ cells = gcnew array<Cell^>(16);
public:
	Cell4(PictureBox^ parrent);    // write to parrent.tag nmask ex:"0110"!
	void Rotate(Cell4^ src);
	Cell^ getCell(int index){return cells[index];}
	Cell^ getCell(int ncell, int nitem){return cells[4 * ncell + nitem];}
	void dump();
private:
	String^ sval;
	int width, heigh;
	System::Void cell4_Paint(System::Object^ sender, Forms::PaintEventArgs^ e);
	System::Void cell4_MouseDown(System::Object^ sender, Forms::MouseEventArgs^ e);
}; // ***************************************************************************
}
