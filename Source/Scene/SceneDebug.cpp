#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "System/Misc.h"
#include "Debug/DebugCereal.h"

#include "Camera/Camera.h"

#pragma region 画像出力
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

struct RGBA {
	uint8_t r, g, b, a; //赤, 緑, 青, 透過
	RGBA() = default;
	constexpr RGBA(const uint8_t r_, const uint8_t g_, const uint8_t b_, const uint8_t a_) :r(r_), g(g_), b(b_), a(a_) {}
};
struct MyRGBA
{
	DirectX::XMFLOAT3 color;
};

const int width{ 512 }, height{ 512 }; //幅と高さ
int OutPutImage() 
{
	std::unique_ptr<RGBA[][width]> rgba(new(std::nothrow) RGBA[height][width]);
	//RGBA rgbas[height][width]{};

	// std::vectorを使用して2次元配列を作成
	std::vector<std::vector<RGBA>> rgbas;
	rgbas.resize(height);
	for (auto& r : rgbas)
	{
		r.resize(width);
	}

	// 配列の初期化（例：すべてのピクセルを赤に設定）
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			rgba[y][x].r = 255;
			rgba[y][x].g = 0;
			rgba[y][x].b = 0;
			rgba[y][x].a = 255;
		}
	}

	if (!rgba) return -1;

	stbi_write_png("picture_0.png", width, height, static_cast<int>(sizeof(RGBA)), rgba.get(), 0);
	//stbi_write_png("picture_0.png", width, height, static_cast<int>(sizeof(RGBA)), rgbas.data(), 0);
}
#pragma endregion 画像出力

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

	// アニメーション再生
	for (int i = 0; i < 100; ++i)
	{
		instancing_model->PlayAnime(i, i % 10, true);
	}

	//OutPutImage();

	// 出力テクスチャ(仮)作成
	{
		//const Graphics& graphics = Graphics::Instance();
		//ID3D11Device* device = graphics.GetDevice();
		//ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();

		//HRESULT hr = S_OK;

		//const int iPixSize = 32;//縦横ピクセル数

		////テクスチャの作成
		//Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DTexture;
		//D3D11_TEXTURE2D_DESC td;
		//td.Width = iPixSize;
		//td.Height = iPixSize;
		//td.MipLevels = 1;
		//td.ArraySize = 1;
		//td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//td.SampleDesc.Count = 1;
		//td.SampleDesc.Quality = 0;
		//td.Usage = D3D11_USAGE_DYNAMIC;
		//td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//td.MiscFlags = 0;
		//hr = device->CreateTexture2D(&td, nullptr, &D3DTexture);
		//_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		////テクスチャ書き替え
		//D3D11_MAPPED_SUBRESOURCE msr;
		//deviceContext->Map(D3DTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		//uint8_t srcData[width * height * 4] = { 0 };//ビットマップを黒で初期化
		//for (int i = 0; i < iPixSize * iPixSize * 4; i += 4)
		//{
		//	srcData[i] = static_cast<uint8_t>(rand() % 255);//Red
		//	srcData[i + 1] = static_cast<uint8_t>(rand() % 255);//Green
		//	srcData[i + 2] = static_cast<uint8_t>(rand() % 255);//Blue
		//}
		//memcpy(msr.pData, srcData, sizeof(srcData));

		//deviceContext->Unmap(D3DTexture.Get(), 0);

		//// png書き出し
		//{
		//	std::unique_ptr<RGBA[][width]> rgba(new(std::nothrow) RGBA[height][width]);
		//	//RGBA rgbas[height][width]{};


		//	if (!rgba) return;

		//	//stbi_write_png("picture_0.png", width, height, static_cast<int>(sizeof(RGBA)), rgba.get(), 0);
		//}
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

	// アニメーション更新
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
#if 1
		{
			instance_shader->Begin(dc, rc);

			instance_shader->Draw(dc, this->instancing_model.get());

			instance_shader->End(dc);
		}
#endif

		// 通常描画
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
