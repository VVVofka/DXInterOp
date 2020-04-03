#pragma once
#include <memory>
#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include <numeric>
#include <iomanip>
#include <DirectXMath.h>
#include "DXInterOp.h"
#include "Model2D.h"
#include "Masks.h"
#include "RunA.h"
#include "RunD.h"
#include "RunDlast.h"
//#include <ppl.h>
//#define AMPDBG
#define THETA 3.1415f/1024  
extern Model2D model;

using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::direct3d;

class AMPEngine2{
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
	AMPEngine2(ID3D11Device* d3ddevice) : m_accl_view(create_accelerator_view(d3ddevice)){}
#ifndef MYAREA
	void initialize_data(const std::vector<Vertex2D>& data){
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(data.size(), data.begin(), m_accl_view));
#else
	void initialize_data(){
		//setConsole();
		auto layscnt = model.v_areas.size();
		for(size_t nlay = 0; nlay < layscnt; nlay++){
			int sizey = model.sizeY(nlay);
			int sizex = model.sizeX(nlay);

			var_areas.push_back(std::unique_ptr<array<int, 2>>());
			var_areas[nlay] = std::unique_ptr<array<int, 2>>
				(new array<int, 2>(sizey, sizex, model.v_areas[nlay].begin(), m_accl_view));

			if(nlay < layscnt - 1){
				var_dirs.push_back(std::unique_ptr<array<DrShiftQuadro, 2>>());
				var_dirs[nlay] = std::unique_ptr<array<DrShiftQuadro, 2>>
					(new array<DrShiftQuadro, 2>(sizey, sizex, model.v_shiftdirs[nlay].begin(), m_accl_view));
			}
		}
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(model.posLast()->size(), model.posLast()->begin(), m_accl_view));
		last_dirs = std::unique_ptr<array<FLT2, 2>>(new array<FLT2, 2>(model.sizeY(), model.sizeX(), model.last_dirs.begin(), m_accl_view));
		amask = std::unique_ptr<array<int, 1>>(new array<int, 1>(16, AMask, m_accl_view));
#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run(){
		//return;
		int nlastlay = model.LaysCnt() - 1;
		RunA::runlast(*var_areas[nlastlay], *var_areas[nlastlay - 1], *amask);
		for(int nlay = nlastlay - 1; nlay > 0; nlay--){
			RunA::run(*var_areas[nlay], *var_areas[nlay - 1], *amask);
		}
		// Back to down
#ifdef AMPDBG
		//dumpA();
#endif
		for(int nlay = 1; nlay < nlastlay; nlay++){
			RunD::run(*var_dirs[nlay - 1], *var_dirs[nlay], *var_areas[nlay]);
#ifdef AMPDBG
			//dumpD(nlay);
#endif
		}
		array<FLT2, 2>& dirs = *last_dirs;
		RunDlast::run(*var_dirs[nlastlay - 1], *m_data, *var_areas[nlastlay], dirs, model.sizeY(), model.sizeX());
        //concurrency::copy(*m_data, vpos.data());
		//for(int n=0; n<(int)vpos.size(); n++) printf("%d\t%f\t%f\n", n, vpos[n].Pos.y, vpos[n].Pos.x);
#ifdef AMPDBG
		//dumpPos();
		//dumpDLast();
		//dumpA();
#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////

	void runbak(){
		array<Vertex2D, 1>& data_ref = *m_data;
		parallel_for_each(m_data->extent, [=, &data_ref](index<1> idx) restrict(amp){
			// Rotate the vertex by angle THETA
			DirectX::XMFLOAT2 pos = data_ref[idx].Pos;
			data_ref[idx].Pos.y = pos.y * cos(THETA) - pos.x * sin(THETA);
			data_ref[idx].Pos.x = pos.y * sin(THETA) + pos.x * cos(THETA);
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Vertex2D>* return_data(std::vector<Vertex2D> * vreturn){
		concurrency::copy(*m_data, begin(*vreturn));
		return vreturn;
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void dumpA(int nlay){
		const char separ[] = " ";
		if(nlay < 0) nlay = model.LaysCnt() - 1;
		setConsole();
		array<int, 2> av(*var_areas[nlay].get());
		std::cout << "A[" << nlay << "] y*x: " << av.extent[0] << "*" << av.extent[1] << std::endl;
		if(nlay == int(var_areas.size() - 1)){
			for(int y = 0; y < av.extent[0]; y++){
				for(int x = 0; x < av.extent[1]; x++){
					int q = av[y][x];
					if(q < 0)
						std::cout << " . ";
					else
						printf("%2d ", q);
				}
				std::cout << std::endl;
			}
		} else{
			for(int y = 0; y < av.extent[0]; y++){
				for(int x = 0; x < av.extent[1]; x++){
					int q = av[y][x];
					if(q < 0)
						std::cout << ". ";
					else
						std::cout << (q >> 3) << (1 & (q >> 2)) << (1 & (q >> 1)) << (1 & q) << separ;
				}
				std::cout << std::endl;
			}
		}
	} // ////////////////////////////////////////////////////////////////
	void dumpA(){
		setConsole();
		for(int nlay = 0; nlay < model.LaysCnt(); nlay++){
			dumpA(nlay);
			std::cout << std::endl;
		}
	} // ////////////////////////////////////////////////////////////////////////////////////////
	void dumpD(int nlay){
		if(nlay < 0) nlay = model.LaysCnt() - 1;
		setConsole();
		array<DrShiftQuadro, 2> av(*var_dirs[nlay].get());
		std::cout << "Dirs[" << nlay << "] y*x: " << av.extent[0] << "*" << av.extent[1] << std::endl;
		for(int y = 0; y < av.extent[0]; y++){
			for(int x = 0; x < av.extent[1]; x++){
				//if(av[y][x].not0()){
				std::cout << " Y=" << y << " X=" << x << std::endl;
				av[y][x].dump();
				std::cout << std::endl;
			//}
			}
		}
		std::cout << std::endl;
	} // ////////////////////////////////////////////////////////////////
	void dumpD(){
		for(int nlay = 0; nlay < model.LaysCnt() - 1; nlay++)
			dumpD(nlay);
		dumpDLast();
	} // ////////////////////////////////////////////////////////////////////////////////////////
	void dumpDLast(){
		int szy = last_dirs->extent[0],
			szx = last_dirs->extent[1];
		if(szy <= 0 || szx <= 0) return;
		array<FLT2, 2> av(*last_dirs);
		std::cout << "DirsLast[" << model.LaysCnt() - 1 << "] y*x: " << szy << "*" << szx;
		for(int y = 0; y < szy; y++){
			printf("\n%+.2f", av[0][0].y);
			for(int x = 1; x < szx; x++)
				printf("  %+.2f", av[y][x].y);

			printf("\n%+.2f", av[0][0].x);
			for(int x = 1; x < szx; x++)
				printf("  %+.2f", av[y][x].x);
			printf("\n");
		}
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void dumpPos(){
		array_view<Vertex2D, 1> avpos(m_data->extent[0]);
		m_data.get()->copy_to(avpos);
		for(int n = 0; n < (int)avpos.extent[0]; n++){
			auto p = avpos[n].Pos;
			printf("%d:\t%+.3f %+.3f\n", n, p.y, p.x);
		}
	} // ////////////////////////////////////////////////////////////////////////////////////////
}; // ******************************************************************************************************
#ifdef AMPDBG
#undef AMPDBG
#endif
