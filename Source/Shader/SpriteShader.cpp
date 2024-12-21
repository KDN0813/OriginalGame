#include <algorithm>
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // _DEBUG
#include "SpriteShader.h"
#include "Graphics/Graphics.h"

#include "Object\Object.h"
#include "Component/BaseSpriteComponent.h"

SpriteShader::SpriteShader()
	:Singleton(this)
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get() == nullptr)return;
	ID3D11Device* device = graphics->GetDevice();

	HRESULT hr;

	// ブレンドステート作成
	{
		D3D11_BLEND_DESC blend_desc{};
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
		hr = device->CreateBlendState(&blend_desc, this->blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステート作成
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザステート
	{
		D3D11_RASTERIZER_DESC rasterizer_desc{};
		rasterizer_desc.FrontCounterClockwise = TRUE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = device->CreateRasterizerState(&rasterizer_desc, this->rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステートの生成
	{
		D3D11_SAMPLER_DESC sampler_desc{};
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
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
		hr = device->CreateSamplerState(&sampler_desc, this->sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void SpriteShader::Render()
{
    Graphics::Instance graphics = Graphics::GetInstance();
    if (!graphics.Get()) return;
    ID3D11DeviceContext* const dc = graphics->GetDeviceContext();
	if (!dc) return;

	// 初期設定
	{
		// ブレンドステート設定
		dc->OMSetBlendState(this->blend_state.Get(), nullptr, 0xFFFFFFFF);
		// 深度ステンシルステートの設定
		dc->OMSetDepthStencilState(this->depth_stencil_state.Get(), 0);
		// ラスタライザーステートの設定
		dc->RSSetState(this->rasterizer_state.Get());
		// サンプラーステートの設定
		dc->PSSetSamplers(0, 1, this->sampler_state.GetAddressOf());
	}

    // 有効でないweak_ptrを除去する
    this->sprite_pool.erase(
        std::remove_if(this->sprite_pool.begin(), this->sprite_pool.end(),
            [](const std::weak_ptr<BaseSpriteComponent>& weakPtr)
            {
                return weakPtr.expired();  // 無効なweak_ptrかどうかを確認
            }),
        this->sprite_pool.end());

    for (auto sprite_Wptr : this->sprite_pool)
    {
        auto sprite = sprite_Wptr.lock();
        if (!sprite) continue;
        if (!sprite->GetIsActive())continue;
        if (sprite->GetOwner() && !sprite->GetOwner()->GetIsActive())continue;

        sprite->Render(dc);
    }
}

void SpriteShader::AddSprite(std::shared_ptr<BaseSpriteComponent> sprite)
{
    this->sprite_pool.emplace_back(sprite);
}

#ifdef _DEBUG

void SpriteShader::DrawDebugGUI()
{
	std::string text{};
	for (size_t i = 0; i < this->sprite_pool.size(); ++i)
	{
		const auto sprite_Wptr = this->sprite_pool[i];
		std::string owner_name{};

		// 所有者の名前取得
		if (auto sprite = sprite_Wptr.lock())
		{
			if (const auto owner = sprite->GetOwner())
			{
				owner_name = owner->GetName();
			}
		}
		text = "[" + std::to_string(i) + "]:" + owner_name + "#" + std::to_string(i);
		ImGui::Text(text.c_str());
	}
}

#endif // DEBUG