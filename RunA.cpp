#include "RunA.h"
#include <cassert>

void RunA::run(const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
	parallel_for_each(dst.extent, [&dst, &src, &mask](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int y2 = y * 2;
		const int x = idx[1];
		const int x2 = x * 2;

		// yx: l-left, r-right
		int tl = src[y2][x2];
		int tr = src[y2][x2 + 1];
		int bl = src[y2 + 1][x2];
		int br = src[y2 + 1][x2 + 1];

		int sum0 = mask[(tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3)];
		int sum1 = mask[((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3)];
		int sum2 = mask[((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3)];
		int sum3 = mask[(tl >> 1) + ((tr >> 1) << 1) + ((bl >> 1) << 2) + ((br >> 1) << 3)];
		dst[y][x] = (sum3 << 3) | (sum2 << 2) | (sum1 << 1) | sum0;
	});
} // /////////////////////////////////////////////////////////////////////////////////////////////////////
void RunA::runlast(const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
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

void RunA::RunLast(int shift, const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
	assert(shift < 4);
	const int shifty = shift >> 1;
	const int shiftx = shift & 1;
	parallel_for_each(dst.extent, [&dst, &src, &mask, shifty, shiftx](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int y0 = y * 2 + shifty;
		const int y1 = (y0 + 1) % src.extent[0];

		const int x = idx[1];
		const int x0 = x * 2 + shiftx;
		const int x1 = (x0 + 1) % src.extent[1];

		// yx: l-left, r-right, t - top, b - bottom
		const int tl = src[y0][x0] < 0 ? 0 : 1;  // << 0
		const int tr = src[y0][x1] < 0 ? 0 : 2;  // << 1
		const int bl = src[y1][x0] < 0 ? 0 : 4;  // << 2
		const int br = src[y1][x1] < 0 ? 0 : 8;  // << 3
		dst[y][x] = mask[tl + tr + bl + br];
	});
} // ///////////////////////////////////////////////////////////////////////////////////////////////
void RunA::Run(int shift, const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
	assert(shift < 4);
	const int shifty = shift >> 1;
	const int shiftx = shift & 1;
	parallel_for_each(dst.extent, [&dst, &src, &mask, shifty, shiftx](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int y0 = y * 2 + shifty;
		const int y1 = (y0 + 1) % src.extent[0];

		const int x = idx[1];
		const int x0 = x * 2 + shiftx;
		const int x1 = (x0 + 1) % src.extent[1];

		int adr = (((((src[y1][x1] << 1) | src[y1][x0]) << 1) | src[y0][x1]) << 1) | src[y0][x0];
		dst[y][x] = mask[adr];
	});
} // /////////////////////////////////////////////////////////////////////////////////////////////////////
