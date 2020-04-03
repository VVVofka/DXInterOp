//Для работы с ini файлами в папке программы
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VVVIni.h"
#include <stdlib.h>
#include <stdio.h>
#include <WinBase.h>

//#pragma warning (disable : 4996)
CVVVIni::CVVVIni()
{	Section = "Options";
	CreateFName();
}
CVVVIni::~CVVVIni()
{}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::CreateFName()
{	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char filename[_MAX_FNAME];
	char ext[_MAX_EXT];
	HMODULE hInst = GetModuleHandle(NULL); // имя файла модуля, или NULL чтобы получить хэндл главного модуля (exe) 
	GetModuleFileName(hInst, path_buffer, _MAX_PATH);
  // _splitpath( path_buffer, drive, dir, fname, ext ); // C4996
   _splitpath_s(path_buffer, drive, _MAX_DRIVE, dir, _MAX_DIR, filename, _MAX_FNAME, ext, _MAX_EXT); 
	FileDir.Format("%s%s", drive, dir);
	FileName.Format("%s", filename);
	FileExt = "ini";
} /////////////////////////////////////////////////////////////////////////////////////
CString CVVVIni::LoadS(char* param_name, char* val_default)
{	if(val_default==NULL)
		GetPrivateProfileString(Section, param_name, "", s, LEN, fname());
	else
		GetPrivateProfileString(Section, param_name, val_default, s, LEN, fname());
	return s;
}////////////////////////////////////////////////////////////////////////////////////////
CString CVVVIni::LoadS(CString* param_name, char* val_default)
{	if(val_default==NULL)
		GetPrivateProfileString(Section, LPCSTR(*param_name), "", s, LEN, fname());
	else
		GetPrivateProfileString(Section, LPCSTR(*param_name), val_default, s, LEN, fname());
	return s;
}////////////////////////////////////////////////////////////////////////////////////////
int CVVVIni::LoadI(char* param_name, int val_default, char* section)
{	CString sdef;
	sdef.Format("%d", val_default);
	if(section == NULL)
		GetPrivateProfileString(Section, param_name, sdef, s, LEN, fname());
	else
		GetPrivateProfileString(section, param_name, sdef, s, LEN, fname());
	return atoi(s);
}////////////////////////////////////////////////////////////////////////////////////////
int CVVVIni::LoadI(CString* param_name, int val_default, char* section)
{	CString sdef;
	sdef.Format("%d", val_default);
	if(section == NULL)
		GetPrivateProfileString(Section, LPCSTR(*param_name), sdef, s, LEN, fname());
	else
		GetPrivateProfileString(section, LPCSTR(*param_name), sdef, s, LEN, fname());
	return atoi(s);
}////////////////////////////////////////////////////////////////////////////////////////
int CVVVIni::LoadI(CString param_name, int val_default, char* section)
{	CString sdef;
	sdef.Format("%d", val_default);
	if(section == NULL)
		GetPrivateProfileString(Section, param_name, sdef, s, LEN, fname());
	else
		GetPrivateProfileString(section, param_name, sdef, s, LEN, fname());
	return atoi(s);
}////////////////////////////////////////////////////////////////////////////////////////
double CVVVIni::LoadD(char* param_name, double val_default)
{	CString sdef;
	sdef.Format("%f", (float)val_default);
	GetPrivateProfileString(Section, param_name, sdef, s, LEN, fname());
	return atof(s);
}////////////////////////////////////////////////////////////////////////////////////////
double CVVVIni::LoadD(CString* param_name, double val_default)
{	CString sdef;
	sdef.Format("%f", (float)val_default);
	GetPrivateProfileString(Section, LPCSTR(*param_name), sdef, s, LEN, fname());
	return atof(s);
}////////////////////////////////////////////////////////////////////////////////////////
double CVVVIni::LoadD(CString param_name, double val_default)
{	CString sdef;
	sdef.Format("%f", (float)val_default);
	GetPrivateProfileString(Section, param_name, sdef, s, LEN, fname());
	return atof(s);
}////////////////////////////////////////////////////////////////////////////////////////
float CVVVIni::LoadF(char* param_name, float val_default)
{	CString sdef;
	sdef.Format("%f", val_default);
	GetPrivateProfileString(Section, param_name, sdef, s, LEN, fname());
	_CRT_FLOAT ret;
	ret.f = 0.f;
	_atoflt(&ret, s);
	return ret.f;
}////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveS(char* param_name, char* s)
{	WritePrivateProfileString(Section, param_name, s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveS(char* param_name, CString s)
{	WritePrivateProfileString(Section, param_name, s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveS(CString* param_name, CString* s)
{	WritePrivateProfileString(Section, LPCSTR(*param_name), *s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveS(char* param_name, CString* ps)
{	WritePrivateProfileString(Section, param_name, *ps, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveI(char* param_name, int val, const char* section)
{	CString s;	s.Format("%d", val);
	if(section == NULL)
		WritePrivateProfileString(Section, param_name, s, fname());
	else
		WritePrivateProfileString(section, param_name, s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveI(CString param_name, int val, const char* section)
{	CString s;	s.Format("%d", val);
	if(section == NULL)
		WritePrivateProfileString(Section, param_name, s, fname());
	else
		WritePrivateProfileString(section, param_name, s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveI(CString* param_name, int val, const char* section)
{	CString s;	s.Format("%d", val);
	if(section == NULL)
		WritePrivateProfileString(Section, LPCSTR(*param_name), s, fname());
	else
		WritePrivateProfileString(section, LPCSTR(*param_name), s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveD(char* param_name, double val)
{	CString s;
	s.Format("%g", val);
	WritePrivateProfileString(Section, param_name, s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveD(CString param_name, double val)
{	CString s;
	s.Format("%g", val);
	WritePrivateProfileString(Section, param_name, s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveD(CString* param_name, double val)
{	CString s;
	s.Format("%g", val);
	WritePrivateProfileString(Section, LPCSTR(*param_name), s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveF(char* param_name, float val)
{	CString s;
	s.Format("%f", val);
	WritePrivateProfileString(Section, param_name, s, fname());
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveS(char* param_name, CWnd* ctrl)
{	ctrl->GetWindowText(s, LEN);
	SaveS(param_name, s);
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveI(char* param_name, CWnd* ctrl)
{	ctrl->GetWindowText(s, LEN);
	SaveI(param_name, atoi(s));
}////////////////////////////////////////////////////////////////////////////////////////
void CVVVIni::SaveD(char* param_name, CWnd* ctrl)
{	ctrl->GetWindowText(s, LEN);
	SaveD(param_name, atof(s));
}////////////////////////////////////////////////////////////////////////////////////////



