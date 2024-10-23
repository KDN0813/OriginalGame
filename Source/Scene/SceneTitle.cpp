#include "SceneTitle.h"
#include "Scene/SceneLoading.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneManager.h"
#include "Graphics/Graphics.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

#include "Component/SpriteComponent.h"

void SceneTitle::Initialize()
{
    // シェーダー作成
    {
        this->sprite_shader = std::make_unique<SpriteShader>();
    }

	// オブジェクト作成
	{
		// タイトル背景
		{
			auto sprite_bg = object_manager.Create();
			sprite_bg->SetName("Title Back Sprite");
			SpriteComponent::SpriteParam param{};
			param.color = { 0.0f,1.0f, 0.0f, 1.0f };
			param.filename = "Data/Debug/Sprite/Title.png";
			auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);

			this->sprite_shader->AddSprite(sprite);
		}
	}
}

void SceneTitle::Finalize()
{
}

void SceneTitle::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	// スペースキーでゲーム画面に遷移(仮)
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		SceneManager::Instance()->ChangeScene(new SceneLoading(new SceneGame()));
	}
}

void SceneTitle::Render()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics->GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics->GetDepthStencilView();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 2Dスプライト描画
	{
		this->sprite_shader->Render();
	}

	// 2Dデバッグ描画
	{
		DebugDrawGUI();
	}
}

void SceneTitle::DebugDrawGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(300.0f, 400.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_MenuBar))
	{
		this->object_manager.DrawDebugGUI();
	}
	ImGui::End();
}
