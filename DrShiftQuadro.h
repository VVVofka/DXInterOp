#pragma once
#include <iostream>
#include "Utils.h"
struct DrShiftQuadro{
	struct DrQuadro{
		FLT2 items[4];
		bool not0(){ return items[0].not0() || items[1].not0() || items[2].not0() || items[3].not0(); }
		void dump(){
			for(int i = 0; i < 4; i++){
				printf("\ti:%d y:%+.1f x:%+.1f", i, items[i].y, items[i].x);
			}
		} // ///////////////////////////////////////////////////////////////////////////////////////
	}; // ********************************************************************************************
	DrQuadro shifts[4];
	bool not0(){ return shifts[0].not0() || shifts[1].not0() || shifts[2].not0() || shifts[3].not0(); }
	void dump(){
		for(int sh = 0; sh < 4; sh++){
			std::cout << "shift:" << sh << "\t";
			shifts[sh].dump();
			std::cout << std::endl;
		}
	} // //////////////////////////////////////////////////////////////////////////////////////////
}; // ********************************************************************************************
