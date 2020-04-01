#pragma once
#include <amp.h>
#include <Vertex.h>
#include <DrShiftQuadro.h>
#include <amp_graphics.h>
#include <D3D11.h>
#include "Model2D.h"

extern Model2D model;

using namespace Concurrency;
//using namespace concurrency::direct3d;

class AMPEng2{
	accelerator_view					m_accl_view;
	//std::unique_ptr<array<int, 2>>      ar_area;

	std::unique_ptr<array<Vertex2D, 1>>	m_data;

	std::unique_ptr<array<FLT2, 2>> last_dirs;

	std::vector<std::unique_ptr<array<int, 2>>> var_areas;
	std::vector<std::unique_ptr<array<DrShiftQuadro, 2>>> var_dirs;

						// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
						// 00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
						// 00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
	const int AMask[16] = {0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};
	std::unique_ptr<array<int, 1>> amask;

public:
	AMPEng2(ID3D11Device* d3ddevice) : m_accl_view(Concurrency::direct3d::create_accelerator_view(d3ddevice)){}
	void initialize_data();

}; // *******************************************************************************************************

