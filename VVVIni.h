//Для работы с ini файлами в папке программы
//////////////////////////////////////////////////////////////////////
#pragma once
//#include "Vctr.h"
#include "stdafx.h"
class CVVVIni  
{
public:
	CVVVIni();
	virtual ~CVVVIni();

	int Save();
	CString LoadS(char* param_name, char* val_default=NULL);
	CString LoadS(CString* param_name, char* val_default=NULL);
	int LoadI(char* param_name, int val_default=0, char* section = NULL);
	int LoadI(CString* param_name, int val_default=0, char* section = NULL);
	int LoadI(CString param_name, int val_default=0, char* section = NULL);
	double LoadD(char* param_name, double val_default=0.0);
	double LoadD(CString* param_name, double val_default=0.0);
	double LoadD(CString param_name, double val_default=0.0);
	float LoadF(char* param_name, float val_default=0.0f);
	void SaveS(char* param_name, char* val);
	void SaveS(char* param_name, CString val);
	void SaveS(CString* param_name, CString* val);
	void SaveS(char* param_name, CString* val);
	void SaveI(CString param_name, int val, const char* section = NULL);
	void SaveI(CString* param_name, int val, const char* section = NULL);
	void SaveI(char* param_name, int val, const char* section = NULL);
	void SaveD(char* param_name, double val);
	void SaveD(CString param_name, double val);
	void SaveD(CString* param_name, double val);
	void SaveF(char* param_name, float val);

	void SaveS(char* param_name, CWnd* ctrl);
	void SaveI(char* param_name, CWnd* ctrl);
	void SaveD(char* param_name, CWnd* ctrl);

	CString Section;
	CString FileDir;
	CString FileName;
	CString FileExt;
private:
	const static int LEN = 255;
	char s[LEN];
	void CreateFName();
	CString fname(){return FileDir + FileName + "." + FileExt;};
}; //////////////////////////////////////////////////////////////////
