#include "RunA.h"
void RunA::RunLast(INT2 shift, const array<aType, 2>& src, array<int, 2>& dst, const array<int, 1>& amask){
	parallel_for_each(dst.extent, [&dst, &src, &amask, shift](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int x = idx[1];
		const int y0 = (y * 2 + shift.y) % src.extent[0];
		const int y1 = (y0 + 1) % src.extent[0];

		const int x0 = (x * 2 + shift.x) % src.extent[1];
		const int x1 = (x0 + 1) % src.extent[1];

		// yx: l-left, r-right, t - top, b - bottom
		// t - tale;   h - head
		int tl[2] = {0, 0}; // tl[0] = 1 : tale
		int tr[2] = {0, 0};
		int bl[2] = {0, 0};
		int br[2] = {0, 0};

		aType a = src[y0][x0];
		if(a.adress >= 0)
			tl[a.talehead & 1] = 1;

		a = src[y0][x1];
		if(a.adress >= 0)
			tr[a.talehead & 1] = 2;

		a = src[y1][x0];
		if(a.adress >= 0)
			bl[a.talehead & 1] = 4;

		a = src[y1][x1];
		if(a.adress >= 0)
			br[a.talehead & 1] = 1;

		const int mskt = amask[tl[0] + tr[0] + bl[0] + br[0]];
		const int mskh = amask[tl[1] + tr[1] + bl[1] + br[1]];
		if(amask[adrt])
		dst[y][x] = amask[tl + tr + bl + br];
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
