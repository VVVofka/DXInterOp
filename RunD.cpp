#include "RunD.h"

void RunD::Run(const array<DrQuadro, 2>& srcd,
			   array<DrQuadro, 2>& dstd,
			   const array<int, 2>& dsta,
			   const array<FLT2, 1>& masksD){
	parallel_for_each(srcd.extent, [&srcd, &dstd, &dsta, &masksD](index<2> idx) restrict(amp){
		INT2 src(idx);
		INT2 dst = src * 2;

		int tl = dsta[dst.y][dst.x];
		int tr = dsta[dst.y][dst.x + 1];
		int bl = dsta[dst.y + 1][dst.x];
		int br = dsta[dst.y + 1][dst.x + 1];
		int mask = ((tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3));
		int nmask = 16 * mask;
		auto srcdcell = &srcd[src.y][src.x];
		for(int ncell = 0; ncell < 4; ncell++){
			FLT2 s = srcdcell->items[ncell];
			auto item = &dstd[dst.y + ncell / 2][dst.x + ncell % 2];
			{FLT2* d = &item->items[0]; d->x = s.x + masksD[nmask].x; d->y = s.y + masksD[nmask++].y; }
			{FLT2* d = &item->items[1]; d->x = s.x + masksD[nmask].x; d->y = s.y + masksD[nmask++].y; }
			{FLT2* d = &item->items[2]; d->x = s.x + masksD[nmask].x; d->y = s.y + masksD[nmask++].y; }
			{FLT2* d = &item->items[3]; d->x = s.x + masksD[nmask].x; d->y = s.y + masksD[nmask++].y; }
		}
	});
} // ///////////////////////////////////////////////////////////////////////////////////////////////