#include "pch.h"
using namespace System;
//int main(array<System::String ^> ^args)
//{
//    return 0;
//}
#include "Form1.h"

using namespace System::Windows::Forms;
using namespace CppCLRWinformsProjekt;

//int* dirout;
//[STAThread]
//int main(){ // int* dirs
//	Application::EnableVisualStyles();
//	Application::SetCompatibleTextRenderingDefault(false);
//	auto frm = gcnew CppCLRWinformsProjekt::Form1();
//	Application::Run(frm);
//	return 0;
//	int* inp = dirout;
//	for(int a = 0; a < 16; a++)
//		for(int c = 0; c < 4; c++)
//			for(int i = 0; i < 4; i++){
//				frm->setDir(a, c, i, *inp);
//				inp++;
//			}
//	//Application::Run(frm);
//	int* out = dirout;
//	for(int a = 0; a < 16; a++)
//		for(int c = 0; c < 4; c++)
//			for(int i = 0; i < 4; i++){
//				*out = frm->getDir(a, c, i);
//				out++;
//			}
//
//	return 0;
//} // ////////////////////////////////////////////////////////////////////////////////////////
[STAThread]
int tst(int* dirs){
	//dirout = dirs;
	//int ret = main();
	Application::EnableVisualStyles();
	//if(dirs[0] == 1){
	Application::SetCompatibleTextRenderingDefault(false);
	auto frm = gcnew CppCLRWinformsProjekt::Form1();
	Application::Run(gcnew CppCLRWinformsProjekt::Form1());
	//}
//$(TargetPath)
	return 3;
} // ////////////////////////////////////////////////////////////////////////////////////////
[STAThread]
int main(){ // int* dirs
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CppCLRWinformsProjekt::Form1());

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CppCLRWinformsProjekt::Form1());

	return 0;
} // //////////////////////////////////////////////////////////////////////////////////