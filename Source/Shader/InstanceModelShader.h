#pragma once
#include <d3d11.h>
#include "Shader/Shader.h"
#include "Model/InstancingModelResource.h"
#include "Model/ModelResource.h"

class InstancingModelShaderComponent;
class InstancingModelComponent;
class Transform3DComponent;

class InstancingModelShader : public Shader
{
public:
	static const int MaxBones = 128;
	static const int MAX_INSTANCES = 512;
private:
	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};
	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	materialColor;
	};
	// インスタンシング描画で使用する共通の定数
	struct CommonDataConstantBuffer
	{
		UINT bone_transform_count; // 1回の描画で使用するボーントランスフォームの数
		DirectX::XMUINT3 dummy;
	};
	struct MeshConstantBuffer
	{
		UINT offset;				// バッファ内でメッシュの開始位置を示すオフセット値
		DirectX::XMUINT3 dummy;
	};
	/**
	* \brief GPUに送るデータ
	*
	* \param animation_start_offset バッファ内で使用するアニメーションの開始位置を示すオフセット値
	* \param anime_frame 現在のフレーム
	* \param world_transform ワールドトランスフォーム
	*/
	struct InstanceData
	{
		UINT animation_start_offset;
		UINT anime_frame;
		DirectX::XMFLOAT4X4 world_transform{};
	};
public:
	InstancingModelShader(ID3D11Device* device);
	~InstancingModelShader() override {}

	void Render(ID3D11DeviceContext* dc, const RenderContext& rc)override;
	
	const char* GetName()override { return "InstancingModelShader"; }

	// インスタンシング描画開始
	void InstancingStart();
	// インスタンスの追加
	void InstancingAdd(InstancingModelComponent* model, Transform3DComponent* transform);
	// インスタンシング描画修了
	void InstancingEnd(ID3D11DeviceContext* dc, InstancingModelComponent* model);

	// 描画するobjectのシェーダーを追加
	void AddShaderComponent(InstancingModelShaderComponent* shader_component);

	/**
	 * \fn InstancingModelShaderComponent
	 * \brief シェーダーコンポーネントが有効であるか判定する。
	 * 描画に必用なパラメータを所持しているか判定する。
	 * 
	 * \param shader_component 判定するシェーダーコンポーネント
	 * \return 有効なら引数のポインタ。有効でないならnullptrを返す
	 */
	InstancingModelShaderComponent* IsShaderValid(InstancingModelShaderComponent* shader_component);
private:
	// インスタンシング描画
	void InstancingRender(ID3D11DeviceContext* dc, InstancingModelComponent* model);
	void DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset);

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			common_data_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;

	// インスタンス毎のワールドトランスフォームをGPUに渡すためのデータ
	int instance_count = 0;
	InstanceData instance_datas[MAX_INSTANCES];	// インスタンス毎のデータ
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_data_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> instance_data_structured_buffer;

	// 描画するインスタンスのシェーダー
	std::vector<InstancingModelShaderComponent*> shader_component_vector;
};

