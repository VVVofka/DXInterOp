#pragma once
#include <random>
#include <cmath>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4005 26812) 
#include <D3D11.h>
#include <DirectXMath.h>
#pragma warning(pop)

#include "DXInterOp.h"
#include "Utils.h"
#include "Vertex.h"

class Model2D{
public:
	std::vector<std::vector<Vertex2D>> v_poss;
	std::vector<std::vector<int>> v_areas;
	//std::vector<std::vector<DrShiftQuadro>> v_shiftdirs;
	std::vector<std::vector<DrQuadro>> v_dirs;
	std::vector<FLT2> last_dirs;

	std::vector<int> lastArea(){ return v_areas[v_areas.size() - 1]; }
	std::vector<Vertex2D> lastPoss(){ return v_poss[v_poss.size() - 1]; }

	std::vector<INT2> vsz;
	int sizeY(int nlay){ return vsz[nlay].y; }
	int sizeY(){ return vsz[vsz.size() - 1].y; }
	int sizeX(int nlay){ return vsz[nlay].x; }
	int sizeX(){ return vsz[vsz.size() - 1].x; }
	const INT2& sizeYX(){return vsz[vsz.size() - 1];}
	int LaysCnt(){ return (int)v_areas.size(); }
	std::vector<Vertex2D>* posLast(){ return &v_poss[v_poss.size() - 1]; }

	void Create(INT2& minsz, int maxszXY, double kRnd, double kSigmaY, double kSigmaX){
		const int RESERV_LAYS_CNT = 16;
		v_poss.reserve(RESERV_LAYS_CNT);
		v_areas.reserve(RESERV_LAYS_CNT);
		v_dirs.reserve(RESERV_LAYS_CNT);
		vsz.reserve(RESERV_LAYS_CNT);
		size_t nlay = 0;
		INT2 sz(minsz);
		int szmaxxy = sz.Max();
		while(szmaxxy <= maxszXY / 2){ // without last lay (it don't contnent v_dirs)
			vsz.push_back(sz);

			size_t szarea = size_t(sz.x * sz.y);
			v_areas.push_back(std::vector<int>(szarea, -1)); // -1 - empty value
			for(auto q : v_areas[nlay]) q = -1;

			v_dirs.push_back(std::vector<DrQuadro>(szarea));

			v_poss.push_back(std::vector<Vertex2D>());

			sz *= 2; szmaxxy *= 2;
			nlay++;
		} // while(szmaxxy <= maxszXY / 2){ // without last lay (it not contnent v_dirs)

		 // Last lay
		vsz.push_back(INT2(sz.y, sz.x));
		size_t szarea = sz.x * sz.y;
		v_areas.push_back(std::vector<int>(szarea, -1)); // -1 - empty value
		last_dirs.resize(szarea, FLT2(0, 0));

		// fill v_poss (for screen only) & v_areas for the last lay
		v_poss.push_back(std::vector<Vertex2D>());
		fillrnd(nlay, szarea, kRnd, kSigmaY, kSigmaX);
		//filltest(nlay);
	} // //////////////////////////////////////////////////////////////////////////////////
	Vertex2D norm(int curpos, INT2 sizes){
		int iy = curpos / sizes.x;
		float y = NORMAL_TO_AREA(iy, sizes.y);
		int ix = curpos % sizes.x;
		float x = NORMAL_TO_AREA(ix, sizes.x);
		return Vertex2D(y, x);
	} // /////////////////////////////////////////////////////////////////////////////////
	void fillrnd(int nlay, size_t szarea, double kFill, double kSigmaY, double kSigmaX){
		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(2020);  // to seed mersenne twister. rand: gen(rd())
		std::uniform_int_distribution<int> dist(0, int(szarea) - 1);
		size_t szpos = size_t(szarea * kFill + 0.5);
		v_poss[nlay].reserve(szpos);

		int szy = vsz[nlay].y, szx = vsz[nlay].x;
		std::normal_distribution<> distry(szy * 0.5, szy * 0.3 * kSigmaY);
		std::normal_distribution<> distrx(szx * 0.5, szx * 0.3 * kSigmaX);

		while(v_poss[nlay].size() < szpos){
			int curpos;
			do{
				//curpos = dist(gen);
				int y, x;
				do{
					y = (int)distry(gen);
				} while(y < 0 || y >= szy);
				do{
					x = (int)distrx(gen);
				} while(x < 0 || x >= szx);

				curpos = y * szx + x;
				assert(curpos < (int)v_areas[nlay].size());
			} while(v_areas[nlay][curpos] >= 0);
			v_areas[nlay][curpos] = (unsigned int)v_poss[nlay].size(); // 0 ... szpos-1

			Vertex2D vert2 = norm(curpos, vsz[nlay]);
			v_poss[nlay].push_back(vert2);
		} // 	while(v_poss[nlay].size() < szpos)
	} // /////////////////////////////////////////////////////////////////////////////////
	void filltest(int nlay){
		int vcurpos[] =
		//{1, 4, 9, 11, 14, 19, 20, 21, 24, 25, 29, 34, 44};
		{0, 8, 16, 4 * 17, 4 * 17 + 8, 4 * 17 + 16, 8 * 17, 8 * 17 + 8, 8 * 17 + 16};
		for(auto curpos : vcurpos){
			v_areas[nlay][curpos] = (unsigned int)v_poss[nlay].size();
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

