#pragma once

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// DirectX 12 specific headers.
#include <d3d12.h>
#include <wrl.h>
#include "inc/sge_window.h"

// D3D12 extension library.

namespace SGE::Engine
{
	class SGEApp
	{
	public:
		SGEApp(const SGE::Windows::WindowBase& renderTargetMain);
		void Initialize();

	private:
		std::shared_ptr<Windows::WindowBase> _renderTargetMain;
		Microsoft::WRL::ComPtr<ID3D12Device> _d3d12Device;
	};
}
