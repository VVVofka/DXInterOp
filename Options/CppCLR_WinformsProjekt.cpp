#include "pch.h"
using namespace System;
//int main(array<System::String ^> ^args)
//{
//    return 0;
//}
#include "Form1.h"
using namespace System::Windows::Forms;

[STAThread]
int main(){
	static bool first = true;
	Application::EnableVisualStyles();
	if(first)
		first = false, Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CppCLRWinformsProjekt::Form1());
	return 0;
} // /////////////////////////////////////////////////////////////////////////////////////////////
// 111
// 222
int openDlgOptions(int* dirs){
	static bool first = true;
	Application::EnableVisualStyles();
	if(first)
		first = false, Application::SetCompatibleTextRenderingDefault(false);
	auto frm = gcnew CppCLRWinformsProjekt::Form1();
	for(int a = 0, n = 0; a < 16; a++)
		for(int ncell = 0; ncell < 4; ncell++)
			for(int nitem = 0; nitem < 4; nitem++)
				frm->setDir(a, ncell, nitem, dirs[n++]);
	Application::Run(frm);
	for(int a = 0, n = 0; a < 16; a++)
		for(int ncell = 0; ncell < 4; ncell++)
			for(int nitem = 0; nitem < 4; nitem++)
				dirs[n++] = frm->getDir(a, ncell, nitem);
	return 0;
} // //////////////////////////////////////////////////////////////////////////////////////////////