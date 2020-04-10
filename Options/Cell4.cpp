#include "pch.h"
#include "Cell4.h"

Cell4::Cell4(PictureBox^ parrent){
	for(int ncell = 0; ncell < 4; ncell++){
		for(int nitem = 0; nitem < 4; nitem++){
			cells[ncell * 4 + nitem] = gcnew Cell();

		}
	}
	int x0 = 1, y0 = 1;
	int x1 = parrent->Width / 2 + 3;
	int y1 = parrent->Height / 2 + 3;
	assert(parrent->Tag != nullptr);
	sval = parrent->Tag->ToString();
	width = parrent->Width;
	heigh = parrent->Height;
	int val = 0;
	for(int j = 0; j < 4; j++)
		if(sval[j] != '0')
			val += 1 << j;
	parrent->Paint += gcnew PaintEventHandler(this, &Cell4::cell4_Paint);
	parrent->MouseDown += gcnew MouseEventHandler(this, &Cell4::cell4_MouseDown);
} // ///////////////////////////////////////////////////////////////////////
System::Void Cell4::cell4_Paint(System::Object^ sender, Forms::PaintEventArgs^ e){
	int shift = 1;
	Graphics^ g = e->Graphics;
	SolidBrush^ blueBrush = gcnew Drawing::SolidBrush(Color::LightSteelBlue); // Brushes::LightSteelBlue
	SolidBrush^ whiteBrush = gcnew Drawing::SolidBrush(Color::GhostWhite); // Brushes::LightSteelBlue
	Pen^ greyPen = gcnew Drawing::Pen(Color::DarkGray, 1); // Brushes::LightSteelBlue
	for(int j = 0; j < 4; j++){
		int x0 = (j & 1) * width / 2 + shift;
		int y0 = (j / 2) * heigh / 2 + shift;
		int w = width / 2 - 2 * shift;
		int h = heigh / 2 - 2 * shift;
		if(sval[j] != '0'){
			g->FillRectangle(blueBrush, x0, y0, w, h);
		} else
			g->FillRectangle(whiteBrush, x0, y0, w, h);
		g->DrawRectangle(greyPen, x0, y0, w, h);
		cells[j * 4 + 0]->drawArrow(g, x0 + 1, y0 + 1, w / 2 - 1, h / 2 - 1);
		cells[j * 4 + 1]->drawArrow(g, x0 + 1 + w / 2, y0 + 1, w / 2 - 1, h / 2 - 1);
		cells[j * 4 + 2]->drawArrow(g, x0 + 1, y0 + 1 + h / 2, w / 2 - 1, h / 2 - 1);
		cells[j * 4 + 3]->drawArrow(g, x0 + 1 + w / 2, y0 + 1 + h / 2, w / 2 - 1, h / 2 - 1);
	}
	return System::Void();

	// Draw a string on the PictureBox.
	//g->DrawString("This is a diagonal line drawn on the control",
	//	fnt, Brushes::Blue, new Point(30, 30));
	//// Draw a line in the PictureBox.
	//g->DrawLine(System.Drawing.Pens.Red, pictureBox1.Left, pictureBox1.Top,
	//	pictureBox1.Right, pictureBox1.Bottom);
} // //////////////////////////////////////////////////////////////////////////////////
System::Void Cell4::cell4_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e){
	const int x = e->X, y = e->Y;
	const int nxcell = (2 * x) / width;
	const int nycell = (2 * y) / heigh;
	const int ncell = 2 * nycell + nxcell;

	const int nxitem = (4 * x) / width;
	const int nyitem = (4 * y) / heigh;
	const int nitem = 2 * (nyitem % 2) + (nxitem % 2);

	const int shift = e->Button == MouseButtons::Left ? -1 : 1;
	cells[ncell * 4 + nitem]->nextDir(shift);
	PictureBox^ pb = (PictureBox^)sender;
	pb->Refresh();
	return System::Void();
} // //////////////////////////////////////////////////////////////////////////////////////////////