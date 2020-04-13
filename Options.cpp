#include "Options.h"

Options::Options(){
	dirs = iArr;
	if(!loadAuto())
		setDefault();
} // //////////////////////////////////////////////////////////////////////////////////
bool Options::saveAuto() const{
	bool ret = save(autoDirsFName);
	return ret;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::loadAuto(){
	bool ret = load(autoDirsFName);
	return ret;
} // ///////////////////////////////////////////////////////////////////////////
int Options::showDlg(){
	retDlg = openDlgOptions(iArr);
	return retDlg;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::setDefault(){
	blocks2D2.setDefault();
	blocks2D2.toDirs(dirs);
	bool ret = save(autoDirsFName);
	return ret;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::save(const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "wb");
	if(err == 0){
		int szwrite = (int)fwrite(iArr, sizeof(iArr[0]), szArr, f);
		if(szwrite != szArr){
			fclose(f);
			MessageBoxA(NULL, fname, "Error#1 write file!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		int check = 10;
		for(int j = 0; j < szArr; j++)
			check += iArr[j];
		szwrite = (int)fwrite(&check, sizeof(check), 1, f);
		fclose(f);
		if(szwrite == 1)
			return true;
	}
	MessageBoxA(NULL, fname, "Error#2 write file!", MB_ICONEXCLAMATION | MB_OK);
	return false;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::load(const char* fname){
	FILE* f;
	errno_t err = fopen_s(&f, fname, "rb");
	if(err == 0){
		int szread = (int)fread(iArr, sizeof(iArr[0]), szArr, f);
		if(szread != szArr){
			fclose(f);
			MessageBoxA(NULL, fname, "Error#1 read file!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		int check = 10;
		for(int j = 0; j < szArr; j++)
			check += iArr[j];
		int checkread = 0;
		szread = (int)fread(&checkread, sizeof(checkread), 1, f);
		fclose(f);
		if(szread == 1 && checkread == check)
			return true;
	}
	MessageBoxA(NULL, fname, "Error#2 read file!", MB_ICONEXCLAMATION | MB_OK);
	return false;
} // ///////////////////////////////////////////////////////////////////////////

