#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <mutex>
#include <vector>
#include "System/ClassBase/Singleton.h"

#include "Graphics/RenderContext.h"

class Graphics : public Singleton<Graphics>
{
public:
	Graphics(HWND hWnd);
	~Graphics() override;

	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return immediate_context.Get(); }
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }
	float GetScreenWidth() const { return screen_width; }
	float GetScreenHeight() const { return screen_height; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediate_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;

	enum class SAMPLER_STATE { POINT_WRAP, LINEAR_WRAP, ANISOTROPIC_WRAP, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> sampler_states;

	enum class DEPTH_STATE { ZT_ON_ZW_ON/*�[�x�e�X�g_ON �[�x�o�b�t�@��������_ON*/, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> depth_stencil_states;

	enum class BLEND_STATE { NONE, ALPHA, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11BlendState>> blend_states;

	enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11RasterizerState>> rasterizer_states;

	float	screen_width;
	float	screen_height;
};

