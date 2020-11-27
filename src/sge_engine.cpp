
#include "inc/sge_engine.h"
#include "inc/sge_utils.h"

SGE::Engine::SGEApp::SGEApp(const SGE::Windows::WindowBase& renderTargetMain)
{
	_renderTargetMain = std::make_shared<Windows::WindowBase>(renderTargetMain);
}

void SGE::Engine::SGEApp::Initialize()
{
	SGE_CHECKED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), (IID_PPV_ARGS(&_d3d12Device))));
}
