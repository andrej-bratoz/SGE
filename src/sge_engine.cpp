
#include "inc/sge_engine.h"
#include "inc/sge_utils.h"
/*
SGE::DirectX12::Engine::Engine() : _d3d12Device(nullptr),
								_swapChain(nullptr),
								_commandQueue(nullptr),
								_rtvDescriptorHeap(nullptr),
								_renderTargets{},
								_commandAllocator{},
								_commandList(nullptr),
								_fence{},
								_fenceEvent(nullptr),
								_fenceValue{},
								_frameIndex(0),
								_rtvDescriptorSize(0)
{
}

void SGE::DirectX12::Engine::Initialize(HWND target)
{
	
	HRESULT hr;
	IDXGIFactory4* dxgiFactory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	_renderTarget = target;
}

*/
SGE::Engine::SGEApp::SGEApp(const SGE::Windows::WindowBase& renderTargetMain)
{
	_renderTargetMain = std::make_shared<Windows::WindowBase>(renderTargetMain);
}

void SGE::Engine::SGEApp::Initialize()
{
	SGE_CHECKED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), (IID_PPV_ARGS(&_d3d12Device))));
}
