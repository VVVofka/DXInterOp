#pragma once
#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::direct3d;
class RunAlast{
public:
	static void run(const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
		parallel_for_each(dst.extent, [&dst, &src, &mask](index<2> idx) restrict(amp){
			// yx: c-centre, l-left, r-right
			const int y = idx[0];
			const int y2t = y * 2;
			const int y2c = y2t + 1;
			const int y2b = y2c + 1;

			const int x = idx[1];
			const int x2l = x * 2;
			const int x2c = x2l + 1;
			const int x2r = x2c + 1;

			const int cc = src[y2c][x2c] < 0 ? 0 : 1;  // << 0
			const int tc = src[y2t][x2c] < 0 ? 0 : 2;  // << 1
			const int cl = src[y2c][x2l] < 0 ? 0 : 4;  // << 2
			const int tl = src[y2t][x2l] < 0 ? 0 : 8;  // << 3

			const int br = src[y2b][x2r] < 0 ? 0 : 8;
			const int cr = src[y2c][x2r] < 0 ? 0 : 4;
			const int bc = src[y2b][x2c] < 0 ? 0 : 2;
			int sum = mask[cc + bc + cr + br];
			const int bl = src[y2b][x2l] < 0 ? 0 : 8;
			sum = (sum << 1) | mask[cc + bc + cl + bl];
			const int tr = src[y2t][x2r] < 0 ? 0 : 8;
			sum = (sum << 1) | mask[cc + tc + cr + tr];
			dst[y][x] = (sum << 1) | mask[cc + tc + cl + tl];
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
}; // ***********************************************************************************************************

