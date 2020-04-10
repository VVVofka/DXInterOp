#pragma once
#include <cassert>
#include "Cell.h"
#include "Types.h"

using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Drawing;

ref class Cell4{
public:
	Cell4(PictureBox^ parrent);    // write to parrent.tag nmask ex:"0110"!
	array<Cell^>^ cells = gcnew array<Cell^>(16);
private:
	String^ sval;
	int width, heigh;
	System::Void cell4_Paint(System::Object^ sender, Forms::PaintEventArgs^ e);
	System::Void cell4_MouseDown(System::Object^ sender, Forms::MouseEventArgs^ e);
}; // ***************************************************************************

