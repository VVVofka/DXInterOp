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
int Options::saveAuto() const{
	int ret = savei(iArr, szArr, autoDirsFName);
	if(ret) return ret;
	return saved(dArr, szArr, autoDirsFName);
} // ///////////////////////////////////////////////////////////////////////////
int Options::loadAuto(){
	int ret = loadi(iArr, szArr, autoDirsFName);
	if(ret) return ret;
	return loadd(dArr, szArr, autoDirsFName);
} // ///////////////////////////////////////////////////////////////////////////
int Options::savei(const int* dirs, const int sz, const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "wb");
	if(err == 0){
		fwrite(dirs, sizeof(dirs[0]), sz + 1, f);
		fclose(f);
	}
	return int(err);
} // ///////////////////////////////////////////////////////////////////////////
bool Options::loadi(int* dirs, const int sz, const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "rb");
	if(err == 0){
		int szread = (int)fread(dirs, sizeof(dirs[0]), sz, f);
		if(szread != sz){
			fclose(f);
			return false;
		}
		int check = 10;
		for(int j = 0; j < sz; j++)
			check += dirs[j];
		int checkread = 0;
		szread = (int)fread(&checkread, sizeof(checkread), 1, f);
		fclose(f);
		return (szread == 1 && checkread == check);
	}return false;
} // ///////////////////////////////////////////////////////////////////////////
int Options::saved(const double* dirs, const int sz, const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "wb");
	if(err == 0){
		fwrite(dirs, sizeof(dirs[0]), sz, f);
		fclose(f);
	}
	return int(err);
} // ///////////////////////////////////////////////////////////////////////////
int Options::loadd(double* dirs, const int sz, const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "rb");
	if(err == 0){
		fread(dirs, sizeof(dirs[0]), sz, f);
		fclose(f);
	}
	return int(err);
} // ///////////////////////////////////////////////////////////////////////////

