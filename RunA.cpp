#include "RunA.h"
#define TORSPACE
void RunA::RunLast(INT2 shift, const array<int, 2>& srca, array<int, 2>& dsta, const array<int, 1>& mask){
	parallel_for_each(dsta.extent, [&dsta, &srca, &mask, shift](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int x = idx[1];
		const int y0 = (y * 2 + shift.y) % srca.extent[0];
		const int y1 = (y0 + 1) % srca.extent[0];

		const int x0 = (x * 2 + shift.x) % srca.extent[1];
		const int x1 = (x0 + 1) % srca.extent[1];

		// yx: l-left, r-right, t - top, b - bottom
		const int tl = srca[y0][x0] < 0 ? 0 : 1;  // << 0
		const int tr = srca[y0][x1] < 0 ? 0 : 2;  // << 1
		const int bl = srca[y1][x0] < 0 ? 0 : 4;  // << 2
		const int br = srca[y1][x1] < 0 ? 0 : 8;  // << 3
		dsta[y][x] = mask[tl + tr + bl + br];
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
void RunA::RunUnTorLast(INT2 shift, const array<int, 2>& src, array<int, 2>& dst, const array<int, 1>& mask){
	parallel_for_each(dst.extent, [&dst, &src, &mask, shift](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int x = idx[1];
		const int szy = src.extent[0], szx = src.extent[1];
		const int y0 = 2 * y + shift.y - szy / 2;
		const int y1 = y0 + 1;

		const int x0 = 2 * x + shift.x - szx / 2;
		const int x1 = x0 + 1;

		if(y1 >= szy || x1 >= szx || y0 < 0 || x0 < 0)
			dst[y][x] = 0;
		else{
				// yx: l-left, r-right, t - top, b - bottom
			const int tl = src[y0][x0] < 0 ? 0 : 1;  // << 0
			const int tr = src[y0][x1] < 0 ? 0 : 2;  // << 1
			const int bl = src[y1][x0] < 0 ? 0 : 4;  // << 2
			const int br = src[y1][x1] < 0 ? 0 : 8;  // << 3
			dst[y][x] = mask[tl + tr + bl + br];
		}
	});
} // ///////////////////////////////////////////////////////////////////////////////////////////////
