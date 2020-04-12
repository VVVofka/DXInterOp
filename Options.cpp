#include "Options.h"
extern Model2D model;

int Options::loadDirs(){
	int dirs[16 * 4 * 4];
	model.blocks2D2.toDirs(dirs);
	openDlgOptions(dirs);
	model.blocks2D2.fromDirs(dirs);

	//fopen_s()
	//setConsole();for(auto q : dirs) printf("%d", q);
	//printf("\n");
	return 0;
} // ///////////////////////////////////////////////////////////////////////////
