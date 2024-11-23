#include <stdio.h>
#include <memory>
#include <imgui.h>
#include "System/Misc.h"
#include "Graphics/Graphics.h"
#include "Camera/CameraManager.h"
#include "Debug/DebugPrimitiveRenderer.h"

#include "Component/CameraComponent.h"

void SphereParam::DrawDebugGUI(std::string header_name)
{
	header_name += "##" + std::to_string(id);

	ImGui::Indent(30.0f);
	std::string label;
	label = "##Sphere" + std::to_string(id);
	ImGui::Checkbox(label.c_str(), &is_draw);
	ImGui::SameLine();
	if (!this->is_draw) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色
	if (ImGui::CollapsingHeader(header_name.c_str()))
	{
		label = "Color##Sphere" + std::to_string(id);
		ImGui::ColorEdit4(label.c_str(), &this->color.x);
		label = "Center##Sphere" + std::to_string(id);
		ImGui::DragFloat3(label.c_str(), &this->center.x, 0.1f);
		label = "Radius##Sphere" + std::to_string(id);
		ImGui::DragFloat(label.c_str(), &this->radius, 0.01f);
	}
	if (!this->is_draw) ImGui::PopStyleColor();
	ImGui::Unindent(30.0f);
}

void CylinderParam::DrawDebugGUI(std::string header_name)
{
	header_name += std::to_string(id);

	ImGui::Indent(30.0f);
	std::string label;
	label = "##Sphere" + std::to_string(id);
	ImGui::Checkbox(label.c_str(), &is_draw);
	ImGui::SameLine();
	if (!this->is_draw) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色
	if (ImGui::CollapsingHeader(header_name.c_str()))
	{
		label = "Color##Sphere" + std::to_string(id);
		ImGui::ColorEdit4(label.c_str(), &this->color.x);
		label = "Position##Sphere" + std::to_string(id);
		ImGui::DragFloat3(label.c_str(), &this->position.x, 0.1f);
		label = "Radius##Sphere" + std::to_string(id);
		ImGui::DragFloat(label.c_str(), &this->radius, 0.01f);
		label = "Height##Sphere" + std::to_string(id);
		ImGui::DragFloat(label.c_str(), &this->height, 0.01f);
	}
	if (!this->is_draw) ImGui::PopStyleColor();
	ImGui::Unindent(30.0f);
}

DebugPrimitiveRenderer::DebugPrimitiveRenderer(ID3D11Device* device)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\DebugVS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// 頂点シェーダー生成
		HRESULT hr = device->CreateVertexShader(cso_data.get(), cso_size, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(input_element_desc, ARRAYSIZE(input_element_desc), cso_data.get(), cso_size, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\DebugPS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(cso_data.get(), cso_size, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbMesh);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 球メッシュ作成
	CreateSphereMesh(device, 1.0f, 16, 16);

	// 円柱メッシュ作成
	CreateCylinderMesh(device, 1.0f, 1.0f, 0.0f, 1.0f, 16, 1);
}

// 描画開始
void DebugPrimitiveRenderer::Render()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11DeviceContext* context = graphics->GetDeviceContext();

	// シェーダー設定
	context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	context->PSSetShader(pixel_shader.Get(), nullptr, 0);
	context->IASetInputLayout(input_layout.Get());

	// 定数バッファ設定
	context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// レンダーステート設定
	const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blend_state.Get(), blendFactor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	context->RSSetState(rasterizer_state.Get());

	// ビュープロジェクション行列作成
	MYMATRIX View_projection{};

	if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		View_projection = camera_manager->GetViewProjectionMatrix();
	}

	// プリミティブ設定
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 球描画
	context->IASetVertexBuffers(0, 1, sphere_vertex_buffer.GetAddressOf(), &stride, &offset);
	for (const Sphere& sphere : spheres)
	{
		// ワールドビュープロジェクション行列作成
		MYMATRIX S, T;
		S.SetScalingMatrix(sphere.radius);
		T.SetTranslationMatrix(sphere.center);
		MYMATRIX W = S * T;
		MYMATRIX WVP = W * View_projection;

		// 定数バッファ更新
		CbMesh cbMesh{};
		cbMesh.color = sphere.color;
		WVP.GetFlaot4x4(cbMesh.wvp);

		context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbMesh, 0, 0);
		context->Draw(sphere_vertex_count, 0);
	}
	spheres.clear();

	// 円柱描画
	context->IASetVertexBuffers(0, 1, cylinder_vertex_buffer.GetAddressOf(), &stride, &offset);
	for (const Cylinder& cylinder : cylinders)
	{
		// ワールドビュープロジェクション行列作成
		MYMATRIX S, T;
		S.SetScalingMatrix(cylinder.radius, cylinder.height, cylinder.radius);
		T.SetTranslationMatrix(cylinder.position);
		MYMATRIX W = S * T;
		MYMATRIX WVP = W * View_projection;

		// 定数バッファ更新
		CbMesh cbMesh{};
		cbMesh.color = cylinder.color;
		WVP.GetFlaot4x4(cbMesh.wvp);

		context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbMesh, 0, 0);
		context->Draw(cylinder_vertex_count, 0);
	}
	cylinders.clear();

	// 初期化
	context->IASetInputLayout(nullptr);
	context->IASetVertexBuffers(0, 0, nullptr, 0, 0);
}

// 球描画
void DebugPrimitiveRenderer::DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color)
{
	Sphere sphere;
	sphere.center = center;
	sphere.radius = radius;
	sphere.color = color;
	spheres.emplace_back(sphere);
}

void DebugPrimitiveRenderer::DrawSphere(SphereParam sphere_param)
{
	if (!sphere_param.GetIsDraw()) return;
	Sphere sphere;
	sphere.center = sphere_param.GetCenter();
	sphere.radius = sphere_param.GetRadius();
	sphere.color = sphere_param.GetColor();
	spheres.emplace_back(sphere);
}

// 円柱描画
void DebugPrimitiveRenderer::DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color)
{
	Cylinder cylinder;
	cylinder.position = position;
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.color = color;
	cylinders.emplace_back(cylinder);
}

void DebugPrimitiveRenderer::DrawCylinder(CylinderParam cylinder_param)
{
	if (!cylinder_param.GetIsDraw()) return;

	Cylinder cylinder;
	cylinder.position = cylinder_param.GetPosition();
	cylinder.radius = cylinder_param.GetRadius();
	cylinder.height = cylinder_param.GetHeight();
	cylinder.color = cylinder_param.GetColor();
	cylinders.emplace_back(cylinder);
}

// 球メッシュ作成
void DebugPrimitiveRenderer::CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks)
{
	sphere_vertex_count = stacks * slices * 2 + slices * stacks * 2;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(sphere_vertex_count);

	float phiStep = DirectX::XM_PI / stacks;
	float thetaStep = DirectX::XM_2PI / slices;

	DirectX::XMFLOAT3* p = vertices.get();
	
	for (int i = 0; i < stacks; ++i)
	{
		float phi = i * phiStep;
		float y = radius * cosf(phi);
		float r = radius * sinf(phi);

		for (int j = 0; j < slices; ++j)
		{
			float theta = j * thetaStep;
			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;

			theta += thetaStep;

			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;
		}
	}

	thetaStep = DirectX::XM_2PI / stacks;
	for (int i = 0; i < slices; ++i)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(i * thetaStep);
		for (int j = 0; j < stacks; ++j)
		{
			float theta = j * thetaStep;
			DirectX::XMVECTOR V1 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			DirectX::XMVECTOR P1 = DirectX::XMVector3TransformCoord(V1, M);
			DirectX::XMStoreFloat3(p++, P1);

			int n = (j + 1) % stacks;
			theta += thetaStep;

			DirectX::XMVECTOR V2 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			DirectX::XMVECTOR P2 = DirectX::XMVector3TransformCoord(V2, M);
			DirectX::XMStoreFloat3(p++, P2);
		}
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * sphere_vertex_count);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresourceData.pSysMem = vertices.get();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, sphere_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

// 円柱メッシュ作成
void DebugPrimitiveRenderer::CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks)
{
	cylinder_vertex_count = 2 * slices * (stacks + 1) + 2 * slices;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(cylinder_vertex_count);

	DirectX::XMFLOAT3* p = vertices.get();

	float stackHeight = height / stacks;
	float radiusStep = (radius2 - radius1) / stacks;

	// vertices of ring
	float dTheta = DirectX::XM_2PI / slices;

	for (int i = 0; i < slices; ++i)
	{
		int n = (i + 1) % slices;

		float c1 = cosf(i * dTheta);
		float s1 = sinf(i * dTheta);

		float c2 = cosf(n * dTheta);
		float s2 = sinf(n * dTheta);

		for (int j = 0; j <= stacks; ++j)
		{
			float y = start + j * stackHeight;
			float r = radius1 + j * radiusStep;

			p->x = r * c1;
			p->y = y;
			p->z = r * s1;
			p++;

			p->x = r * c2;
			p->y = y;
			p->z = r * s2;
			p++;
		}

		p->x = radius1 * c1;
		p->y = start;
		p->z = radius1 * s1;
		p++;

		p->x = radius2 * c1;
		p->y = start + height;
		p->z = radius2 * s1;
		p++;
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * cylinder_vertex_count);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresourceData.pSysMem = vertices.get();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, cylinder_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}
