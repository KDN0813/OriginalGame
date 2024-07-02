#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "System/Misc.h"
#include "Debug/DebugCereal.h"

#include "Camera/Camera.h"

SceneDebug::SceneDebug()
	: stage("Data/Model/ExampleStage/ExampleStage.mdl")
	, instancing_model(std::make_unique<InstancingModel>("Data/Model/Cube/Cube.mdl"))
{
	float offset = 3.0f;
	for (int x = 0; x < 10; ++x)
	{
		for (int z = 0; z < 10; ++z)
		{
			DirectX::XMFLOAT3 pos =
			{
				static_cast<float>(x) * offset,
				0.0f,
				static_cast<float>(z) * offset,
			};
			const int index = instancing_model->AllocateInstancingIndex();
			DirectX::XMMATRIX m;
			m = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
			m *= DirectX::XMMatrixRotationY(0);
			m *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
			DirectX::XMFLOAT4X4 tm;
			DirectX::XMStoreFloat4x4(&tm, m);
			instancing_model->UpdateTransform(index, tm);
			//objects.emplace_back(std::make_unique<DebugObject>("Data/Model/Jammo/Jammo.mdl", pos));
		}
	}
}

void SceneDebug::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	// カメラ初期設定
	Camera& camera = Camera::Intance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45.0f),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
}

void SceneDebug::Finalize()
{
}

void SceneDebug::Update(float elapsed_time)
{
	// カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = {};
	target.y += 0.5f;	// プレイヤーの腰当たりをターゲットに設定
	cameraController.SetTarget(target);
	cameraController.Update(elapsed_time);
}

void SceneDebug::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 3Dモデルの描画
	{
		Graphics& graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics.GetDeviceContext();
		InstanceShader* instance_shader = graphics.GetInstanceShader();
		Shader* temporary_shader = graphics.GetTemporaryShader();
		Camera& camera = Camera::Intance();
		RenderContext rc;
		rc.view = camera.GetView();
		rc.projection = camera.getProjection();

		// インスタンシング描画
		{
			instance_shader->Begin(dc, rc);

			//objects[0]->Render(dc, shader);

			const ModelResource* model_resource = this->instancing_model->GetResource();

			for (const ModelResource::Mesh& mesh : model_resource->GetMeshes())
			{
				for (const ModelResource::Subset& subset : mesh.subsets)
				{
					instance_shader->SetBuffers(dc, this->instancing_model->GetBufferData(mesh));

					//	サブセット単位で描画
					instance_shader->DrawSubset(dc, subset);
				}
			}

			instance_shader->End(dc);
		}

		// 通常描画
		{
			temporary_shader->Begin(dc, rc);

			stage.Render(dc, temporary_shader);

			temporary_shader->End(dc);
		}
	}

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

void SceneDebug::DrawImGui()
{
}
