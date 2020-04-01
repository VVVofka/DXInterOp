#include "RunDlast.h"
#include "FLT2.h"
//#define AMPDBG_DLAST
void RunDlast::run(const array<DrShiftQuadro, 2>& srcd,
			  array<Vertex2D, 1>& dstpos,
			  array<int, 2>& dsta,
			  array<FLT2, 2>& dstd,
			  const int szy,
			  const int szx){
	for(int nshift = 0; nshift < 4; nshift++){
		parallel_for_each(srcd.extent, [=, &srcd, &dstd](index<2> idx) restrict(amp){
			const int y = idx[0];
			const int x = idx[1];
			auto q = srcd[y][x].shifts[nshift].items;
			const int y2 = y * 2 + nshift / 2; // y*2+yshift
			const int x2 = x * 2 + nshift % 2;

			dstd[y2][x2].y += q->y;
			dstd[y2][x2].x += q->x;

			dstd[y2][x2 + 1].y += (++q)->y;
			dstd[y2][x2 + 1].x += q->x;

			dstd[y2 + 1][x2].y += (++q)->y;
			dstd[y2 + 1][x2].x += q->x;

			dstd[y2 + 1][x2 + 1].y += (++q)->y;
			dstd[y2 + 1][x2 + 1].x += q->x;
		}); // parallel_for_each(srcd.extent,
	} // for(nshift

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

#ifdef AMPDBG_DLAST
	setConsole();
	printf("Before move:\n");
	//dumpA(-1);
	//dumpDLast();
	//dumpPos();
	struct myStruct4{
		int y, x, adry, adrx, newy, newx, aold, anew;
		FLT2 dirold, dirnew;
	};
	std::vector<myStruct4> dbg(dsta.extent.size());
	array_view<myStruct4, 2> av = array_view<myStruct4, 2>(dsta.extent[0], dsta.extent[1], dbg);
#endif
	const float kwidthy = float(szy - 1) / szy, kwidthx = float(szx - 1) / szx;
	const int leny = szy - 1, lenx = szx - 1;
	const float ky = (leny <= 0) ? 0 : 2.f / float(leny);
	const float kx = (lenx <= 0) ? 0 : 2.f / float(lenx);
	for(int nshift = 0; nshift < 4; nshift++){
		const int yshift = nshift / 2;
		const int xshift = nshift % 2;
#ifdef AMPDBG_DLAST
		printf("\nshift = %d\n", nshift);
#endif
		parallel_for_each(srcd.extent, [=, &dsta, &dstd, &dstpos](index<2> idx) restrict(amp){
			const int mask[7] = {0,0,1, 9, -1,0,0};
			int y0 = idx[0] * 2 + yshift;
			int x0 = idx[1] * 2 + xshift;
			for(int ncell = 0; ncell < 4; ncell++){
				int ycell = ncell / 2;
				int xcell = ncell % 2;
				int y = y0 + ycell;
				int x = x0 + xcell;
				int aold = dsta[y][x];
#ifdef AMPDBG_DLAST
				av[y][x].dirold.y = dstd[y][x].y;
				av[y][x].dirold.x = dstd[y][x].x;
				av[y][x].x = x;
				av[y][x].y = y;
				av[y][x].aold = aold;
#endif
				if(aold < 0) continue;
				int signy = sign(int(dstd[y][x].y));
				int signx = sign(int(dstd[y][x].x));
				if((signx == xcell * 2 - 1) || (signy == ycell * 2 - 1)) continue;
				int adry = ycell * 4 + signy + 1; // 0..1 | 0..2
				int adrx = xcell * 4 + signx + 1;
				int newy = y + mask[adry];
				int newx = x + mask[adrx];
				int anew = dsta[newy][newx];
#ifdef AMPDBG_DLAST
				av[y][x].adry = adry;
				av[y][x].adrx = adrx;
				av[y][x].newy = newy;
				av[y][x].newx = newx;
				av[y][x].anew = anew;
				av[y][x].dirnew.y = dstd[newy][newx].y;
				av[y][x].dirnew.x = dstd[newy][newx].x;
#endif
				if(anew >= 0) continue;

				dsta[y][x] = anew;
				dsta[newy][newx] = aold;

				//dstd[newy][newx].y = dstd[newy][newx].x = 0; // Block next moves by ncell
				dstd[y][x].y = dstd[y][x].x = 0;

				//dstpos[aold].Pos.y = (+(ky * newy - 1.0f)) * kwidthy;
				//dstpos[aold].Pos.x = (kx * newx - 1.0f) * kwidthx;
				dstpos[aold].Pos.y = normal(newy, szy);
				dstpos[aold].Pos.x = normal(newx, szx);
			}
		}); // parallel_for_each(srcd.extent,
#ifdef AMPDBG_DLAST
		av.synchronize();
		//avpos.synchronize();
		//dumpA(-1);
		//dumpDLast();
		//dumpPos();
#endif
	} // for(nshift
} // ///////////////////////////////////////////////////////////////////////////////////////////////
float normal(int pos, int width) restrict(amp, cpu){
	return float(2 * pos + 1) / width - 1.f;
} // //////////////////////////////////////////////////////////////////////////////////////////////

#ifdef AMPDBG_DLAST
#undef AMPDBG_DLAST
#endif
