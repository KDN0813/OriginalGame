#include "SceneTest.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "Debug/Test.h"

void SceneTest::Initialize()
{
	cerealTest = {};
}

void SceneTest::Finalize()
{
}

void SceneTest::Update(float elapsed_time)
{
}

void SceneTest::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	FLOAT color[] = { 0.0f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

void SceneTest::DrawImGui()
{
	this->cerealTest.DrawImGui();
}
