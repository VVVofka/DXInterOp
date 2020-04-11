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
}
//int tst(int a, int b){return a + b;}