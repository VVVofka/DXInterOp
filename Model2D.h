#pragma once
#include <random>
#include <cmath>
#include <vector>
#include <DirectXMath.h>
#include "DXInterOp.h"
#include "Sz2D.h"
//def in Model2D.h
// float x,y
struct FLT2{
	float y, x;
	FLT2() : x(0), y(0){}
	FLT2(float X, float Y) : x(X), y(Y){}
	void operator +=(FLT2& other){ x+= other.x; y+= other.y;}
}; // ********************************************************************************************

struct DrShiftQuadro{
	struct DrQuadro{
		FLT2 items[4];
	}; // ********************************************************************************************
	DrQuadro shifts[4];
}; // ********************************************************************************************

class Model2D{
public:
	std::vector<std::vector<Vertex2D>> v_poss;
	std::vector<std::vector<int>> v_areas;
	std::vector<std::vector<DrShiftQuadro>> v_dirs;
	std::vector<FLT2> last_dirs;

	std::vector<int> lastArea(){ return v_areas[v_areas.size() - 1]; }
	std::vector<Vertex2D> lastPoss(){ return v_poss[v_poss.size() - 1]; }

	std::vector<Sz2D> vsz;
	int sizeY(int nlay){ return vsz[nlay].y; }
	int sizeY(){ return vsz[vsz.size() - 1].y; }
	int sizeX(int nlay){ return vsz[nlay].x; }
	int sizeX(){ return vsz[vsz.size() - 1].x; }
	int LaysCnt(){ return v_areas.size(); }
	std::vector<int>* dataArea(int nlay){ return &v_areas[nlay]; }

	void Create(Sz2D minsz, int maxszXY, double kFill){
		const int RESERV_LAYS_CNT = 16;
		v_poss.reserve(RESERV_LAYS_CNT);
		v_areas.reserve(RESERV_LAYS_CNT);
		v_dirs.reserve(RESERV_LAYS_CNT);
		vsz.reserve(RESERV_LAYS_CNT);
		int nlay = 0;
		Sz2D sz(minsz);
		//if(sz.y & 1 || sz.x & 1)			sz *= 2;
		int szmaxxy = sz.Max();
		while(szmaxxy <= maxszXY / 2){ // without last lay (it don't contnent v_dirs)
			vsz.push_back(sz);

			size_t szarea = sz.x * sz.y;
			v_areas.push_back(std::vector<int>(szarea));
			for(auto q : v_areas[nlay]) q = -1;

			v_dirs.push_back(std::vector<DrShiftQuadro>(szarea));

			v_poss.push_back(std::vector<Vertex2D>());

			sz *= 2; szmaxxy *= 2;
			nlay++;
		} // while(szmaxxy <= maxszXY / 2){ // without last lay (it not contnent v_dirs)

		 // Last lay
		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(2020);  // to seed mersenne twister. rand: gen(rd())

		vsz.push_back(sz);
		size_t szarea = (sz.x + 1) * (sz.y + 1);
		v_areas.push_back(std::vector<int>(szarea));
		v_dirs.push_back(std::vector<DrShiftQuadro>(0)); // TODO: not use. Use last_dirs
		last_dirs.resize(szarea, FLT2(0, 0));

		std::uniform_int_distribution<int> dist(0, szarea - 1);

		v_poss.push_back(std::vector<Vertex2D>());
		size_t szpos = int(szarea * kFill + 0.5);
		v_poss[nlay].reserve(szpos);

		while(v_poss[nlay].size() < szpos){
			int curpos;
			do{
				curpos = dist(gen);
			} while(v_areas[nlay][curpos] < 0);

			v_areas[nlay][curpos] = v_poss[nlay].size();

			float y = (((2 * curpos) / vsz[nlay].x) - vsz[nlay].y) / float(vsz[nlay].y);
			float x = ((2 * (curpos % vsz[nlay].x)) - vsz[nlay].x) / float(vsz[nlay].x);
			v_poss[nlay].push_back(Vertex2D(y, x));

			//_RPT4(0, "%d \tpos:%d \t %f � %f\n", v_pos.size(), curpos, x, y);
		} // 	while(v_poss[nlay].size() < szpos)

	} // //////////////////////////////////////////////////////////////////////////////////
}; // *****************************************************************************

