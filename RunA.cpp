#include "RunA.h"
#define TORSPACE
void RunA::RunLast(INT2 shift, const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
	parallel_for_each(dst.extent, [&dst, &src, &mask, shift](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int x = idx[1];
		const int y0 = (y * 2 + shift.y) % src.extent[0];
		const int y1 = (y0 + 1) % src.extent[0];

		const int x0 = (x * 2 + shift.x) % src.extent[1];
		const int x1 = (x0 + 1) % src.extent[1];

		// yx: l-left, r-right, t - top, b - bottom
		const int tl = src[y0][x0] < 0 ? 0 : 1;  // << 0
		const int tr = src[y0][x1] < 0 ? 0 : 2;  // << 1
		const int bl = src[y1][x0] < 0 ? 0 : 4;  // << 2
		const int br = src[y1][x1] < 0 ? 0 : 8;  // << 3
		dst[y][x] = mask[tl + tr + bl + br];
	});
} // ///////////////////////////////////////////////////////////////////////////////////////////////
void RunA::Run(const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
	parallel_for_each(dst.extent, [&dst, &src, &mask](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int y0 = y * 2;
		const int y1 = (y0 + 1) % src.extent[0];

		const int x = idx[1];
		const int x0 = x * 2;
		const int x1 = (x0 + 1) % src.extent[1];

		int adr = (((((src[y1][x1] << 1) | src[y1][x0]) << 1) | src[y0][x1]) << 1) | src[y0][x0];
		dst[y][x] = mask[adr];
	});
} // /////////////////////////////////////////////////////////////////////////////////////////////////////
