#include "SceneResult.h"
#include "Graphics/Graphics.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"

#include "Component/SpriteComponent.h"

void SceneResult::Initialize()
{
	// �V�F�[�_�[�쐬
	{
		this->sprite_shader = std::make_unique<SpriteShader>();
	}

	// �I�u�W�F�N�g�쐬
	{
		// �^�C�g���w�i
		{
			auto sprite_bg = object_manager.Create();
			sprite_bg->SetName("Title Back Sprite");
			SpriteComponent::SpriteParam param{};
			param.filename = "Data/Debug/Sprite/Title.png";
			param.color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);

			this->sprite_shader->AddSprite(sprite);
		}
	}
}

void SceneResult::Finalize()
{
}

void SceneResult::Update(float elapsed_time)
{
	this->object_manager.Update(elapsed_time);

	// �X�y�[�X�L�[�ŃQ�[����ʂɑJ��(��)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance()->ChangeScene(new SceneTitle());
	}
}

void SceneResult::Render()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics->GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics->GetDepthStencilView();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 2D�X�v���C�g�`��
	{
		this->sprite_shader->Render();
	}
}
