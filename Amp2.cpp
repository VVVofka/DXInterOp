#include "Model2D.h"
#include <amp.h>
using namespace concurrency;

//void getMaskDir(const int y2, const int x2, array<int, 2>& dsta, int* mask) restrict(amp){
//	// yx: l-left, r-right
//	int tl = dsta[y2][x2];
//	int tr = dsta[y2][x2 + 1];
//	int bl = dsta[y2 + 1][x2];
//	int br = dsta[y2 + 1][x2 + 1];
//	mask[0] = 16 * ((tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3));
//	mask[1] = 16 * (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
//	mask[2] = 16 * (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
//	mask[3] = 16 * (((tl >> 1) & 1) + (((tr >> 1) & 1) << 1) + (((bl >> 1) & 1) << 2) + (((br >> 1) & 1) << 3));
//} // //////////////////////////////////////////////////////////////////////////////////

