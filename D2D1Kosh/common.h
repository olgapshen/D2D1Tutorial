#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <atlbase.h>
#include <d3d11.h>
#include <dxgi1_3.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define RET_HR(_f) hr = _f; if (FAILED(hr)) return hr;
#define RET_HR_NULL( _f, _obj ) hr = _f; if (FAILED(hr) || !_obj) return FAILED(hr) ? hr : S_FALSE;
#define SIZEOF_ARRAY(arr)	(sizeof(arr)/sizeof((arr)[0]))

enum eGPU_Type
{
    eGPU_Any = 0,
    eGPU_AMD = 0x10021022, // 0x1002, 0x1022 ?
    eGPU_NVidia = 0x10DE,
    eGPU_Intel = 0x8086163C // 0x163C, 0x8086, 0x8087 ?
};

template<class Interface>
inline void SafeRelease(Interface * *ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

HRESULT DXGI_AdapterByType(eGPU_Type _eGPUType, IDXGIAdapter** _ppDXGIAdapter);
HRESULT DX11_CreateDevice(IDXGIAdapter* _pAdapter, ID3D11Device** _ppD3D1Device, ID3D11DeviceContext** _ppD2D1DeviceContext = NULL);