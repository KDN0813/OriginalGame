#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "Debug/DebugCereal.h"

#include "Camera/Camera.h"

SceneDebug::SceneDebug()
	:player("Data/Model/Jammo/Jammo.mdl")
{
}

void SceneDebug::Initialize()
{
	cereal_test = {};

	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Intance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
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
	player.Update(elapsed_time);
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

	// 3Dƒ‚ƒfƒ‹‚Ì•`‰æ
	{
		Graphics& graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics.GetDeviceContext();
		Shader* shader = graphics.GetShader();
		Camera& camera = Camera::Intance();
		RenderContext rc;
		rc.view = camera.GetView();
		rc.projection = camera.getProjection();

		shader->Begin(dc, rc);
		
		player.Render(dc,shader);

		shader->End(dc);
	}

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

void SceneDebug::DrawImGui()
{
	this->cereal_test.DrawImGui();
	this->player.DrawImGUi();
}
