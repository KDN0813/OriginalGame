#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "System/Misc.h"
#include "Debug/DebugCereal.h"

#include "Camera/Camera.h"

SceneDebug::SceneDebug()
	: stage("Data/Model/ExampleStage/ExampleStage.mdl")
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
			objects.emplace_back(std::make_unique<DebugObject>("Data/Model/Jammo/Jammo.mdl", pos));
		}
	}

	obj_max = objects.size();
	// インスタンスごとのデータ作成
	{
		inputData.resize(obj_max);
		for (size_t i = 0; i < obj_max; ++i)
		{
			objects[i]->UpdateTransform();
			inputData[i] = objects[i]->GetTransform();
		}
	}

	// インスタンスごとの行列を保持するバッファ作成
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT4X4) * obj_max);
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		subresource_data.pSysMem = inputData.data();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;

		Graphics& graphics = Graphics::Instance();
		ID3D11Device* device = graphics.GetDevice();
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, inputBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
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
	DirectX::XMFLOAT3 target = objects[0]->GetPosition();
	target.y += 0.5f;	// プレイヤーの腰当たりをターゲットに設定
	cameraController.SetTarget(target);
	cameraController.Update(elapsed_time);

	for (auto& obj : objects)
	{
		obj->Update(elapsed_time);
	}
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
		Shader* shader = graphics.GetShader();
		Camera& camera = Camera::Intance();
		RenderContext rc;
		rc.view = camera.GetView();
		rc.projection = camera.getProjection();

		shader->Begin(dc, rc);
		
		shader->DrawInstance(dc, objects[0]->GetModel(), this->inputBuffer.Get(),this->obj_max);

		//stage.Render(dc, shader);

		shader->End(dc);
	}

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

void SceneDebug::DrawImGui()
{
	//this->player.DrawImGUi();
}
