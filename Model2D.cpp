#include "Model2D.h"

// /////////////////////////////////////////////////////////////////////////////////

void Model2D::dumpA(int nlay) const{
	setConsole();
	std::cout << " y*x: " << sizeY(nlay) << "*" << sizeX(nlay) << std::endl;
	for(int y = 0; y < sizeY(nlay); y++){
		for(int x = 0; x < sizeX(nlay); x++){
			int val = v_areas[nlay][y * sizeX(nlay) + x];
			printf(" %c", val >= 0 ? '*' : '.');
		}
		printf("\n");
	}
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

void Model2D::dumpD(int nlay) const{
	setConsole();
	std::cout << " y*x: " << sizeY(nlay) << "*" << sizeX(nlay) << std::endl;
	for(int y = 0; y < sizeY(nlay); y++){
		for(int x = 0; x < sizeX(nlay); x++){
			int val = v_areas[nlay][y * sizeX(nlay) + x];
			printf(" %c", val >= 0 ? '*' : '.');
		}
		printf("\n");
	}
}
