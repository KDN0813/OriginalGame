#pragma once
#include <d3d11.h>
#include <map>
#include "Shader/Shader.h"
#include "Model/InstancingModelResource.h"
#include "Model/ModelResource.h"
#include "Model\InstancingConstants.h"

class InstancingModelShaderComponent;
class InstancedModelWithAnimationComponent;
class Transform3DComponent;

// アニメ着きインスタンシング + トゥーンシェーダー
class InstanceModelToonShader : public Shader
{
private:
	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4 viewProjection;
		DirectX::XMFLOAT4 light_direction;
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
		UINT animation_start_offset = {};
		UINT anime_frame = {};
		DirectX::XMFLOAT4X4 world_transform = {};;
	};
public:
	InstanceModelToonShader();
	~InstanceModelToonShader() override {}

	void Render()override;

	const char* GetName()override { return "InstanceModelToonShader"; }

	// インスタンシング描画開始
	void InstancingStart();
	// インスタンスの追加
	void InstancingAdd(InstancedModelWithAnimationComponent* model, Transform3DComponent* transform);
	// インスタンシング描画修了
	void InstancingEnd(ID3D11DeviceContext* dc, InstancedModelWithAnimationComponent* model);

	// 描画するobjectのシェーダーを追加
	void AddShaderComponent(InstancingModelShaderComponent* shader_component);

	// objectのシェーダーを削除する
	void RemoveShaderComponent(InstancingModelShaderComponent* shader_component);

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
	void InstancingRender(ID3D11DeviceContext* dc, InstancedModelWithAnimationComponent* model);
	void DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset);

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			common_data_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	// インスタンス毎のワールドトランスフォームをGPUに渡すためのデータ
	int instance_count = 0;
	InstanceData instance_datas[MAX_INSTANCES];	// インスタンス毎のデータ
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_data_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> instance_data_structured_buffer;

	// 描画するインスタンスのシェーダー
	std::map<int, std::vector<InstancingModelShaderComponent*>> shader_component_vec_map;

#ifdef _DEBUG
public:
	void DrawDebugGUI();

#endif // _DEBUG
};

