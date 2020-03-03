#pragma once
#include <random>
#include <cmath>
#include <vector>
#include <DirectXMath.h>
//using namespace std;
class Model2D {
public:
	std::vector<DirectX::XMFLOAT2> v_pos;
	std::vector<int> v_area;
	int szx = 0;
	int szy = 0;
	void Create(int minszX, int minszY, int maxsz, double kFill) {
		szx = minszX;
		szy = minszY;
		int sz = __max(minszX, minszY);
		while (sz <= maxsz) {
			// TODO: fill lays here
			szy *= 2, szx *= 2, sz *= 2;
		}
		size_t szarea = ++szx * ++szy;
		v_area.resize(szarea);
		for (auto q : v_area) q = -1;
		size_t szpos = int(szarea * kFill + 0.5);
		v_pos.reserve(szpos);

		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(rd());  // to seed mersenne twister.
		std::uniform_int_distribution<int> dist(0, szarea - 1);
		while (v_pos.size() < szpos) {
			int curpos;
			do {
				curpos = dist(gen);
			} while (v_area[curpos] < 0);
			v_area[curpos] = v_pos.size();
			int y = curpos / szy;
			int x = curpos % szy;
			v_pos.push_back(DirectX::XMFLOAT2(float(x), float(y)));
		}
	} // //////////////////////////////////////////////////////////////////////////////////
}; // *****************************************************************************

