#include "Options.h"
extern Model2D model;

int Options::loadDirs(){
	int dirs[16 * 4 * 4];
	model.blocks2D2.toDirs(dirs);
	int ret = openDlgOptions(dirs);
	model.blocks2D2.fromDirs(dirs);
	save(dirs, _countof(dirs), autoDirsFName);
	//setConsole();for(auto q : dirs) printf("%d", q);
	//printf("\n");
	return ret & ReturnOptions::Restart ? 1 : 0;
} // ///////////////////////////////////////////////////////////////////////////
void Options::save(const int* dirs, const int sz, const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "wb");
	if(err == 0){
		fwrite(dirs, sizeof(dirs[0]), sz, f);
		fclose(f);
	}
} // ///////////////////////////////////////////////////////////////////////////
void Options::load(int* dirs, const int sz, const char* fname) const{
	FILE* f;
	errno_t err = fopen_s(&f, fname, "rb");
	if(err == 0){
		fread(dirs, sizeof(dirs[0]), sz, f);
		fclose(f);
	}
} // ///////////////////////////////////////////////////////////////////////////

