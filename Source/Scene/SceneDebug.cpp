#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "System/Misc.h"
#include "Debug/DebugCereal.h"

#include "Camera/Camera.h"

#include "Component/TransformComponent.h"
#include "Component/ShaderComponent.h"

SceneDebug::SceneDebug()
	: stage("Data/Model/ExampleStage/ExampleStage.mdl")
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();
	instancing_model = std::make_unique<InstancingModel>(device,"Data/Model/Jammo/Jammo.mdl");

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

			models.emplace_back(std::make_unique<DebugObject>("Data/Model/Jammo/Jammo.mdl", pos));

			const int index = instancing_model->AllocateInstancingIndex();
			DirectX::XMMATRIX m;
			m = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
			m *= DirectX::XMMatrixRotationY(0);
			m *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
			DirectX::XMFLOAT4X4 tm;
			DirectX::XMStoreFloat4x4(&tm, m);
			instancing_model->UpdateTransform(index, tm);
			//objects.emplace_back(std::make_unique<DebugObject>("Data/Model/Jammo/Jammo.mdl", pos));
		}
	}

	// �A�j���[�V�����Đ�
	for (int i = 0; i < 100; ++i)
	{
		instancing_model->PlayAnime(i, i % 10, true);
	}

	
	// �f�o�b�O�I�u�W�F�N�g�쐬
	{
		debug_object = std::make_shared<GameObject>();
		debug_object->AddComponent<TransformComponent>();
	}
}

void SceneDebug::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	// �J���������ݒ�
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
	// �J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = {};
	target.y += 0.5f;	// �v���C���[�̍���������^�[�Q�b�g�ɐݒ�
	cameraController.SetTarget(target);
	cameraController.Update(elapsed_time);

	// �A�j���[�V�����X�V
	for (int i = 0; i < 100; ++i)
	{
		instancing_model->UpdateAnimationFrame(i);
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

	// 3D���f���̕`��
	{
		Graphics& graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics.GetDeviceContext();
		FormerInstanceShader* instance_shader = graphics.GetInstanceShader();
		DebugShader* temporary_shader = graphics.GetTemporaryShader();
		Camera& camera = Camera::Intance();
		RenderContext rc;
		rc.view = camera.GetView();
		rc.projection = camera.getProjection();

		// �C���X�^���V���O�`��
#if 1
		{
			instance_shader->Begin(dc, rc);

			instance_shader->Draw(dc, this->instancing_model.get());

			instance_shader->End(dc);
		}
#endif

		// �ʏ�`��
		{
			temporary_shader->Begin(dc, rc);

#if 0
			for (auto& o : models)
			{
				o->Render(dc, temporary_shader);
			}
#endif

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
