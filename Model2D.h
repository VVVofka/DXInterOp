#pragma once
#include <random>
#include <cmath>
#include <vector>

//#pragma warning(push)
//#pragma warning(disable:4005 26812) 
#include <D3D11.h>
#include <DirectXMath.h>
//#pragma warning(pop)

#include "Utils.h"
#include "Vertex.h"
#include "Masks.h"
#include "Options.h"

class Model2D{
public:
	Model2D(){}
	Options options;
	std::vector<std::vector<Vertex2D>> v_poss;
	std::vector<std::vector<int>> v_areas;
	std::vector<std::vector<DrQuadro>> v_dirs;
	std::vector<FLT2> last_dirs;
	std::vector<INT2> vsz;

	std::vector<int> lastArea() const { return v_areas[v_areas.size() - 1]; }
	std::vector<Vertex2D> lastPoss() const { return v_poss[v_poss.size() - 1]; }

	int sizeY(int nlay) const { return vsz[nlay].y; }
	int sizeY() const { return vsz[vsz.size() - 1].y; }
	int sizeX(int nlay) const { return vsz[nlay].x; }
	int sizeX() const { return vsz[vsz.size() - 1].x; }
	const INT2& sizeYX(int nlay) const {return vsz[nlay];}
	const INT2& sizeYX() const {return vsz[vsz.size() - 1];}
	int LaysCnt() const { return (int)v_areas.size(); }

	void Create();
	void dumpA(int nlay) const;
	void dumpD(int nlay) const;
	FLT2* getFLT2(){return options.getFLT2();}

private:
	Vertex2D norm(int curpos, INT2 sizes) const;
	void fillrnd(int nlay, size_t szarea, double kFill, DBL2 kSigma);
	void filltest(int nlay);
}; // *****************************************************************************

