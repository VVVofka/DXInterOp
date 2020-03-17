//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#include <DirectXMath.h>
#define MYAREA
//#define MY2D
//#define MY3D


#ifndef SAFE_DELETE
    #define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    

#ifndef SAFE_DELETE_ARRAY
    #define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE_DELETE
    #define SAFE_RELEASE_DELETE(p)	{ if(p) { (p)->Release(); delete (p); (p)=NULL; } }
#endif
#ifndef RETURN_IF_FAIL
    #define RETURN_IF_FAIL(x)    { HRESULT thr = (x); if (FAILED(thr)) { return E_FAIL; } }
#endif

struct Vertex2D{
    Vertex2D(){Pos.x = Pos.y = 0;}
    Vertex2D(float y, float x){ Pos.x = x; Pos.y = y; }
    DirectX::XMFLOAT2 Pos;
};
struct Vertex3D {
    DirectX::XMFLOAT3 Pos;
};
