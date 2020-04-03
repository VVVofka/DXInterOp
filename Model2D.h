#pragma once
#include <random>
#include <cmath>
#include <vector>
#include <DirectXMath.h>
#include "DXInterOp.h"
#include "Sz2D.h"
#include "DrShiftQuadro.h"
#include "Utils.h"
#include "Vertex.h"

//def in Model2D.h
// float x,y

class Model2D{
public:
	std::vector<std::vector<Vertex2D>> v_poss;
	std::vector<std::vector<int>> v_areas;
	//std::vector<std::vector<DrShiftQuadro>> v_shiftdirs;
	std::vector<std::vector<DrQuadro>> v_dirs;
	std::vector<FLT2> last_dirs;

	std::vector<int> lastArea(){ return v_areas[v_areas.size() - 1]; }
	std::vector<Vertex2D> lastPoss(){ return v_poss[v_poss.size() - 1]; }

	std::vector<Sz2D> vsz;
	int sizeY(int nlay){ return vsz[nlay].y; }
	int sizeY(){ return vsz[vsz.size() - 1].y; }
	int sizeX(int nlay){ return vsz[nlay].x; }
	int sizeX(){ return vsz[vsz.size() - 1].x; }
	int LaysCnt(){ return v_areas.size(); }
	std::vector<Vertex2D>* posLast(){return &v_poss[v_poss.size()-1];}

	void Create(Sz2D& minsz, int maxszXY){
		const int RESERV_LAYS_CNT = 16;
		v_poss.reserve(RESERV_LAYS_CNT);
		v_areas.reserve(RESERV_LAYS_CNT);
		//v_shiftdirs.reserve(RESERV_LAYS_CNT);
		v_dirs.reserve(RESERV_LAYS_CNT);
		vsz.reserve(RESERV_LAYS_CNT);
		int nlay = 0;
		Sz2D sz(minsz);
		//if(sz.y & 1 || sz.x & 1)			sz *= 2;
		int szmaxxy = sz.Max();
		while(szmaxxy <= maxszXY / 2){ // without last lay (it don't contnent v_dirs)
			vsz.push_back(sz);

			size_t szarea = sz.x * sz.y;
			v_areas.push_back(std::vector<int>(szarea, -1)); // -1 - empty value
			for(auto q : v_areas[nlay]) q = -1;

			//v_shiftdirs.push_back(std::vector<DrShiftQuadro>(szarea));
			v_dirs.push_back(std::vector<DrQuadro>(szarea));

			v_poss.push_back(std::vector<Vertex2D>());

			sz *= 2; szmaxxy *= 2;
			nlay++;
		} // while(szmaxxy <= maxszXY / 2){ // without last lay (it not contnent v_dirs)

		 // Last lay
		vsz.push_back(Sz2D(sz.y + 1, sz.x + 1));
		size_t szarea = (sz.x + 1) * (sz.y + 1);
		v_areas.push_back(std::vector<int>(szarea, -1)); // -1 - empty value
		//v_dirs.push_back(std::vector<DrShiftQuadro>(0)); // not use. Use last_dirs
		last_dirs.resize(szarea, FLT2(0, 0));

		// fill v_poss (for screen only) & v_areas for the last lay
		v_poss.push_back(std::vector<Vertex2D>());
		fillrnd(nlay, szarea, 0.05);
		//filltest(nlay);
	} // //////////////////////////////////////////////////////////////////////////////////
	Vertex2D norm(int curpos, Sz2D sizes){
		int iy = curpos / sizes.x;
		//float y = sizes.y <= 1 ? 0 : 2.f * iy / (sizes.y - 1.f) - 1.f;
		float y = normal(iy, sizes.y);
		int ix = curpos % sizes.x;
		//float x = sizes.x <= 1 ? 0 : 2.f * ix / (sizes.x - 1.f) - 1.f;
		float x = normal(ix, sizes.x);
		return Vertex2D(y, x);
	} // /////////////////////////////////////////////////////////////////////////////////
	void fillrnd(int nlay, size_t szarea, double kFill){
		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(2020);  // to seed mersenne twister. rand: gen(rd())
		std::uniform_int_distribution<int> dist(0, szarea - 1);
		size_t szpos = int(szarea * kFill + 0.5);
		v_poss[nlay].reserve(szpos);
		while(v_poss[nlay].size() < szpos){
			int curpos;
			do{
				curpos = dist(gen);
			} while(v_areas[nlay][curpos] >= 0);
			v_areas[nlay][curpos] = v_poss[nlay].size(); // 0 ... szpos-1

			Vertex2D vert2 = norm(curpos, vsz[nlay]);
			v_poss[nlay].push_back(vert2);
		} // 	while(v_poss[nlay].size() < szpos)
	} // /////////////////////////////////////////////////////////////////////////////////
	void filltest(int nlay){
		int vcurpos[] =
		//{1, 4, 9, 11, 14, 19, 20, 21, 24, 25, 29, 34, 44};
		{0, 8, 16, 4*17, 4*17+8, 4*17+16, 8*17, 8*17+8, 8*17+16};
		for(auto curpos : vcurpos){
			v_areas[nlay][curpos] = v_poss[nlay].size();
			Vertex2D vert2 = norm(curpos, vsz[nlay]);
			v_poss[nlay].push_back(vert2);
		}
	} // /////////////////////////////////////////////////////////////////////////////////
	void dumpA(int nlay){
		setConsole();
		std::cout << " y*x: " << sizeY(nlay) << "*" << sizeX(nlay) << std::endl;
		for(int y = 0; y < sizeY(nlay); y++){
			for(int x = 0; x < sizeX(nlay); x++){
				int val = v_areas[nlay][y * sizeX(nlay) + x];
				printf(" %c", val >= 0 ? '*' : '.');
			}
			printf("\n");
		}
	} // ////////////////////////////////////////////////////////////////////////////////////////////////
	void dumpD(int nlay){
		setConsole();
		std::cout << " y*x: " << sizeY(nlay) << "*" << sizeX(nlay) << std::endl;
		for(int y = 0; y < sizeY(nlay); y++){
			for(int x = 0; x < sizeX(nlay); x++){
				int val = v_areas[nlay][y * sizeX(nlay) + x];
				printf(" %c", val >= 0 ? '*' : '.');
			}
			printf("\n");
		}
	} // ////////////////////////////////////////////////////////////////////////////////////////////////
}; // *****************************************************************************

