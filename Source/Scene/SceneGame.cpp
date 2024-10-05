#include <imgui.h>
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"
#include "Scene/SceneManager.h"

#ifdef _DEBUG
#include "Debug/DebugManager.h"

#endif // _DEBUG

#include "ConstantManager.h"
#include "Input/GamePad.h"

#include "Collision/Collision.h"

#include "Component/ModelComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/InstancingModelComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/GravityComponent.h"

#include "Model/AnimeTransitionJudgementDerived.h"
#include "Camera/CameraControllerDerived.h"


void SceneGame::Initialize()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11Device* device = graphics->GetDevice();
	// シェーダーの作成
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>(device);
		model_shader = std::make_unique<ModelShader>(device);
	}

	// オブジェクト作成
	{
		// ステージ
		{
			auto stage = object_manager.Create();
			stage->SetName("Stage");
			stage->AddComponent<ModelComponent>(device, "Data/Model/ExampleStage/ExampleStage.mdl");
			auto transform = stage->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
			// シェーダー設定
			auto shader_component =
				stage->AddComponent<ModelShaderComponent>(model_shader.get());

			// GameObjectに設定
			GameObject::Instance()->SetGameObject(GameObject::OBJECT_TYPE::STAGE, stage);
		}

		// プレイヤー
		{
			auto player = object_manager.Create();
			player->SetName("Player");
			auto model = player->AddComponent<ModelComponent>(device, "Data/Model/Player/Player.mdl");
			// アニメーション設定
			{
				// 待機
				model->PlayAnimation(PLAYER_ANIMATION::IDLE, true);
				model->SetAnimationState(PLAYER_ANIMATION::IDLE, true);
				model->AddAnimationTransition(PLAYER_ANIMATION::IDLE, PLAYER_ANIMATION::MOVE_FWD, std::make_unique<Judgement_Move>(player), 0.15f);
				model->AddAnimationTransition(PLAYER_ANIMATION::IDLE, PLAYER_ANIMATION::ATTACK01, std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X), 0.3f);

				// 前方移動
				model->SetAnimationState(PLAYER_ANIMATION::MOVE_FWD, true);
				model->AddAnimationTransition(PLAYER_ANIMATION::MOVE_FWD, PLAYER_ANIMATION::IDLE, std::make_unique<Judgement_Move>(player, true), 0.5f);
				model->AddAnimationTransition(PLAYER_ANIMATION::MOVE_FWD, PLAYER_ANIMATION::ATTACK01, std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X), 0.3f);

				// 攻撃01
				model->SetAnimationState(PLAYER_ANIMATION::ATTACK01, false);
				model->AddAnimationTransition(PLAYER_ANIMATION::ATTACK01, PLAYER_ANIMATION::IDLE, std::make_unique<Judgement_TransitionReady>(player, false, true), 0.5f);
			}
			
			auto transform = player->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.005f, 0.005f, 0.005f));
			player->AddComponent<MovementComponent>();
			player->AddComponent<PlayerComponent>();
			// シェーダー設定
			auto shader_component =
				player->AddComponent<ModelShaderComponent>(model_shader.get());
			// カメラ設定
			auto camera = player->AddComponent<CameraComponent>(CameraManager::Instance());
			camera->SetCameraController(std::make_unique<GamepadCameraController>(player));
			camera->SetPerspectiveFov(
				DirectX::XMConvertToRadians(45.0f),
				graphics->GetScreenWidth() / graphics->GetScreenHeight(),
				0.1f,
				1000.0f
			);
			camera->SetRange(10.0f);
			camera->SetRotateX(0.4f);
			camera->SetMainCamera();
			// 重力
			player->AddComponent<GravityComponent>();

			// GameObjectに設定
			GameObject::Instance()->SetGameObject(GameObject::OBJECT_TYPE::PLAYER, player);
		}

		// 敵
		{
			for (int x = 0; x < 10; ++x)
			{
				for (int y = 0; y < 10; ++y)
				{

					const char* faile_name_arry[]
					{
						{"Data/Model/Jammo/Jammo.mdl"},
						{"Data/Model/Player/Player.mdl"},
					};

					auto enemy = object_manager.Create();
					auto model = enemy->AddComponent<InstancingModelComponent>(device, faile_name_arry[x % 2]);
					model->PlayAnimetion(x % 10, true);
					auto transform = enemy->AddComponent<Transform3DComponent>();

					float offset = 2.0f;

					DirectX::XMFLOAT3 pos =
					{
						offset * x,
						0.0f,
						offset * y ,
					};
					transform->SetPosition(pos);
					transform->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));

					// シェーダー設定
					auto shader_component =
						enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
				}
			}
		}

#ifdef _DEBUG	// デバッグ用object
		auto debug_camera = object_manager.Create();
		debug_camera->SetName("Debug Camera");
		debug_camera->AddComponent<Transform3DComponent>();
		auto debug_camera_component = debug_camera->AddComponent<CameraComponent>(CameraManager::Instance());
		debug_camera_component->SetCameraController(std::make_unique<DebugCameraController>(debug_camera));
		debug_camera_component->SetPerspectiveFov(
			DirectX::XMConvertToRadians(45.0f),
			graphics->GetScreenWidth() / graphics->GetScreenHeight(),
			0.1f,
			1000.0f
		);
		debug_camera_component->SetRange(10.0f);
		debug_camera_component->SetRotateX(0.4f);

		CameraManager::Instance()->SetDebugCamera(debug_camera_component.get());
#endif // _DEBUG
	}
}

void SceneGame::Finalize()
{
}

void SceneGame::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);



	auto stage = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
	auto player = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::PLAYER);

	auto p_transform = player->GetComponent<Transform3DComponent>();
	auto p_gravity = player->GetComponent<GravityComponent>();
	auto p_movement = player->GetComponent<MovementComponent>();

	// キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0.0f,1.0f,0.0f };

	float step0ffset = 0.2f;
	// レイキャストY軸(テスト)
	{
		float my = p_gravity->GetGravity() * elapsed_time;
		float slope_rate = 1.0f;   // 傾斜率

		//p_gravity->SetIsGrounded(true);
		//if (0.0f)
		{

			// レイの開始位置は足元より少し上
			DirectX::XMFLOAT3 start = { p_transform->GetPosition().x,p_transform->GetPosition().y + step0ffset,p_transform->GetPosition().z };
			// レイの終点位置は移動語の位置
			DirectX::XMFLOAT3 end = { p_transform->GetPosition().x,p_transform->GetPosition().y + my,p_transform->GetPosition().z };

			// デバッグプリミティブ表示
			{
				DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
				debug_render->DrawSphere(start, 0.05f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
				debug_render->DrawSphere(end, 0.05f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));
			}

			// レイキャストによる地面判定
			auto s_model = stage->GetComponent<ModelComponent>();
			HitResult hit;
			if (Collision::IntersectRayVsModel(start, end, s_model.get(), hit))
			{
				// 法線ベクトル取得
				normal = hit.normal;

				// 地面に接地している
				p_transform->SetPosition(hit.position);

				// 傾斜率の計算
				float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
				slope_rate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

				p_gravity->SetIsGrounded(true);
			}
			else
			{
				DirectX::XMFLOAT3 pos = p_transform->GetPosition();
				pos.y += my;
				p_transform->SetPosition(pos);

				p_gravity->SetIsGrounded(false);
			}
		}
	}

	// レイキャストXZ軸(テスト)
	{
		DirectX::XMFLOAT3 velocity = p_movement->GetVelocity();
		float speed = p_movement->GetSpeed();

		float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
		if (velocityLengthXZ > 0.0f)
		{
			// 水平移動[13]
			float mx = velocity.x * elapsed_time * speed;
			float mz = velocity.z * elapsed_time * speed;

			// レイの開始位置と終点位置[13]
			DirectX::XMFLOAT3 start = { p_transform->GetPosition().x,p_transform->GetPosition().y + step0ffset,p_transform->GetPosition().z };
			DirectX::XMFLOAT3 end = { p_transform->GetPosition().x + mx,p_transform->GetPosition().y + step0ffset,p_transform->GetPosition().z + mz };

			// デバッグプリミティブ表示
			{
				DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
				debug_render->DrawSphere(start, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
				debug_render->DrawSphere(end, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
			}

			// レイキャスト壁判定[13]
			auto s_model = stage->GetComponent<ModelComponent>();
			HitResult hit;
			if (Collision::IntersectRayVsModel(start, end, s_model.get(), hit))
			{
				// 壁からレイの終点までのベクトル[13]
				DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
				DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
				DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

				// 壁の法線[13]
				DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

				// 入射ベクトルを法線に射影[13]
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
				Dot = DirectX::XMVectorScale(Dot, 1.1f);

				// 補正位置の計算[13]
				DirectX::XMVECTOR CorrectionPositon = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
				DirectX::XMFLOAT3 correctionPositon;
				DirectX::XMStoreFloat3(&correctionPositon, CorrectionPositon);

				// 壁ずり方向へのレイキャスト
				HitResult hit2;
				if (!Collision::IntersectRayVsModel(start, correctionPositon, s_model.get(), hit2))
				{
					DirectX::XMFLOAT3 pos = p_transform->GetPosition();
					pos.x = correctionPositon.x;
					pos.z = correctionPositon.z;
					p_transform->SetPosition(pos);
				}
				else
				{
					DirectX::XMFLOAT3 pos = p_transform->GetPosition();
					pos.x = hit2.position.x;
					pos.z = hit2.position.z;
					p_transform->SetPosition(pos);
				}
			}
			else
			{
				DirectX::XMFLOAT3 pos = p_transform->GetPosition();
				pos.x += mx;
				pos.z += mz;
				p_transform->SetPosition(pos);
			}
		}
	}

	// 地面の向きに沿うようにXZ軸回転
	//{
	//	// Y軸が法線ベクトル方向を吹くオイラー角回転を算出する
	//	float ax = atan2f(normal.z, normal.y);
	//	float az = -atan2f(normal.x, normal.y);

	//	angle.x = Mathf::Lerp(angle.x, ax, 0.2f);
	//	angle.z = Mathf::Lerp(angle.z, az, 0.2f);
	//}

	CameraManager::Instance()->Update(elapsed_time);
}

void SceneGame::Render()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics->GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics->GetDepthStencilView();

	CameraManager* camera_manager = CameraManager::Instance();
	auto camera = camera_manager->GetMainCamera();
	RenderContext rc;
	rc.view = camera->GetViewTransform();
	rc.projection = camera->GetProjectionTransform();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 3Dモデルの描画
	{
		// モデル描画
		this->model_shader->Render(dc, rc);

		// インスタンシング描画
		this->instancing_model_shader->Render(dc, rc);
	}

#ifdef _DEBUG
	// 3Dデバッグ描画
	{
		object_manager.DrawDebugPrimitive();

		DebugManager::Instance()->GetDebugRenderer()->Render(dc, rc.view, rc.projection);;
	}

	DrawImGui();
#endif // _DEBUG
}

#ifdef _DEBUG

void SceneGame::DrawImGui()
{
	ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(300.0f, 400.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			// モード切り替え処理
			if (ImGui::BeginMenu("Mode"))
			{
				if (ImGui::MenuItem("Object"))
				{
					mode_index = SceneGame::ImGuiMode::Object;
				}
				if (ImGui::MenuItem("Shader"))
				{
					mode_index = SceneGame::ImGuiMode::Shader;
				}
				if (ImGui::MenuItem("Camera"))
				{
					mode_index = SceneGame::ImGuiMode::Camera;
				}
				if (ImGui::MenuItem("System"))
				{
					mode_index = SceneGame::ImGuiMode::System;
				}

				ImGui::EndMenu();
			}

			// カメラ切り替え
			if (ImGui::BeginMenu("Camera"))
			{
				bool& debug_flag = CameraManager::Instance()->debug_flag;

				std::string label = debug_flag ? "normal camera" : "debug camera";

				if (ImGui::MenuItem(label.c_str()))
				{
					debug_flag = !debug_flag;
					CameraManager::Instance()->SetDebugCamera();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// 各モード表示
		switch (mode_index)
		{
		case SceneGame::ImGuiMode::Object:
			ImGui::Text("Object");
			object_manager.DrawDebugGUI();
			break;
		case SceneGame::ImGuiMode::Shader:
			ImGui::Text("System");
			DrawShaderImGui();
			break;
		case SceneGame::ImGuiMode::Camera:
			ImGui::Text("Camera");
			CameraManager::Instance()->DrawDebugGUI();
			break;
		case SceneGame::ImGuiMode::System:
			ImGui::Text("System");
			break;
		default:
			break;
		}
	}
	ImGui::End();
}

void SceneGame::DrawShaderImGui()
{
	if (ImGui::CollapsingHeader(model_shader->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		model_shader->DrawDebugGUI();
	}
	if (ImGui::CollapsingHeader(instancing_model_shader->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		instancing_model_shader->DrawDebugGUI();
	}
}

#endif // _DEBUG