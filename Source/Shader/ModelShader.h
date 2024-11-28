#pragma once
#include <memory>
#include <wrl.h>
#include <vector>
#include "Shader.h"
#include "Graphics/RenderContext.h"
#include "Shader\LightParam.h"

class ModelComponent;
class ModelShaderComponent;

class ModelShader : public Shader
{
private:
	static const int MaxBones = 128;

	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4 view_projection;
	};

	struct MeshConstantBuffer
	{
		DirectX::XMFLOAT4X4	bone_transforms[MaxBones];
	};

	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	material_color;

		// Hack 地形モデルを変更したら削除する
		float tile_count = 1.0f;
		DirectX::XMFLOAT3 dummy;
	};

	// ライト用定数
	struct LightConstantBuffer
	{
		DirectX::XMFLOAT4 ambient_color;
		DirectionalLights directional_lights;
		DirectX::XMFLOAT4 dummy;
	};
public:
    ModelShader();

    void Render() override;
    const char* GetName() override { return "ModelShader"; };

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
	void Draw(ID3D11DeviceContext* dc, const ModelComponent* model);
	void End(ID3D11DeviceContext* dc);

	// 描画するobjectのシェーダーを追加
	void AddShaderComponent(ModelShaderComponent* shader_component);

	// objectのシェーダーを削除する
	void RemoveShaderComponent(ModelShaderComponent* shader_component);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			light_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	std::vector<ModelShaderComponent*> shader_component_vec;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;

#endif // _DEBUG
};

