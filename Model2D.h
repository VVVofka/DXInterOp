#pragma once
#include <random>
#include <cmath>
#include <vector>
#include <DirectXMath.h>
#include "DXInterOp.h"
#include "Sz2D.h"

//using namespace std;
class Model2D{
public:
	std::vector<std::vector<Vertex2D>> v_poss;
	std::vector<std::vector<int>> v_areas;
	std::vector<std::vector<DirectX::XMFLOAT2>> v_dirs;

	std::vector<int> lastArea(){ return v_areas[v_areas.size() - 1]; }
	std::vector<Vertex2D> lastPoss(){ return v_poss[v_poss.size() - 1]; }


	std::vector<Sz2D> vsz;

	void Create(Sz2D minsz, int maxszXY, double kFill){
		const int RESERV_LAYS_CNT = 16;
		v_poss.reserve(RESERV_LAYS_CNT);
		v_areas.reserve(RESERV_LAYS_CNT);
		v_dirs.reserve(RESERV_LAYS_CNT - 2);
		vsz.reserve(RESERV_LAYS_CNT);
		int nlay = 0;
		//Sz2D sz(minsz);
		vsz.push_back(minsz);
		if(minsz.y & 1 || minsz.x & 1)
			vsz[0] *= 2;
		int szmaxxy = vsz[0].Max();
		while(szmaxxy <= maxszXY / 2){ // without last lay (it don't contnent v_dirs)

			size_t szarea = sz.area();
			v_areas.push_back(std::vector<int>(szarea));
			for(auto q : v_areas[nlay])
				q = -1;

			v_dirs.push_back(std::vector<DirectX::XMFLOAT2>(szarea));
			for(auto q : v_dirs[nlay])
				q.x = q.y = 0;

			v_poss.push_back(std::vector<Vertex2D>());
			v_dirs.reserve(szarea);

			sz *= 2; szmaxxy *= 2;
			vsz.push_back(sz);
			nlay++;
		} // while(szmaxxy <= maxszXY / 2){ // without last lay (it not contnent v_dirs)

		 // Last lay
		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(2020);  // to seed mersenne twister. rand: gen(rd())

		size_t szarea = sz.area();
		v_areas.push_back(std::vector<int>(szarea));
		std::uniform_int_distribution<int> dist(0, szarea - 1);

		size_t szpos = int(szarea * kFill + 0.5);
		v_poss.push_back(std::vector<Vertex2D>());
		v_poss[nlay].reserve(szpos);

		while(v_poss[nlay].size() < szpos){
			int curpos;
			do{
				curpos = dist(gen);
			} while(v_areas[nlay][curpos] < 0);
			v_areas[nlay][curpos] = v_poss[nlay].size();
			float y = (((2 * curpos) / sz.x) - sz.y) / float(sz.y);
			float x = ((2 * (curpos % sz.x)) - sz.x) / float(sz.x);
			v_poss[nlay].push_back(Vertex2D(x, y));
			//_RPT4(0, "%d \tpos:%d \t %f õ %f\n", v_pos.size(), curpos, x, y);
		} // 	while(v_poss[nlay].size() < szpos)
	} // //////////////////////////////////////////////////////////////////////////////////
}; // *****************************************************************************

