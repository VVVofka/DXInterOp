#include "RunDlast.h"
//#define AMPDBG_DLAST
void RunDlast::Run(const INT2 shift,
				   const array<DrQuadro, 2>& srcd,
				   array<Vertex2D, 1>& dstpos,
				   array<int, 2>& dsta,
				   array<FLT2, 2>& dstd,
				   const INT2 sz){
	parallel_for_each(srcd.extent, [=, &srcd, &dstd](index<2> idx) restrict(amp){
		const INT2 src(idx);
		const int y0 = (src.y * 2 + shift.y) % dstd.extent[0];
		const int x0 = (src.x * 2 + shift.x) % dstd.extent[1];
		const int y1 = (y0 + 1) % dstd.extent[0];
		const int x1 = (x0 + 1) % dstd.extent[1];

		auto q = srcd[src.y][src.x].items;

		dstd[y0][x0].y = q->y;
		dstd[y0][x0].x = q->x;

		dstd[y0][x1].y = (++q)->y;
		dstd[y0][x1].x = q->x;

		dstd[y1][x0].y = (++q)->y;
		dstd[y1][x0].x = q->x;

		dstd[y1][x1].y = (++q)->y;
		dstd[y1][x1].x = q->x;
	}); // parallel_for_each(srcd.extent,

	//parallel_for_each(dstd.extent, [&dstd](index<2> idx) restrict(amp){
	//	int y = idx[0];
	//	int x = idx[1];
	//	dstd[y][x].y += 1.f + y / (-0.5f * dstd.extent[0]);
	//	dstd[y][x].x += 1.f + x / (-0.5f * dstd.extent[1]);
	//}); // parallel_for_each(srcd.extent,

	//parallel_for_each(dstd.extent, [&dstd](index<2> idx) restrict(amp){
	//	int y = idx[0];
	//	int x = idx[1];
	//	float diry = dstd[y][x].y;
	//	float absdiry = fabsf(diry);
	//	float dirx = dstd[y][x].x;
	//	float absdirx = fabsf(dirx);
	//	if(absdirx >= 2 * absdiry)
	//		diry = 0;
	//	else if(absdiry >= 2 * absdirx)
	//		dirx = 0;
	//	dstd[y][x].y = diry;
	//	dstd[y][x].x = dirx;
	//}); // parallel_for_each(srcd.extent,

	const float kwidthy = float(sz.y - 1) / sz.y, kwidthx = float(sz.x - 1) / sz.x;
	const int leny = sz.y - 1, lenx = sz.x - 1;
	const float ky = (leny <= 0) ? 0 : 2.f / float(leny);
	const float kx = (lenx <= 0) ? 0 : 2.f / float(lenx);

#ifdef AMPDBG_DLAST
	struct myStruct{
		int x=-9, y = -9;
		int absx=-9, absy = -9;
		int cell = -9, aold = -9, anew = -9;
		int newx = -9, newy = -9;
		int maskx = -9, masky = -9;
	};
	std::vector<myStruct> dbg(dsta.extent.size());
	array_view<myStruct, 2> av = array_view<myStruct, 2>(dsta.extent[0], dsta.extent[1], dbg);
	//setConsole();
	//printf("shift:%d %d\n", shift.y, shift.x);
	//printf("sz:%d %d\n", dsta.extent[0], dsta.extent[1]);
#endif
	parallel_for_each(srcd.extent, [=, &dsta, &dstd, &dstpos](index<2> idx) restrict(amp){
		const int mask[7] = {0,0,1, 9, -1,0,0};
		const int y0 = idx[0] * 2 + shift.y;
		const int x0 = idx[1] * 2 + shift.x;
		for(int ncell = 0; ncell < 4; ncell++){
			const int ycell = ncell / 2;
			const int xcell = ncell % 2;
			const int y = (y0 + ycell) % dstd.extent[0];
			const int x = (x0 + xcell) % dstd.extent[1];
			const int aold = dsta[y][x];
#ifdef AMPDBG_DLAST
			av[y][x].cell = ncell;
			av[y][x].y = y; av[y][x].x = x;
			av[y][x].absy = idx[0] * 2 + ycell; av[y][x].absx = idx[1] * 2 + xcell;
			av[y][x].aold = aold;
#endif
			if(aold < 0) continue;
			const int signy = sign(int(dstd[y][x].y));
			if(signy == ycell * 2 - 1) continue;
			const int signx = sign(int(dstd[y][x].x));
			if(signx == xcell * 2 - 1) continue;
			const int adry = ycell * 4 + signy + 1; // 0..1 | 0..2
			const int adrx = xcell * 4 + signx + 1;
			const int newy = (y + mask[adry] + dstd.extent[0]) % dstd.extent[0];
			const int newx = (x + mask[adrx] + dstd.extent[1]) % dstd.extent[1];
			const int anew = dsta[newy][newx];
#ifdef AMPDBG_DLAST
			av[y][x].newy = newy; av[y][x].newx = newx;
			av[y][x].masky = mask[adry]; av[y][x].maskx = mask[adrx];
			av[y][x].anew = anew;
#endif
			if(anew >= 0) continue;

			dsta[y][x] = anew; // anew == -1;
			dsta[newy][newx] = aold;

			//dstd[newy][newx].y = dstd[newy][newx].x = 0; // Block next moves by ncell
			dstd[y][x].y = dstd[y][x].x = 0;

			dstpos[aold].Pos.y = normal(newy, sz.y);
			dstpos[aold].Pos.x = normal(newx, sz.x);
		} // for(ncell
	}); // parallel_for_each(srcd.extent,
#ifdef AMPDBG_DLAST
	av.synchronize();
	for(int j = 0; j < dbg.size(); j++){
		myStruct& q = dbg[j];
		if(q.anew == -1 && q.aold > 0){
			printf("%d\t%d\t(%d %d) ->\t(%d %d)\n",j, q.aold, q.y, q.x, q.newy, q.newx);
		}
	}
#endif
} // ///////////////////////////////////////////////////////////////////////////////////////////////
float normal(int pos, int width) restrict(amp, cpu){
	return float(2 * pos + 1) / width - 1.f;
} // //////////////////////////////////////////////////////////////////////////////////////////////

#ifdef AMPDBG_DLAST
#undef AMPDBG_DLAST
#endif
