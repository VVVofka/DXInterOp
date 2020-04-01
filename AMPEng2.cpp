#include "AMPEng2.h"
void AMPEng2::initialize_data(){
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
				(new array<DrShiftQuadro, 2>(sizey, sizex, model.v_dirs[nlay].begin(), m_accl_view));
		}
	}
	m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(model.posLast()->size(), model.posLast()->begin(), m_accl_view));
	last_dirs = std::unique_ptr<array<FLT2, 2>>(new array<FLT2, 2>(model.sizeY(), model.sizeX(), model.last_dirs.begin(), m_accl_view));
	amask = std::unique_ptr<array<int, 1>>(new array<int, 1>(16, AMask, m_accl_view));
} // ///////////////////////////////////////////////////////////////////////////////////////////////

