#include "SceneTest.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "Debug/Test.h"

#include "Camera/Camera.h"

SceneTest::SceneTest()
	:test_boject("Data/Model/Jammo/Jammo.mdl")
{
}

void SceneTest::Initialize()
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

void SceneTest::Finalize()
{
}

void SceneTest::Update(float elapsed_time)
{
	test_boject.Update(elapsed_time);
}

void SceneTest::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	test_boject.Render();

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

void SceneTest::DrawImGui()
{
	this->cereal_test.DrawImGui();
	this->test_boject.DrawImGUi();
}
