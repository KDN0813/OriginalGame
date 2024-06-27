#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Graphics/Shader/ShaderLoader.h"
#include "Model/Model.h"

#include "Graphics/Shader/LambertShader.h"

Graphics* Graphics::instance = nullptr;

Graphics::Graphics(HWND hWnd)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	// 画面のサイズを取得する。
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT screen_width_uint = rc.right - rc.left;
	UINT screen_height_uint = rc.bottom - rc.top;

	this->screen_width = static_cast<float>(screen_width_uint);
	this->screen_height = static_cast<float>(screen_height_uint);

	HRESULT hr = S_OK;

	// デバイス＆スワップチェーンの生成
	{
		UINT create_device_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// スワップチェーンを作成するための設定オプション
		DXGI_SWAP_CHAIN_DESC swapchain_desc;
		{
			swapchain_desc.BufferDesc.Width = screen_width_uint;
			swapchain_desc.BufferDesc.Height = screen_height_uint;
			swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.SampleDesc.Quality = 0;
			swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchain_desc.BufferCount = 1;
			swapchain_desc.OutputWindow = hWnd;
			swapchain_desc.Windowed = TRUE;
			swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchain_desc.Flags = 0;
		}

		D3D_FEATURE_LEVEL featureLevel;

		// デバイス＆スワップチェーンの生成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			create_device_flags,
			feature_levels,
			ARRAYSIZE(feature_levels),
			D3D11_SDK_VERSION,
			&swapchain_desc,
			this->swapchain.GetAddressOf(),
			this->device.GetAddressOf(),
			&featureLevel,
			this->immediate_context.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// レンダーターゲットビューの生成
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), nullptr, this->render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルビューの生成
	{
		D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc;
		depth_stencil_buffer_desc.Width = screen_width_uint;
		depth_stencil_buffer_desc.Height = screen_height_uint;
		depth_stencil_buffer_desc.MipLevels = 1;
		depth_stencil_buffer_desc.ArraySize = 1;
		depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_buffer_desc.SampleDesc.Count = 1;
		depth_stencil_buffer_desc.SampleDesc.Quality = 0;
		depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_buffer_desc.CPUAccessFlags = 0;
		depth_stencil_buffer_desc.MiscFlags = 0;
		hr = this->device->CreateTexture2D(&depth_stencil_buffer_desc, nullptr, this->depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = this->device->CreateDepthStencilView(this->depth_stencil_buffer.Get(), nullptr, this->depth_stencil_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ビューポートの設定
	{
		// 画面のどの領域にDirectXで描いた画を表示するかの設定。
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screen_width_uint);
		viewport.Height = static_cast<float>(screen_height_uint);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		this->immediate_context->RSSetViewports(1, &viewport);
	}

	// サンプラーステート
	{
		this->sampler_states.resize(static_cast<size_t>(SAMPLER_STATE::MAX));

		D3D11_SAMPLER_DESC sampler_desc{};
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = this->device->CreateSamplerState(&sampler_desc, this->sampler_states[static_cast<size_t>(SAMPLER_STATE::POINT_WRAP)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = this->device->CreateSamplerState(&sampler_desc, this->sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_WRAP)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = this->device->CreateSamplerState(&sampler_desc, this->sampler_states[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC_WRAP)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルステートの生成
	{
		this->depth_stencil_states.resize(static_cast<size_t>(DEPTH_STATE::MAX));

		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		depth_stencil_desc.DepthEnable = FALSE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		depth_stencil_desc.DepthEnable = FALSE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ブレンドステートの生成
	{
		this->blend_states.resize(static_cast<size_t>(BLEND_STATE::MAX));

		D3D11_BLEND_DESC blend_desc{};
		blend_desc.AlphaToCoverageEnable = FALSE;
		blend_desc.IndependentBlendEnable = FALSE;
		blend_desc.RenderTarget[0].BlendEnable = FALSE;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = this->device->CreateBlendState(&blend_desc, this->blend_states[static_cast<size_t>(BLEND_STATE::NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		blend_desc.AlphaToCoverageEnable = FALSE;
		blend_desc.IndependentBlendEnable = FALSE;
		blend_desc.RenderTarget[0].BlendEnable = TRUE;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = this->device->CreateBlendState(&blend_desc, this->blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].GetAddressOf());
	}

	// ラスタライザーステートの生成
	{
		this->rasterizer_states.resize(static_cast<size_t>(RASTER_STATE::MAX));

		D3D11_RASTERIZER_DESC rasterizer_desc{};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.CullMode = D3D11_CULL_BACK;
		rasterizer_desc.FrontCounterClockwise = TRUE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = this->device->CreateRasterizerState(&rasterizer_desc, this->rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizer_desc.CullMode = D3D11_CULL_BACK;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = this->device->CreateRasterizerState(&rasterizer_desc, this->rasterizer_states[static_cast<size_t>(RASTER_STATE::WIREFRAME)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = this->device->CreateRasterizerState(&rasterizer_desc, this->rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = this->device->CreateRasterizerState(&rasterizer_desc, this->rasterizer_states[static_cast<size_t>(RASTER_STATE::WIREFRAME_CULL_NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// TODO(06/27) GPUインスタンシング実装後シェーダの設計を考える
	// シェーダの作成
	{
		shader = std::make_unique<LambertShader>(this->device.Get());
	}
}

Graphics::~Graphics()
{
}