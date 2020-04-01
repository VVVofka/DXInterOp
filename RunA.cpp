#include "RunA.h"

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
