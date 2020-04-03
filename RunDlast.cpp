#include "RunDlast.h"
//#define AMPDBG_DLAST
void RunDlast::Run(INT2 shift,
				   const array<DrQuadro, 2>& srcd,
				   array<Vertex2D, 1>& dstpos,
				   array<int, 2>& dsta,
				   array<FLT2, 2>& dstd,
				   const int szy,
				   const int szx){
	parallel_for_each(srcd.extent, [=, &srcd, &dstd](index<2> idx) restrict(amp){
		const int y = idx[0];
		const int y0 = (y * 2 + shift.y) % dstd.extent[0];
		const int y1 = (y0 + 1) % dstd.extent[0];

		const int x = idx[1];
		const int x0 = (x * 2 + shift.x) % dstd.extent[1];
		const int x1 = (x0 + 1) % dstd.extent[1];

		auto q = srcd[y][x].items;

		dstd[y0][x0].y = q->y;
		dstd[y0][x0].x = q->x;

		dstd[y0][x1].y = (++q)->y;
		dstd[y0][x1].x = q->x;

		dstd[y1][x0].y = (++q)->y;
		dstd[y1][x0].x = q->x;

		dstd[y1][x1].y = (++q)->y;
		dstd[y1][x1].x = q->x;
	}); // parallel_for_each(srcd.extent,

	parallel_for_each(dstd.extent, [&dstd](index<2> idx) restrict(amp){
		int y = idx[0];
		int x = idx[1];
		float diry = dstd[y][x].y;
		float absdiry = fabsf(diry);
		float dirx = dstd[y][x].x;
		float absdirx = fabsf(dirx);
		if(absdirx >= 2 * absdiry)
			diry = 0;
		else if(absdiry >= 2 * absdirx)
			dirx = 0;
		dstd[y][x].y = diry;
		dstd[y][x].x = dirx;
	}); // parallel_for_each(srcd.extent,

	const float kwidthy = float(szy - 1) / szy, kwidthx = float(szx - 1) / szx;
	const int leny = szy - 1, lenx = szx - 1;
	const float ky = (leny <= 0) ? 0 : 2.f / float(leny);
	const float kx = (lenx <= 0) ? 0 : 2.f / float(lenx);
	parallel_for_each(srcd.extent, [=, &dsta, &dstd, &dstpos](index<2> idx) restrict(amp){
		const int mask[7] = {0,0,1, 9, -1,0,0};
		const int y0 = idx[0] * 2 + shift.y;
		const int x0 = idx[1] * 2 + shift.x;
		for(int ncell = 0; ncell < 4; ncell++){
			int ycell = ncell / 2;
			int xcell = ncell % 2;
			int y = (y0 + ycell) % dstd.extent[0];
			int x = (x0 + xcell) % dstd.extent[1];
			int aold = dsta[y][x];
			if(aold < 0) continue;
			int signy = sign(int(dstd[y][x].y));
			if(signy == ycell * 2 - 1) continue;
			int signx = sign(int(dstd[y][x].x));
			if(signx == xcell * 2 - 1) continue;
			int adry = ycell * 4 + signy + 1; // 0..1 | 0..2
			int adrx = xcell * 4 + signx + 1;
			int newy = (y + mask[adry]) % dstd.extent[0];
			int newx = (x + mask[adrx]) % dstd.extent[1];
			int anew = dsta[newy][newx];
			if(anew >= 0) continue;

			dsta[y][x] = anew; // anew == -1;
			dsta[newy][newx] = aold;

			dstd[newy][newx].y = dstd[newy][newx].x = 0; // Block next moves by ncell
			dstd[y][x].y = dstd[y][x].x = 0;

			dstpos[aold].Pos.y = normal(newy, szy);
			dstpos[aold].Pos.x = normal(newx, szx);
		} // for(ncell
	}); // parallel_for_each(srcd.extent,
} // ///////////////////////////////////////////////////////////////////////////////////////////////
float normal(int pos, int width) restrict(amp, cpu){
	return float(2 * pos + 1) / width - 1.f;
} // //////////////////////////////////////////////////////////////////////////////////////////////

#ifdef AMPDBG_DLAST
#undef AMPDBG_DLAST
#endif
