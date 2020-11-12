
#include "inc/sge_dx12.h"
#include "inc/sge_macros.h"

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
