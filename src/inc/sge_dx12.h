#pragma once

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <windows.h>

#include "sge_macros.h"
// D3D12 extension library.

namespace SGE::DirectX12
{
	
	class Engine
	{
	public:
		Engine();
		void Initialize(HWND target);
		void Update();
	private:
		ID3D12Device* _d3d12Device;					// direct3d device
		IDXGISwapChain* _swapChain;					// swapchain used to switch between render targets
		ID3D12CommandQueue* _commandQueue;			// container for command lists
		ID3D12DescriptorHeap* _rtvDescriptorHeap;	// a descriptor heap to hold resources like the render targets
		ID3D12Resource* _renderTargets[SGE_ENGINE_FRAME_BUFFER_COUNT];				// number of render targets equal to buffer count
		ID3D12CommandAllocator* _commandAllocator[SGE_ENGINE_FRAME_BUFFER_COUNT];	// we want enough allocators for each buffer * number of threads (we only have one thread)
		ID3D12GraphicsCommandList* _commandList;									// a command list we can record commands into, then execute them to render the frame
		ID3D12Fence* _fence[SGE_ENGINE_FRAME_BUFFER_COUNT];							// an object that is locked while our command list is being executed by the gpu. We need as many 
		HANDLE _fenceEvent;															// a handle to an event when our fence is unlocked by the gpu
		UINT64 _fenceValue[SGE_ENGINE_FRAME_BUFFER_COUNT];							// this value is incremented each frame. each fence will have its own value
		int _frameIndex;															// current rtv we are on
		int _rtvDescriptorSize;														// size of the rtv descriptor on the device (all front and back buffers will be the same size)
		HWND _renderTarget;
	};
}
