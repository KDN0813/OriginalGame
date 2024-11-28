#include "SkyBox.h"
#include "System\Misc.h"
#include "Graphics\Graphics.h"
#include "Shader\ShaderLoader.h"
#include "System\MyMath\MYMATRIX.h"
#include "Camera\CameraManager.h"

#include "Component\SpriteComponent.h"
#include "Component\CameraComponent.h"

SkyBox::SkyBox()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr{};

	// 入力レイアウト
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// 頂点シェーダー
	hr = CreateShader::VsFromCso(device, "Shader\\SkyBoxVS.cso", this->vertex_shader.GetAddressOf(), this->input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ピクセルシェーダー
	hr = CreateShader::PsFromCso(device, "Shader\\SkyBoxPS.cso", this->pixel_shader.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(SceneConstantBuffer);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, this->scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, this->blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, this->depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, this->rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		HRESULT hr = device->CreateSamplerState(&desc, this->sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void SkyBox::Render()
{
	const auto& texture = texture_Wptr.lock();
	if (!texture) return;

	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();

	dc->VSSetShader(this->vertex_shader.Get(), nullptr, 0);
	dc->PSSetShader(this->pixel_shader.Get(), nullptr, 0);
	dc->IASetInputLayout(this->input_layout.Get());

	ID3D11Buffer* constant_buffers[] =
	{
		this->scene_constant_buffer.Get(),
	};
	dc->VSSetConstantBuffers(0, ARRAYSIZE(constant_buffers), constant_buffers);
	dc->PSSetConstantBuffers(0, ARRAYSIZE(constant_buffers), constant_buffers);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(this->blend_state.Get(), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(this->depth_stencil_state.Get(), 0);
	dc->RSSetState(this->rasterizer_state.Get());
	dc->PSSetSamplers(0, 1, this->sampler_state.GetAddressOf());

	// シーン用定数バッファ更新
	SceneConstantBuffer scene_CB{};
	RenderContext rc{};

	// RenderContext設定
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		if (!camera_manager.Get()) return;
		std::shared_ptr<CameraComponent> camera = camera_manager->GetCurrentCamera();
		if (camera)
		{
			rc.view = camera->GetViewTransform();
			rc.projection = camera->GetProjectionTransform();
		}
	}

	MYMATRIX View = rc.view;
	MYMATRIX Projection = rc.projection;
	MYMATRIX View_projection = View * Projection;

	View_projection.GetFlaot4x4(scene_CB.view_projection);

	dc->UpdateSubresource(this->scene_constant_buffer.Get(), 0, 0, &scene_CB, 0, 0);

	texture->Render(dc);
}

#ifdef _DEBUG

void SkyBox::DrawDebugGUI()
{
}

#endif // DEBUG