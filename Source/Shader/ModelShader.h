#pragma once
#include <memory>
#include <wrl.h>
#include <vector>
#include "Shader.h"
#include "Graphics/RenderContext.h"

class ModelComponent;
class ModelShaderComponent;

class ModelShader : public Shader
{
private:
	static const int MaxBones = 128;

	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	view_projection;
	};

	struct MeshConstantBuffer
	{
		DirectX::XMFLOAT4X4	bone_transforms[MaxBones];
	};

	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	material_color;
	};
public:
    ModelShader(ID3D11Device* device);

    void Render(ID3D11DeviceContext* dc, const RenderContext& rc) override;
    const char* GetName() override { return "ModelShader"; };

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
	void Draw(ID3D11DeviceContext* dc, const ModelComponent* model);
	void End(ID3D11DeviceContext* dc);

	// �`�悷��object�̃V�F�[�_�[��ǉ�
	void AddShaderComponent(std::shared_ptr<ModelShaderComponent> shader_component);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	std::vector<std::weak_ptr<ModelShaderComponent>> shader_component_Wptr_vec;
};
