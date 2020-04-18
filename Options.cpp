#include "Options.h"

Options::Options(){
	dirs = iArr;
	bool isload = loadAuto();
	if(!isload)
		setDefault();
} // //////////////////////////////////////////////////////////////////////////////////
bool Options::saveAuto() const{
	bool ret = save(autoDirsFName);
	return ret;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::loadAuto(){
	bool ret = load(autoDirsFName);
	if(ret)
		loadAll();
	return ret;
} // ///////////////////////////////////////////////////////////////////////////
int Options::showDlg(){
	retDlg = openDlgOptions(iArr, dArr);
	save(autoDirsFName);
	loadAll();
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
		int sziwrite = (int)fwrite(iArr, sizeof(iArr[0]), sziArr, f);
		if(sziwrite != sziArr){
			fclose(f);
			MessageBoxA(NULL, fname, "Error#1 write i file!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		double checki = 10;
		for(int j = 0; j < sziArr; j++)
			checki += iArr[j];
		sziwrite = (int)fwrite(&checki, sizeof(checki), 1, f);

		int szdwrite = (int)fwrite(dArr, sizeof(dArr[0]), szdArr, f);
		if(szdwrite != szdArr){
			fclose(f);
			MessageBoxA(NULL, fname, "Error#1 write d file!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		double checkd = 10.0;
		for(int j = 0; j < szdArr; j++)
			checkd += dArr[j];
		szdwrite = (int)fwrite(&checkd, sizeof(checkd), 1, f);
		fclose(f);
		if(szdwrite == 1 && sziwrite == 1)
			return true;
	}
	MessageBoxA(NULL, fname, "Error#2 write file!", MB_ICONEXCLAMATION | MB_OK);
	return false;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::load(const char* fname){
	FILE* f;
	errno_t err = fopen_s(&f, fname, "rb");
	if(err == 0){
		int sziread = (int)fread(iArr, sizeof(iArr[0]), sziArr, f);
		if(sziread != sziArr){
			fclose(f);
			MessageBoxA(NULL, fname, "Error#1 read i file!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		int checki = 10;
		for(int j = 0; j < sziArr; j++)
			checki += iArr[j];
		int checkiread = 0;
		sziread = (int)fread(&checkiread, sizeof(checkiread), 1, f);
		
		int szdread = (int)fread(dArr, sizeof(dArr[0]), szdArr, f);
		if(szdread != szdArr){
			fclose(f);
			MessageBoxA(NULL, fname, "Error#1 read d file!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		double checkd = 10.0;
		for(int j = 0; j < szdArr; j++)
			checkd += dArr[j];
		int checkdread = 0;
		szdread = (int)fread(&checkdread, sizeof(checkdread), 1, f);

fclose(f);
		if(sziread == 1 && szdread == 1 && checkiread == checki && checkdread == checkd)
			return true;
	}
	MessageBoxA(NULL, fname, "Error#2 read file!", MB_ICONEXCLAMATION | MB_OK);
	return false;
} // ///////////////////////////////////////////////////////////////////////////
void Options::loadAll(){
	blocks2D2.fromDirs(dirs);// TODO: add other parametrs
} // ///////////////////////////////////////////////////////////////////////////

