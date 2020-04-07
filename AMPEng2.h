#pragma once
#include <amp.h>
#include <amp_graphics.h>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4005 26812) 
#include <D3D11.h>
#pragma warning(pop)

#include "Model2D.h"
#include "Vertex.h"
#include "RunA.h"
#include "RunD.h"
#include "RunDlast.h"
#include "Masks.h"
#include "Utils.h"

extern Model2D model;

using namespace Concurrency;
//using namespace concurrency::direct3d;

class AMPEng2{
	accelerator_view					m_accl_view;

	std::unique_ptr<array<Vertex2D, 1>>	m_data; // for screen
	std::vector<std::unique_ptr<array<int, 2>>> var_areas;
	std::vector<std::unique_ptr<array<DrQuadro, 2>>> var_dirs;
	std::unique_ptr<array<FLT2, 2>> last_dirs;
	std::unique_ptr<array<int, 1>> amask;
	std::unique_ptr<array<FLT2, 1>> dmask;

public:
	AMPEng2(ID3D11Device* d3ddevice) : m_accl_view(Concurrency::direct3d::create_accelerator_view(d3ddevice)){
	} // /////////////////////////////////////////////////////////////////////////////////////////////////////

	void initialize_data();
	void run();

	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return Concurrency::direct3d::get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////

private:
						// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
						// 00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
						// 00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
    const int AMask[16] = {0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};  // line=1 diag=1 ++ (large cave)
  //const int AMask[16] = {0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1};  // line=1 diag=0 +
  //const int AMask[16] = {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};  // line=0 diag=1 quadro
  //const int AMask[16] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1};  // line=0 diag=0 quadro
	
	std::random_device rd;   // non-deterministic generator
	std::mt19937 gen;  // to seed mersenne twister. rand: gen(rd())
	std::uniform_int_distribution<int> distLastAY, distLastAX;
	int nlastlay = -1;  // N last lay
	Blocks2D2 blocks2D2;

public:
	void dumpA(int nlay);
	void dumpA();
	void dumpD(int nlay);
	void dumpD();
	void dumpDLast();
	void dumpPos();
}; // *******************************************************************************************************

