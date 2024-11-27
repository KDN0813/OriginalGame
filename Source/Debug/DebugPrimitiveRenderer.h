#pragma once

#include <vector>
#include <wrl.h>
#include <string>
#include <d3d11.h>
#include "System/MyMath/MYMATRIX.h"

using DebugPrimitiveId = size_t;

static DebugPrimitiveId DebugPrimitiveAllocate()
{
	static DebugPrimitiveId id = 0;
	return id++;
}

class SphereParam
{
public:
	SphereParam() : color(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)), center(), radius(0.1f), id(DebugPrimitiveAllocate()) {};
	SphereParam(DirectX::XMFLOAT4 color, float radius) :color(color), center(), radius(radius), id(DebugPrimitiveAllocate()) {};

	DirectX::XMFLOAT4 GetColor() { return this->color; }
	DirectX::XMFLOAT3 GetCenter() { return this->center; }
	float GetRadius() { return this->radius; }
	float GetColor(float radius) { this->radius = radius; }
	bool GetIsDraw() { return this->is_draw; }
	void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
	void SetCenter(DirectX::XMFLOAT3 center) { this->center = center; }
	void SetRadius(float radius) { this->radius = radius; }
	void SetIsDraw(bool is_draw) { this->is_draw = is_draw; }
	void DrawDebugGUI(std::string header_name);
private:
	DirectX::XMFLOAT4	color;
	DirectX::XMFLOAT3	center;
	float				radius;
	DebugPrimitiveId	id;
	bool is_draw = true;
};

class CylinderParam
{
public:
	CylinderParam() : color(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)), position(), radius(0.1f), height(0.1f), id(DebugPrimitiveAllocate()) {};
	CylinderParam(DirectX::XMFLOAT4 color, float radius, float height) :color(color), position(), radius(radius), height(height), id(DebugPrimitiveAllocate()) {};

	DirectX::XMFLOAT4 GetColor() { return this->color; }
	DirectX::XMFLOAT3 GetPosition() { return this->position; }
	float GetRadius() { return this->radius; }
	float GetHeight() { return this->height; }
	float GetColor(float radius) { this->radius = radius; }
	bool GetIsDraw() { return this->is_draw; }
	void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
	void SetPosition(DirectX::XMFLOAT3 position) { this->position = position; }
	void SetRadius(float radius) { this->radius = radius; }
	void SetHeight(float height) { this->height = height; }
	void SetIsDraw(bool is_draw) { this->is_draw = is_draw; }
	void DrawDebugGUI(std::string header_name);
private:
	DirectX::XMFLOAT4	color;
	DirectX::XMFLOAT3	position;
	float				radius;
	float				height;
	DebugPrimitiveId	id;
	bool is_draw = true;
};

class AABBCorners
{
public:
	AABBCorners(DirectX::XMFLOAT4 color, float radius) : color(color), center(), radius(radius), id(DebugPrimitiveAllocate()) {}
	AABBCorners() : AABBCorners(DirectX::XMFLOAT4(), 0.1f) {}

	DirectX::XMFLOAT4 GetColor() { return this->color; }
	DirectX::XMFLOAT3 GetCenter(size_t i) { return this->center[i]; }
	float GetRadius() { return this->radius; }
	float GetColor(float radius) { this->radius = radius; }
	void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
	bool GetIsDraw() { return this->is_draw; }
	void SetCenter(DirectX::XMFLOAT3 c[8]);
	void SetRadius(float radius) { this->radius = radius; }
	void SetIsDraw(bool is_draw) { this->is_draw = is_draw; }
	void DrawDebugGUI(std::string header_name);
private:
	DirectX::XMFLOAT4	color;
	float				radius;
	DebugPrimitiveId	id;
	DirectX::XMFLOAT3	center[8];
	bool is_draw = true;
};

class DebugPrimitiveRenderer
{
public:
	DebugPrimitiveRenderer(ID3D11Device* device);
	~DebugPrimitiveRenderer() {}

public:
	// •`‰æÀs
	void Render();

	// ‹…•`‰æ
	void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);
	// ‹…•`‰æ
	void DrawSphere(SphereParam sphere_param);

	// ‰~’Œ•`‰æ
	void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);
	// ‰~’Œ•`‰æ
	void DrawCylinder(CylinderParam cylinder_param);

	// AABB‚Ì’¸“_•`‰æ
	void DrawAABBCorners(AABBCorners AABB_corners);

private:
	// ‹…ƒƒbƒVƒ…ì¬
	void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);

	// ‰~’ŒƒƒbƒVƒ…ì¬
	void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);

private:
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4	color;
	};

	struct Sphere
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	center;
		float				radius;
	};

	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sphere_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cylinder_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	std::vector<Sphere>		spheres;
	std::vector<Cylinder>	cylinders;

	UINT	sphere_vertex_count = 0;
	UINT	cylinder_vertex_count = 0;
};