#include "RunD.h"
void RunD::run(const array<DrShiftQuadro, 2>& srcd, array<DrShiftQuadro, 2>& dstd, const array<int, 2>& dsta){
	parallel_for_each(srcd.extent, [&srcd, &dstd, &dsta](index<2> idx) restrict(amp){
		const float vdirsX[16 * 16] = {
//cell	0			1				2			3
	-1,-0,-1,-1, -0,+1,+1,+1,  -1,-1,-1,-0, +1,+1,-0,+1, //0000
	-0,-0,-0,-0, -0,-1,-0,-1,  +1,+1,+1,+1, -0,-1,-0,-1, //0001
	+1,-0,+1,-0, -0,-0,-0,-0,  +1,-0,+1,-0, -1,-1,-1,-1, //0010
	-0,-0,-0,-0, -0,-0,-0,-0,  -1,-1,-1,-1, +1,+1,+1,+1, //0011
	+1,+1,+1,+1, -0,-1,-0,-1,  -0,-0,-0,-0, -0,-1,-0,-1, //0100
	-0,-0,-0,-0, -1,-1,-0,-1,  -0,-0,-0,-0, -0,-1,-1,-1, //0101
	+1,+1,-1,+1, -0,-0,-0,-0,  -0,-0,-0,-0, -1,+1,-1,-1, //0110
	-0,-0,-0,-0, -0,-0,-0,-0,  -0,-0,-0,-0, +1,+1,-1,+1, //0111
	+1,-0,+1,-0, -1,-1,-1,-1,  +1,-0,+1,-0, -0,-0,-0,-0, //1000
	-0,-0,-0,-0, -1,-1,-1,+1,  -1,+1,+1,+1, -0,-0,-0,-0, //1001
	+1,+1,+1,-0, -0,-0,-0,-0,  +1,-0,+1,+1, -0,-0,-0,-0, //1010
	-0,-0,-0,-0, -0,-0,-0,-0,  -1,-1,-1,+1, -0,-0,-0,-0, //1011
	-1,-1,-1,-1, +1,+1,+1,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1100
	-0,-0,-0,-0, -1,+1,+1,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1101
	-1,+1,-1,-1, -0,-0,-0,-0,  -0,-0,-0,-0, -0,-0,-0,-0, //1110
	-1,-0,-1,-1, -0,+1,+1,+1,  -1,-1,-1,-0, +1,+1,-0,+1};//1111
		const float vdirsY[16 * 16] = {
//cell	0			1				2			3
	-1,-1,-0,-1, -1,-1,-1,-0,  -0,+1,+1,+1, +1,-0,+1,+1, //0000
	-0,-0,-0,-0, +1,+1,+1,+1,  -0,-0,-1,-1, -0,-0,-1,-1, //0001
	+1,+1,+1,+1, -0,-0,-0,-0,  -0,-0,-1,-1, -0,-0,-1,-1, //0010
	-0,-0,-0,-0, -0,-0,-0,-0,  -1,-0,-1,-1, -0,-1,-1,-1, //0011
	+1,+1,-0,-0, +1,+1,-0,-0,  -0,-0,-0,-0, -1,-1,-1,-1, //0100
	-0,-0,-0,-0, -1,-1,-1,-1,  -0,-0,-0,-0, +1,+1,+1,+1, //0101
	+1,-1,+1,+1, -0,-0,-0,-0,  -0,-0,-0,-0, -1,-1,+1,-1, //0110
	-0,-0,-0,-0, -0,-0,-0,-0,  -0,-0,-0,-0, +1,-1,+1,+1, //0111
	+1,+1,-0,-0, +1,+1,-0,-0,  -1,-1,-1,-1, -0,-0,-0,-0, //1000
	-0,-0,-0,-0, -1,+1,+1,+1,  -1,-1,-1,+1, -0,-0,-0,-0, //1001
	-1,-1,-1,-1, -0,-0,-0,-0,  +1,+1,+1,+1, -0,-0,-0,-0, //1010
	-0,-0,-0,-0, -0,-0,-0,-0,  -1,+1,+1,+1, -0,-0,-0,-0, //1011
	+1,+1,+1,-0, +1,+1,-0,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1100
	-0,-0,-0,-0, -1,-1,-1,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1101
	-1,-1,+1,-1, -0,-0,-0,-0,  -0,-0,-0,-0, -0,-0,-0,-0, //1110
	-1,-1,-0,-1, -1,-1,-1,-0,  -0,+1,+1,+1, +1,-0,+1,+1};//1111
//cell	0			1				2			3

		const int y = idx[0];
		const int x = idx[1];
		const int y2 = y * 2;
		const int x2 = x * 2;

		int tl = dsta[y2][x2];
		int tr = dsta[y2][x2 + 1];
		int bl = dsta[y2 + 1][x2];
		int br = dsta[y2 + 1][x2 + 1];
		int mask[4]; // shift
		mask[0] = ((tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3));
		mask[1] = (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
		mask[2] = (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
		mask[3] = ((tl >> 1) + ((tr >> 1) << 1) + ((bl >> 1) << 2) + ((br >> 1) << 3));

		auto srcdcell = &srcd[y][x];
		for(int shift = 0; shift < 4; shift++){
			int nmask = 16 * mask[shift];
			auto srcsh = &srcdcell->shifts[shift];
			for(int ncell = 0; ncell < 4; ncell++){
				FLT2 src = srcsh->items[ncell];
				auto item = &dstd[y2 + ncell / 2][x2 + ncell % 2].shifts[shift];
				{FLT2* dst = &item->items[0]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
				{FLT2* dst = &item->items[1]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
				{FLT2* dst = &item->items[2]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
				{FLT2* dst = &item->items[3]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
			}
		}
	});
} // ///////////////////////////////////////////////////////////////////////////////////////////////
void RunD::Run(const array<DrQuadro, 2>& srcd,
			   array<DrQuadro, 2>& dstd,
			   const array<int, 2>& dsta,
			   const array<FLT2, 1>& masksD){
	parallel_for_each(srcd.extent, [&srcd, &dstd, &dsta, &masksD](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int x = idx[1];
		const int y2 = y * 2;
		const int x2 = x * 2;

		int tl = dsta[y2][x2];
		int tr = dsta[y2][x2 + 1];
		int bl = dsta[y2 + 1][x2];
		int br = dsta[y2 + 1][x2 + 1];
		int mask = ((tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3));
		int nmask = 16 * mask;
		auto srcdcell = &srcd[y][x];
		for(int ncell = 0; ncell < 4; ncell++){
			FLT2 src = srcdcell->items[ncell];
			auto item = &dstd[y2 + ncell / 2][x2 + ncell % 2];
			{FLT2* dst = &item->items[0]; dst->x = src.x + masksD[nmask].x; dst->y = src.y + masksD[nmask++].y; }
			{FLT2* dst = &item->items[1]; dst->x = src.x + masksD[nmask].x; dst->y = src.y + masksD[nmask++].y; }
			{FLT2* dst = &item->items[2]; dst->x = src.x + masksD[nmask].x; dst->y = src.y + masksD[nmask++].y; }
			{FLT2* dst = &item->items[3]; dst->x = src.x + masksD[nmask].x; dst->y = src.y + masksD[nmask++].y; }
		}
	});
} // ///////////////////////////////////////////////////////////////////////////////////////////////