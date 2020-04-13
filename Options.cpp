#include "Options.h"
extern Model2D model;

Options::Options(){
	dirs = iArr;
} // //////////////////////////////////////////////////////////////////////////////////
int Options::loadDirs(){
	int dirs[16 * 4 * 4];
	model.blocks2D2.toDirs(dirs);
	int ret = openDlgOptions(dirs);
	model.blocks2D2.fromDirs(dirs);
	savei(dirs, _countof(dirs), autoDirsFName);
	//setConsole();for(auto q : dirs) printf("%d", q);
	//printf("\n");
	return ret & ReturnOptions::Restart ? 1 : 0;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::saveAuto() const{
	bool ret = save(autoDirsFName);
	return ret;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::loadAuto(){
	bool ret = load(autoDirsFName);
	return ret;
} // ///////////////////////////////////////////////////////////////////////////
void Options::setDefault(){

} // ///////////////////////////////////////////////////////////////////////////
bool Options::save(const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "wb");
	if(err == 0){
		int szwrite = (int)fwrite(iArr, sizeof(iArr[0]), szArr, f);
		if(szwrite != szArr){
			fclose(f);
			return false;
		}
		int check = 10;
		for(int j = 0; j < szArr; j++)
			check += iArr[j];
		szwrite = (int)fwrite(&check, sizeof(check), 1, f);
		fclose(f);
		return (szwrite == 1);
	}return false;
} // ///////////////////////////////////////////////////////////////////////////
bool Options::load(const char* fname){
	FILE* f;
	errno_t err = fopen_s(&f, fname, "rb");
	if(err == 0){
		int szread = (int)fread(iArr, sizeof(iArr[0]), szArr, f);
		if(szread != szArr){
			fclose(f);
			return false;
		}
		int check = 10;
		for(int j = 0; j < szArr; j++)
			check += iArr[j];
		int checkread = 0;
		szread = (int)fread(&checkread, sizeof(checkread), 1, f);
		fclose(f);
		return (szread == 1 && checkread == check);
	}return false;
} // ///////////////////////////////////////////////////////////////////////////

