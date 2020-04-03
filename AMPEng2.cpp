#include "AMPEng2.h"
void AMPEng2::initialize_data(){
	gen = std::mt19937(2020);  // to seed mersenne twister. rand: gen(rd())
	distLastAY = std::uniform_int_distribution<int>(0, model.sizeY());
	distLastAX = std::uniform_int_distribution<int>(0, model.sizeX());
	nlastlay = model.LaysCnt() - 1; // N last lay

	//setConsole();
	auto layscnt = model.v_areas.size();
	for(int nlay = 0; nlay < layscnt; nlay++){
		int sizey = model.sizeY(nlay);
		int sizex = model.sizeX(nlay);

		var_areas.push_back(std::unique_ptr<array<int, 2>>());
		var_areas[nlay] = std::unique_ptr<array<int, 2>>
			(new array<int, 2>(sizey, sizex, model.v_areas[nlay].begin(), m_accl_view));

		if(nlay < layscnt - 1){
			var_dirs.push_back(std::unique_ptr<array<DrQuadro, 2>>());
			var_dirs[nlay] = std::unique_ptr<array<DrQuadro, 2>>
				(new array<DrQuadro, 2>(sizey, sizex, model.v_dirs[nlay].begin(), m_accl_view));
		}
	}
	m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(int(model.posLast()->size()), model.posLast()->begin(), m_accl_view));
	last_dirs = std::unique_ptr<array<FLT2, 2>>(new array<FLT2, 2>(model.sizeY(), model.sizeX(), model.last_dirs.begin(), m_accl_view));
	amask = std::unique_ptr<array<int, 1>>(new array<int, 1>(16, AMask, m_accl_view));
	dmask = std::unique_ptr<array<FLT2, 1>>(new array<FLT2, 1>(16 * 16, blocks2D2.vin, m_accl_view));
} // ///////////////////////////////////////////////////////////////////////////////////////////////
void AMPEng2::run(){
	//return;
	INT2 rnd = INT2(distLastAY(gen), distLastAX(gen));
	RunA::RunLast(rnd, *var_areas[nlastlay], *var_areas[nlastlay - 1], *amask);

	for(int nlay = nlastlay - 1; nlay > 0; nlay--){
		RunA::Run(*var_areas[nlay], *var_areas[nlay - 1], *amask);
	}
	// Back to down
	for(int nlay = 1; nlay < nlastlay; nlay++){
		RunD::Run(*var_dirs[nlay - 1], *var_dirs[nlay], *var_areas[nlay], *dmask);
		//concurrency::copy(*m_data, vpos.data());
		//for(int n=0; n<(int)vpos.size(); n++) printf("%d\t%f\t%f\n", n, vpos[n].Pos.y, vpos[n].Pos.x);
	}
	array<FLT2, 2>& dirs = *last_dirs;
	RunDlast::Run(rnd, *var_dirs[nlastlay - 1], *m_data, *var_areas[nlastlay], dirs, model.sizeY(), model.sizeX());
} // ///////////////////////////////////////////////////////////////////////////////////////////////

