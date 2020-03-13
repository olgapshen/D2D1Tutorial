#include "common.h"

HRESULT DX11_CreateDevice(IDXGIAdapter* _pAdapter, ID3D11Device** _ppD3D1Device, ID3D11DeviceContext** _ppD2D1DeviceContext)
{
    DXGI_ADAPTER_DESC dxgiDesc = {};
    CComQIPtr<IDXGIAdapter> qpDXGIAdapter(_pAdapter);
    if (qpDXGIAdapter)
        qpDXGIAdapter->GetDesc(&dxgiDesc);

    CComPtr<ID3D11Device> cpD3D11Device;
    D3D_FEATURE_LEVEL d3dFeature = (D3D_FEATURE_LEVEL)0;
    D3D_FEATURE_LEVEL arrD3DFeatures[] = {
        //D3D_FEATURE_LEVEL_11_2, // TODO
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1 };


    HRESULT hr = S_OK;

    if (!cpD3D11Device)
    {
        RET_HR(D3D11CreateDevice(
            qpDXGIAdapter,
            qpDXGIAdapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            arrD3DFeatures,
            SIZEOF_ARRAY(arrD3DFeatures),
            D3D11_SDK_VERSION,
            &cpD3D11Device,
            &d3dFeature,
            _ppD2D1DeviceContext
        ));
    }

    CComQIPtr<ID3D10Multithread> qpMTProtect(cpD3D11Device);
    ATLASSERT(qpMTProtect);
    if (qpMTProtect)
        qpMTProtect->SetMultithreadProtected(TRUE);

    return cpD3D11Device.QueryInterface(_ppD3D1Device);
}

HRESULT DXGI_AdapterByType(eGPU_Type _eGPUType, IDXGIAdapter** _ppDXGIAdapter)
{
	CComPtr<IDXGIFactory> cpDXGIFactory;
	HRESULT hr = S_OK;

	RET_HR(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&cpDXGIFactory)));

	CComPtr<IDXGIAdapter> cpDXGIAdapterRes;
	int nIndex = 0;
	while (true)
	{
		CComPtr<IDXGIAdapter> cpDXGIAdapter;
		cpDXGIFactory->EnumAdapters(0, &cpDXGIAdapter);
		if (!cpDXGIAdapter)
			break;

		DXGI_ADAPTER_DESC dxgiDesc = {};
		cpDXGIAdapter->GetDesc(&dxgiDesc);

		LPWORD pwVendorID = (LPWORD)&_eGPUType;

		if (_eGPUType == eGPU_Any ||
			pwVendorID[0] == dxgiDesc.VendorId ||
			pwVendorID[1] == dxgiDesc.VendorId)
		{
			cpDXGIAdapterRes = cpDXGIAdapter.p;
            break;
		}

		++nIndex;
	}

    return cpDXGIAdapterRes.QueryInterface(_ppDXGIAdapter);
}