#pragma once

#include <memory>
#include <d3d11_1.h>
#include <wrl.h>
#include <mutex>
#include <vector>
#include <dxgi1_6.h>
#include "System/ClassBase/Singleton.h"

#include "Graphics/RenderContext.h"

class Graphics : public Singleton<Graphics>
{
public:
	Graphics(HWND hWnd);
	~Graphics() override;

	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return immediate_context.Get(); }
	IDXGISwapChain* GetSwapChain() const { return swap_chain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }
	float GetScreenWidth() const { return screen_width; }
	float GetScreenHeight() const { return screen_height; }
	BOOL GetTearingSupported()const { return this->tearing_supported; }

	// 描画準備
	// レンダーターゲットビューとデプスステンシルビューのクリアおよびレンダリングターゲットの設定を行う
	void PrepareRenderTargets(DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediate_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>			swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;

	enum class SAMPLER_STATE { POINT_WRAP, LINEAR_WRAP, ANISOTROPIC_WRAP, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> sampler_states;

	enum class DEPTH_STATE { ZT_ON_ZW_ON/*深度テスト_ON 深度バッファ書き込み_ON*/, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> depth_stencil_states;

	enum class BLEND_STATE { NONE, ALPHA, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11BlendState>> blend_states;

	enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE, MAX };
	std::vector<Microsoft::WRL::ComPtr<ID3D11RasterizerState>> rasterizer_states;

	float	screen_width;
	float	screen_height;

	BOOL tearing_supported{ FALSE };
};

